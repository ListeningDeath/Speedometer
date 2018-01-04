#include <QMessageBox>
#include <QDebug>
#include <QQueue>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "protocol/interaction.h"
#include "protocol/information.h"

Q_DECLARE_METATYPE(QSerialPortInfo)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    sp(new QSerialPort(this))
{
    ui->setupUi(this);
    setWindowFlags((this->windowFlags()&~Qt::WindowMinMaxButtonsHint)|Qt::WindowMinimizeButtonHint);
    setFixedSize(width(), height());

    //读取串口信息
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QVariant var;
        var.setValue(info);
        ui->cbSerialPortName->addItem(info.portName(), var);
    }

    connect(this, &MainWindow::getProtocol, this, &MainWindow::protocolDeal);

    //初始化
    initCombobox();
    initChart();
    changeState(false);

//    sp->setPortName("COM3");
//    sp->setBaudRate(QSerialPort::Baud2400);
//    sp->setDataBits(QSerialPort::Data8);
//    sp->setStopBits(QSerialPort::OneStop);
//    sp->setParity(QSerialPort::NoParity);
//    sp->setFlowControl(QSerialPort::NoFlowControl);
//    sp->open(QIODevice::ReadWrite);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeState(bool isConnected)
{
    ui->gbSerialPortInfo->setEnabled(!isConnected);
    ui->lblSerialPortState->setText(QString(isConnected ? "已连接" : "未连接"));
    ui->btnSerialPortConnect->setText(QString(isConnected ? "断开(&D)" : "连接(&C)"));
    ui->gbCalibration->setEnabled(isConnected);
    ui->gbDataCollection->setEnabled(isConnected);
}

void MainWindow::initCombobox()
{
    ui->cbSerialPortBaudRate->addItem(QString("1200"), QSerialPort::Baud1200);
    ui->cbSerialPortBaudRate->addItem(QString("2400"), QSerialPort::Baud2400);
    ui->cbSerialPortBaudRate->addItem(QString("4800"), QSerialPort::Baud4800);
    ui->cbSerialPortBaudRate->addItem(QString("9600"), QSerialPort::Baud9600);
    ui->cbSerialPortBaudRate->addItem(QString("19200"), QSerialPort::Baud19200);
    ui->cbSerialPortBaudRate->addItem(QString("38400"), QSerialPort::Baud38400);
    ui->cbSerialPortBaudRate->addItem(QString("57600"), QSerialPort::Baud57600);
    ui->cbSerialPortBaudRate->addItem(QString("115200"), QSerialPort::Baud115200);
    ui->cbSerialPortBaudRate->setCurrentText("2400");

    ui->cbSerialPortDataBits->addItem(QString("5"), QSerialPort::Data5);
    ui->cbSerialPortDataBits->addItem(QString("6"), QSerialPort::Data6);
    ui->cbSerialPortDataBits->addItem(QString("7"), QSerialPort::Data7);
    ui->cbSerialPortDataBits->addItem(QString("8"), QSerialPort::Data8);
    ui->cbSerialPortDataBits->setCurrentText("8");

    ui->cbSerialPortStopBits->addItem(QString("1"), QSerialPort::OneStop);
    ui->cbSerialPortStopBits->addItem(QString("1.5"), QSerialPort::OneAndHalfStop);
    ui->cbSerialPortStopBits->addItem(QString("2"), QSerialPort::TwoStop);

    ui->cbSerialPortParity->addItem(QString("None"), QSerialPort::NoParity);
    ui->cbSerialPortParity->addItem(QString("Even"), QSerialPort::EvenParity);
    ui->cbSerialPortParity->addItem(QString("Odd"), QSerialPort::OddParity);
    ui->cbSerialPortParity->addItem(QString("Space"), QSerialPort::SpaceParity);
    ui->cbSerialPortParity->addItem(QString("Mark"), QSerialPort::MarkParity);

    ui->cbSerialPortFlowControl->addItem(QString("None"), QSerialPort::NoFlowControl);
    ui->cbSerialPortFlowControl->addItem(QString("Hardware"), QSerialPort::HardwareControl);
    ui->cbSerialPortFlowControl->addItem(QString("Software"), QSerialPort::SoftwareControl);
}

void MainWindow::initChart()
{
//    // 测试用样点
//    *ui->chartView->series() << QPointF(1300, 8000);
//    *ui->chartView->series() << QPointF(1800, 9000);
//    *ui->chartView->series() << QPointF(1200, 14000);
//    *ui->chartView->series() << QPointF(1800, 18000);
}

void MainWindow::printCaliText(Protocol &data)
{
    ui->txtCaliSoundSpeed->setText(QString::number(data.getSoundSpeedFrame()));
    ui->txtCaliTemperature->setText(QString::number(data.getTemperatureFrame()));
    ui->txtCaliPressure->setText(QString::number(data.getPressureFrame()));
    ui->txtCaliQuat1->setText(QString::number(data.getQuat1Frame()));
    ui->txtCaliQuat2->setText(QString::number(data.getQuat2Frame()));
    ui->txtCaliQuat3->setText(QString::number(data.getQuat3Frame()));
    ui->txtCaliQuat4->setText(QString::number(data.getQuat4Frame()));
    ui->txtCaliVertSpeed->setText(QString::number(data.getVerticalSpeedFrame()));
    ui->txtCaliVoltage->setText(QString::number(data.getVoltageFrame()));
}

void MainWindow::printDataText(Protocol &data)
{
    ui->txtDataSoundSpeed->setText(QString::number(data.getSoundSpeedFrame()));
    ui->txtDataTemperature->setText(QString::number(data.getTemperatureFrame()));
    ui->txtDataPressure->setText(QString::number(data.getPressureFrame()));
    ui->txtDataVertSpeed->setText(QString::number(data.getVerticalSpeedFrame()));
    ui->txtDataQuat1->setText(QString::number(data.getQuat1Frame()));
    ui->txtDataQuat2->setText(QString::number(data.getQuat2Frame()));
    ui->txtDataQuat3->setText(QString::number(data.getQuat3Frame()));
    ui->txtDataQuat4->setText(QString::number(data.getQuat4Frame()));
}

void MainWindow::printPoint(Protocol &data)
{
    QPointF point(data.getPressureFrame(), data.getSoundSpeedFrame());
    pointList << point;
    *ui->chartView->series() << point;
}

void MainWindow::readCali()
{
    Interaction protocol;
    protocol.setCommandFrame(Interaction::READ_COMMAND);
    protocol.setCRCFrame();
    sendProtocol(&protocol);
}

void MainWindow::writeCali(float soundSpeed, float temperature,
                           float pressure, float quat1, float quat2,
                           float quat3, float quat4,
                           float verticalSpeed, float voltage)
{
    Interaction protocol;
    protocol.setCommandFrame(Interaction::WRITE_COMMAND);
    protocol.setSoundSpeedFrame(soundSpeed);
    protocol.setTemperatureFrame(temperature);
    protocol.setPressureFrame(pressure);
    protocol.setQuat1Frame(quat1);
    protocol.setQuat2Frame(quat2);
    protocol.setQuat3Frame(quat3);
    protocol.setQuat4Frame(quat4);
    protocol.setVerticalSpeedFrame(verticalSpeed);
    protocol.setVoltageFrame(voltage);
    protocol.setCRCFrame();
    sendProtocol(&protocol);
}

void MainWindow::on_btnSerialPortConnect_clicked()
{
    if(sp->isOpen() == false)
    {
        if(ui->cbSerialPortName->currentIndex() == -1)
        {
            QMessageBox::critical(NULL, "错误", "未选择任何串口");
            return;
        }
        //打开串口
        QSerialPortInfo info = ui->cbSerialPortName->currentData().value<QSerialPortInfo>();
        sp->setPortName(info.portName());
        sp->setBaudRate(QSerialPort::BaudRate(ui->cbSerialPortBaudRate->currentData().toInt()));
        sp->setDataBits(QSerialPort::DataBits(ui->cbSerialPortDataBits->currentData().toInt()));
        sp->setStopBits(QSerialPort::StopBits(ui->cbSerialPortStopBits->currentData().toInt()));
        sp->setParity(QSerialPort::Parity(ui->cbSerialPortParity->currentData().toInt()));
        sp->setFlowControl(QSerialPort::FlowControl(ui->cbSerialPortFlowControl->currentData().toInt()));
        if(!sp->open(QIODevice::ReadWrite))
        {
            QMessageBox::critical(NULL, "错误", "串口打开失败");
            return;
        }
        //绑定数据接收槽
        QObject::connect(sp, &QSerialPort::readyRead, this, &MainWindow::storeBytes);
        changeState(true);
        // 发送请求读取校准值协议
        readCali();
    }
    else
    {
        sp->close();
        changeState(false);
    }
}

void MainWindow::protocolDeal()
{
    if(m_protocol.size() == Interaction::PROTOCOL_SIZE)
    {
        Interaction data;
        data.setData(m_protocol);
        // 校验CRC
        if(data.CheckCRC())
        {
            printCaliText(data);
        }
    }
    else if(m_protocol.size() == Information::PROTOCOL_SIZE)
    {
        Information data;
        data.setData(m_protocol);
        // 校验CRC
        if(data.CheckCRC())
        {
            printDataText(data);
            printPoint(data);
        }
    }
}

void MainWindow::storeBytes()
{
    receiverMutex.lock();
    receivedBytes.append(sp->readAll());
    int startFrameLocation, endFrameLocation;
    if((startFrameLocation = findFrameOf(true)) != -1)
    {
        receivedBytes.remove(0, startFrameLocation);
    }
    if((endFrameLocation = findFrameOf(false)) != -1 && startFrameLocation != -1)
    {
        m_protocol = receivedBytes.mid(0, endFrameLocation - startFrameLocation + 1);
        receivedBytes.remove(0, endFrameLocation);
        qDebug() << "find end" << m_protocol;
        emit getProtocol();
    }
    receiverMutex.unlock();
}

int MainWindow::findFrameOf(bool isStart)
{
    int index = -1;
    BYTE2 symbolFrames[2] =
    {isStart ? Interaction::START_FRAME : Interaction::END_FRAME,
     isStart ? Information::START_FRAME : Information::END_FRAME};
    for(int i = 0; i < receivedBytes.size(); i++)
    {
        BYTE2 t = (BYTE2)((receivedBytes[i + 1] << 8) |
                          (receivedBytes[i] & 0xFF));
        if(t == symbolFrames[0] || t == symbolFrames[1])
        {
            index = isStart ? i : i + 1;
            break;
        }
    }
    return index;
}

void MainWindow::sendProtocol(Protocol* protocol)
{
    sp->write(protocol->getQByteArray());
}

void MainWindow::on_btnCaliVerify_clicked()
{
    writeCali(ui->txtCaliSoundSpeed->text().toFloat(),
              ui->txtCaliTemperature->text().toFloat(),
              ui->txtCaliPressure->text().toFloat(),
              ui->txtCaliQuat1->text().toFloat(),
              ui->txtCaliQuat2->text().toFloat(),
              ui->txtCaliQuat3->text().toFloat(),
              ui->txtCaliQuat4->text().toFloat(),
              ui->txtCaliVertSpeed->text().toFloat(),
              ui->txtCaliVoltage->text().toFloat());
}

void MainWindow::on_btnCaliReset_clicked()
{
    writeCali(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void MainWindow::on_btnCaliRefresh_clicked()
{
    readCali();
}
