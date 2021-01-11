#include "portaudioplusplus.hpp"

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

void test_simple_play()
{
    portaudio::Portaudio audio("test simple playback");
    assert(audio.id().size() > 0);
    auto dev = audio.enumerator().default_device();
    assert(dev);
    printf("Default device name is: %s\n", dev->name);

    struct mysine_t : public portaudio::AudioCallback
    {
        unsigned long n = 0;
        virtual portaudio::CallbackResult
        onCallback(portaudio::CallbackInfo info) noexcept
        {
            float *out = (float *)info.output;
            for (unsigned long i = 0; i < info.frameCount; i++, n++)
            {
                float v = sin(440 * 2 * M_PI * n / 44100);
                *out++ = v;
                *out++ = v;
            }
            return portaudio::CallbackResult::Continue;
        }
    };

    mysine_t mycb;
    auto stream = audio.preparePlay(&mycb);

    assert(stream);

    constexpr auto num_seconds = 5;
    printf("Playing portaudio stream for %d seconds ...\n\n", num_seconds);
    audio.play();

    PaTime dtime = 0;
    while (dtime < num_seconds)
    {
        dtime = audio.elapsedSeconds();
        printf("\rStream Elapsed: %.2f", dtime);
        fflush(stdout);
        std::cout.flush();
        portaudio::sleep_ms(10);
    }
    std::cout << std::endl;
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
