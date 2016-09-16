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
        QString name = fi.fileName();
        QDateTime dt = fi.lastModified();

        QImage cover(fi.fileName() + "/Cover.jpg"); // TODO: Make extension independant.
        if (cover.isNull())
            cover.load(":/Resources/Defaults/Cover.jpg");

        QImage bg(fi.fileName() + "/bg.jpg");
        if (bg.isNull())
            bg.load(":/Resources/Defaults/Backgound.jpg");

        CaptionWidget* cw      = new CaptionWidget(_LANsSW);
        cw->targetSize         = Qt_Gen::sizePerc(Qt_Gen::random(10, 20));
        cw->imagePath          = ":/Resources/Defaults/Cover.jpg"; // TODO: Fix.
        cw->image.borderRadius = {0, 0};
        cw->text.borderRadius  = {0, 0};
        cw->mainText.text      = name;
        cw->subText.text       = "Here's your subtext.";
        cw->setup();

        addToSW.append(cw);

        LANs.append( {true, name, dt.toString("hh:mm"), dt.toString("ddd dd/mm/yy"),
                      cover, bg, cw} );
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
