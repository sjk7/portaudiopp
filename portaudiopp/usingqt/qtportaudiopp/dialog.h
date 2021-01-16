#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "portaudioplusplus.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_cboHostApi_currentIndexChanged(const QString &arg1);

    void on_cboDevice_currentIndexChanged(int index);

    void on_btnTest_clicked();

private:
    Ui::Dialog *ui;
    portaudio::Portaudio m_portaudio;
    void showEvent( QShowEvent* event ) {
        QWidget::showEvent( event );
        Pasetup();
    }
    void Pasetup();
    void Log(const QString& s);
    void popDevices(int apiIndex, const QString& selApi);
    QString m_hostSelText;
    portaudio::PaDeviceInfoEx m_device;
    bool m_bpopping =false;

};
#endif // DIALOG_H
