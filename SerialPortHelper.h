#ifndef SERIALPORTHELP_H
#define SERIALPORTHELP_H

#include <QObject>
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>

class SerialPortHelper : public QObject
{
    Q_OBJECT
public:
    SerialPortHelper(QSerialPortInfo&);
    ~SerialPortHelper();
    bool isConnected();
    void setBaudRate(QSerialPort::BaudRate);
    void setDataBits(QSerialPort::DataBits);
    void setStopBits(QSerialPort::StopBits);
    void setParity(QSerialPort::Parity);
    void setFlowControl(QSerialPort::FlowControl);
    bool open();
    void close();
    void send(QString&);

private:
    QSerialPort handle;
    QSerialPortInfo info;
    bool state;
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::StopBits stopBits;
    QSerialPort::Parity parity;
    QSerialPort::FlowControl flowControl;

signals:
    void receiveMessage();

private slots:
    void on_message_received();
};

#endif // SERIALPORTHELP_H
