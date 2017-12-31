#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "serial_port_helper.h"

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
    void changeState(bool);
    void initCombobox();

public slots:
    void on_message_received(QString&);
    void dataFrameReceived(Protocol&);

private slots:
    void on_btnSerialPortConnect_clicked();
    void on_btnMessageSend_clicked();
    void on_btnMessageEmpty_clicked();
};

#endif // MAINWINDOW_H
