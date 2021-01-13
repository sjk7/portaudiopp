#pragma once
// portaudioplusplus header file. Written by Steve. Monday January 11th 2021.
// A C++ wrapper for Portaudio.

#include <atomic>
#include <cassert>
#include <cstring>
#include <iostream>
#include <math.h>
#include <portaudio.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace strings
{
template <typename... Args>
void make_string(std::ostringstream &stream, Args &&... args) noexcept
{
    ((stream << args << ' '), ...);
}
} // namespace strings

namespace portaudio
{

static inline void sleep_ms(const long ms) noexcept { Pa_Sleep(ms); }
class Exception : public std::runtime_error
{
    std::ostringstream m_stream;
    std::string m_str;
    PaError m_errcode;

  public:
    static inline auto error_string(PaError errCode) noexcept
    {
        return Pa_GetErrorText(errCode);
    }
    template <typename... Args>
    auto error_string_ex(PaError errCode, Args &&... args) noexcept
    {
        m_stream.str().clear();
        m_stream << "PortAudio error code: " << errCode << "\n"
                 << error_string(errCode) << "\n";
        strings::make_string(m_stream, args...);
        std::string annoying_temp = m_stream.str();
        return annoying_temp;
    }

    Exception(const PaError errCode) noexcept
        : m_str(error_string(errCode)), std::runtime_error(""),
          m_errcode(errCode)
    {
    }

    template <typename... ARGS>
    Exception(const PaError errCode, ARGS &&... args) noexcept
        : m_str(error_string_ex(errCode, std::forward<ARGS>(args)...)),
          std::runtime_error(""), m_errcode(errCode)
    {
    }

    virtual const char *what() const noexcept
    {
        const char *ps = m_str.c_str();
        return ps;
    }

    PaError errorCode() const noexcept { return m_errcode; }
};

using hostApiList = std::vector<const PaHostApiInfo *>;
using deviceList = std::vector<const PaDeviceInfo *>;

using apiDeviceList = std::unordered_map<std::string, deviceList>;
struct CallbackInfo
{
    CallbackInfo(PaTime elapsed_seconds, const void *input, void *output,
                 unsigned long frameCount,
                 const PaStreamCallbackTimeInfo *timeInfo,
                 const PaStreamCallbackFlags flags, void *userdata,
                 int samplerate)
        : elapsed_time(elapsed_seconds), input(input), output(output),
          frameCount(frameCount), timeInfo(timeInfo), statusFlags(flags),
          userdata(userdata), samplerate(samplerate)

    {
    }
    PaTime elapsed_time = {0};
    const void *input;
    void *output;
    unsigned long frameCount;
    const PaStreamCallbackTimeInfo *timeInfo;
    PaStreamCallbackFlags statusFlags;
    void *userdata;
    int samplerate;
};

enum class CallbackResult : unsigned int
{
    Continue = 0, /**< Signal that the stream should continue invoking the
                     callback and processing audio. */
    Complete = 1, /**< Signal that the stream should stop invoking the callback
                     and finish once all output samples have played. */
    Abort = 2 /**< Signal that the stream should stop invoking the callback and
                 finish as soon as possible. */
};

// Inherit from me to grab the audio callback.
struct AudioCallback
{
    virtual CallbackResult onCallback(CallbackInfo cbInfo) noexcept = 0;
    virtual ~AudioCallback() noexcept {};
};

namespace detail
{

static inline hostApiList enum_apis()
{
    hostApiList list;
    const int cnt = Pa_GetHostApiCount();
    for (int i = 0; i < cnt; ++i)
    {
        auto inf = Pa_GetHostApiInfo(i);
        list.push_back(inf);
    }
    return list;
}
static inline deviceList enum_devices()
{
    deviceList list;
    for (int i = 0; i < Pa_GetDeviceCount(); ++i)
    {
        auto inf = Pa_GetDeviceInfo(i);
        list.push_back(inf);
    }
    return list;
}

struct CallbackUserData : protected AudioCallback
{
    void *UserData = nullptr;
    CallbackResult callback(CallbackInfo cbInfo) noexcept
    {
        return AudioCallback::onCallback(cbInfo);
    }
};

} // namespace detail

struct enumerator_t
{
    const hostApiList &apis(bool force_refresh = false) const
    {
        if (m_apis.empty() || force_refresh)
        {
            m_apis = detail::enum_apis();
        }
        return m_apis;
    }

    const deviceList &devices(bool force_refresh = false) const
    {
        if (m_devices.empty() || force_refresh)
        {
            m_devices = detail::enum_devices();
        }
        return m_devices;
    }

    const apiDeviceList devicesByApi(bool force_refresh = false) const
    {
        const hostApiList &apis = this->apis(force_refresh);
        const deviceList devices = this->devices(force_refresh);

        apiDeviceList retval;

        for (const auto api : apis)
        {
            auto it = retval.find(api->name);
            if (it == retval.end())
            {
                auto pr = retval.insert({std::string(api->name), deviceList{}});
                it = pr.first;
            }
        }

        for (const auto &d : devices)
        {
            const auto api = Pa_GetHostApiInfo(d->hostApi);
            auto pr = retval.find(api->name);
            pr->second.push_back(d);
        }

        return retval;
    }

    const PaDeviceInfo *default_device() const noexcept
    {
        const auto idx = Pa_GetDefaultOutputDevice();
        return Pa_GetDeviceInfo(idx);
    }

  private:
    mutable hostApiList m_apis;
    mutable deviceList m_devices;
    mutable apiDeviceList m_apiDeviceList;
};

struct SampleFormat
{
    static inline PaSampleFormat Float32 = paFloat32;
    static inline PaSampleFormat Int32 = paInt32;
    static inline PaSampleFormat Int24 = paInt24;
    static inline PaSampleFormat Int16 = paInt16;
    static inline PaSampleFormat Int8 = paInt8;
    static inline PaSampleFormat UInt8 = paUInt8;
    static inline PaSampleFormat CustomFormat = paCustomFormat;
    static inline PaSampleFormat NonInterleaved = paNonInterleaved;
};

struct StreamSetupInfo
{
    // PaStreamCallback *streamCallback = {nullptr};
    PaStream *stream = {nullptr};
    int inputChannelCount = 2;
    int outputChannelCount = 2;
    PaSampleFormat sampleFormat = SampleFormat::Float32;
    double sampleRate = 44100;
    unsigned long framesPerBuffer = 512;
    void *userData = {nullptr};
    const PaStreamParameters *inParams = {nullptr};
    const PaStreamParameters *outParams = {nullptr};
    PaStreamFlags flags = {0};
};

namespace detail
{

// We generate our own time stamps, since, at least on Linux,
// All Portaudio's currentTime() api calls just always return zero!
class TimeStampGen
{
  public:
    TimeStampGen() {}
    TimeStampGen(const TimeStampGen &rhs) = delete;
    TimeStampGen &operator=(const TimeStampGen &rhs) = delete;
    TimeStampGen &operator=(TimeStampGen &&rhs)
    {
        m_CurrentMillis = (rhs.elapsedMillis());
        m_nframes = rhs.m_nframes;
        m_Samplerate = rhs.m_Samplerate;
        rhs.m_Samplerate = 0;
        return *this;
    }
    TimeStampGen(TimeStampGen &&rhs)
    {
        m_Samplerate = rhs.m_Samplerate;
        m_nframes = rhs.m_nframes;
        m_CurrentMillis = (rhs.elapsedMillis());
        rhs.m_Samplerate = 0;
    }
    int samplerate() const noexcept { return m_Samplerate; }
    double elapsedMillis() const noexcept { return m_CurrentMillis; }
    double elapsedSeconds() const noexcept
    {
        return (double)elapsedMillis() / 1000.0;
    }

  protected:
    void reset(const int newSamplerate)
    {
        assert(newSamplerate > 0);
        m_Samplerate = newSamplerate;
        m_CurrentMillis = 0;
        m_nframes = 0;
    }
    // the portaudio methods for getting stream time are flaky.
    // On Linux, I get a forever current time as 0. So use this instead
    PaTime generateTimeStamps(unsigned long frameCount) noexcept
    {

        const auto frames_now = (m_nframes += frameCount);
        assert(m_Samplerate);
        m_CurrentMillis = ((PaTime)frames_now / (PaTime)m_Samplerate) * 1000ULL;
        return elapsedSeconds();
    }

  private:
    std::atomic<uint64_t> m_CurrentMillis = {0};
    int m_Samplerate = {0};
    uint64_t m_nframes = {0};
}; // TimeStampGen

} // namespace detail

template <typename AUDIOCALLBACK> class Stream : public detail::TimeStampGen
{
  private:
    StreamSetupInfo m_info;

    static inline int
    callback_dispatcher(const void *input, void *output,
                        unsigned long frameCount,
                        const PaStreamCallbackTimeInfo *timeInfo,
                        PaStreamCallbackFlags statusFlags, void *userData)
    {
        Stream *p = (Stream *)userData;
        assert(p && "stream context not set. FATAL");

        const auto elapsed_time = p->generateTimeStamps(frameCount);
        return (int)p->m_cb({elapsed_time, input, output, frameCount, timeInfo,
                             statusFlags, userData, p->samplerate()});
    }

  public:
    explicit Stream(AUDIOCALLBACK &&cb, std::string_view id = "")
        : m_cb(cb), m_sid(id)
    {
    }
    Stream(const Stream &rhs) = delete;
    Stream &operator=(const Stream &rhs) = delete;

    Stream(Stream &&rhs) : m_cb(std::move(rhs.m_cb))
    {
        memcpy(&m_info, &rhs.m_info, sizeof(m_info));
        m_sid = std::move(rhs.m_sid);
        rhs.Close();
        if (m_info.inParams || m_info.outParams)
        {
            openSpecific(m_info);
        }
        else
        {
            openDefault(m_info);
        }
    }

    Stream &&operator=(Stream &&rhs) = delete;

    void openDefault()
    {
        StreamSetupInfo info;

        int err = Pa_OpenDefaultStream(
            &info.stream, info.inputChannelCount, info.outputChannelCount,
            info.sampleFormat, info.sampleRate, info.framesPerBuffer,
            callback_dispatcher, (void *)this);

        if (err)
        {
            throw Exception(err, "Failed to openDefaultStream()");
        }
        m_info = info;
        TimeStampGen::reset(info.sampleRate);
    }

    void openDefault(StreamSetupInfo &info)
    {

        int err = Pa_OpenDefaultStream(
            &info.stream, info.inputChannelCount, info.outputChannelCount,
            info.sampleFormat, info.sampleRate, info.framesPerBuffer,
            callback_dispatcher, (void *)this);

        if (err)
        {
            throw Exception(err, "Failed to openDefaultStream()");
        }
        m_info = info;
        TimeStampGen::reset(info.sampleRate);
    }

    void openSpecific(StreamSetupInfo &info)
    {

        const auto err =
            Pa_OpenStream(&info.stream, info.inParams, info.outParams,
                          info.sampleRate, info.framesPerBuffer, info.flags,
                          callback_dispatcher, (void *)this);

        if (err)
        {
            throw Exception(err, "Failed to openSpecificStream()");
        }
        m_info = info;
        TimeStampGen::reset(info.sampleRate);
    }

    void Stop()
    {
        TimeStampGen::reset(m_info.sampleRate);
        if (!m_info.stream)
        {
            throw Exception(-1, "Stop(): unexpected: no stream to start");
        }
        int ret = Pa_StopStream(m_info.stream);
        if (ret)
        {
            throw Exception(ret, "Error Stopping Stream");
        }
    }

    void Start()
    {
        TimeStampGen::reset(m_info.sampleRate);
        if (!m_info.stream)
        {
            throw Exception(-1, "Start(): unexpected: no stream to start");
        }
        int ret = Pa_StartStream(m_info.stream);
        if (ret)
        {
            throw Exception(ret, "Error Starting Stream");
        }
    }

    void Close()
    {
        if (m_info.stream)
        {
            Pa_CloseStream(m_info.stream);
            m_info.stream = nullptr;
        }
    }

    ~Stream() { Close(); }

    std::string_view id() const noexcept { return m_sid; }

  private:
    AUDIOCALLBACK m_cb;
    std::string m_sid;

}; // namespace portaudio

class Portaudio
{
  public:
    Portaudio(const std::string_view id = "")
        : info(*Pa_GetVersionInfo()), m_id(id)
    {
        PaError err = Pa_Initialize();
        if (err != PaErrorCode::paNoError)
        {
            throw std::runtime_error(Pa_GetErrorText(err));
        }
        m_instances++;
    }
    ~Portaudio() noexcept
    {
        Pa_Terminate();
        m_instances--;
    }
    const PaVersionInfo info = {0};
    static int instances() noexcept { return m_instances; }
    const enumerator_t &enumerator() const noexcept { return m_enum; }
    std::string_view id() const noexcept { return m_id; }

    template <typename CALLBACK>
    Stream<CALLBACK> openDefaultStream(CALLBACK &&cb)
    {
        Stream<CALLBACK> s(std::forward<CALLBACK>(cb));
        s.openDefault();
        return std::move(s);
    }

  private:
    static inline int m_instances = 0;
    const std::string m_id;
    mutable hostApiList m_apis;
    mutable enumerator_t m_enum;
};

} // namespace portaudio