#pragma once
// portaudioplusplus header file. Written by Steve. Monday January 11th 2021.
// A C++ wrapper for Portaudio.

#include "../../../portaudio/include/portaudio.h"
#include <algorithm> // std::max()
#include <atomic>
#include <cassert>
#define _USE_MATH_DEFINES
#include <array>
#include <cmath>   // std::abs
#include <cstdint> // uint_64
#include <cstring>
#include <iostream>
#include <limits> // numeric_limits
#include <math.h>
#include <memory> // unique_ptr
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace portaudio
{

template <typename Float>
static inline bool is_almost_equal(const Float f1, const Float f2)
{
    return std::abs(f1 - f2) < std::numeric_limits<Float>::epsilon();
}

namespace strings
{
template <typename... Args>
void make_string(std::ostringstream &stream, Args &&... args) noexcept
{
    ((stream << args << ' '), ...);
}
} // namespace strings

namespace dsp
{
#ifdef _MSC_VER
#define PA_FORCE_INLINE __forceinline
#else
#define PA_FORCE_INLINE __attribute__((always_inline))
#endif
static inline float next_sine_sample(unsigned int &sample_num,
                                     unsigned int samplerate,
                                     unsigned int freq = 440)
{
    if (sample_num >= samplerate) sample_num = 0;

    return (float)sin(freq * 2 * M_PI * sample_num++ / samplerate);
}

template <typename T> class fader
{
    T m_destValue;
    float m_secToDest;
    std::atomic<int> m_steps;
    float m_samplerate;
    float m_startValue = 0;
    float m_step = 0;
    float m_vol = 0;
    int m_nch = 2;
    void calc()
    {
        if (m_secToDest <= 0 || m_samplerate <= 0) return;
        const float fsteps = (m_secToDest * m_samplerate) * m_nch;
        m_steps = (int)(fsteps + 0.5f);
        m_step = (m_destValue - m_startValue) / m_steps;
        m_vol = m_startValue;
    }

  public:
    fader() : m_destValue(0), m_secToDest(0), m_steps(0), m_samplerate(0) {}
    fader(T destValue, float secToDest, float samplerate, int nch = 2)
        : m_destValue(destValue), m_secToDest(secToDest), m_steps(0),
          m_samplerate(samplerate), m_nch(nch), m_vol(0)
    {
        calc();
    }

    T volume() const noexcept { return m_vol; }
    void arm(T destval, float samplerate, float secToDest)
    {
        m_startValue = m_vol;
        m_destValue = destval;
        m_samplerate = samplerate;
        m_secToDest = secToDest;
        calc();
    }

    PA_FORCE_INLINE void processSample(T &sample)
    {
        if (m_steps > 0)
        {
            sample *= m_vol;
            m_vol += m_step;
            if (m_vol < 0) m_vol = 0;
            --m_steps;
            if (m_steps <= 0)
            {
                m_vol = this->m_destValue;
                return;
            }
        }
    }

    void processSamples(int nFrames, T *samples, const int nch)
    {
        while (nFrames > 0)
        {
            if (active())
            {
                for (auto ch = 0; ch < nch; ++ch)
                {
                    T &val = *samples++;
                    processSample(val);
                }
                nFrames--;
            }
            else
            {
                if (!is_almost_equal(1.0f, m_destValue))
                {
                    for (auto ch = 0; ch < nch; ++ch)
                    {
                        *samples = *samples * m_vol;
                        ++samples;
                    }
                    nFrames--;
                }
                else
                {
                    return; // nothing to do here: just multiply by one.
                }
            }
        };
    }

    bool active() const noexcept { return m_steps > 0; }
};

template <typename T> struct Atomic
{
    std::atomic<T> atomic;

    Atomic() : atomic(T()) {}

    explicit Atomic(T const &v) : atomic(v) {}
    explicit Atomic(std::atomic<T> const &a) : atomic(a.load()) {}

    Atomic(Atomic const &other) : atomic(other.atomic.load()) {}

    Atomic &operator=(Atomic const &other)
    {
        atomic.store(other.atomic.load());
        return *this;
    }
    Atomic &operator=(const T value)
    {
        atomic = value;
        return *this;
    }

    operator T() { return atomic; }
};

typedef Atomic<double> AtomicDouble;

template <int Channels = 2, typename Value = float> class EnvelopeFollower
{
    std::array<AtomicDouble, 2> m_env;

  public:
    EnvelopeFollower()
    {
        assert(m_env[0].atomic.is_lock_free());

        for (int i = 0; i < Channels; i++)
            m_env[i] = 0;
    }

    Value operator[](unsigned int channel) const { return m_env[channel]; }

    void Setup(int sampleRate, double attackMs, double releaseMs)
    {
        m_a = pow(0.01, 1.0 / (attackMs * sampleRate * 0.001));
        m_r = pow(0.01, 1.0 / (releaseMs * sampleRate * 0.001));
    }

    void Process(size_t samples, const Value **src)
    {

        for (int i = 0; i < Channels; i++)
        {
            const Value *cur = src[i];

            double e = m_env[i];
            for (int n = samples; n; n--)
            {
                double v = std::abs(*cur++);
                if (v > e)
                    e = m_a * (e - v) + v;
                else
                    e = m_r * (e - v) + v;
            }
            m_env[i] = e;
        }
    }

  protected:
    double m_a = 0;
    double m_r = 0;
};
} // namespace dsp

[[maybe_unused]] static inline void sleep_ms(const long ms) noexcept
{
    Pa_Sleep(ms);
}
[[maybe_unused]] static inline void SleepmS(const long ms) noexcept
{
    Pa_Sleep(ms);
}

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
        if (errCode == -1)
        {
            m_stream << "Generic error\n";
        }
        else
        {
            std::string pe = error_string(errCode);
            m_stream << "PortAudio error code: " << errCode << " (" << pe
                     << ")\n";
        }

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
    unsigned int samplerate = 44100;
    unsigned long framesPerBuffer = 512;
    void *userData = {nullptr};
    PaStreamParameters inParams = {paNoDevice, -1, 0, -1, nullptr};
    PaStreamParameters outParams = {paNoDevice, -1, 0, -1, nullptr};
    PaStreamFlags flags = {0};
    PaTime inputLatency = {0};
    PaTime outputLatency = {0};
};
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

struct CallbackUserData : protected AudioCallback
{
    void *UserData = nullptr;
    CallbackResult callback(CallbackInfo cbInfo) noexcept
    {
        return AudioCallback::onCallback(cbInfo);
    }
};

struct DeviceType
{

    static inline DeviceType DeviceTypeFromInfo(const PaDeviceInfo *info)
    {
        DeviceType ret;
        if (info->maxInputChannels > 0)
        {
            ret.m_value |= (unsigned int)DeviceType::types::input;
        }
        if (info->maxOutputChannels > 0)
        {
            ret.m_value |= (unsigned int)DeviceType::types::output;
        }
        if (ret.m_value == (unsigned int)types::duplex)
        {
            assert(info->maxInputChannels > 0 && info->maxOutputChannels > 0);
        }
        return ret;
    }
    enum class types : unsigned int
    {
        none = 0,
        input = 1,
        output = 2,
        duplex = input | output
    };
    DeviceType() { m_value = (unsigned int)types::none; }
    DeviceType(types t) { m_value = (unsigned int)t; }
    types value() const noexcept { return (types)m_value; }

    bool is_input_only() const noexcept { return value() == types::input; }
    bool is_output_only() const noexcept { return value() == types::output; }
    bool is_duplex() const noexcept { return value() == types::duplex; }
    bool is_valid() const noexcept { return value() == types::none; }
    void set(types type) { m_value = (unsigned int)type; }

  private:
    unsigned int m_value = (unsigned int)types::none;
};

static inline int constexpr INVALID_PA_DEVICE_INDEX = -1;
struct PaHostApiInfoEx;

struct PaDeviceInfoEx
{
    const PaDeviceInfo *info = nullptr;
    int global_device_index = INVALID_PA_DEVICE_INDEX;
    StreamSetupInfo streamSetupInfo = {0};
    const PaHostApiInfo *hostApiInfo = {nullptr};
    int api_device_index = INVALID_PA_DEVICE_INDEX;
    int input_api_device_index = INVALID_PA_DEVICE_INDEX;
    int output_api_device_index = INVALID_PA_DEVICE_INDEX;
    int duplex_api_device_index = INVALID_PA_DEVICE_INDEX;
    DeviceType deviceType;
    void deviceTypeSet(DeviceType::types type) { deviceType.set(type); }
};

using deviceList = std::vector<PaDeviceInfoEx>;
using inputDeviceList = deviceList;
using outputDeviceList = deviceList;
using apiDeviceList = deviceList;

struct enumerator_t;
struct PaHostApiInfoEx
{
    friend struct enumerator_t;
    const PaHostApiInfo info = {};
    int api_index = -1;
    int defaultInputDeviceGlobalApiIndex = INVALID_PA_DEVICE_INDEX;
    int defaultOutputDeviceGlobalApiIndex = INVALID_PA_DEVICE_INDEX;
    const deviceList &inputDevices() const noexcept { return m_inputDevices; }
    const deviceList &outputDevices() const noexcept { return m_outputDevices; }
    const deviceList &allDevices() const noexcept { return m_allDevices; }
    const deviceList &duplexDevices() const noexcept { return m_duplexDevices; }
    const PaDeviceInfoEx *defaultInputDevice() const noexcept
    {
        return &m_DefaultInputDevice;
    }
    const PaDeviceInfoEx *defaultOutputDevice() const noexcept
    {
        return &m_DefaultOutputDevice;
    }
    const PaDeviceInfoEx *defaultDuplexDevice() const noexcept
    {
        if (m_duplexDevices.empty()) return nullptr;
        return &m_duplexDevices.at(0);
    }
    PaHostApiInfoEx(const PaHostApiInfo *info, int api_index)
        : info(*info), api_index(api_index)
    {
        defaultInputDeviceGlobalApiIndex = info->defaultInputDevice;
        defaultOutputDeviceGlobalApiIndex = info->defaultOutputDevice;
    }

  private:
    deviceList m_inputDevices;
    deviceList m_outputDevices;
    deviceList m_allDevices;
    deviceList m_duplexDevices;
    PaDeviceInfoEx m_DefaultInputDevice;
    PaDeviceInfoEx m_DefaultOutputDevice;
};

using hostApiList = std::vector<PaHostApiInfoEx>;
namespace detail
{

template <typename T> class no_copy
{
  protected:
    no_copy() = default;
    no_copy &operator=(const no_copy &) = delete;
    no_copy(const no_copy &) = delete;
};

static inline hostApiList enum_apis()
{
    hostApiList list;
    const int cnt = Pa_GetHostApiCount();
    for (int i = 0; i < cnt; ++i)
    {
        auto inf = Pa_GetHostApiInfo(i);
        PaHostApiInfoEx d(inf, i);
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
        DeviceType dtype = DeviceType::DeviceTypeFromInfo(inf);
        auto api_inf = Pa_GetHostApiInfo(inf->hostApi);
        PaDeviceInfoEx d{inf, i, {0}, api_inf, inf->hostApi, -1, -1, -1, dtype};
        assert(d.hostApiInfo != nullptr);
        list.push_back(d);
        const auto &dback = list.at(list.size() - 1);
        assert(dback.hostApiInfo != nullptr);
    }
    return list;
}

} // namespace detail

struct enumerator_t : detail::no_copy<enumerator_t>
{
    void populate() { m_apiDeviceList = devicesByApi(true); }
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
        if (!force_refresh && !apis().empty() && !this->devices().empty() &&
            !this->m_apiDeviceList.empty())
        {
            return m_apiDeviceList;
        }
        this->apis(force_refresh);
        this->devices(force_refresh);
        apiDeviceList retval;

        for (auto &api : m_apis)
        {
            auto &api_indevs = api.m_inputDevices;
            auto &api_outdevs = api.m_outputDevices;
            auto &api_alldevs = api.m_allDevices;
            auto &api_duplexdevs = api.m_duplexDevices;
            api_indevs.clear();
            api_outdevs.clear();
            api_alldevs.clear();
            api_duplexdevs.clear();

            for (int i = 0; i < api.info.deviceCount; ++i)
            {

                const auto global_device_index =
                    Pa_HostApiDeviceIndexToDeviceIndex(api.api_index, i);
                auto &dev_ref = m_devices.at(global_device_index); // NOT a copy
                assert(dev_ref.hostApiInfo != nullptr);
                auto dev = m_devices.at(global_device_index); // it *is* a copy.
                assert(dev_ref.hostApiInfo != nullptr);
                assert(dev.global_device_index == global_device_index);
                dev.api_device_index = i;

                api_alldevs.push_back(dev);
                if (dev.deviceType.is_input_only())
                {
                    dev.input_api_device_index = api_indevs.size();
                    dev_ref.input_api_device_index = dev.input_api_device_index;
                    api_indevs.push_back(dev);
                }
                else if (dev.deviceType.is_output_only())
                {
                    dev.output_api_device_index = api_outdevs.size();
                    dev_ref.output_api_device_index =
                        dev.output_api_device_index;
                    api_outdevs.push_back(dev);
                }
                else if (dev.deviceType.is_duplex())
                {
                    dev.input_api_device_index = api_indevs.size();
                    dev.output_api_device_index = api_outdevs.size();
                    dev.duplex_api_device_index = api_duplexdevs.size();
                    dev_ref.output_api_device_index =
                        dev.output_api_device_index;
                    dev_ref.input_api_device_index = dev.input_api_device_index;
                    dev_ref.duplex_api_device_index =
                        dev.duplex_api_device_index;
                    api_indevs.push_back(dev);
                    api_outdevs.push_back(dev);
                    api_duplexdevs.push_back(dev);
                }
                auto definput = findDevice(
                    this->m_devices, api.defaultInputDeviceGlobalApiIndex);
                auto defoutput = findDevice(
                    m_devices, api.defaultOutputDeviceGlobalApiIndex);
                assert(definput && defoutput);
                api.m_DefaultInputDevice = *definput;
                api.m_DefaultOutputDevice = *defoutput;
            };
        };

        for (const auto &a : m_apis)
        {
            assert(a.defaultInputDevice()->api_device_index !=
                   INVALID_PA_DEVICE_INDEX);
            assert(a.defaultOutputDevice()->api_device_index !=
                   INVALID_PA_DEVICE_INDEX);
            assert(a.defaultInputDevice()->global_device_index !=
                   INVALID_PA_DEVICE_INDEX);
            assert(a.defaultOutputDevice()->global_device_index !=
                   INVALID_PA_DEVICE_INDEX);
        }
        return retval;
    }

    // returns an *instance* (a copy) of an input device where you might want to
    // adjust its properties perhaps before calling openStream() etc on it.
    const PaDeviceInfoEx defaultInputDevice() const noexcept
    {

        const auto idx = Pa_GetDefaultInputDevice();
        const auto &devs = devices();
        assert((size_t)idx < devs.size());
        return m_devices.at(idx);
    }
    // returns an *instance* (a copy) of an output device where you might want
    // to adjust its properties perhaps before calling openStream() etc on it.
    const PaDeviceInfoEx defaultDevice() const noexcept
    {

        const auto idx = Pa_GetDefaultOutputDevice();
        const auto &devs = devices();
        assert((size_t)idx < devs.size());
        return m_devices.at(idx);
    }
    const PaHostApiInfoEx &defaultHostApi() const noexcept
    {
        const auto def_index = Pa_GetDefaultHostApi();
        assert((size_t)def_index < m_apis.size());
        return m_apis.at(def_index);
    }

    const PaHostApiInfoEx *
    findApi(const std::string_view api_name) const noexcept
    {
        for (const auto &api : m_apis)
        {
            std::string_view name{api.info.name};
            if (api_name == name)
            {
                return &api;
            }
        }
        return nullptr;
    }

    const PaHostApiInfoEx *findApi(unsigned int api_index) const noexcept
    {
        assert(api_index < m_apis.size());
        if (api_index >= m_apis.size()) return nullptr;
        return &m_apis.at(api_index);
    }

    const PaDeviceInfoEx *findDevice(const deviceList &list,
                                     unsigned int deviceIndex) const noexcept
    {
        assert(deviceIndex < list.size());
        if (deviceIndex < list.size()) return &list.at(deviceIndex);
        return nullptr;
    }

    const PaDeviceInfoEx *
    findDeviceByGlobalIndex(unsigned int globalDeviceIndex) const noexcept
    {
        return findDevice(m_devices, globalDeviceIndex);
    }

    const PaDeviceInfoEx *findDevice(
        const int apiIndex, unsigned int deviceIndex,
        const DeviceType::types type = DeviceType::types::output) const noexcept
    {
        auto api = findApi(apiIndex);
        assert(api);
        if (!api) return nullptr;
        if (type == DeviceType::types::output)
        {
            if (deviceIndex >= api->outputDevices().size())
            {
                assert("Are you sending me the correct index? For an output "
                       "device, for example, send the OUTPUT DEVICE api index"
                       " (not the global device index -- just the OUTPUT "
                       "device index." == nullptr);
                return nullptr;
            }
            return &api->outputDevices().at(deviceIndex);
        }
        else if (type == DeviceType::types::input)
        {
            if (deviceIndex >= api->outputDevices().size())
            {
                assert("Are you sending me the correct index? For an input "
                       "device, for example, send the INPUT DEVICE api index"
                       " (not the global device index -- just the INPUT device "
                       "index." == nullptr);
                return nullptr;
            }
            return &api->inputDevices().at(deviceIndex);
        }
        else
        {
            assert(type == DeviceType::types::duplex &&
                   "findDevice: You should either want an input device, an "
                   "output device, or a duplex device. Which is it?" !=
                       nullptr);
            return &api->duplexDevices().at(deviceIndex);
        }
    }

    const PaDeviceInfoEx *findDevice(
        const PaHostApiInfoEx &, unsigned int,
        const DeviceType::types = DeviceType::types::output) const noexcept
    {
        assert(0);
        return nullptr;
    }

    const PaDeviceInfoEx *
    findDevice(const deviceList &devices,
               std::string_view &deviceName) const noexcept
    {
        for (const auto &d : devices)
        {
            std::string_view name{d.info->name};
            if (name == deviceName) return &d;
        }
        return nullptr;
    }

    const PaDeviceInfoEx *findDevice(const PaHostApiInfoEx &,
                                     std::string_view) const noexcept
    {
        return nullptr;
    }

  private:
    mutable hostApiList m_apis;
    mutable deviceList m_devices;
    mutable deviceList m_input_devices;
    mutable deviceList m_output_devices;
    mutable apiDeviceList m_apiDeviceList;
    deviceList &devices_non_const() noexcept { return m_devices; }
};

[[maybe_unused]] static inline StreamSetupInfo
makeStreamSetupInfo(const PaDeviceInfoEx &devInfo, PaStreamParameters *inParams,
                    PaStreamParameters *outParams,
                    PaSampleFormat fmt = SampleFormat::Float32,
                    int samplerate = -1)
{
    StreamSetupInfo s;
    if (devInfo.deviceType.is_input_only() && outParams)
    {
        throw Exception(
            -1, "Error: you must not set 'inParams' on an output device!");
    }
    if (devInfo.deviceType.is_output_only() && inParams)
    {
        throw Exception(
            -1, "Error: you must not set 'outParams' on an input device!");
    }
    if (samplerate == -1)
        s.samplerate = (unsigned int)devInfo.info->defaultSampleRate;
    else
        s.samplerate = samplerate;
    s.sampleFormat = fmt;
    if (inParams) s.inParams = *inParams;
    if (outParams) s.outParams = *outParams;

    s.outputLatency = devInfo.info->defaultLowOutputLatency;
    s.inputLatency = devInfo.info->defaultLowInputLatency;
    if (inParams)
    {
        s.inputChannelCount = inParams->channelCount;
    }
    else
    {
        s.inputChannelCount = 0;
    }
    if (outParams)
    {
        s.outputChannelCount = outParams->channelCount;
    }
    else
    {
        s.outputChannelCount = 0;
    }
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
    uint64_t elapsedMillis() const noexcept { return m_CurrentMillis; }
    uint64_t nframes() const noexcept { return m_nframes; }
    double elapsedSeconds() const noexcept
    {
        auto ret = (double)elapsedMillis()/ 1000.0;
        return ret;
    }

  protected:
    void reset(const unsigned int newSamplerate)
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
        const uint64_t samples_per_ms = m_Samplerate / 1000;
        m_CurrentMillis = frames_now / samples_per_ms;
        return elapsedSeconds();
    }

  private:
    std::atomic<uint64_t> m_CurrentMillis = {0};
    std::atomic<unsigned int> m_Samplerate = {0};
    std::atomic<uint64_t> m_nframes = {0};
}; // TimeStampGen

struct StreamBase
{
    static inline unsigned int StreamsActive() { return m_StreamsActive; }
    StreamBase() { m_StreamsActive++; }
    virtual ~StreamBase() { m_StreamsActive--; }

  protected:
    static inline std::atomic<unsigned int> m_StreamsActive;
};

} // namespace detail

template <typename AUDIOCALLBACK, typename SAMPLE = float, size_t NCH = 2>
class Stream : public detail::TimeStampGen, public detail::StreamBase
{
  private:
    dsp::EnvelopeFollower<NCH, SAMPLE> m_env;
    StreamSetupInfo m_info;
    std::atomic<int> m_runstate{0};
    void setRunState(int newState) { m_runstate = newState; }

    static inline int
    callback_dispatcher(const void *input, void *output,
                        unsigned long frameCount,
                        const PaStreamCallbackTimeInfo *timeInfo,
                        PaStreamCallbackFlags statusFlags, void *userData)
    {
        Stream *p = (Stream *)userData;
        assert(p && "stream context not set. FATAL");
        const SAMPLE *samples = (const SAMPLE *)input;
        if (!samples) samples = (SAMPLE *)output;
        p->m_env.Process(frameCount, &samples);

        const auto elapsed_time = p->generateTimeStamps(frameCount);
        const auto ret =
            p->m_cb({elapsed_time, input, output, frameCount, timeInfo,
                     statusFlags, userData, p->samplerate()});
        if (p->m_fader.active())
        {
            p->m_fader.processSamples(frameCount, (float *)output,
                                      p->m_info.outputChannelCount);
        }
        if (ret != CallbackResult::Continue)
        {
            p->setRunState(
                0); // we don't call StopStream() here due to possible deadlock,
            // but we set the runstate so anyone waiting on isRunning() knows we
            // have stopped.
        }
        return (int)ret;
    }
    dsp::fader<float> m_fader;

    // yes, this is meant to be private. I just use it for delegation
    // so the object is fully constructed even if we are calling back from a
    // public constructor.
    Stream(AUDIOCALLBACK &&cb, StreamSetupInfo info, std::string_view id = "")
        : m_info(info), m_cb(cb), m_sid(id)
    {
    }

  public:
    Stream(StreamSetupInfo &info, AUDIOCALLBACK &&cb)
        : Stream(std::forward<AUDIOCALLBACK>(cb), info)
    {
        openSpecific(info);
    }

    Stream(PaDeviceInfoEx &dev, AUDIOCALLBACK &&cb)
        : Stream(std::forward<AUDIOCALLBACK>(cb), dev.streamSetupInfo)
    {
        openSpecific(dev.streamSetupInfo);
    }
    virtual ~Stream() { Close(); }
    Stream(const Stream &rhs) = delete;
    Stream &operator=(const Stream &rhs) = delete;

    Stream(Stream &&rhs) : m_cb(std::move(rhs.m_cb))
    {
        memcpy(&m_info, &rhs.m_info, sizeof(m_info));
        m_sid = std::move(rhs.m_sid);
        rhs.Close();
        openSpecific(m_info);
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
        ret.samplerate = (unsigned int)painfo->sampleRate;

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

    void openSpecific(StreamSetupInfo &info)
    {

        if (info.inParams.device == paNoDevice &&
            info.outParams.device == paNoDevice)
            throw Exception(-1, "openSpecific: Either or both inParams and "
                                "outParams must be set.");

        if (info.framesPerBuffer == 0) info.framesPerBuffer = 512;
        std::string devname;
        // we refer right back to PortAudio here so that any diagnostic
        // output will show us which device he's *really* trying to open.
        auto in_def = Pa_GetDefaultInputDevice();
        std::string defInName = Pa_GetDeviceInfo(in_def)->name;
        auto out_def = Pa_GetDefaultOutputDevice();
        std::string defOutName = Pa_GetDeviceInfo(out_def)->name;

        std::cout << "Default input device is: " << defInName << std::endl;
        std::cout << "Default output device is: " << defOutName << std::endl;
        auto myInParams =
            info.inParams.device == paNoDevice ? nullptr : &info.inParams;
        auto myOutParams =
            info.outParams.device == paNoDevice ? nullptr : &info.outParams;
        if (info.inParams.device != paNoDevice)
        {
            devname = Pa_GetDeviceInfo(info.inParams.device)->name;
        }
        if (devname.empty() && info.outParams.device != paNoDevice)
        {
            devname = Pa_GetDeviceInfo(info.outParams.device)->name;
        }
        if (devname.empty())
        {
            devname = "[unexpected: no device]";
        }

        const auto err =
            Pa_OpenStream(&info.stream, myInParams, myOutParams,
                          info.samplerate, info.framesPerBuffer, info.flags,
                          callback_dispatcher, (void *)this);

        if (err)
        {
            throw Exception(
                err, "Failed to openSpecificStream(), for device: ", devname);
        }
        m_info = info;
        m_info = actualStreamInfo();
        info = m_info; // so caller knows what he actually got
        m_env.Setup(m_info.samplerate, 20, 500);
        TimeStampGen::reset(info.samplerate);
    }

    void Stop(float fadeOutSecs = 0.25)
    {
        TimeStampGen::reset(m_info.samplerate);
        if (!m_info.stream)
        {
            throw Exception(-1, "Stop(): unexpected: no stream to start");
        }

        if (m_runstate)
        {
            m_fader.arm(0, (float)this->samplerate(), fadeOutSecs);

            int slept = 0;
            while (m_fader.active())
            {
                Pa_Sleep(10);
                slept += 10;
                if (slept >= 5000)
                {
                    break;
                }
            };
        }

        int ret = Pa_StopStream(m_info.stream);
        if (ret && ret != paStreamIsStopped)
        {
            throw Exception(ret, "Error Stopping Stream");
        }

        m_runstate = 0;
    }

    void Start(float fadeInSecs = 0.1)
    {
        TimeStampGen::reset(m_info.samplerate);

        if (!m_info.stream)
        {
            throw Exception(-1, "Start(): unexpected: no stream to start");
        }

        m_fader.arm(1.0f, (float)this->samplerate(), fadeInSecs);
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
            try
            {
                Stop(0.5);
            }
            catch (...)
            {
            }
            Pa_CloseStream(m_info.stream);
            m_info.stream = nullptr;
        }
    }

    SAMPLE envelope(unsigned int channel) const { return this->m_env[channel]; }

    std::string_view id() const noexcept { return m_sid; }
    void id(std::string_view newId) { m_sid = newId; }

  private:
    AUDIOCALLBACK m_cb;
    std::string m_sid;

}; // namespace portaudio
namespace detail{
[[maybe_unused]]
static inline void deviceSanityForOpenStream(PaDeviceInfoEx& device){

    if (device.streamSetupInfo.outParams.device == paNoDevice &&
        device.deviceType.is_output_only())
    {
        throw Exception(-1, "Output device: invalid argument (no internal device set for PortAudio)");
    }
    if (device.streamSetupInfo.inParams.device == paNoDevice &&
        device.deviceType.is_input_only())
    {
        throw Exception(-1, "Input device: invalid argument (no internal device set for PortAudio)");
    }

    if (device.deviceType.is_duplex()){
        if (device.streamSetupInfo.inParams.device == paNoDevice){
            throw Exception(-1, "Duplex device requires both an input and output device to be set!. There is no device in 'inParams'");
        }
        if (device.streamSetupInfo.outParams.device == paNoDevice){
            throw Exception(-1, "Duplex device requires both an input and output device to be set!. There is no device in 'outParams'");
        }
    }
}
}
class Portaudio
{
  public:
    static inline int StreamsActive()
    {
        return detail::StreamBase::StreamsActive();
    }
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
        return s;
    }

    template <typename CALLBACK>
    auto openStream(portaudio::PaDeviceInfoEx &device, CALLBACK &&cb)
    {
        detail::deviceSanityForOpenStream(device);
        Stream s(device, std::forward<CALLBACK>(cb));
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

[[maybe_unused]] static inline auto
makeStreamParams(const Portaudio &pa, PaDeviceInfoEx *device = nullptr) noexcept
{
    PaStreamParameters params = {};

    params.channelCount = 2;
    if (device == nullptr)
        params.device = pa.enumerator().defaultDevice().global_device_index;
    else
        params.device = device->global_device_index;

    params.sampleFormat = SampleFormat::Float32;
    if (device)
    {
        if (device->deviceType.is_input_only())
        {
            params.suggestedLatency = device->info->defaultLowInputLatency;
            params.channelCount = (std::min)(device->info->maxInputChannels, 2);
        }
        else if (device->deviceType.is_output_only())
        {
            params.suggestedLatency = device->info->defaultLowOutputLatency;
            params.channelCount =
                (std::min)(device->info->maxOutputChannels, 2);
        }
        else
        {
            params.suggestedLatency =
                (std::max)(device->info->defaultLowInputLatency,
                           device->info->defaultLowOutputLatency);
            params.channelCount = (std::min)(device->info->maxOutputChannels,
                                             device->info->maxInputChannels);
            if (params.channelCount > 2) params.channelCount = 2;
        }
    }

    params.hostApiSpecificStreamInfo = nullptr;
    return params;
}

namespace dsp
{

[[maybe_unused]] static inline void
fill_buffer_sine(unsigned int &nsample, portaudio::CallbackInfo &info,
                 const unsigned int nch, int freq = 440)
{
    float *out = (float *)info.output;
    for (unsigned int i = 0; i < info.frameCount; i++)
    {
        auto v = next_sine_sample(nsample, info.samplerate, freq);
        for (unsigned int ch = 0; ch < nch; ++ch)
        {
            *out++ = v;
        }
    }
}
} // namespace dsp

} // namespace portaudio
