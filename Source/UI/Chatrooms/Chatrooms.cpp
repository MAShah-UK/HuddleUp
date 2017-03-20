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
    // TODO: Fix so that only the time of the messages is looked at.
    //       Currently any change will mess up the order.
    for (const QFileInfo& fi : dir.entryInfoList(dirFilter, QDir::Time))
    {
        QString path(fi.absoluteFilePath());
        // TODO: Check if 12/24 hour format changes with current system.
        // TODO: Replace LAN folder names by an ID number as multiple places
        //       may use the same name.
        LANData curLAN;

        // Set from Properties.txt.
        QFile LANPropertiesFile(path + "/Properties.txt");
        LANPropertiesFile.open(QIODevice::ReadOnly);

        QTextStream PropertiesStream(&LANPropertiesFile);
        QList<QString> Properties;

        while(!PropertiesStream.atEnd())
            Properties.append(PropertiesStream.readLine());

        curLAN.name    = Properties[0];
        curLAN.message = Properties[1];
        curLAN.admin   = Properties[2];
        // TODO: Fix accesses at later stage.
        curLAN.writeAccess = Properties[3];
        curLAN.readAccess  = Properties[4];

        // Set from other files.
        // TODO: Change to one of the session's LM.
        curLAN.lastModified = fi.lastModified();
        curLAN.cover = QImage(path + "/Cover");
        curLAN.bg    = QImage(curLAN.name + "/bg");

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
