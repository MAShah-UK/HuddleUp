#include "TallRootWidget.h"

#include <QVBoxLayout>
#include <qDebug>

#include "../CaptionWidget/CaptionWidget.h"

#include "../SlideWidget/SlideWidget.h"

TallRootWidget::TallRootWidget(QWidget* parent)
    : QWidget(parent)
{
    layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
    // BUG?: Calling layout->setAlignment(...) hid the widgets from setupMenuBar somehow.

    setupMenuBar();
    setupChatsMenu();
    setupMediaMenu();
    setupSettingsMenu();
    setupSearchMenu();
}

void TallRootWidget::setupMenuBar()
{
    // Setup slideWidget.

    SWProperties SWProps;
    SWProps.shouldUpscale = true;
    SWProps.spacing       = Qt_Gen::sizePerc(10, size()).height();
    SWProps.styleVariant  = SWProperties::SV_Single;

    slideWidget = new SlideWidget(this, SWProps);
    Qt_Gen::setBackgroundColor(slideWidget, QColor(50, 50, 50));
    layout->addWidget(slideWidget);

    // Add menu icons.

    CWProperties CWProps;
    CWProps.parent = this;
    CWProps.maxScreenPercentage = 10;
    CWProps.


    slideWidget->addWidget({
    Qt_Gen::iconWidget("://Resources/Icons/MenuBar/Chats.png",    length, 10),
    Qt_Gen::iconWidget("://Resources/Icons/MenuBar/Media.png",    length, 10),
    Qt_Gen::iconWidget("://Resources/Icons/MenuBar/Settings.png", length, 10),
    Qt_Gen::iconWidget("://Resources/Icons/MenuBar/Search.png",   length, 10)});
}

void TallRootWidget::setupChatsMenu()
{


    QVBoxLayout* chatsLayout = new QVBoxLayout;
}

void TallRootWidget::setupMediaMenu()
{

}

void TallRootWidget::setupSettingsMenu()
{

}

void TallRootWidget::setupSearchMenu()
{

}
