#ifndef MESSAGESWINDOW_H
#define MESSAGESWINDOW_H

#include <QWidget>
#include <rosteritem.h>
#include "threadcheck.h"

#define FORCEDCHECK 20

namespace Ui {
class MessagesWindow;
}

class MessagesWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MessagesWindow(RosterItem * item, QWidget *parent = 0);
    void closeEvent(QCloseEvent *event);
    ~MessagesWindow();

private slots:
    void sendMsg();
    void updateMessageHistory();
    void checkNewMessage();

private:
    Ui::MessagesWindow *ui;
    RosterItem * m_item;
    ThreadCheck * m_thread;
    static int m_forcedCheck;
};

#endif // MESSAGESWINDOW_H
