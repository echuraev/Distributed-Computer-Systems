#ifndef THREADCHECK_H
#define THREADCHECK_H

#include <QThread>

class ThreadCheck : public QThread
{
    Q_OBJECT
public:
    ThreadCheck(unsigned int sec);
    void run();
private slots:
    void sendSignals();
signals:
    void timeToUpdate();
private:
    unsigned int m_timeout;
};

#endif // THREADCHECK_H
