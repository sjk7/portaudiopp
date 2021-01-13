#include "portaudioplusplus.h"

void test_setup_teardown()
{
    namespace pa = portaudio;

    {

        pa::Portaudio mypa;
        puts(mypa.info.versionText);
        assert(strlen(mypa.info.versionText) > 0);
        assert(pa::Portaudio::instances() == 1);
    } // namespace pa=portaudio;

    assert(pa::Portaudio::instances() == 0);
}

void print_apis(const portaudio::hostApiList &list)
{
    puts("\napis:");
    for (const auto &api : list)
    {
        puts(api->name);
    }
    puts("\n");
}

void print_devices(const portaudio::deviceList &list)
{
    puts("devices:");
    for (const auto &d : list)
    {
        puts(d->name);
    }
    puts("\n");
}

void print_devices_by_api(const portaudio::apiDeviceList &list)
{
    puts("Devices by api:");
    for (const auto &pr : list)
    {
        printf("Api name: %s: \n", pr.first.c_str());
        puts("Devices:");
        if (pr.second.empty())
        {
            puts("None");
        }
        for (const auto &d : pr.second)
        {
            puts(d->name);
        }
        puts("\n");
    }
    puts("Devices by api complete.");
}

void test_enum()
{
    namespace pa = portaudio::detail;
    auto apis = pa::enum_apis();
    assert(!apis.empty());
    auto devs = pa::enum_devices();
    assert(!devs.empty());

    print_apis(apis);
    print_devices(devs);
}

void test_enumerator()
{
    {
        portaudio::Portaudio audio("test"); // better have one of these else
                                            // portaudio won't be initialized!
        test_enum();
        assert(!audio.enumerator().apis().empty());
        assert(!audio.enumerator().devices().empty());
        print_devices_by_api(audio.enumerator().devicesByApi());
    }
}

static inline float next_sine_sample(uint64_t sample_num, int samplerate)
{
    return sin(440 * 2 * M_PI * sample_num / samplerate);
}

static inline void fill_frame_sine(uint64_t &nsample, float *out,
                                   portaudio::CallbackInfo &info,
                                   bool left = true, bool right = true)
{
    for (uint64_t i = 0; i < info.frameCount; i++)
    {
        auto v = next_sine_sample(nsample++, info.samplerate);
        // clang-format off
            if (left) *out++ = v; else *out++ = 0;
            if (right) *out++ = 0;else *out++ = v;
        // clang-format on
    }
}

void test_simple_play()
{
    namespace pa = portaudio;
    pa::Portaudio audio("test simple playback");
    uint64_t n = 0;
    auto mystream = audio.openDefaultStream([&](pa::CallbackInfo info) {
        auto *out = static_cast<float *>(info.output);
        fill_frame_sine(n, out, info);
        return portaudio::CallbackResult::Continue;
    });

    constexpr auto num_seconds = 5;
    printf("Playing portaudio stream for %d seconds ...\n\n", num_seconds);
    try
    {
        mystream.Start();
    }
    catch (pa::Exception &e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << std::endl;
        assert(0);
    }

    while (mystream.elapsedSeconds() < num_seconds)
    {
        pa::sleep_ms(10);
        printf("Seconds elapsed: %.2f\r", mystream.elapsedSeconds());
        fflush(stdout);
    }
    puts("\n");

    return;
}

void test_my_exceptions()
{
    namespace pa = portaudio;

    try
    {
        pa::Exception mye(-10000, "Additional Info 1", "Additional Info 2", 77,
                          42);
        throw mye;
    }
    catch (const std::runtime_error &e)
    {
        const auto txt = std::string(e.what());
        assert(txt ==
               "PortAudio error code: -10000\nPortAudio not "
               "initialized\nAdditional Info 1 Additional Info 2 77 42 ");
    }

    try
    {
        pa::Exception ex(-10000);
        throw ex;
    }
    catch (const pa::Exception &e)
    {
        const auto txt = std::string(e.what());
        assert(txt == "PortAudio not initialized");
        assert(e.errorCode() == -10000);
    }
}

int main(int, char **)
{

    test_my_exceptions();
    test_setup_teardown();
    test_enumerator();
    test_simple_play();
}
