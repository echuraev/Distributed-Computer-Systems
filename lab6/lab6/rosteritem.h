#ifndef ROASTERITEM_H
#define ROASTERITEM_H

#include <QtCore>

const QString online = "/home/echuraev/discributed_computing_system/lab6/lab6/online.png";
const QString offline = "/home/echuraev/discributed_computing_system/lab6/lab6/offline.png";
const QString not_access = "/home/echuraev/discributed_computing_system/lab6/lab6/not_access.png";
const QString friendIcon = "/home/echuraev/discributed_computing_system/lab6/lab6/friend.png";

class RosterItem
{
public:
    RosterItem();
    ~RosterItem();
    unsigned int id;
    QString name;
    QString logo;
    QString buttonText;
    bool isFriend;
    bool unread;
};

Q_DECLARE_METATYPE(RosterItem *)

#endif // ROASTERITEM_H
