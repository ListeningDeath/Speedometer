#include "SerialPortHelper.h"

SerialPortHelper::SerialPortHelper(QSerialPortInfo &info)
{
    this->info = info;
}

SerialPortHelper::~SerialPortHelper()
{
    handle.~QSerialPort();
    delete &handle;
    info.~QSerialPortInfo();
    delete &info;
}

bool SerialPortHelper::isConnected()
{
    return state;
}

void SerialPortHelper::setBaudRate(QSerialPort::BaudRate value)
{
    baudRate = value;
}

void SerialPortHelper::setDataBits(QSerialPort::DataBits value)
{
    dataBits = value;
}

void SerialPortHelper::setStopBits(QSerialPort::StopBits value)
{
    stopBits = value;
}

void SerialPortHelper::setParity(QSerialPort::Parity value)
{
    parity = value;
}

void SerialPortHelper::setFlowControl(QSerialPort::FlowControl value)
{
    flowControl = value;
}

bool SerialPortHelper::open()
{
    handle.setPort(info);
    if(handle.open(QIODevice::ReadWrite))
    {
        state = true;
        handle.setBaudRate(baudRate);
        handle.setDataBits(dataBits);
        handle.setStopBits(stopBits);
        handle.setParity(parity);
        handle.setFlowControl(flowControl);
        handle.clearError();
        handle.clear();
        QObject::connect(&handle, SIGNAL(readyRead()), this, SLOT(on_message_received()));
        return true;
    }
    else
    {
        state = false;
        return false;
    }
}

void SerialPortHelper::close()
{
    handle.close();
    state = false;
}

void SerialPortHelper::send(QString& message)
{
    QByteArray array = message.toLatin1();
    handle.write(array);
}

void SerialPortHelper::on_message_received()
{
    QByteArray array = handle.readAll();
    QString message = QString(array);
    emit receiveMessage();
}
