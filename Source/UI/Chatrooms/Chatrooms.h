/*
 * This class will keep track of and manage all chatroom data.
 */

#include <QString>
#include <QList>
#include <QImage>
#include <QMediaPlayer>
#include <QDateTime>

class QLabel;
class SlideWidget;
class QDir;

class Chatrooms
{
    // Load locally stored LAN data.
    void loadLANs(const QDir& dir);

public:

    struct Data
    {
        // LANs and sessions will only be loaded as needed.
        bool isLoaded;

        QString name, message, admin, writeAccess, readAccess;
        QDateTime lastModified;
        QImage  cover, bg;
        QLabel* caption;
    };

    struct SessionData : Data
    {
        QList<QString> user, messages;
        QList<QDateTime> received;
        QList<QImage> images;
        //QList<QMediaPlayer> clips;
    };

    struct LANData : Data
    {
        QList<SessionData> sessions;
    };
    QList<LANData> LANs;

    Chatrooms(const QString& dir);
};
