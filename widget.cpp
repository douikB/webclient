#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , settings(CONF_PATH, QSettings::NativeFormat)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    loadSettings();

    QObject::connect(&socket_, &QAbstractSocket::connected, this, &Widget::doConnected);
    QObject::connect(&socket_, &QAbstractSocket::disconnected, this, &Widget::doDisconnected);
    QObject::connect(&socket_, &QAbstractSocket::readyRead, this, &Widget::doReadyRead);

    updateButtonStates();
}

Widget::~Widget()
{
    saveSettings();
    delete ui;
}

void Widget::on_cb_SSL_clicked()
{
    if (ui->cb_SSL->isChecked()) {
        ui->le_Port->setText("443");
    }
    else {
        ui->le_Port->setText("80");
    }
}

void Widget::doConnected()
{
    ui->pte_Message->insertPlainText("Connected\n");
    updateButtonStates();
}

void Widget::doDisconnected()
{
    ui->pte_Message->insertPlainText("Disconnected\n");
    updateButtonStates();
}

void Widget::doReadyRead()
{
    ui->pte_Message->insertPlainText(socket_.readAll());
    updateButtonStates();
}

void Widget::on_pb_Connect_clicked()
{
    if (ui->cb_SSL->isChecked()) {
        socket_.connectToHostEncrypted(ui->le_Host->text(), ui->le_Port->text().toShort());
    }
    else {
        socket_.connectToHost(ui->le_Host->text(), ui->le_Port->text().toShort());
    }

}

void Widget::on_pb_Disconnect_clicked()
{
    socket_.close();
}

void Widget::on_pb_Send_clicked()
{
    socket_.write(ui->pte_Send->toPlainText().toUtf8());
}

void Widget::on_pb_Clear_clicked(){
    ui->pte_Message->clear();
}


void Widget::updateButtonStates() {
    state = socket_.state();
    ui->pb_Connect->setEnabled(state == QAbstractSocket::UnconnectedState);
    ui->pb_Disconnect->setEnabled(state == QAbstractSocket::ConnectedState);
    ui->pb_Send->setEnabled(state == QAbstractSocket::ConnectedState);
}

void Widget::saveSettings() {

    settings.setValue("webcli/Host", ui->le_Host->text());
    settings.setValue("webcli/Port", ui->le_Port->text().toShort());
    settings.setValue("webcli/SendMsg", ui->pte_Send->toPlainText().toUtf8());
    settings.setValue("webcli/isSSL", ui->cb_SSL->isChecked());
    settings.setValue("webcli/position", this->pos());
}

void Widget::loadSettings() {

    if (fileinfo.exists(CONF_PATH)) {
        ui->le_Host->setText(settings.value("webcli/Host").toString());
        ui->le_Port->setText(settings.value("webcli/Port").toString());
        ui->pte_Send->setPlainText(settings.value("webcli/SendMsg").toString());
        ui->cb_SSL->setChecked(settings.value("webcli/isSSL").toBool());

        QPoint pos = settings.value("webcli/position", QPoint(200,200)).toPoint();

        this->move(pos);
    }
}

