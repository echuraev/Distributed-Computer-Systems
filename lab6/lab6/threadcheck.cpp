#include "threadcheck.h"
#include <QTimer>

ThreadCheck::ThreadCheck(unsigned int sec) : m_timeout(sec)
{
}

void ThreadCheck::run()
{
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), SLOT(sendSignals()));
    timer.start(m_timeout * 1000);

    exec();
}

void ThreadCheck::sendSignals()
{
    emit timeToUpdate();
}
