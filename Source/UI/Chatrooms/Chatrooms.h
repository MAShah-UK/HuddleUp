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

class Chatrooms
{
    struct Data
    {
        // LANs and sessions will only be loaded as needed.
        bool isLoaded;

        QString name;
        QDateTime lastModified;
        QImage  cover, bg;
        QLabel* caption;
    };

    struct SessionData : Data
    {
        QList<QString> user;
        QList<QString> messages;
        QList<QDateTime> received;
        QList<QImage> images;
        //QList<QMediaPlayer> clips;
    };

    struct LANData : Data
    {
        QList<SessionData> sessions;
    };
    QList<LANData> LANs;

    SlideWidget* _LANsSW;

    void loadLANs();

public:
    Chatrooms(); // Will load all pre-existing data.
    SlideWidget* LANsSW();
};
