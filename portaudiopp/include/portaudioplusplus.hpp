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
    static inline auto error_string(PaError errCode) noexcept
    {
        return Pa_GetErrorText(errCode);
    }
    Exception(const PaError errCode) noexcept
        : m_str(error_string(errCode)), std::runtime_error(m_str.c_str()),
          m_errcode(errCode)
    {
    }

    template <typename... ARGS>
    Exception(const PaError errCode, ARGS &&... args) noexcept
        : m_str(error_string_ex(errCode, std::forward<ARGS>(args)...)),
          std::runtime_error(m_str.c_str()), m_errcode(errCode)
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
    CallbackInfo(PaTime elapsed_seconds, const void *input, const void *output,
                 unsigned long frameCount,
                 const PaStreamCallbackTimeInfo *timeInfo,
                 const PaStreamCallbackFlags flags, void *userdata)
        : elapsed_time(elapsed_seconds), input(input), output(output),
          frameCount(frameCount), timeInfo(timeInfo), statusFlags(flags),
          userdata(userdata)

    {
    }
    PaTime elapsed_time = {0};
    const void *input;
    const void *output;
    unsigned long frameCount;
    const PaStreamCallbackTimeInfo *timeInfo;
    PaStreamCallbackFlags statusFlags;
    void *userdata;
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
struct Portaudio
{
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
    AudioCallback *m_callback = {nullptr};

    PaStream *preparePlay(AudioCallback *callback)
    {
        m_callback = callback;
        PaStream *stream = nullptr;
        auto ret = Pa_OpenDefaultStream(&stream, 2, 2, paFloat32, 44100, 512,
                                        callback_dispatcher, (void *)this);
        if (ret)
        {
            throw Exception(ret, "Error whilst opening default stream");
        }

        m_HostApiTypeId = Pa_GetStreamHostApiType(stream);

        m_stream = stream;
        m_streamInfo = *Pa_GetStreamInfo(m_stream);
        assert(m_streamInfo.sampleRate);
        return m_stream;
    }

    int play()
    {
        if (!m_stream)
        {
            throw Exception(
                -1, "Cannot play() without a stream. Call prepareplay first.");
        }
        auto ret = Pa_StartStream(m_stream);
        if (ret)
        {
            throw Exception(ret, "Error starting portaudio stream");
        }
        return ret;
    }

    int stop()
    {
        assert(m_stream);
        Pa_StopStream(m_stream);
        m_nframes = 0;
        m_CurrentTimeSeconds = 0;
    }
    PaTime elapsedSeconds() const noexcept
    {
        assert(m_stream && m_streamInfo.sampleRate);
        return m_CurrentTimeSeconds;
    }

  private:
    static inline int m_instances = 0;
    const std::string m_id;
    mutable hostApiList m_apis;
    mutable enumerator_t m_enum;
    // put in a stream class:
    PaStream *m_stream = {nullptr};
    unsigned long m_nframes = {0};
    PaHostApiTypeId m_HostApiTypeId = PaHostApiTypeId::paBeOS;
    std::atomic<double> m_CurrentTimeSeconds = {0};
    PaStreamInfo m_streamInfo = {0};

    // the portaudio methods for getting stream time are flaky.
    // On Linux, I get a forever current time as 0. So use this instead
    PaTime generateTimeStamps(unsigned long frameCount) noexcept
    {

        const auto frames_now = (m_nframes += frameCount);
        assert(m_streamInfo.sampleRate);
        m_CurrentTimeSeconds =
            (PaTime)frames_now / (PaTime)m_streamInfo.sampleRate;
        return m_CurrentTimeSeconds;
    }

    static inline auto callback_dispatcher =
        [](const void *input, void *output, unsigned long frameCount,
           const PaStreamCallbackTimeInfo *timeInfo,
           PaStreamCallbackFlags statusFlags, void *userData) {
            Portaudio *p = (Portaudio *)userData;
            assert(p && "portaudio context not set. FATAL");

            const auto elapsed_time = p->generateTimeStamps(frameCount);
            auto *callback = p->m_callback;
            assert(callback && "user callback not set. FATAL");
            return (int)p->m_callback->onCallback(

                {elapsed_time, input, output, frameCount, timeInfo, statusFlags,
                 userData});
        };
};

} // namespace portaudio