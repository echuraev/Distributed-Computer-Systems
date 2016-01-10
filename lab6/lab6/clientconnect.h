#ifndef CLIENTCONNECT_H
#define CLIENTCONNECT_H

#include "im.h"
#include <QMutex>
#include <QString>

#define HOSTNAME "127.0.0.1"

class ClientConnect
{
public:
    static ClientConnect* getInstance();
    CLIENT * getClient();
    void setSession(Session * session);
    void setUser(User * user);
    Session * getSession();
    User * getUser();
private:
    ClientConnect();
    static ClientConnect * m_instance;
    static QMutex * m_mutex;
    CLIENT * m_clnt;
    Session * m_session;
    User * m_user;
};

#endif
