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

static inline QString FormatSeconds(float seconds)
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

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::Pasetup()
{
    ui->cboHostApi->clear();
    ui->cboInput->clear();
    ui->cboOutput->clear();
    for (const auto& d : m_portaudio.enumerator().apis()){
       ui->cboHostApi->addItem(d.info->name);
    }

    auto def_api_name = m_portaudio.enumerator().defaultHostApi().info->name;
    auto def_api = m_portaudio.enumerator().findApi(def_api_name);

    ui->cboHostApi->setCurrentIndex(def_api->api_index);
}


void Dialog::on_cboHostApi_currentIndexChanged(int index)
{
    m_hostApiIndex = index;
    popDevices(index);

}

void Dialog::popDevices(const portaudio::deviceList& devices, QComboBox* cbo){
    cbo->clear();
    m_bpopping = true;
    for (const auto& d : devices){
        cbo->addItem(d.info->name);
    }

    m_bpopping = false;
}

void Dialog::Log(const QString& s)
{
    QDateTime dt = QDateTime::currentDateTime();
    QString out = dt.time().toString() + " " + s;
    ui->listWidget->addItem(out);
    ui->listWidget->scrollToBottom();
    ui->listWidget->repaint();
}

void Dialog::selectDefaultDevice(const portaudio::PaHostApiInfoEx* api, QComboBox* cbo){

    m_bpopping = true;
    auto idx = 0;
    if (cbo == ui->cboInput){
        idx = api->defaultInputDevice()->input_api_device_index;
    }else if(cbo == ui->cboInput){
        idx = api->defaultOutputDevice()->output_api_device_index;
    }else if (cbo == ui->cboDuplex){
        auto dup = api->defaultDuplexDevice();
        if (dup){
            idx = dup->duplex_api_device_index;
        }else{
            idx = -1;
        }
    }
    cbo->setCurrentIndex(idx);
    m_bpopping = false;
}

void Dialog::popDevices(int apiIndex)
{

    ui->cboInput->clear();
    ui->cboOutput->clear();
    ui->cboDuplex->clear();
    const auto& enummer = m_portaudio.enumerator();
    const auto* api = enummer.findApi(apiIndex);
    Q_ASSERT(api);
    popDevices(api->inputDevices(), ui->cboInput);
    popDevices(api->outputDevices(), ui->cboOutput);
    popDevices(api->duplexDevices(), ui->cboDuplex);
    selectDefaultDevice(api, ui->cboInput);
    selectDefaultDevice(api, ui->cboOutput);
    selectDefaultDevice(api, ui->cboDuplex);

}


QString channels_to_string( const PaDeviceInfo* info){
    QString out("maxInputChannels = " +
        QString::number(info->maxInputChannels) +
        ", maxOutputChannels = " + QString::number(info->maxOutputChannels));
    return out;
}

void Dialog::on_cboOutput_currentIndexChanged(int index)
{
    this->m_outputDeviceIndex = index;
    if (index >= 0){
        this->Log("Output device index changed to: " + QString::number(index));
        auto api = m_portaudio.enumerator().findApi(this->m_hostApiIndex);
        auto dev = m_portaudio.enumerator().findDevice(this->m_hostApiIndex, index, portaudio::DeviceType::types::output);
        Log("Output device has api: " + QString(api->info->name));
        Log("Output device name: " + QString(dev->info->name));
        Log("Output device: " + channels_to_string(dev->info));
    }
}

// input device
void Dialog::on_cboInput_currentIndexChanged(int index)
{
    this->m_inputDeviceIndex = index;
    if (index >= 0){

        this->Log("Input device index changed to: " + QString::number(index));
        auto api = m_portaudio.enumerator().findApi(this->m_hostApiIndex);
        auto dev = m_portaudio.enumerator().findDevice(this->m_hostApiIndex, index, portaudio::DeviceType::types::input);
        Log("Input device has api: " + QString(api->info->name));
        Log("Input device name: " + QString(dev->info->name));
        Log("Input device: " + channels_to_string(dev->info));
    }
}

void Dialog::on_cboDuplex_currentIndexChanged(int index)
{
    this->m_duplexDeviceIndex = index;
    if (index >= 0){
        this->Log("Duplex device index changed to: " + QString::number(index));
        auto api = m_portaudio.enumerator().findApi(this->m_hostApiIndex);
        auto dev = m_portaudio.enumerator().findDevice(this->m_hostApiIndex, index, portaudio::DeviceType::types::duplex);
        Log("Duplex device has api: " + QString(api->info->name));
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
    if (checked)
    {
        auto api = m_portaudio.enumerator().findApi(this->m_hostApiIndex);
        assert(api);
        auto dev = m_portaudio.enumerator().findDevice(
            api->inputDevices(), this->m_inputDeviceIndex);
        assert(dev);
        assert(dev->deviceType.is_input_only() || dev->deviceType.is_duplex());
        portaudio::PaDeviceInfoEx mydevinstance(*dev);

        auto streamParams =
            portaudio::streamParamsDefault(m_portaudio, &mydevinstance);

        auto mysetupInfo = portaudio::makeStreamSetupInfo(
            mydevinstance, &streamParams, nullptr);

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

        Log("Preparing to record to file from input device: " +
            QString(dev->info->name) + ", using api " +
            QString(dev->hostApiInfo->name));

        try
        {
            auto rec_stream = m_portaudio.openStream(
                mysetupInfo, [&](portaudio::CallbackInfo info) {
                    float *finput = (float *)info.input;
                    size_t cb = sizeof(float) * mysetupInfo.inputChannelCount *
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
                ui->btnTestInput->setChecked(false);
            }
        }
        catch (portaudio::Exception &e)
        {
            Log("Failed to set up record stream for device: " +
                QString(mydevinstance.info->name) + " " + QString(e.what()));
            ui->btnTestInput->setChecked(false);
        }

        if (m_WantQuit)
        {
            reject();
            return;
        }
    }
}

void Dialog::on_btnTestOutput_toggled(bool checked)
{

}




void Dialog::on_btnTestDuplex_toggled(bool checked)
{

}
