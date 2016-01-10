#include "searchwindow.h"
#include "ui_searchwindow.h"
#include "rosteritem.h"
#include "listrosterwidget.h"
#include "clientconnect.h"
#include <QMessageBox>

SearchWindow::SearchWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchWindow)
{
    ui->setupUi(this);

    connect(ui->backButton, SIGNAL(clicked()), parent, SLOT(onSearchBack()));
    connect(parent, SIGNAL(updateSearch()), SLOT(updateSearch()));
}

void SearchWindow::setSearchQuery(QString str)
{
    m_searchQuery = str;
    ui->queryLabel->setText(m_searchQuery);
}

void SearchWindow::updateSearch()
{
    qDebug () << "Update";
    ClientConnect *client = ClientConnect::getInstance();
    if (m_searchQuery == "Friendship Requests")
    {
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

        QList<RosterItem *> lst;

        ListRosterModel* model = new ListRosterModel ();
        for (int i(0); i < (int)result_9->users.users_len; ++i)
        {
            RosterItem *item = new RosterItem;
            item->id = result_9->users.users_val[i].id;
            item->name = result_9->users.users_val[i].name;
            if(!result_9->users.users_val[i].isFriend)
            {
                item->logo = not_access;
                item->buttonText = "Add";
            }
            else
            {
                if (result_9->users.users_val[i].onlineStatus)
                    item->logo = online;
                else
                    item->logo = offline;
                item->buttonText = "Delete";
            }
            item->unread = false;
            item->isFriend = result_9->users.users_val[i].isFriend;
            lst.append(item);
        }
        model->list = lst;
        ui->searchList->setModel(model);
        RosterItemDelegate * rid = new RosterItemDelegate();
        connect(rid, SIGNAL(buttonClicked(QModelIndex)), this, SLOT(buttonClicked(QModelIndex)));
        ui->searchList->setItemDelegate(rid);
    }
    else
    {
        Args_findUser  finduser_3_arg;
        finduser_3_arg.session = *client->getSession();
        char * name = new char [1];
        strcpy(name, "\0");
        finduser_3_arg.name = name;
        char * seachNick = new char [m_searchQuery.length() + 1];
        strcpy(seachNick, m_searchQuery.toStdString().c_str());
        finduser_3_arg.nick = seachNick;
        Result_findUser * result_8 = finduser_3(&finduser_3_arg, client->getClient());
        if (result_8 == (Result_findUser *) NULL) {
            clnt_perror (client->getClient(), "call failed");
        }
        if (!result_8->status.stat)
        {
            QString text = result_8->status.descr;
            QMessageBox::critical(0, "Client Error", text);
            return;
        }

        QList<RosterItem *> lst;

        ListRosterModel* model = new ListRosterModel ();
        for (int i(0); i < (int)result_8->users.users_len; ++i)
        {
            RosterItem *item = new RosterItem;
            item->id = result_8->users.users_val[i].id;
            item->name = result_8->users.users_val[i].name;
            if(!result_8->users.users_val[i].isFriend)
            {
                item->logo = not_access;
                item->buttonText = "Add";
            }
            else
            {
                if (result_8->users.users_val[i].onlineStatus)
                    item->logo = online;
                else
                    item->logo = offline;
                item->buttonText = "Delete";
            }
            item->unread = false;
            item->isFriend = result_8->users.users_val[i].isFriend;
            lst.append(item);
        }
        model->list = lst;
        ui->searchList->setModel(model);
        RosterItemDelegate * rid = new RosterItemDelegate();
        connect(rid, SIGNAL(buttonClicked(QModelIndex)), this, SLOT(buttonClicked(QModelIndex)));
        ui->searchList->setItemDelegate(rid);
    }
}

void SearchWindow::buttonClicked(QModelIndex mi)
{
    ClientConnect * client = ClientConnect::getInstance();
    RosterItem * ri = mi.data().value<RosterItem *>();
    if (ri->isFriend)
    {
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
    }
    else
    {
        Args_addUserToRoster addusertoroster_3_arg;
        addusertoroster_3_arg.session = *client->getSession();
        addusertoroster_3_arg.userId = ri->id;
        Status * result_6 = addusertoroster_3(&addusertoroster_3_arg, client->getClient());
        if (result_6 == (Status *) NULL) {
            clnt_perror (client->getClient(), "call failed");
        }
        if (!result_6->stat)
        {
            QString text = result_6->descr;
            QMessageBox::critical(0, "Client Error", text);
            return;
        }
        if (m_searchQuery != "Friendship Requests")
            QMessageBox::information(0, "Congratulation!", "Your friendship request was sent.");
    }
    updateSearch();
}

SearchWindow::~SearchWindow()
{
    delete ui;
}
