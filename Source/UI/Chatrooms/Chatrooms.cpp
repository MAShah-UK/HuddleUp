#include "Chatrooms.h"

#include <qDebug>
#include <QDir>
#include <QDateTime>
#include <QStringList>

#include "../CaptionWidget/CaptionWidget.h"

// TODO: Make it so that the chatroom widget only parses required information.
//       The actual designing should be done by another class.
void Chatrooms::loadLANs(const QDir& dir)
{
    QFlags<QDir::Filter> dirFilter = QDir::NoDotAndDotDot | QDir::AllDirs;

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
        if (curLAN.cover.isNull()) // TODO: Loading here isn't ideal as I'm making extra copies for no reason.
            curLAN.cover.load(":/Resources/Defaults/Cover.jpg");

        curLAN.bg = QImage(curLAN.name + "/bg.jpg");
        if (curLAN.bg.isNull()) // TODO: Same issue.
            curLAN.bg.load(":/Resources/Defaults/Backgound.jpg");

        LANs.append(curLAN);
    }
}

Chatrooms::Chatrooms(const QString& dir)
{
    loadLANs(dir);

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
