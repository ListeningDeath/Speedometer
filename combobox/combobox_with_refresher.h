#ifndef COMBOBOX_WITH_REFRESHER_H
#define COMBOBOX_WITH_REFRESHER_H
#include <QComboBox>


class ComboBoxWithRefresher : public QComboBox
{
    Q_OBJECT

public:
    ComboBoxWithRefresher(QWidget *parent = nullptr);

protected:
    void showPopup();

signals:
    void popupShow();
};

#endif // COMBOBOX_WITH_REFRESHER_H
