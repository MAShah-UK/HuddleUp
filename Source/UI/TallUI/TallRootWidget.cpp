#include "TallRootWidget.h"

#include <QVBoxLayout>
#include <qDebug>

#include "../SlideWidget/SlideWidget.h"
#include "../CaptionWidget/CaptionWidget.h"

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

    CaptionWidget captionWidget;
    CWProperties CWProps;
    CWProps.parent = this;
    CWProps.maxScreenPercentage = 10;
    CWProps.paths.append("://Resources/Icons/MenuBar/Chats.png");
    CWProps.paths.append("://Resources/Icons/MenuBar/Media.png");
    CWProps.paths.append("://Resources/Icons/MenuBar/Settings.png");
    CWProps.paths.append("://Resources/Icons/MenuBar/Search.png");

    slideWidget->addWidget({captionWidget(CWProps, 0), captionWidget(CWProps, 1),
                            captionWidget(CWProps, 2), captionWidget(CWProps, 3)});
}

void TallRootWidget::setupChatsMenu()
{

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
