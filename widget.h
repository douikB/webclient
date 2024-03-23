#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QSslSocket>
#include <QUdpSocket>
#include <QSettings>
#include <QFileInfo>

#define CONF_PATH   "/home/kali/webclient/conf/webcli_settings.conf"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QSslSocket socket_;
    QAbstractSocket::SocketState state;

    QSettings settings;
    QFileInfo fileinfo;

public slots:
    void doConnected();
    void doDisconnected();
    void doReadyRead();

private slots:
    void on_pb_Connect_clicked();

    void on_pb_Disconnect_clicked();

    void on_pb_Clear_clicked();

    void on_pb_Send_clicked();

    void on_cb_SSL_clicked();

    void updateButtonStates();

    void saveSettings();

    void loadSettings();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
