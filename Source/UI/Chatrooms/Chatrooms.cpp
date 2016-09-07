#include "Chatrooms.h"

#include <qDebug>
#include <QDir>
#include <QDateTime>
#include <QStringList>

#include "../SlideWidget/SlideWidget.h"
#include "../CaptionWidget/CaptionWidget.h"

Chatrooms::Chatrooms(QWidget* parent)
{
    SWProperties SWProps;
    SWProps.isHorizontal = false;
    SWProps.spacing      = Qt_Gen::sizePerc(1).width();
    SWProps.styleVariant = SWProperties::SV_Queue;
    _LANsSW              = new SlideWidget(parent, SWProps);

    CWProperties CWProps;
    CWProps.parent = _LANsSW;
    CaptionWidget captionWidget(CWProps);

    QDir dir("Chatrooms");
    QFlags<QDir::Filter> dirFilter = QDir::NoDotAndDotDot | QDir::AllDirs;

    QList<QWidget*> addToSW;

    // We want the most recently updated LAN on top.
    for (const QFileInfo& fi : dir.entryInfoList(dirFilter, QDir::Time))
    {
        // TODO: Check if 12/24 hour format changes with current system.
        QString name = fi.fileName();
        QDateTime dt = fi.lastModified();

        QImage cover(fi.fileName() + "/cover.jpg"); // TODO: Make extension independant.
        if (cover.isNull())
            cover.load("://Resources/Defaults/cover.jpg");

        QImage bg(fi.fileName() + "/bg.jpg");
        if (bg.isNull())
            bg.load("://Resources/Defaults/bg.jpg");

        CWProps.size               = Qt_Gen::sizePerc(10);
        CWProps.imagePath          = "://Resources/Defaults/cover.jpg"; // TODO: Fix
        CWProps.image.borderRadius = {0, 0};
        CWProps.text.borderRadius  = {0, 0};
        CWProps.mainText.text      = name;
        CWProps.subText.text       = "Here's your subtext.";

        QLabel* caption = captionWidget.getLabel();
        addToSW.append(caption);

        LANs.append( {true, name, dt.toString("hh:mm"), dt.toString("ddd dd/mm/yy"),
                      cover, bg, caption} );
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
