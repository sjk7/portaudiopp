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
        puts(api.info->name);
    }
    puts("\n");
}

void print_devices(const portaudio::deviceList &list)
{
    puts("devices:");
    for (const auto &d : list)
    {
        puts(d.info->name);
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
            puts(d.info->name);
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

    for (const auto &d : devs)
    {
        assert(d.device_index >= 0); // forgot to index device correctly
    }

    for (const auto &api : apis)
    {
        assert(api.api_index >= 0); // forgot to index apis correctly
    }

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

static inline float next_sine_sample(uint64_t sample_num, int samplerate,
                                     int freq = 440)
{
    return sin(freq * 2 * M_PI * sample_num / samplerate);
}

static inline void fill_buffer_sine(uint64_t &nsample, float *out,
                                    portaudio::CallbackInfo &info,
                                    bool left = true, bool right = true,
                                    int freq = 440)
{
    for (uint64_t i = 0; i < info.frameCount; i++)
    {
        auto v = next_sine_sample(nsample++, info.samplerate, freq);
        // clang-format off
            if (left) *out++ = v; else *out++ = 0;
            if (right) *out++ = v;else *out++ = 0;
        // clang-format on
    }
}
// Explicitly test that the callbacks are seperate per instance of Stream:
void test_dual_play(int num_seconds = 5)
{

    namespace pa = portaudio;
    pa::Portaudio audio("test simple playback");
    uint64_t nL = 0;
    auto mystreamL = audio.openDefaultStream([&](pa::CallbackInfo info) {
        auto *out = static_cast<float *>(info.output);
        fill_buffer_sine(nL, out, info, true, false);
        return portaudio::CallbackResult::Continue;
    });

    uint64_t nR = 0;
    auto mystreamR = audio.openDefaultStream([&](pa::CallbackInfo info) {
        auto *out = static_cast<float *>(info.output);
        fill_buffer_sine(nR, out, info, false, true, 1000);
        return portaudio::CallbackResult::Continue;
    });

    printf("Playing (dual) portaudio stream for %d seconds ...\n\n",
           num_seconds);
    try
    {
        mystreamL.Start();
        mystreamR.Start();
    }
    catch (pa::Exception &e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << std::endl;
        assert(0);
    }

    while (mystreamR.elapsedSeconds() < num_seconds)
    {
        pa::sleep_ms(50);
        printf("Seconds elapsed: %.2f\r", mystreamR.elapsedSeconds());
        fflush(stdout);
        if (mystreamL.elapsedSeconds() >= (double)(num_seconds / 2.0) &&
            mystreamL.isRunning())
        {
            mystreamL.Stop();
            puts("Left channel stream stopped, now the right only ...");
        }
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
               "PortAudio error code: -10000 (PortAudio not "
               "initialized)\nAdditional Info 1 Additional Info 2 77 42 ");
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

void test_simple_play(int num_seconds = 5)
{
    namespace pa = portaudio;
    pa::Portaudio audio("test simple playback");
    uint64_t n = 0;

    auto mystream = audio.openDefaultStream([&](pa::CallbackInfo info) {
        auto *out = static_cast<float *>(info.output);
        fill_buffer_sine(n, out, info);
        return portaudio::CallbackResult::Continue;
    });

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
        pa::sleep_ms(100);
        printf("Seconds elapsed: %.2f\r", mystream.elapsedSeconds());
        fflush(stdout);
    }
    puts("\n");

    return;
}

void test_advanced_play(portaudio::Portaudio &paObj, int samplerate)
{
    namespace pa = portaudio;
    pa::StreamSetupInfo myinfo;
    myinfo.samplerate = samplerate;
    std::atomic<int> the_samplerate = 0;
    auto myparms = portaudio::streamParamsDefault(paObj);
    myinfo.outParams = &myparms;

    pa::Stream mystream = paObj.openStream(myinfo, [&](pa::CallbackInfo info) {
        assert(info.samplerate == 48000);
        the_samplerate = info.samplerate;
        return pa::CallbackResult::Continue;
    });

    mystream.Start();
    int slept = 0;
    while (the_samplerate == 0)
    {
        pa::sleep_ms(50);
        slept++;
        assert(slept < 20);
    }

    assert(the_samplerate == 48000);
}

struct myCallbackClass : public portaudio::AudioCallback
{
    unsigned long n = 0;
    portaudio::CallbackResult
    onCallback(portaudio::CallbackInfo info) noexcept override
    {
        auto *out = static_cast<float *>(info.output);
        fill_buffer_sine(n, out, info);
        if (info.elapsed_time >= 5)
        {
            return portaudio::CallbackResult::Complete;
        }
        return portaudio::CallbackResult::Continue;
    }
};

void test_callback_stream()
{
    namespace pa = portaudio;
    pa::Portaudio p;
    pa::StreamSetupInfo myinfo;
    auto ffs = portaudio::streamParamsDefault(p);
    myinfo.outParams = &ffs;

    try
    {
        myCallbackClass mycb;
        p.openAndRunStream(myinfo, mycb);
        puts("Stream complete.\n");
    }
    catch (const pa::Exception &e)
    {
        std::cerr << e.what() << std::endl;
        assert("Did not expect simple callback openAndRunStream to fail." ==
               nullptr);
    }
}

int main(int, char **)
{

    test_enumerator();
    test_my_exceptions();
    test_setup_teardown();

    test_dual_play(1);

    {
        portaudio::Portaudio paObj;
        test_advanced_play(paObj, 48000);
    }

    test_simple_play(1);
    test_callback_stream();
}
