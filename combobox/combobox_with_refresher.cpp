#include "combobox_with_refresher.h"

ComboBoxWithRefresher::ComboBoxWithRefresher(QWidget *parent) : QComboBox(parent)
{

}

void ComboBoxWithRefresher::showPopup()
{
    emit popupShow();
    QComboBox::showPopup();
}
