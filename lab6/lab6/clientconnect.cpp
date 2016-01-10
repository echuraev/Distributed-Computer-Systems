#include "clientconnect.h"
#include <QDebug>

ClientConnect * ClientConnect::m_instance = NULL;
QMutex * ClientConnect::m_mutex = new QMutex();

ClientConnect * ClientConnect::getInstance()
{
    if (m_instance == NULL)
    {
        m_mutex->lock();
        if (m_instance == NULL)
        {
            m_instance = new ClientConnect();
        }
        m_mutex->unlock();
    }
    return m_instance;
}

CLIENT * ClientConnect::getClient()
{
    return m_clnt;
}

ClientConnect::ClientConnect()
{
    m_clnt = clnt_create(HOSTNAME, IM_MESSENGER, VER, "udp");
    if (m_clnt == NULL)
    {
        clnt_pcreateerror (HOSTNAME);
        exit (1);
    }
}

Session * ClientConnect::getSession()
{
    return m_session;
}
User * ClientConnect::getUser()
{
    return m_user;
}
void ClientConnect::setSession(Session* session)
{
    m_session = session;
}
void ClientConnect::setUser(User* user)
{
    m_user = user;
}
