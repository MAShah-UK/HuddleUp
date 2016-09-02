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
    SWProps.shouldUpscale = false;
    SWProps.spacing       = Qt_Gen::sizePerc(1).height();
    SWProps.styleVariant  = SWProperties::SV_Single;

    slideWidget = new SlideWidget(this, SWProps);
    Qt_Gen::setBackgroundColor(slideWidget, QColor(50, 50, 50));
    layout->addWidget(slideWidget);

    // Add menu icons.

    CWProperties CWProps(this);
    CWProps.imagePath = "://Resources/Icons/MenuBar/Chats.png";
    CWProps.imagePath = "://Resources/Icons/MenuBar/Media.png";
    CWProps.imagePath = "://Resources/Icons/MenuBar/Settings.png";
    CWProps.imagePath = "://Resources/Icons/MenuBar/Search.png";
    CWProps.imagePath = "://Resources/Icons/MenuBar/stuff.png";

    CaptionWidget captionWidget(CWProps);

    slideWidget->addWidget({captionWidget(), captionWidget(),
                            captionWidget(), captionWidget()});
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
