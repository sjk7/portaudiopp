#ifndef DIALOG_H
#define DIALOG_H

#include "portaudioplusplus.h"
#include <QDialog>
class QComboBox;

QT_BEGIN_NAMESPACE
namespace Ui
{
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

  public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

  private slots:
    void on_btnTest_toggled(bool checked);
    void on_cboOutput_currentIndexChanged(int index);
    void on_cboHostApi_currentIndexChanged(int index);
    void on_btnTestInput_toggled(bool checked);
    void on_btnTestOutput_toggled(bool checked);
    void on_cboInput_currentIndexChanged(int index);

    void on_cboDuplex_currentIndexChanged(int index);

    void on_btnTestDuplex_toggled(bool checked);

  private:
    Ui::Dialog *ui;
    void showError(bool err);
    portaudio::Portaudio m_portaudio;
    void showEvent(QShowEvent *event)
    {
        static bool shown = false;
        if (shown) return;
        QWidget::showEvent(event);
        shown = true;
        Pasetup();
    }
    void Pasetup();
    void Log(const QString &s);
    void popDevices(int apiIndex);
    void popDevices(const portaudio::deviceList &devices, QComboBox *cbo);
    int m_hostApiIndex = -1;
    int m_inputDeviceIndex = -1;
    int m_outputDeviceIndex = -1;
    int m_duplexDeviceIndex = -1;
    portaudio::PaDeviceInfoEx m_device;
    bool m_bpopping = false;
    void selectDefaultDevice(const portaudio::PaHostApiInfoEx *api,
                             QComboBox *cbo);
    std::atomic<int> m_WantQuit = 0;

    void reject();
};
#endif // DIALOG_H
