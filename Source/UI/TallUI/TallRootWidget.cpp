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

    Chatrooms cr("Chatrooms");

    SWProperties SWProps;
    SWProps.isHorizontal = false;
    SWProps.spacing      = Qt_Gen::sizePerc(0.5).width();
    SWProps.styleVariant = SWProperties::SV_Queue;

    SlideWidget* LANsSW  = new SlideWidget(this, SWProps);
    Qt_Gen::setBackgroundColor(LANsSW, QColor(70, 70, 70));
    layout->addWidget(LANsSW);

    QList<QWidget*> addToSW;
    for (const Chatrooms::LANData& LAN : cr.LANs)
    {
        CaptionWidget* cw = new CaptionWidget;
        cw->targetSize  = {1000, 500};
        cw->image = LAN.cover;
        cw->imageDD.borderRadius = {0, 0};
        cw->textDD.borderRadius  = {0, 0};
        cw->mainText.text = LAN.name;
        cw->subText.text  = LAN.message;
        //cw->setMinimumSize(200, 200); // TODO: There is an issue with this.
        cw->setup();

        addToSW.append(cw);
    }

    LANsSW->addWidget(addToSW);
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
