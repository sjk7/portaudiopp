#include "dialog.h"
#include "ui_dialog.h"
#include <QDateTime>
#include <QMessageBox>


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog), m_portaudio("QtTest")
{
    ui->setupUi(this);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::Pasetup()
{
    ui->cboHostApi->clear();
    ui->cboDevice->clear();
    for (const auto& d : m_portaudio.enumerator().apis()){
       ui->cboHostApi->addItem(d.info->name);
    }

    ui->cboHostApi->setCurrentIndex(0);

}

void Dialog::Log(const QString& s)
{
    QDateTime dt = QDateTime::currentDateTime();
    QString out = dt.time().toString() + " " + s;
    ui->listWidget->addItem(out);
    ui->listWidget->scrollToBottom();
    ui->listWidget->repaint();
}

void Dialog::popDevices(int apiIndex, const QString &selApi)
{
    ui->cboDevice->clear();
    const auto& devsbyapi = m_portaudio.enumerator().devicesByApi();
    const auto apiName = m_portaudio.enumerator().apis().at(apiIndex).info->name;
    Log("Found Portaudio HostAPI with name: " + QString(apiName) + " at index: " + QString::number(apiIndex));
    QString myName(apiName);
    Q_ASSERT(myName == selApi);
    const auto& found = devsbyapi.find(apiName);

    if (found != devsbyapi.cend()){
        const auto& pr = *found;
        const auto& vec = pr.second;
        Log("Found " + QString::number(vec.size()) + " devices for HostAPI " + myName);
        for (const auto& d : vec){
            ui->cboDevice->addItem(d.info->name);
        }
        const auto& apis= m_portaudio.enumerator().apis();
        const auto& cur_api = apis.at(apiIndex);
        auto def_index = cur_api.defaultDeviceApiIndex;
        m_bpopping = false;

        const auto n = ui->cboDevice->count();
        assert(def_index < n);
        if (def_index == paNoDevice){
            ui->cboDevice->setCurrentIndex(0);
        }else{
            ui->cboDevice->setCurrentIndex(def_index);
        }
    }else{
        Log("Error! HostAPI with name: " + QString(apiName) + " Could not find the devices for this HostAPI" );
    }
}


void Dialog::on_cboHostApi_currentIndexChanged(const QString &arg1)
{
    Log("HostApi combobox current index changed (" + arg1 + ")");
    m_hostSelText = arg1;
    m_bpopping =true;
    popDevices(ui->cboHostApi->currentIndex(), arg1);
    m_bpopping = false;
}

void Dialog::on_cboDevice_currentIndexChanged(int index)
{
    if (m_bpopping) return;

    if (index >= 0){
        Log("Device selector current index has changed to: " + QString::number(index));

        const auto& devsbyapi = m_portaudio.enumerator().devicesByApi();
        const auto found = devsbyapi.find(m_hostSelText.toStdString());
        Q_ASSERT(found != devsbyapi.cend());
        const auto& pr = *found;
        const auto& devices = pr.second;
        const auto& device = devices.at(index);
        this->m_device = device;
        Log("Selected device is: " + QString(device.info->name));
        const auto& apis = m_portaudio.enumerator().apis();
        const auto& selapi = apis.at(device.info->hostApi);
        const QString apiName = selapi.info->name;
        Q_ASSERT(apiName == m_hostSelText);
        Log("Selected api is: " + apiName);
    }else{
        Log("Device selector has nothing selected.");
    }

}

void Dialog::on_btnTest_clicked()
{
    if (m_device.global_device_index == portaudio::INVALID_PA_DEVICE_INDEX){
        QMessageBox::critical(this, "No Selected device", "There is no device selected, so cannot test");
    } else{
        PaStreamParameters params = portaudio::streamParamsDefault(m_portaudio, &m_device);

        m_device.streamSetupInfo.outParams = &params;
        uint64_t n = 0;

        try{
        auto s = m_portaudio.openStream(m_device, [&](portaudio::CallbackInfo info){
            portaudio::fill_buffer_sine(n, info);
            return portaudio::CallbackResult::Continue;
        });

        Log ("Playing tone to device: " + QString(m_device.info->name) + " ...");

        this->ui->cboDevice->setEnabled(false);
        this->ui->cboHostApi->setEnabled(false);
        s.Start();
        while (this->ui->btnTest->isChecked()){
            portaudio::sleep_ms(10);
            QCoreApplication::processEvents();
        };

        this->ui->cboDevice->setEnabled(true);
        this->ui->cboHostApi->setEnabled(true);

        Log ("Playing tone to device: " + QString(m_device.info->name) + " Complete.");
        }
        catch(const portaudio::Exception& e){
            Log("Error! Opening device " + QString(m_device.info->name) + " failed" + QString(e.what()));
        }


    }
}
