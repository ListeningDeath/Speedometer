#ifndef SERIALPORTHELP_H
#define SERIALPORTHELP_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "protocol/interaction.h"
#include "protocol/information.h"

class SerialPortHelper : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortHelper(QSerialPortInfo&, QObject *parent = nullptr);
    ~SerialPortHelper();
    bool isConnected() const;
    void setReadBufferSize(int);
    void setBaudRate(QSerialPort::BaudRate);
    void setDataBits(QSerialPort::DataBits);
    void setStopBits(QSerialPort::StopBits);
    void setParity(QSerialPort::Parity);
    void setFlowControl(QSerialPort::FlowControl);
    bool open();
    void close();
    void send(Protocol&);
    void send(QString&);

private:
    QSerialPort handle;
    QSerialPortInfo info;
    bool state;
    int bufferSize;
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::StopBits stopBits;
    QSerialPort::Parity parity;
    QSerialPort::FlowControl flowControl;
    bool readFrameGroup(QSerialPort*, QByteArray*);

signals:
    void receiveMessage(QString&);
    void protocolReady(Protocol&);

private slots:
    void on_message_received();
};

#endif // SERIALPORTHELP_H
