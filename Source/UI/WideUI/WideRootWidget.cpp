#include "WideRootWidget.h"

#include <QVBoxLayout>

WideRootWidget::WideRootWidget(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout;
    setLayout(layout);
}
