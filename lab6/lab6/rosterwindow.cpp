#include "rosterwindow.h"
#include "ui_rosterwindow.h"
#include "messageswindow.h"
#include "listrosterwidget.h"
#include "rosteritem.h"
#include "searchwindow.h"
#include <QVariant>
#include "clientconnect.h"
#include <QMessageBox>

IconLabel::IconLabel(QWidget * parent)
:QLabel(parent)
{
}

void IconLabel::mousePressEvent (QMouseEvent *)
{
    emit clicked();
}



RosterWindow::RosterWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RosterWindow)
{
    ui->setupUi(this);

    QPixmap user_icon;
    user_icon.load(online);
    ui->icon->setPixmap(user_icon.scaled(32, 32, Qt::KeepAspectRatio));
    QPixmap friend_icon;
    friend_icon.load(friendIcon);
    m_friendIconLabel = new IconLabel();
    m_friendIconLabel->setPixmap(friend_icon.scaled(32, 32, Qt::KeepAspectRatio));
    m_friendIconLabel->setMaximumSize(32,32);
    ui->headerLayout->addWidget(m_friendIconLabel);
    m_friendIconLabel->setVisible(false);
    m_thread = NULL;

    // connect
    connect(ui->searchLine, SIGNAL(selectionChanged()), SLOT(clearSearch()));
    connect(ui->rosterList, SIGNAL(doubleClicked(QModelIndex)), SLOT(selectUser(QModelIndex)));
    connect(ui->searchButton, SIGNAL(clicked()), parent, SLOT(onSearch()));
    connect(m_friendIconLabel, SIGNAL(clicked()), parent, SLOT(friendshipRequests()));
    connect(ui->searchLine, SIGNAL(textChanged(QString)), SLOT(setSearchQuery(QString)));
}

void RosterWindow::clearSearch()
{
    ui->searchLine->clear();
}
void RosterWindow::setSearchQuery(QString str)
{
    m_searchQuery = str;
}
QString RosterWindow::getSearchQuery()
{
    return m_searchQuery;
}
void RosterWindow::updateRoster()
{
    qDebug () << "Updatetetete";
    if (m_thread == NULL)
    {
        m_thread = new ThreadCheck(20);
        connect(m_thread, SIGNAL(timeToUpdate()), SLOT(updateRoster()));
        m_thread->start();
    }
    ClientConnect *client = ClientConnect::getInstance();
    ui->user_name->setText(client->getUser()->name);

    Result_getUserRoster * result = getuserroster_3(client->getSession(), client->getClient());
    if (result == (Result_getUserRoster *) NULL)
    {
        clnt_perror (client->getClient(), "call failed");
    }
    if (!result->status.stat)
    {
        QString text = result->status.descr;
        QMessageBox::critical(0, "Client Error", text);
        return;
    }

    QList<RosterItem *> lst;

    ListRosterModel* model = new ListRosterModel ();
    for (int i(0); i < (int)result->roster.roster_len; ++i)
    {
        qDebug() << i << result->roster.roster_val[i].name;
        RosterItem *item = new RosterItem();
        item->id = result->roster.roster_val[i].id;
        item->name = result->roster.roster_val[i].name;
        if(!result->roster.roster_val[i].isFriend)
            item->logo = not_access;
        else
        {
            if (result->roster.roster_val[i].onlineStatus)
                item->logo = online;
            else
                item->logo = offline;
        }
        item->buttonText = "Delete";
        Result_getUnreadMessages * result_4 = getunreadmessages_3(client->getSession(), client->getClient());
        if (result_4 == (Result_getUnreadMessages *) NULL)
        {
            clnt_perror (client->getClient(), "call failed");
        }
        if (!result_4->status.stat)
        {
            QString text = result_4->status.descr;
            QMessageBox::critical(0, "Client Error", text);
            return;
        }
        int counter = 0;
        for (int j(0); j < (int)result_4->messages.messages_len; ++j)
        {
            if (result_4->messages.messages_val[j].sender.id == item->id)
                counter++;
        }
        item->isFriend = result->roster.roster_val[i].isFriend;
        item->unread = counter; // bool unread = if counter = 0 - don't show message picture, else show
        counter = 0;
        lst.append(item);
    }
    model->list = lst;
    ui->rosterList->setModel(model);
    RosterItemDelegate * rid = new RosterItemDelegate();
    connect(rid, SIGNAL(buttonClicked(QModelIndex)), this, SLOT(onDeleteUser(QModelIndex)));
    ui->rosterList->setItemDelegate(rid);


    Result_getFriendshipRequests * result_9 = getfriendshiprequests_3(client->getSession(), client->getClient());
    if (result_9 == (Result_getFriendshipRequests *) NULL) {
        clnt_perror (client->getClient(), "call failed");
    }
    if (!result_9->status.stat)
    {
        QString text = result_9->status.descr;
        QMessageBox::critical(0, "Client Error", text);
        return;
    }
    if (result_9->users.users_len)
        m_friendIconLabel->setVisible(true);
    else
        m_friendIconLabel->setVisible(false);
}

void RosterWindow::selectUser(QModelIndex mi)
{
    RosterItem * ri = mi.data().value<RosterItem *>();
    updateRoster();
    MessagesWindow * messagesWindow = new MessagesWindow(ri);
    messagesWindow->show();
}
void RosterWindow::onDeleteUser(QModelIndex mi)
{
    ClientConnect * client = ClientConnect::getInstance();
    RosterItem * ri = mi.data().value<RosterItem *>();
    QString title = "Delete user ";
    title += ri->name;
    title += " from roster?";
    QString text = "Are you sure what do you want delete ";
    text += ri->name;
    text += " from your roster?";
    QMessageBox * dial = new QMessageBox(QMessageBox::Information, title, text, QMessageBox::Yes | QMessageBox::No);
    int n = dial->exec();
    delete dial;
    if (n != QMessageBox::Yes)
        return;
    Args_deleteUserFromRoster deleteuserfromroster_3_arg;
    deleteuserfromroster_3_arg.session = *client->getSession();
    deleteuserfromroster_3_arg.userId = ri->id;
    Status * result_7 = deleteuserfromroster_3(&deleteuserfromroster_3_arg, client->getClient());
    if (result_7 == (Status *) NULL) {
        clnt_perror (client->getClient(), "call failed");
    }
    if (!result_7->stat)
    {
        QString text = result_7->descr;
        QMessageBox::critical(0, "Client Error", text);
        return;
    }
    updateRoster();
}

RosterWindow::~RosterWindow()
{
    delete ui;
}
