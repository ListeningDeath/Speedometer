/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGroupBox *gbSerialPortInfo;
    QComboBox *cbSerialPortName;
    QLabel *lblSerialPortName;
    QComboBox *cbSerialPortBaudRate;
    QLabel *lblSerialPortBaudRate;
    QComboBox *cbSerialPortDataBits;
    QLabel *lblSerialPortDataBit;
    QComboBox *cbSerialPortStopBits;
    QLabel *lblSerialPortStopBit;
    QComboBox *cbSerialPortParity;
    QLabel *lblSerialPortCheck;
    QPushButton *btnSerialPortConnect;
    QLabel *lblSerialPortState;
    QLabel *lblSerialPortFlowControl;
    QComboBox *cbSerialPortFlowControl;
    QGroupBox *gbMessage;
    QLineEdit *txtMessageSend;
    QPushButton *btnMessageSend;
    QTextEdit *txtMessageReceived;
    QPushButton *btnMessageEmpty;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(684, 283);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gbSerialPortInfo = new QGroupBox(centralWidget);
        gbSerialPortInfo->setObjectName(QStringLiteral("gbSerialPortInfo"));
        gbSerialPortInfo->setGeometry(QRect(10, 10, 201, 261));
        cbSerialPortName = new QComboBox(gbSerialPortInfo);
        cbSerialPortName->setObjectName(QStringLiteral("cbSerialPortName"));
        cbSerialPortName->setGeometry(QRect(80, 30, 111, 22));
        lblSerialPortName = new QLabel(gbSerialPortInfo);
        lblSerialPortName->setObjectName(QStringLiteral("lblSerialPortName"));
        lblSerialPortName->setGeometry(QRect(20, 30, 51, 16));
        cbSerialPortBaudRate = new QComboBox(gbSerialPortInfo);
        cbSerialPortBaudRate->setObjectName(QStringLiteral("cbSerialPortBaudRate"));
        cbSerialPortBaudRate->setGeometry(QRect(80, 60, 111, 22));
        lblSerialPortBaudRate = new QLabel(gbSerialPortInfo);
        lblSerialPortBaudRate->setObjectName(QStringLiteral("lblSerialPortBaudRate"));
        lblSerialPortBaudRate->setGeometry(QRect(20, 60, 51, 16));
        cbSerialPortDataBits = new QComboBox(gbSerialPortInfo);
        cbSerialPortDataBits->setObjectName(QStringLiteral("cbSerialPortDataBits"));
        cbSerialPortDataBits->setGeometry(QRect(80, 90, 111, 22));
        lblSerialPortDataBit = new QLabel(gbSerialPortInfo);
        lblSerialPortDataBit->setObjectName(QStringLiteral("lblSerialPortDataBit"));
        lblSerialPortDataBit->setGeometry(QRect(20, 90, 51, 16));
        cbSerialPortStopBits = new QComboBox(gbSerialPortInfo);
        cbSerialPortStopBits->setObjectName(QStringLiteral("cbSerialPortStopBits"));
        cbSerialPortStopBits->setGeometry(QRect(80, 120, 111, 22));
        lblSerialPortStopBit = new QLabel(gbSerialPortInfo);
        lblSerialPortStopBit->setObjectName(QStringLiteral("lblSerialPortStopBit"));
        lblSerialPortStopBit->setGeometry(QRect(20, 120, 51, 16));
        cbSerialPortParity = new QComboBox(gbSerialPortInfo);
        cbSerialPortParity->setObjectName(QStringLiteral("cbSerialPortParity"));
        cbSerialPortParity->setGeometry(QRect(80, 150, 111, 22));
        lblSerialPortCheck = new QLabel(gbSerialPortInfo);
        lblSerialPortCheck->setObjectName(QStringLiteral("lblSerialPortCheck"));
        lblSerialPortCheck->setGeometry(QRect(20, 150, 51, 16));
        btnSerialPortConnect = new QPushButton(gbSerialPortInfo);
        btnSerialPortConnect->setObjectName(QStringLiteral("btnSerialPortConnect"));
        btnSerialPortConnect->setGeometry(QRect(100, 230, 91, 21));
        lblSerialPortState = new QLabel(gbSerialPortInfo);
        lblSerialPortState->setObjectName(QStringLiteral("lblSerialPortState"));
        lblSerialPortState->setGeometry(QRect(20, 230, 71, 16));
        lblSerialPortFlowControl = new QLabel(gbSerialPortInfo);
        lblSerialPortFlowControl->setObjectName(QStringLiteral("lblSerialPortFlowControl"));
        lblSerialPortFlowControl->setGeometry(QRect(20, 180, 51, 16));
        cbSerialPortFlowControl = new QComboBox(gbSerialPortInfo);
        cbSerialPortFlowControl->setObjectName(QStringLiteral("cbSerialPortFlowControl"));
        cbSerialPortFlowControl->setGeometry(QRect(80, 180, 111, 22));
        gbMessage = new QGroupBox(centralWidget);
        gbMessage->setObjectName(QStringLiteral("gbMessage"));
        gbMessage->setGeometry(QRect(220, 10, 451, 261));
        txtMessageSend = new QLineEdit(gbMessage);
        txtMessageSend->setObjectName(QStringLiteral("txtMessageSend"));
        txtMessageSend->setGeometry(QRect(10, 230, 231, 20));
        btnMessageSend = new QPushButton(gbMessage);
        btnMessageSend->setObjectName(QStringLiteral("btnMessageSend"));
        btnMessageSend->setGeometry(QRect(250, 230, 91, 21));
        txtMessageReceived = new QTextEdit(gbMessage);
        txtMessageReceived->setObjectName(QStringLiteral("txtMessageReceived"));
        txtMessageReceived->setGeometry(QRect(10, 30, 431, 191));
        txtMessageReceived->setReadOnly(true);
        btnMessageEmpty = new QPushButton(gbMessage);
        btnMessageEmpty->setObjectName(QStringLiteral("btnMessageEmpty"));
        btnMessageEmpty->setGeometry(QRect(350, 230, 91, 21));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        gbSerialPortInfo->setTitle(QApplication::translate("MainWindow", "\344\270\262\345\217\243\351\205\215\347\275\256", nullptr));
        lblSerialPortName->setText(QApplication::translate("MainWindow", "\345\220\215\347\247\260", nullptr));
        lblSerialPortBaudRate->setText(QApplication::translate("MainWindow", "\346\263\242\347\211\271\347\216\207", nullptr));
        lblSerialPortDataBit->setText(QApplication::translate("MainWindow", "\346\225\260\346\215\256\344\275\215", nullptr));
        lblSerialPortStopBit->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242\344\275\215", nullptr));
        lblSerialPortCheck->setText(QApplication::translate("MainWindow", "\345\245\207\345\201\266\346\240\241\351\252\214", nullptr));
        btnSerialPortConnect->setText(QApplication::translate("MainWindow", "\350\277\236\346\216\245(&C)", nullptr));
        lblSerialPortState->setText(QApplication::translate("MainWindow", "Status", nullptr));
        lblSerialPortFlowControl->setText(QApplication::translate("MainWindow", "\346\265\201\346\216\247\345\210\266", nullptr));
        gbMessage->setTitle(QApplication::translate("MainWindow", "\346\266\210\346\201\257", nullptr));
        btnMessageSend->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201(&S)", nullptr));
        btnMessageEmpty->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272(&E)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
