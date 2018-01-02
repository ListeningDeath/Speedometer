#define READ_BUFFER_SIZE    2 * 1024
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>

Q_DECLARE_METATYPE(QSerialPortInfo)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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

    //初始化
    spHelper = nullptr;
    initCombobox();
    initChart();
    changeState(false);
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
//    for(float i = 0.0f; i < 50.0f; i++)
//    {
//        float j = i * i;
//        myLine<<QPointF(i, j);
//    }

    // 初始化QChart
    myChart = new QChart();
    // 初始化SerisLine
    myLine = new QLineSeries(myChart);
    myChart->addSeries(myLine);
    myChart->legend()->hide();
//    myChart->createDefaultAxes();
    myChart->setTitle("深度-声速");
    QValueAxis *axisX = new QValueAxis(myChart);
    QValueAxis *axisY = new QValueAxis(myChart);
    axisX->setRange(0, 6000);
    axisX->setTitleText("压力/Pa");
    axisY->setRange(0, 1000);
    axisY->setTitleText("声速/(m/s)");
    myChart->addAxis(axisX, Qt::AlignLeft);
    myChart->addAxis(axisY, Qt::AlignTop);
    myLine->attachAxis(axisX);
    myLine->attachAxis(axisY);

    // 初始化QChartView
    ui->chart->setChart(myChart);
    ui->chart->setRenderHint(QPainter::Antialiasing);
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
    pointList << QPointF(data.getSoundSpeedFrame(), data.getPressureFrame());
    pointList << QPointF(2, 3);
    pointList << QPointF(3, 8);
    pointList << QPointF(4, 20);
    pointList << QPointF(5, 80);
    myLine->replace(pointList);
}

void MainWindow::readCali()
{
    Interaction protocol;
    protocol.setCommandFrame(Interaction::READ_COMMAND);
    protocol.setCRCFrame();
    spHelper->send(protocol);
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
    spHelper->send(protocol);
}

void MainWindow::on_btnSerialPortConnect_clicked()
{
    if(spHelper == nullptr)
    {
        if(ui->cbSerialPortName->currentIndex() == -1)
        {
            QMessageBox::critical(NULL, "错误", "未选择任何串口");
            return;
        }
        //打开串口
        QSerialPortInfo info = ui->cbSerialPortName->currentData().value<QSerialPortInfo>();
        spHelper = new SerialPortHelper(info, this);
        spHelper->setReadBufferSize(READ_BUFFER_SIZE);
        spHelper->setBaudRate(QSerialPort::BaudRate(ui->cbSerialPortBaudRate->currentData().toInt()));
        spHelper->setDataBits(QSerialPort::DataBits(ui->cbSerialPortDataBits->currentData().toInt()));
        spHelper->setStopBits(QSerialPort::StopBits(ui->cbSerialPortStopBits->currentData().toInt()));
        spHelper->setParity(QSerialPort::Parity(ui->cbSerialPortParity->currentData().toInt()));
        spHelper->setFlowControl(QSerialPort::FlowControl(ui->cbSerialPortFlowControl->currentData().toInt()));
        spHelper->open();
        //绑定数据接收槽
        QObject::connect(spHelper, SIGNAL(receiveMessage(QString&)), this, SLOT(on_message_received(QString&)));
        QObject::connect(spHelper, SIGNAL(protocolReady(Protocol&)), this, SLOT(protocolReceived(Protocol&)));
        changeState(true);
        // 发送请求读取校准值协议
        readCali();
    }
    else
    {
        spHelper->close();
        delete spHelper;
        spHelper = nullptr;
        changeState(false);
        return;
    }
    if(spHelper != nullptr && !spHelper->isConnected())
    {
        spHelper->close();
        delete spHelper;
        spHelper = nullptr;
        changeState(false);
    }
}

void MainWindow::protocolReceived(Protocol &data)
{
    if(data.getType() == Protocol::INTERACTION_PROTOCOL)
    {
        printCaliText(data);
    }
    else if(data.getType() == Protocol::INFORMATION_PROTOCOL)
    {
        printDataText(data);
        printPoint(data);
    }
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
