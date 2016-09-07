/*
 * This class will keep track of and manage all chatroom data.
 */

#include <QString>
#include <QList>
#include <QMap>
#include <QImage>

class QLabel;
class SlideWidget;

class Chatrooms
{   
    struct SessionData
    {
        bool isLoaded = false;

        QString name;
        QString lastModTime;
        QString lastModDate;
        QImage cover, bg;
    };

    // TODO: Consider deriving SessionData from LANData if many variables match up.
    struct LANData
    {
        // LANs and sessions will only be loaded as needed.
        bool isLoaded;

        QString name;
        QString lastModTime;
        QString lastModDate;
        QImage cover, bg;
        QLabel* caption;

        QList<SessionData> sessions;
    };
    QList<LANData> LANs;

    SlideWidget* _LANsSW;

public:
    Chatrooms(QWidget* parent = nullptr); // Will load all pre-existing data.
    SlideWidget* LANsSW();
};
