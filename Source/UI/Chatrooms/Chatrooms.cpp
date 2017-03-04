#include "Chatrooms.h"

#include <qDebug>
#include <QDir>
#include <QDateTime>
#include <QStringList>

#include "../SlideWidget/SlideWidget.h"
#include "../CaptionWidget/CaptionWidget.h"

// TODO: Make it so that the chatroom widget only parses required information.
//       The actual designing should be done by another class.
Chatrooms::Chatrooms(QWidget* parent)
{
    SWProperties SWProps;
    SWProps.isHorizontal = false;
    SWProps.spacing      = Qt_Gen::sizePerc(0.5).width();
    SWProps.styleVariant = SWProperties::SV_Queue;
    _LANsSW              = new SlideWidget(parent, SWProps);

    QDir dir("Chatrooms");
    QFlags<QDir::Filter> dirFilter = QDir::NoDotAndDotDot | QDir::AllDirs;

    QList<QWidget*> addToSW;

    // We want the most recently updated LAN on top.
    for (const QFileInfo& fi : dir.entryInfoList(dirFilter, QDir::Time))
    {
        // TODO: Check if 12/24 hour format changes with current system.
        // TODO: Replace LAN folder names by an ID number as multiple places
        //       may use the same name.
        LANData curLAN;
        curLAN.name = fi.fileName();
        curLAN.lastModified = fi.lastModified();

        curLAN.cover = QImage(curLAN.name + "/Cover.png"); // TODO: Make extension independant.
        if (curLAN.cover.isNull())
            curLAN.cover.load(":/Resources/Defaults/Cover.jpg");

        curLAN.bg = QImage(curLAN.name + "/bg.jpg");
        if (curLAN.bg.isNull())
            curLAN.bg.load(":/Resources/Defaults/Backgound.jpg");

        CaptionWidget* cw      = new CaptionWidget(_LANsSW);
        //cw->targetSize         = Qt_Gen::sizePerc(Qt_Gen::random(10, 20));
        cw->imagePath          = fi.absoluteFilePath() + "/Cover"; // ":/Resources/Defaults/Cover.jpg"; // TODO: Fix.
        cw->image.borderRadius = {0, 0};
        cw->text.borderRadius  = {0, 0};
        cw->mainText.text      = curLAN.name;
        cw->subText.text       = "Here's your subtext.";
        cw->setMinimumSize(200, 200); // TODO: There is an issue with this.
        cw->setup();

        addToSW.append(cw);

        LANs.append(curLAN);
    }

    _LANsSW->addWidget(addToSW);

    /*
    const QString root = dir.absolutePath();
    for (const LANData& LAN : LANs)
    {
        // Go through each LAN folder.
        dir.cd(root);
        dir.cd( dir.absoluteFilePath(LAN.name) );

        // Map each LAN to its sessions.
        for (const QFileInfo& fi : dir.entryInfoList(dirFilter, QDir::Time))
        {
            QString name = fi.fileName();
            QDateTime dt = fi.lastModified();
            sessions[&LAN].append( {name, dt.toString("hh:mm"), dt.toString("ddd dd/mm/yy")} );
        }
    } */
}

SlideWidget *Chatrooms::LANsSW()
{
    return _LANsSW;
}
