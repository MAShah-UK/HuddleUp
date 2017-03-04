#include "TallRootWidget.h"

#include <QVBoxLayout>
#include <qDebug>

#include "../SlideWidget/SlideWidget.h"
#include "../CaptionWidget/CaptionWidget.h"
#include "../Chatrooms/Chatrooms.h"

TallRootWidget::TallRootWidget(QWidget* parent)
    : QWidget(parent)
{
    layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setLayout(layout);

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
    SWProps.spacing       = Qt_Gen::sizePerc(1).height();
    SWProps.styleVariant  = SWProperties::SV_Single;

    SlideWidget* slideWidget = new SlideWidget(this, SWProps);
    Qt_Gen::setBackgroundColor(slideWidget, QColor(50, 50, 50));
    layout->addWidget(slideWidget);

    // Add menu icons.

    //CWProperties CWProps;
    //CWProps.parent = this;
    //CWProps.imagePath = "://Resources/Icons/MenuBar/Chats.png";
    //CWProps.imagePath = "://Resources/Icons/MenuBar/Media.png";
    //CWProps.imagePath = "://Resources/Icons/MenuBar/Settings.png";
    //CWProps.imagePath = "://Resources/Icons/MenuBar/Search.png";
    //CWProps.imagePath = "://Resources/Icons/MenuBar/stuff.png";

    //CaptionWidget captionWidget(CWProps);

    //CaptionWidget captionWidget(this);

    //slideWidget->addWidget({captionWidget.getLabel(), captionWidget.getLabel(),
    //                        captionWidget.getLabel(), captionWidget.getLabel()});
}

void TallRootWidget::setupChatsMenu()
{

    // Load chat data from files.

    Chatrooms* chatrooms = new Chatrooms; // TODO: Memory leak.
    Qt_Gen::setBackgroundColor(chatrooms->LANsSW(), QColor(70, 70, 70));
    layout->addWidget(chatrooms->LANsSW());
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
