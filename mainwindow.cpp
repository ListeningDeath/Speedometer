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
    m_serial_port(new QSerialPort(this)),
    m_intValidator(new QIntValidator(0, 4000000, this))
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

    connect(this, &MainWindow::GetProtocol, this, &MainWindow::ProtocolDeal);

    //初始化
    SerialPortConfigInit();
    ChartInit();
    SetState(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetState(bool isConnected)
{
    ui->gbSerialPortInfo->setEnabled(!isConnected);
    ui->lblSerialPortState->setText(QString(isConnected ? "已连接" : "未连接"));
    ui->btnSerialPortConnect->setText(QString(isConnected ? "断开(&D)" : "连接(&C)"));
    ui->gbCalibration->setEnabled(isConnected);
    ui->gbDataCollection->setEnabled(isConnected);
}

void MainWindow::SerialPortConfigInit()
{
    ui->cbSerialPortBaudRate->addItem(QString("1200"), QSerialPort::Baud1200);
    ui->cbSerialPortBaudRate->addItem(QString("2400"), QSerialPort::Baud2400);
    ui->cbSerialPortBaudRate->addItem(QString("4800"), QSerialPort::Baud4800);
    ui->cbSerialPortBaudRate->addItem(QString("9600"), QSerialPort::Baud9600);
    ui->cbSerialPortBaudRate->addItem(QString("19200"), QSerialPort::Baud19200);
    ui->cbSerialPortBaudRate->addItem(QString("38400"), QSerialPort::Baud38400);
    ui->cbSerialPortBaudRate->addItem(QString("57600"), QSerialPort::Baud57600);
    ui->cbSerialPortBaudRate->addItem(QString("115200"), QSerialPort::Baud115200);
    ui->cbSerialPortBaudRate->addItem(QString("Custom"));
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

void MainWindow::ChartInit()
{
//    // 测试用样点
//    *ui->chartView->series() << QPointF(1300, 8000);
//    *ui->chartView->series() << QPointF(1800, 9000);
//    *ui->chartView->series() << QPointF(1200, 14000);
//    *ui->chartView->series() << QPointF(1800, 18000);
}

void MainWindow::PrintCaliText(Protocol *data)
{
    ui->txtCaliSoundSpeed->setText(QString::number(data->GetSoundSpeedFrame()));
    ui->txtCaliTemperature->setText(QString::number(data->GetTemperatureFrame()));
    ui->txtCaliPressure->setText(QString::number(data->GetPressureFrame()));
    ui->txtCaliQuat1->setText(QString::number(data->GetQuat1Frame()));
    ui->txtCaliQuat2->setText(QString::number(data->GetQuat2Frame()));
    ui->txtCaliQuat3->setText(QString::number(data->GetQuat3Frame()));
    ui->txtCaliQuat4->setText(QString::number(data->GetQuat4Frame()));
    ui->txtCaliVertSpeed->setText(QString::number(data->GetVerticalSpeedFrame()));
    ui->txtCaliVoltage->setText(QString::number(data->GetVoltageFrame()));
}

void MainWindow::PrintDataText(Protocol *data)
{
    ui->txtDataSoundSpeed->setText(QString::number(data->GetSoundSpeedFrame()));
    ui->txtDataTemperature->setText(QString::number(data->GetTemperatureFrame()));
    ui->txtDataPressure->setText(QString::number(data->GetPressureFrame()));
    ui->txtDataVertSpeed->setText(QString::number(data->GetVerticalSpeedFrame()));
    ui->txtDataQuat1->setText(QString::number(data->GetQuat1Frame()));
    ui->txtDataQuat2->setText(QString::number(data->GetQuat2Frame()));
    ui->txtDataQuat3->setText(QString::number(data->GetQuat3Frame()));
    ui->txtDataQuat4->setText(QString::number(data->GetQuat4Frame()));
}

void MainWindow::PrintPoint(Protocol *data)
{
    QPointF point(data->GetPressureFrame(), data->GetSoundSpeedFrame());
    m_points << point;
    *ui->chartView->series() << point;
}

void MainWindow::ReadCali()
{
    Interaction *data = new Interaction(this);
    data->SetCommandFrame(Interaction::ReadCommand);
    data->SetCRCFrame();
    SendProtocol(data);
    delete data;
}

void MainWindow::WriteCali(float soundSpeed,
                           float temperature,
                           float pressure,
                           float quat1,
                           float quat2,
                           float quat3,
                           float quat4,
                           float verticalSpeed,
                           float voltage)
{
    Interaction *data = new Interaction(this);
    data->SetCommandFrame(Interaction::WriteCommand);
    data->SetSoundSpeedFrame(soundSpeed);
    data->SetTemperatureFrame(temperature);
    data->SetPressureFrame(pressure);
    data->SetQuat1Frame(quat1);
    data->SetQuat2Frame(quat2);
    data->SetQuat3Frame(quat3);
    data->SetQuat4Frame(quat4);
    data->SetVerticalSpeedFrame(verticalSpeed);
    data->SetVoltageFrame(voltage);
    data->SetCRCFrame();
    SendProtocol(data);
    delete data;
}

void MainWindow::ReadProtocol()
{
    m_receiver.append(m_serial_port->readAll());
    int startFrameLocation, endFrameLocation;
    int startFrameProtocolType, endFrameProtocolType;
    if((startFrameLocation = FindFrameOf(true, &startFrameProtocolType)) != -1)
    {
        m_receiver.remove(0, startFrameLocation);
    }
    if((endFrameLocation = FindFrameOf(false, &endFrameProtocolType)) != -1 && startFrameLocation != -1)
    {
        if(startFrameProtocolType != endFrameProtocolType)
        {
            // 若首尾类型不一致，丢弃
            m_receiver.remove(0, endFrameLocation);
        }
        m_protocol = m_receiver.mid(0, endFrameLocation - startFrameLocation + 1);
        m_receiver.remove(0, endFrameLocation);
//        qDebug() << "find end" << m_protocol;
        emit GetProtocol(startFrameProtocolType);
    }
}

int MainWindow::FindFrameOf(bool isStart, int *protocolType)
{
    int index = -1;
    BYTE2 symbolFrames[2] =
    {isStart ? Interaction::StartFrame : Interaction::EndFrame,
     isStart ? Information::StartFrame : Information::EndFrame};
    for(int i = 0; i < m_receiver.size(); i++)
    {
        BYTE2 t = (BYTE2)((m_receiver[i + 1] << 8) |
                          (m_receiver[i] & 0xFF));
        if(t == symbolFrames[Protocol::InteractionProtocol])
        {
            index = isStart ? i : i + 1;
            *protocolType = Protocol::InteractionProtocol;
            break;
        }
        else if(t == symbolFrames[Protocol::InformationProtocol])
        {
            index = isStart ? i : i + 1;
            *protocolType = Protocol::InformationProtocol;
            break;
        }
    }
    return index;
}

void MainWindow::SendProtocol(Protocol* data)
{
    m_serial_port->write(data->GetQByteArray());
}

void MainWindow::ProtocolDeal(int protocolType)
{
    Protocol *data;
    if(protocolType == Protocol::InteractionProtocol)
    {
        data = new Interaction(this);
        data->SetData(m_protocol);
        // 校验CRC
        if(data->CheckCRC())
        {
            PrintCaliText(data);
        }
    }
    else
    {
        data = new Information(this);
        data->SetData(m_protocol);
        // 校验CRC
        if(data->CheckCRC())
        {
            PrintDataText(data);
            PrintPoint(data);
        }
    }
    delete data;
}

void MainWindow::on_btnSerialPortConnect_clicked()
{
    if(m_serial_port->isOpen() == false)
    {
        if(ui->cbSerialPortName->currentIndex() == -1)
        {
            QMessageBox::critical(NULL, "错误", "未选择任何串口");
            return;
        }
        //打开串口
        QSerialPortInfo info = ui->cbSerialPortName->currentData().value<QSerialPortInfo>();
        m_serial_port->setPortName(info.portName());
        if(ui->cbSerialPortBaudRate->currentIndex() == 8)
        {
            m_serial_port->setBaudRate(ui->cbSerialPortBaudRate->currentText().toInt());
        }
        else
        {
            m_serial_port->setBaudRate(QSerialPort::BaudRate(ui->cbSerialPortBaudRate->currentData().toInt()));
        }
        m_serial_port->setDataBits(QSerialPort::DataBits(ui->cbSerialPortDataBits->currentData().toInt()));
        m_serial_port->setStopBits(QSerialPort::StopBits(ui->cbSerialPortStopBits->currentData().toInt()));
        m_serial_port->setParity(QSerialPort::Parity(ui->cbSerialPortParity->currentData().toInt()));
        m_serial_port->setFlowControl(QSerialPort::FlowControl(ui->cbSerialPortFlowControl->currentData().toInt()));
        if(!m_serial_port->open(QIODevice::ReadWrite))
        {
            QMessageBox::critical(NULL, "错误", "串口打开失败");
            return;
        }
        //绑定数据接收槽
        QObject::connect(m_serial_port, &QSerialPort::readyRead, this, &MainWindow::ReadProtocol);
        SetState(true);
        // 发送请求读取校准值协议
        ReadCali();
    }
    else
    {
        m_serial_port->close();
        SetState(false);
    }
}

void MainWindow::on_btnCaliVerify_clicked()
{
    WriteCali(ui->txtCaliSoundSpeed->text().toFloat(),
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
    WriteCali(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void MainWindow::on_btnCaliRefresh_clicked()
{
    ReadCali();
}

void MainWindow::on_cbSerialPortBaudRate_currentIndexChanged(int index)
{
    if(!ui->cbSerialPortBaudRate->itemData(index).isValid())
    {
        ui->cbSerialPortBaudRate->setEditable(true);
        ui->cbSerialPortBaudRate->clearEditText();
        QLineEdit *edit = ui->cbSerialPortBaudRate->lineEdit();
        edit->setValidator(m_intValidator);
    }
    else
    {
        ui->cbSerialPortBaudRate->setEditable(false);
    }
}
