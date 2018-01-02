#include "serial_port_helper.h"

SerialPortHelper::SerialPortHelper(QSerialPortInfo &info, QObject *parent) : QObject(parent)
{
    this->info = info;
}

SerialPortHelper::~SerialPortHelper()
{

}

bool SerialPortHelper::isConnected() const
{
    return state;
}

void SerialPortHelper::setReadBufferSize(int size)
{
    bufferSize = size;
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
        handle.setReadBufferSize(bufferSize);
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

void SerialPortHelper::send(Protocol &message)
{
    handle.write(message.getQByteArray());
}

void SerialPortHelper::send(QString &message)
{
    QByteArray array = message.toLatin1();
    handle.write(array);
}

void SerialPortHelper::on_message_received()
{
    QByteArray array = handle.readAll();
    QString message = array;
    emit receiveMessage(message);
    QByteArray buffer;
    if(!readFrameGroup(&handle, &buffer))
    {
        return;
    }
    if(buffer.size() == Interaction::PROTOCOL_SIZE)
    {
        Interaction data;
        data.setData(buffer);
        // 校验CRC
        if(data.CheckCRC())
        {
            emit receiveGroup(data);
        }
    }
    else if(buffer.size() == Information::PROTOCOL_SIZE)
    {
        Information data;
        data.setData(buffer);
        // 校验CRC
        if(data.CheckCRC())
        {
            emit receiveGroup(data);
        }
    }
}

bool SerialPortHelper::readFrameGroup(QSerialPort *handle, QByteArray *buffer)
{
    bool findHead = false;
    int protocolSize = 0;
    BYTE2 endFrame;
    for(int i = 0; i < 46; i += 2)
    {
        if(!findHead)
        {
            // 查寻起始帧，每次读取2个字节
            char frame[2];
            handle->read(frame, 2);
            // 构建BYTE2
            BYTE2 frameByte = (BYTE2)(frame[0] | (frame[1] << 8));
            if(Interaction::START_FRAME == frameByte)
            {
                findHead = true;
                protocolSize = Interaction::PROTOCOL_SIZE;
                endFrame = Interaction::END_FRAME;
                buffer->append(frame, 2);
                continue;
            }
            else if(Information::START_FRAME == frameByte)
            {
                findHead = true;
                protocolSize = Information::PROTOCOL_SIZE;
                endFrame = Information::END_FRAME;
                buffer->append(frame, 2);
                continue;
            }
        }
        else
        {
            // 读取非起始/结束帧协议
            char *frame = (char*)malloc(protocolSize - 4);
            handle->read(frame, protocolSize - 4);
            buffer->append(frame, protocolSize - 4);
            free(frame);
            // 读取结束帧
            char readEndFrame[2];
            handle->read(readEndFrame, 2);
            buffer->append(readEndFrame, 2);
            return (BYTE2)(readEndFrame[0] | (readEndFrame[1] << 8)) == endFrame;
        }
    }
    return false;
}

