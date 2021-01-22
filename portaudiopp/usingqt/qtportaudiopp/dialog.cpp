#include "dialog.h"
#include "ui_dialog.h"
#include <QDateTime>
#include <QMessageBox>
#include <QStandardPaths>

#include <fstream>

void Dialog::reject()
{
    m_WantQuit = m_portaudio.StreamsActive();

    ui->btnTestInput->setChecked(false);
    ui->btnTestDuplex->setChecked(false);
    ui->btnTestOutput->setChecked(false);
    if (m_WantQuit == 0)
    {
        QDialog::reject();
    }
}

static auto inline FormatSeconds(float seconds) -> QString
{
    return QDateTime::fromMSecsSinceEpoch(seconds * 1000)
        .toUTC()
        .toString("hh:mm:ss:zzz");
}

Dialog::Dialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::Dialog), m_portaudio("QtTest")
{
    ui->setupUi(this);
    this->setWindowTitle("Portaudio Devices Tester");
}

Dialog::~Dialog() { delete ui; }

void Dialog::showError(bool err)
{
    auto item = ui->listWidget->item(ui->listWidget->count() - 1);
    item->setSelected(true);
    if (err)
    {
        item->setBackground(Qt::red);
        // add this line so it stands out: If it's
        // the last one, the selected one, it won't look red!
        Log("Review: one or more errors encountered.");
    }

    ui->listWidget->scrollToBottom();
}
void Dialog::Pasetup()
{
    ui->cboHostApi->clear();
    ui->cboInput->clear();
    ui->cboOutput->clear();
    for (const auto &d : m_portaudio.enumerator().apis())
    {
        ui->cboHostApi->addItem(d.info.name);
    }

    auto def_api_name = m_portaudio.enumerator().defaultHostApi().info.name;
    auto def_api = m_portaudio.enumerator().findApi(def_api_name);

    ui->cboHostApi->setCurrentIndex(def_api->api_index);
}

void Dialog::on_cboHostApi_currentIndexChanged(int index)
{
    m_hostApiIndex = index;
    popDevices(index);
}

void Dialog::popDevices(const portaudio::deviceList &devices, QComboBox *cbo)
{
    cbo->clear();
    m_bpopping = true;
    for (const auto &d : devices)
    {
        cbo->addItem(d.info->name);
    }

    m_bpopping = false;
}

void Dialog::Log(const QString &s)
{
    QDateTime dt = QDateTime::currentDateTime();
    QString out = dt.time().toString() + " " + s;
    ui->listWidget->addItem(out);
    ui->listWidget->scrollToBottom();
    ui->listWidget->repaint();
}

void Dialog::selectDefaultDevice(const portaudio::PaHostApiInfoEx *api,
                                 QComboBox *cbo)
{

    m_bpopping = true;
    auto idx = 0;
    const auto dup = api->defaultDuplexDevice();
    const int dup_index = dup ? dup->duplex_api_device_index : -1;

    if (cbo == ui->cboInput)
        idx = api->defaultInputDevice()->input_api_device_index;
    else if (cbo == ui->cboOutput)
        idx = api->defaultOutputDevice()->output_api_device_index;
    else if (cbo == ui->cboDuplex)
        idx = dup_index;
    else
        assert("cant find combobox!" == nullptr);

    cbo->setCurrentIndex(idx);
    m_bpopping = false;
}

void Dialog::popDevices(int apiIndex)
{

    ui->cboInput->clear();
    ui->cboOutput->clear();
    ui->cboDuplex->clear();
    const auto &enummer = m_portaudio.enumerator();
    const auto *api = enummer.findApi(apiIndex);
    Q_ASSERT(api);
    popDevices(api->inputDevices(), ui->cboInput);
    popDevices(api->outputDevices(), ui->cboOutput);
    popDevices(api->duplexDevices(), ui->cboDuplex);
    selectDefaultDevice(api, ui->cboInput);
    selectDefaultDevice(api, ui->cboOutput);
    selectDefaultDevice(api, ui->cboDuplex);
}

auto channels_to_string(const PaDeviceInfo *info) -> QString
{
    QString out(
        "maxInputChannels = " + QString::number(info->maxInputChannels) +
        ", maxOutputChannels = " + QString::number(info->maxOutputChannels));
    return out;
}

void Dialog::on_cboOutput_currentIndexChanged(int index)
{
    this->m_outputDeviceIndex = index;
    if (index >= 0)
    {
        this->Log("Output device index changed to: " + QString::number(index));
        auto api = m_portaudio.enumerator().findApi(this->m_hostApiIndex);
        auto dev = m_portaudio.enumerator().findDevice(
            this->m_hostApiIndex, index, portaudio::DeviceType::types::output);
        Log("Output device has api: " + QString(api->info.name));
        Log("Output device name: " + QString(dev->info->name));
        Log("Output device: " + channels_to_string(dev->info));
    }
}

// input device
void Dialog::on_cboInput_currentIndexChanged(int index)
{
    this->m_inputDeviceIndex = index;
    if (index >= 0)
    {

        this->Log("Input device index changed to: " + QString::number(index));
        auto api = m_portaudio.enumerator().findApi(this->m_hostApiIndex);
        auto dev = m_portaudio.enumerator().findDevice(
            this->m_hostApiIndex, index, portaudio::DeviceType::types::input);
        Log("Input device has api: " + QString(api->info.name));
        Log("Input device name: " + QString(dev->info->name));
        Log("Input device: " + channels_to_string(dev->info));
    }
}

void Dialog::on_cboDuplex_currentIndexChanged(int index)
{
    this->m_duplexDeviceIndex = index;
    if (index >= 0)
    {
        this->Log("Duplex device index changed to: " + QString::number(index));
        auto api = m_portaudio.enumerator().findApi(this->m_hostApiIndex);
        auto dev = m_portaudio.enumerator().findDevice(
            this->m_hostApiIndex, index, portaudio::DeviceType::types::duplex);
        Log("Duplex device has api: " + QString(api->info.name));
        Log("Duplex device name: " + QString(dev->info->name));
        Log("Duplex device: " + channels_to_string(dev->info));
    }
}

void Dialog::on_btnTest_toggled(bool)
{
    /*/
    if (m_device.global_device_index == portaudio::INVALID_PA_DEVICE_INDEX)
    {
        QMessageBox::critical(this, "No Selected device",
                              "There is no device selected, so cannot test");
    }
    else
    {
        PaStreamParameters params =
            portaudio::streamParamsDefault(m_portaudio, &m_device);

        m_device.streamSetupInfo.outParams = &params;
        uint64_t n = 0;
        if (checked)
        {
            try
            {
                auto s = m_portaudio.openStream(
                    m_device, [&](portaudio::CallbackInfo info) {
                        portaudio::fill_buffer_sine(n, info);
                        return portaudio::CallbackResult::Continue;
                    });

                Log("Playing tone to device: " + QString(m_device.info->name) +
                    " ...");

                this->ui->cboDevice->setEnabled(false);
                this->ui->cboHostApi->setEnabled(false);
                s.Start();
                this->ui->btnTest->setText("Playing tone. Click again to stop");
                while (this->ui->btnTest->isChecked())
                {
                    portaudio::sleep_ms(10);
                    QCoreApplication::processEvents();
                };

                s.Stop();
                this->ui->cboDevice->setEnabled(true);
                this->ui->cboHostApi->setEnabled(true);
                this->ui->btnTest->setText("Test");


                Log("Playing tone to device: " + QString(m_device.info->name) +
                    " Complete.");
                this->ui->btnTest->setEnabled(true);
                this->ui->btnTest->setChecked(false);
            }
            catch (const portaudio::Exception &e)
            {
                Log("Error! Opening device " + QString(m_device.info->name) +
                    " failed" + QString(e.what()));
                this->ui->btnTest->setEnabled(true);
                 this->ui->btnTest->setChecked(false);
            }
        }
        else
        {
            // unchecked
        }
    }
    /*/
}

void Dialog::on_btnTestInput_toggled(bool checked)
{
    bool err = false;
    if (checked)
    {

        auto filepath =
            QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) +
            "/my_recorded.raw";
        std::fstream f(filepath.toStdString().c_str(),
                       std::ios_base::binary | std::ios_base::out);

        if (!f)
        {
            Log("Error opening recording file: " + filepath);
            ui->btnTestInput->setChecked(false);
            return;
        }

        auto api = m_portaudio.enumerator().findApi(this->m_hostApiIndex);
        assert(api);
        auto dev = m_portaudio.enumerator().findDevice(
            api->inputDevices(), this->m_inputDeviceIndex);
        assert(dev);
        assert(dev->deviceType.is_input_only() || dev->deviceType.is_duplex());
        portaudio::PaDeviceInfoEx mydevinstance(*dev);

        try
        {

            auto streamParams =
                portaudio::makeStreamParams(m_portaudio, &mydevinstance);

            mydevinstance.streamSetupInfo = portaudio::makeStreamSetupInfo(
                mydevinstance, &streamParams, nullptr);
        }
        catch (const portaudio::Exception &e)
        {
            Log("Error whilst setting up input device: " + QString(e.what()));
            err = true;
        }
        if (!err)
        {
            Log("Preparing to record to file from input device: " +
                QString(dev->info->name) + ", using api " +
                QString(dev->hostApiInfo->name));
            try
            {
                auto rec_stream = m_portaudio.openStream(
                    mydevinstance, [&](portaudio::CallbackInfo info) {
                        auto finput = (float *)info.input;
                        size_t cb =
                            sizeof(float) *
                            mydevinstance.streamSetupInfo.inputChannelCount *
                            info.frameCount;
                        f.write((char *)finput, cb);
                        assert(f);

                        if (!ui->btnTestInput->isChecked() || m_WantQuit)
                        {
                            return portaudio::CallbackResult::Complete;
                        }

                        return portaudio::CallbackResult::Continue;
                    });

                try
                {
                    rec_stream.Start();

                    Log("Recording to file: " + filepath + ": Started.");
                    Log("Hit the button again to stop it");
                    ui->listWidget->scrollToBottom();
                    ui->listWidget->item(ui->listWidget->count() - 1)
                        ->setSelected(true);
                    unsigned int i = 0;
                    while (rec_stream.isRunning())
                    {
                        portaudio::SleepmS(10);
                        if (i == 0)
                        {
                            Log("Started input capture to file: " + filepath +
                                " ... ");
                        }
                        if (i % 10 == 0)
                        {
                            ui->btnTestInput->setText(
                                "Recording: " +
                                FormatSeconds(rec_stream.elapsedSeconds()));

                            QCoreApplication::processEvents(); // lovely! :-(
                        }
                        i += 10;
                    };

                    f.close();

                    ui->btnTestInput->setText("Record Input To &File");
                    Log("Recording to file: " + filepath + ": Complete.");
                }
                catch (const portaudio::Exception &e)
                {
                    Log("Failed to start record stream for device: " +
                        QString(mydevinstance.info->name) + " " +
                        QString(e.what()));
                    err = true;
                }
            }
            catch (portaudio::Exception &e)
            {
                Log("Failed to set up record stream for device: " +
                    QString(mydevinstance.info->name) + " " +
                    QString(e.what()));
                err = true;
            }
        }

        if (m_WantQuit)
        {
            reject();
            return;
        }
    }

    showError(err);

    QApplication::restoreOverrideCursor();
    ui->btnTestInput->setChecked(false);
    ui->btnTestInput->setText("Test Input");
    ui->btnTestInput->setEnabled(true);
    if (this->m_WantQuit) QDialog::reject();
}

void Dialog::on_btnTestOutput_toggled(bool checked)
{
    bool err = false;
    if (checked)
    {
        if (m_outputDeviceIndex < 0)
        {
            QMessageBox::critical(this, "Output Device Error",
                                  "It appears no output device is selected.");
        }
        else
        {
            // go go go
            const auto *pmydevice = m_portaudio.enumerator().findDevice(
                m_hostApiIndex, m_outputDeviceIndex);
            assert(pmydevice);
            if (!pmydevice)
            {
                QMessageBox::critical(this, "Unexpected Output Device Error",
                                      "Cannot find any device.");
            }

            else
            {
                auto mydevice = *pmydevice;
                mydevice.deviceTypeSet(portaudio::DeviceType::types::output);
                try
                {
                    auto streamParams =
                        portaudio::makeStreamParams(m_portaudio, &mydevice);

                    auto mysetupInfo = portaudio::makeStreamSetupInfo(
                        mydevice, nullptr, &streamParams);

                    mydevice.streamSetupInfo = mysetupInfo;
                }
                catch (const portaudio::Exception &e)
                {
                    Log("Unexpected error when setting up output device:\n" +
                        QString(mydevice.info->name) + " " + QString(e.what()));
                    err = true;
                }

                if (!err)
                {
                    unsigned int n = 0;

                    try
                    {
                        QApplication::setOverrideCursor(Qt::WaitCursor);

                        QCoreApplication::processEvents();
                        auto stream = m_portaudio.openStream(
                            mydevice, [&](portaudio::CallbackInfo info) {
                                const int nch =
                                    mydevice.streamSetupInfo.outputChannelCount;
                                assert(nch == 2);

                                portaudio::dsp::fill_buffer_sine(n, info, nch);

                                return portaudio::CallbackResult::Continue;
                            });

                        try
                        {
                            stream.Start();
                            Log("Playing test tone to output device. Hit the "
                                "button again to stop it");
                            ui->listWidget->scrollToBottom();
                            ui->listWidget->item(ui->listWidget->count() - 1)
                                ->setSelected(true);
                            while (stream.isRunning())
                            {
                                ui->btnTestOutput->setText(
                                    FormatSeconds(stream.elapsedSeconds()));
                                QCoreApplication::processEvents();
                                portaudio::SleepmS(10);
                                if (m_WantQuit ||
                                    !ui->btnTestOutput->isChecked())
                                    break;
                            }

                            stream.Stop();
                        }
                        catch (const portaudio::Exception &e)
                        {
                            Log("Unexpected error when starting output "
                                "device:\n" +
                                QString(mydevice.info->name) + " " +
                                QString(e.what()));
                        }
                    }
                    catch (const portaudio::Exception &e)
                    {
                        Log("Unexpected error when setting up output "
                            "device:\n" +
                            QString(mydevice.info->name) + " " +
                            QString(e.what()));
                    }
                }
            }
        }
    }

    showError(err);
    QApplication::restoreOverrideCursor();
    ui->btnTestOutput->setChecked(false);
    ui->btnTestOutput->setText("Test Output");
    ui->btnTestOutput->setEnabled(true);
    if (this->m_WantQuit) QDialog::reject();
}

void Dialog::on_btnTestDuplex_toggled(bool checked)
{
    bool err = false;
    if (checked)
    {

        if (m_duplexDeviceIndex < 0)
        {
            QMessageBox::critical(this, "Duplex Device Error",
                                  "It appears no duplex device is selected.");
            ui->btnTestDuplex->setChecked(false);
            return;
        }
        const auto &api =
            m_portaudio.enumerator().findApi(this->m_hostApiIndex);
        const auto &device = api->duplexDevices().at(this->m_duplexDeviceIndex);
        assert(device.deviceType.is_duplex());
        Log("Preparing for duplex test (playing audio in and relaying to audio "
            "out, for: " +
            QString(device.info->name));
        auto mydevice = device;
        try
        {
            auto streamParams =
                portaudio::makeStreamParams(m_portaudio, &mydevice);

            auto mysetupInfo = portaudio::makeStreamSetupInfo(
                mydevice, &streamParams, &streamParams);

            mydevice.streamSetupInfo = mysetupInfo;
        }
        catch (const portaudio::Exception &e)
        {
            Log("Error setting up device for duplex: " + QString(e.what()));
            err = true;
        }

        if (!err)
        {
            try
            {
                QApplication::setOverrideCursor(Qt::WaitCursor);
                QCoreApplication::processEvents();
                auto stream = m_portaudio.openStream(
                    mydevice, [&](portaudio::CallbackInfo info) {
                        assert(mydevice.streamSetupInfo.inputChannelCount ==
                               mydevice.streamSetupInfo.outputChannelCount);
                        // ^^ They ought to, it's passthrough!
                        memcpy(info.output, info.input,
                               sizeof(float) * info.frameCount *
                                   mydevice.streamSetupInfo.outputChannelCount);
                        return portaudio::CallbackResult::Continue;
                    });

                try
                {
                    stream.Start();
                    Log("Loopback device started.");
                    Log("Hit the button again to stop it");
                    ui->listWidget->scrollToBottom();
                    ui->listWidget->item(ui->listWidget->count() - 1)
                        ->setSelected(true);
                    while (stream.isRunning())
                    {
                        portaudio::SleepmS(10);
                        if (!ui->btnTestDuplex->isChecked()) break;
                        ui->btnTestDuplex->setText(
                            FormatSeconds(stream.elapsedSeconds()));
                        QCoreApplication::processEvents();
                    }
                }
                catch (const portaudio::Exception &e)
                {
                    Log("Unexpected error when starting duplex device:\n" +
                        QString(mydevice.info->name) + " " + QString(e.what()));
                }
            }
            catch (const portaudio::Exception &e)
            {
                Log("Unexpected error when setting up duplex device:\n" +
                    QString(mydevice.info->name) + " " + QString(e.what()));
            }
        }
    }
    else
    {
        Log("Duplex test state toggled off");
    }

    if (this->m_WantQuit)
    {
        QDialog::reject();
        return;
    }
    QApplication::restoreOverrideCursor();
    showError(err);
    ui->btnTestDuplex->setChecked(false);
    ui->btnTestDuplex->setText("Test Duplex");
    ui->btnTestDuplex->setEnabled(true);
}
