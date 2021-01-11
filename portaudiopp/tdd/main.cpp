#include <math.h>
#include <portaudio.h>
#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace strings {
template <typename... Args>
void make_string(std::ostringstream& stream, Args&&... args) noexcept {
  ((stream << args << ' '), ...);
}
}  // namespace strings
namespace portaudio {

static inline void sleep_ms(const long ms) noexcept {
  Pa_Sleep(ms);
}
class Exception : public std::runtime_error {
  std::ostringstream m_stream;

 public:
  template <typename... Args>
  auto error_string(const PaError errCode, Args&&... args) noexcept {
    m_stream.str().clear();
    m_stream << "PortAudio error code: " << errCode << "\n"
             << error_string(errCode) << "\n";
    strings::make_string(m_stream, std::forward<Args...>(args...));
    return std::string(m_stream.str());
  }
  static inline auto error_string(PaError errCode) noexcept {
    return Pa_GetErrorText(errCode);
  }
  Exception(const PaError errcode) noexcept
      : std::runtime_error(error_string(errcode)) {}

  template <typename... ARGS>
  Exception(const PaError errcode, ARGS&&... args) noexcept
      : std::runtime_error(
            error_string(errcode, std::forward<ARGS...>(args...))) {}
};

using hostApiList = std::vector<const PaHostApiInfo*>;
using deviceList = std::vector<const PaDeviceInfo*>;

using apiDeviceList = std::unordered_map<std::string, deviceList>;
struct CallbackInfo {
  CallbackInfo(const void* input,
               const void* output,
               unsigned long frameCount,
               const PaStreamCallbackTimeInfo* timeInfo,
               const PaStreamCallbackFlags flags,
               void* userdata)
      : input(input),
        output(output),
        frameCount(frameCount),
        timeInfo(timeInfo),
        statusFlags(flags),
        userdata(userdata)

  {}
  const void* input;
  const void* output;
  unsigned long frameCount;
  const PaStreamCallbackTimeInfo* timeInfo;
  PaStreamCallbackFlags statusFlags;
  void* userdata;
};

enum class CallbackResult : unsigned int {
  Continue = 0, /**< Signal that the stream should continue invoking the
                   callback and processing audio. */
  Complete = 1, /**< Signal that the stream should stop invoking the callback
                   and finish once all output samples have played. */
  Abort = 2 /**< Signal that the stream should stop invoking the callback and
               finish as soon as possible. */
};

// Inherit from me to grab the audio callback.
struct AudioCallback {
  virtual CallbackResult onCallback(CallbackInfo cbInfo) noexcept = 0;
  virtual ~AudioCallback() noexcept {};
};

namespace detail {

static inline hostApiList enum_apis() {
  hostApiList list;
  const int cnt = Pa_GetHostApiCount();
  for (int i = 0; i < cnt; ++i) {
    auto inf = Pa_GetHostApiInfo(i);
    list.push_back(inf);
  }
  return list;
}
static inline deviceList enum_devices() {
  deviceList list;
  for (int i = 0; i < Pa_GetDeviceCount(); ++i) {
    auto inf = Pa_GetDeviceInfo(i);
    list.push_back(inf);
  }
  return list;
}

struct CallbackUserData : protected AudioCallback {
  void* UserData = nullptr;
  CallbackResult callback(CallbackInfo cbInfo) noexcept {
    return AudioCallback::onCallback(cbInfo);
  }
};

}  // namespace detail

struct enumerator_t {
  const hostApiList& apis(bool force_refresh = false) const {
    if (m_apis.empty() || force_refresh) {
      m_apis = detail::enum_apis();
    }
    return m_apis;
  }

  const deviceList& devices(bool force_refresh = false) const {
    if (m_devices.empty() || force_refresh) {
      m_devices = detail::enum_devices();
    }
    return m_devices;
  }

  const apiDeviceList devicesByApi(bool force_refresh = false) const {
    const hostApiList& apis = this->apis(force_refresh);
    const deviceList devices = this->devices(force_refresh);

    apiDeviceList retval;

    for (const auto api : apis) {
      auto it = retval.find(api->name);
      if (it == retval.end()) {
        auto pr = retval.insert({std::string(api->name), deviceList{}});
        it = pr.first;
      }
    }

    for (const auto& d : devices) {
      const auto api = Pa_GetHostApiInfo(d->hostApi);
      auto pr = retval.find(api->name);
      pr->second.push_back(d);
    }

    return retval;
  }

  const PaDeviceInfo* default_device() const noexcept {
    const auto idx = Pa_GetDefaultOutputDevice();
    return Pa_GetDeviceInfo(idx);
  }

 private:
  mutable hostApiList m_apis;
  mutable deviceList m_devices;
  mutable apiDeviceList m_apiDeviceList;
};

struct SampleFormat {
  static inline PaSampleFormat Float32 = paFloat32;
  static inline PaSampleFormat Int32 = paInt32;
  static inline PaSampleFormat Int24 = paInt24;
  static inline PaSampleFormat Int16 = paInt16;
  static inline PaSampleFormat Int8 = paInt8;
  static inline PaSampleFormat UInt8 = paUInt8;
  static inline PaSampleFormat CustomFormat = paCustomFormat;
  static inline PaSampleFormat NonInterleaved = paNonInterleaved;
};
struct Portaudio {
  Portaudio(const std::string_view id = "")
      : info(*Pa_GetVersionInfo()), m_id(id) {
    PaError err = Pa_Initialize();
    if (err != PaErrorCode::paNoError) {
      throw std::runtime_error(Pa_GetErrorText(err));
    }
    m_instances++;
  }
  ~Portaudio() noexcept {
    Pa_Terminate();
    m_instances--;
  }
  const PaVersionInfo info = {0};
  static int instances() noexcept { return m_instances; }
  const enumerator_t& enumerator() const noexcept { return m_enum; }
  std::string_view id() const noexcept { return m_id; }
  AudioCallback* m_callback = {nullptr};

  static inline auto callback_dispatcher =
      [](const void* input,
         void* output,
         unsigned long frameCount,
         const PaStreamCallbackTimeInfo* timeInfo,
         PaStreamCallbackFlags statusFlags,
         void* userData) {
        Portaudio* p = (Portaudio*)userData;
        assert(p && "portaudio context not set. FATAL");
        auto* callback = p->m_callback;
        assert(callback && "user callback not set. FATAL");
        return (int)p->m_callback->onCallback(
            {input, output, frameCount, timeInfo, statusFlags, userData});
      };

  PaStream* play(AudioCallback* callback) {
    m_callback = callback;
    PaStream* stream = nullptr;
    auto ret = Pa_OpenDefaultStream(&stream, 2, 2, paFloat32, 44100, 512,
                                    callback_dispatcher, (void*)this);
    if (ret) {
      throw Exception(ret, "Error whilst play()");
    }

    ret = Pa_StartStream(stream);
    if (ret) {
      throw Exception(ret, "Error starting portaudio stream");
    }

    return stream;
  }

 private:
  static inline int m_instances = 0;
  const std::string m_id;
  mutable hostApiList m_apis;
  mutable enumerator_t m_enum;
};

}  // namespace portaudio

void test_setup_teardown() {
  namespace pa = portaudio;
  {
    pa::Portaudio mypa;
    puts(mypa.info.versionText);
    assert(strlen(mypa.info.versionText) > 0);
    assert(pa::Portaudio::instances() == 1);
  }  // namespace pa=portaudio;

  assert(pa::Portaudio::instances() == 0);
}

void print_apis(const portaudio::hostApiList& list) {
  puts("\napis:");
  for (const auto& api : list) {
    puts(api->name);
  }
  puts("\n");
}

void print_devices(const portaudio::deviceList& list) {
  puts("devices:");
  for (const auto& d : list) {
    puts(d->name);
  }
  puts("\n");
}

void print_devices_by_api(const portaudio::apiDeviceList& list) {
  puts("Devices by api:");
  for (const auto& pr : list) {
    printf("Api name: %s: \n", pr.first.c_str());
    puts("Devices:");
    if (pr.second.empty()) {
      puts("None");
    }
    for (const auto& d : pr.second) {
      puts(d->name);
    }
    puts("\n");
  }
  puts("Devices by api complete.");
}

void test_enum() {
  namespace pa = portaudio::detail;
  auto apis = pa::enum_apis();
  assert(!apis.empty());
  auto devs = pa::enum_devices();
  assert(!devs.empty());

  print_apis(apis);
  print_devices(devs);
}

void test_enumerator() {
  {
    portaudio::Portaudio audio("test");  // better have one of these else
                                         // portaudio won't be initialized!
    test_enum();
    assert(!audio.enumerator().apis().empty());
    assert(!audio.enumerator().devices().empty());
    print_devices_by_api(audio.enumerator().devicesByApi());
  }
}

void test_simple_play() {
  portaudio::Portaudio audio("test simple playback");
  assert(audio.id().size() > 0);
  auto dev = audio.enumerator().default_device();
  assert(dev);
  printf("Default device name is: %s\n", dev->name);

  struct mysine_t : public portaudio::AudioCallback {
    unsigned long n = 0;
    virtual portaudio::CallbackResult onCallback(
        portaudio::CallbackInfo info) noexcept {
      float* out = (float*)info.output;
      // sin(n * FREQ * 2 * PI / SAMPLE_RATE)
      for (unsigned long i = 0; i < info.frameCount; i++, n++) {
        float v = sin(440 * 2 * M_PI * n / 44100);
        *out++ = v;
        *out++ = v;
      }
      return portaudio::CallbackResult::Continue;
    }
  };

  mysine_t mycb;
  auto stream = audio.play(&mycb);
  assert(stream);

  puts("Playing portaudio stream for 2 seconds ...");
  int ctr = 0;
  while (ctr++ < 20000) {
    auto t = Pa_GetStreamTime(stream);
    std::cout << "Stream Elapsed: " << t << "\r";
    std::cout.flush();
    portaudio::sleep_ms(1000);
  }
}

int main(int, char**) {
  namespace pa = portaudio;
  test_setup_teardown();
  test_enumerator();
  test_simple_play();
}
