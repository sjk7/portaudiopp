#pragma once
// portaudioplusplus header file. Written by Steve. Monday January 11th 2021.
// A C++ wrapper for Portaudio.

#include <atomic>
#include <cassert>
#include <cstring>
#include <iostream>
#include <math.h>
#include <memory> // unique_ptr
#include "../../../portaudio/include/portaudio.h"
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
        std::string pe = error_string(errCode);
        m_stream << "PortAudio error code: " << errCode << " (" << pe << ")\n";

        strings::make_string(m_stream, args...);
        std::string annoying_temp = m_stream.str();
        return annoying_temp;
    }

    Exception(const PaError errCode) noexcept
        : std::runtime_error(""), m_str(error_string(errCode)),
          m_errcode(errCode)
    {
    }

    template <typename... ARGS>
    Exception(const PaError errCode, ARGS &&... args) noexcept
        : std::runtime_error(""),
          m_str(error_string_ex(errCode, std::forward<ARGS>(args)...)),
          m_errcode(errCode)
    {
    }

    virtual const char *what() const noexcept
    {

        const char *ps = m_str.c_str();
        return ps;
    }

    PaError errorCode() const noexcept { return m_errcode; }
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
    double samplerate = 44100;
    unsigned long framesPerBuffer = 512;
    void *userData = {nullptr};
    const PaStreamParameters *inParams = {nullptr};
    const PaStreamParameters *outParams = {nullptr};
    PaStreamFlags flags = {0};
    PaTime inputLatency = {0};
    PaTime outputLatency = {0};
};

static inline int constexpr INVALID_PA_DEVICE_INDEX = -1;
struct PaHostApiInfoEx
{
    const PaHostApiInfo *info = nullptr;
    int api_index = -1;
    int defaultDeviceApiIndex = INVALID_PA_DEVICE_INDEX;

};

struct PaDeviceInfoEx
{
    const PaDeviceInfo *info = nullptr;
    int global_device_index = INVALID_PA_DEVICE_INDEX;
    StreamSetupInfo streamSetupInfo = {0};
    const PaHostApiInfoEx* hostApiInfo = { nullptr};
    int api_device_index = INVALID_PA_DEVICE_INDEX;
};



using hostApiList = std::vector<PaHostApiInfoEx>;
using deviceList = std::vector<PaDeviceInfoEx>;

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

template <typename T>
class no_copy{
protected:
    no_copy() = default;
    no_copy& operator=(const no_copy&) = delete;
    no_copy(const no_copy&) = delete;
};

static inline hostApiList enum_apis()
{
    hostApiList list;
    const int cnt = Pa_GetHostApiCount();
    for (int i = 0; i < cnt; ++i)
    {
        auto inf = Pa_GetHostApiInfo(i);
        PaHostApiInfoEx d{inf, i};
        list.push_back(d);
    }
    return list;
}
static inline deviceList enum_devices()
{
    deviceList list;
    for (int i = 0; i < Pa_GetDeviceCount(); ++i)
    {
        auto inf = Pa_GetDeviceInfo(i);
        PaDeviceInfoEx d{inf, i, {0}};
        list.push_back(d);
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

struct enumerator_t : detail::no_copy<enumerator_t>
{
    void populate(){
        m_apiDeviceList = devicesByApi(true);
    }
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
        if (!force_refresh && !apis().empty() && !this->devices().empty()
                && !this->m_apiDeviceList.empty()){
            return m_apiDeviceList;
        }
        this->apis(force_refresh);
        this->devices(force_refresh);
        apiDeviceList retval;

        for (auto& api : m_apis)
        {
            const auto ptrinfo = Pa_GetDeviceInfo(api.info->defaultOutputDevice);
            const std::string default_device_name_for_api = ptrinfo->name;

            auto it = retval.find(api.info->name);
            if (it == retval.end())
            {
                auto pr =
                    retval.insert({std::string(api.info->name), deviceList{}});
                it = pr.first;
            }

            for (int i = 0; i < api.info->deviceCount; ++i){
                const auto global_index = Pa_HostApiDeviceIndexToDeviceIndex(api.api_index, i);
                auto& dev = m_devices.at(global_index);
                std::string name(dev.info->name);
                if (name == default_device_name_for_api){
                    api.defaultDeviceApiIndex = i;
                }
                dev.hostApiInfo = &api;
                dev.api_device_index = i;
                it->second.push_back(dev);
            }
        };

        for (const auto& a : m_apis){
            assert(a.defaultDeviceApiIndex >= 0);
            assert (a.defaultDeviceApiIndex <= a.info->deviceCount);
        }
        return retval;
    }

    // returns an *instance* (a copy) of an input device where you might want to adjust its properties
    // perhaps before calling openStream() etc on it.
    const PaDeviceInfoEx defaultInputDevice() const noexcept
    {

        const auto idx = Pa_GetDefaultInputDevice();
        const auto& devs = devices();
        assert((size_t)idx < devs.size());
        return m_devices.at(idx);
    }
    // returns an *instance* (a copy) of an output device where you might want to adjust its properties
    // perhaps before calling openStream() etc on it.
    const PaDeviceInfoEx defaultDevice() const noexcept
    {

        const auto idx = Pa_GetDefaultOutputDevice();
        const auto& devs = devices();
        assert((size_t)idx < devs.size());
        return m_devices.at(idx);
    }
    const PaHostApiInfoEx& defaultHostApi() const noexcept
    {
        const auto def_index = Pa_GetDefaultHostApi();
        assert((size_t)def_index < m_apis.size());
        return m_apis.at(def_index);
    }


  private:
    mutable hostApiList m_apis;
    mutable deviceList m_devices;
    mutable apiDeviceList m_apiDeviceList;
    deviceList& devices_non_const() noexcept {
        return m_devices;
    }


};



static inline StreamSetupInfo makeStreamSetupInfo(int samplerate = 44100,
                                    PaSampleFormat fmt = SampleFormat::Float32,
                                    PaStreamParameters *inParams = nullptr,
                                    PaStreamParameters *outParams = nullptr)
{
    StreamSetupInfo s;
    if (samplerate > 0) s.samplerate = samplerate;
    s.sampleFormat = (PaSampleFormat)fmt;
    s.inParams = inParams;
    s.outParams = outParams;
    return s;
}



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
    TimeStampGen &operator=(TimeStampGen &&rhs) = delete;

    TimeStampGen(TimeStampGen &&rhs)
        : m_CurrentMillis(rhs.m_CurrentMillis.load()),
          m_Samplerate(rhs.m_Samplerate.load()), m_nframes(rhs.m_nframes.load())
    {

        rhs.m_Samplerate = 0;
    }
    int samplerate() const noexcept { return m_Samplerate; }
    double elapsedMillis() const noexcept { return m_CurrentMillis; }
    uint64_t nframes() const noexcept { return m_nframes; }
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
    std::atomic<int> m_Samplerate = {0};
    std::atomic<uint64_t> m_nframes = {0};
}; // TimeStampGen

} // namespace detail

template <typename AUDIOCALLBACK> class Stream : public detail::TimeStampGen
{
  private:
    StreamSetupInfo m_info;
    std::atomic<int> m_runstate;

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
    Stream(StreamSetupInfo &info, AUDIOCALLBACK &&cb) : m_info(info), m_cb(cb)
    {
        openSpecific(info);
    }
    Stream(AUDIOCALLBACK &&cb, std::string_view id = "") : m_cb(cb), m_sid(id)
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
    bool isRunning() const { return m_runstate > 0; }

    StreamSetupInfo actualStreamInfo()
    {
        if (!m_info.stream)
        {
            throw Exception(-1, "actualStreamInfo() may only be called if "
                                "there is a valid and active stream");
        }
        auto stream = m_info.stream;
        auto painfo = Pa_GetStreamInfo(stream);
        if (painfo == nullptr)
        {
            throw Exception(-1, "actualStreamInfo(): Unexpected: unable to "
                                "retrieve stream info");
        }

        StreamSetupInfo ret = m_info;
        ret.inputLatency = painfo->inputLatency;
        ret.outputLatency = painfo->outputLatency;
        ret.samplerate = painfo->sampleRate;
        return ret;
    }

    void openDefault()
    {
        StreamSetupInfo info;

        int err = Pa_OpenDefaultStream(
            &info.stream, info.inputChannelCount, info.outputChannelCount,
            info.sampleFormat, info.samplerate, info.framesPerBuffer,
            callback_dispatcher, (void *)this);

        if (err)
        {
            throw Exception(err, "Failed to openDefaultStream()");
        }
        m_info = info;
        m_info = actualStreamInfo();

        TimeStampGen::reset(info.samplerate);
    }

    void openDefault(StreamSetupInfo &info)
    {

        int err = Pa_OpenDefaultStream(
            &info.stream, info.inputChannelCount, info.outputChannelCount,
            info.sampleFormat, info.samplerate, info.framesPerBuffer,
            callback_dispatcher, (void *)this);

        if (err)
        {
            throw Exception(err, "Failed to openDefaultStream()");
        }
        m_info = info;
        m_info = actualStreamInfo();

        TimeStampGen::reset(info.samplerate);
    }

    void openSpecific(StreamSetupInfo &info)
    {

        if (info.inParams == nullptr && info.outParams == nullptr)
            throw Exception(-1, "openSpecific: Either or both inParams and outParams must be set.");

        if (info.framesPerBuffer <=0 )info.framesPerBuffer = 512;


        const auto err =
            Pa_OpenStream(&info.stream, info.inParams, info.outParams,
                          info.samplerate, info.framesPerBuffer, info.flags,
                          callback_dispatcher, (void *)this);

        if (err)
        {
            throw Exception(err, "Failed to openSpecificStream()");
        }
        m_info = info;
        m_info = actualStreamInfo();

        TimeStampGen::reset(info.samplerate);
    }

    void Stop()
    {
        TimeStampGen::reset(m_info.samplerate);
        if (!m_info.stream)
        {
            throw Exception(-1, "Stop(): unexpected: no stream to start");
        }
        int ret = Pa_StopStream(m_info.stream);
        if (ret && ret != paStreamIsStopped)
        {
            throw Exception(ret, "Error Stopping Stream");
        }
        m_runstate = 0;
    }

    void Start()
    {
        TimeStampGen::reset(m_info.samplerate);
        if (!m_info.stream)
        {
            throw Exception(-1, "Start(): unexpected: no stream to start");
        }
        int ret = Pa_StartStream(m_info.stream);
        if (ret)
        {
            throw Exception(ret, "Error Starting Stream");
        }
        m_runstate = 1;
    }

    void Abort()
    {
        if (m_info.stream)
        {
            Pa_AbortStream(m_info.stream);
            m_info.stream = nullptr;
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
       m_enum.populate();
    }
    ~Portaudio() noexcept
    {
        Pa_Terminate();
        m_instances--;
    }
    const PaVersionInfo info;
    static int instances() noexcept { return m_instances; }
    const enumerator_t &enumerator() const noexcept { return m_enum; }
    std::string_view id() const noexcept { return m_id; }

    template <typename CALLBACK> auto openDefaultStream(CALLBACK &&cb)
    {
        Stream<CALLBACK> s(std::forward<CALLBACK>(cb));
        s.openDefault();
        return s;
    }

    template <typename CALLBACK>
    auto openStream (portaudio::PaDeviceInfoEx& device, CALLBACK&& cb){
        Stream s(std::forward<CALLBACK> (cb));
        if (device.streamSetupInfo.outParams && device.info->maxOutputChannels == 0){
            throw Exception(-1, "openStream: You are trying to *play* to an input-only device.");
        }
        if (device.streamSetupInfo.inParams && device.info->maxInputChannels== 0){
             throw Exception(-1, "openStream: You are trying to *capture* to an output-only device.");
        }
        s.openSpecific(device.streamSetupInfo);
        return s;
    }

    template <typename CALLBACK>
    auto openStream(StreamSetupInfo &info, CALLBACK &&cb)
    {
        Stream<CALLBACK> s(std::forward<CALLBACK>(cb));
        s.openSpecific(info);
        return s;
    }

    auto openAndRunStream(StreamSetupInfo &info, AudioCallback &cb)
    {
        std::atomic<CallbackResult> streamRetVal(CallbackResult::Continue);

        Stream s(info, [&](CallbackInfo info) {
            auto rv = cb.onCallback(info);
            if (rv != CallbackResult::Continue)
            {
                streamRetVal = rv;
            }
            return rv;
        });

        s.Start();
        while (streamRetVal == CallbackResult::Continue)
        {
            sleep_ms(50);
        }
        s.Stop();
    }

  private:
    static inline int m_instances = 0;
    const std::string m_id;
    mutable hostApiList m_apis;
    mutable enumerator_t m_enum;
};

static inline auto streamParamsDefault(const Portaudio &pa,
                                       PaDeviceInfoEx *device = nullptr)
{
    PaStreamParameters params = {0, 0, 0, 0, 0};

    params.channelCount = 2;
    if (device == nullptr)
        params.device = pa.enumerator().defaultDevice().global_device_index;
    else
        params.device = device->global_device_index;

    params.sampleFormat = SampleFormat::Float32;
    return params;
}

static inline float next_sine_sample(uint64_t sample_num, int samplerate,
                                     int freq = 440)
{
    return sin(freq * 2 * M_PI * sample_num / samplerate);
}


static inline void fill_buffer_sine(uint64_t &nsample,
                                    portaudio::CallbackInfo &info,
                                    bool left = true, bool right = true,
                                    int freq = 440)
{
    float* out = (float*)info.output;
    for (uint64_t i = 0; i < info.frameCount; i++)
    {
        auto v = next_sine_sample(nsample++, info.samplerate, freq);
        // clang-format off
            if (left) *out++ = v; else *out++ = 0;
            if (right) *out++ = v;else *out++ = 0;
        // clang-format on
    }
}

} // namespace portaudio
