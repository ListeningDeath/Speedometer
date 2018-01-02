#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtCharts/QtCharts>
#include <QList>
#include "serial_port_helper.h"

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SerialPortHelper *spHelper;
    QChart *myChart;
    QLineSeries *myLine;
    QList<QPointF> pointList;
    void changeState(bool);
    void initCombobox();
    void initChart();
    void printCaliText(Protocol&);
    void printDataText(Protocol&);
    void printPoint(Protocol&);
    void readCali();
    void writeCali(float, float, float, float, float, float, float, float, float);

public slots:
    void protocolReceived(Protocol&);

private slots:
    void on_btnSerialPortConnect_clicked();

    void on_btnCaliVerify_clicked();
    void on_btnCaliReset_clicked();
};

#endif // MAINWINDOW_H
