#include "messageswindow.h"
#include "ui_messageswindow.h"
#include "clientconnect.h"
#include <QMessageBox>
#include <QDateTime>

int MessagesWindow::m_forcedCheck = FORCEDCHECK;

MessagesWindow::MessagesWindow(RosterItem * item, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessagesWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Dialogue with " + item->name);
    m_item = item;
    updateMessageHistory();
    m_thread = new ThreadCheck(10);
    connect(ui->sendButton, SIGNAL(clicked()), SLOT(sendMsg()));
    connect(m_thread, SIGNAL(timeToUpdate()), SLOT(checkNewMessage()));
    m_thread->start();
}

void MessagesWindow::closeEvent(QCloseEvent *)
{
    m_thread->exit(0);
}

void MessagesWindow::sendMsg()
{
    if (ui->sendText->toPlainText().length() == 0)
        return;
    ClientConnect * client = ClientConnect::getInstance();
    Args_sendMessage  sendmessage_3_arg;
    sendmessage_3_arg.session = *client->getSession();
    sendmessage_3_arg.recipientId = m_item->id;
    char * msg = new char[ui->sendText->toPlainText().length() + 1];
    strcpy(msg, ui->sendText->toPlainText().toStdString().c_str());
    sendmessage_3_arg.msg = msg;
    Status  * result_5 = sendmessage_3(&sendmessage_3_arg, client->getClient());
    if (result_5 == (Status *) NULL) {
        clnt_perror (client->getClient(), "call failed");
    }
    if (!result_5->stat)
    {
        QString text = result_5->descr;
        QMessageBox::critical(0, "Client Error", text);
        return;
    }
    ui->sendText->clear();
    QTextCursor cursor = ui->messageHistory->textCursor();
    QString text = "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">";
    text += "<span style=\" font-weight:600; font-style:italic; color:";
    text += "#ff0000;\">";
    text += client->getUser()->name;
    text += "</span><span style=\" font-style:italic; color:#7d7d7d;\">                 [";
    QDateTime time;
    time.setTime_t(QDateTime::currentDateTime().toTime_t());
    text += time.toString("hh:mm:ss d MMMM yyyy");
    text += "]</span></p>";
    text += "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#000000;\">";
    text += msg;
    text += "</span></p><br>";
    cursor.insertHtml(text);
    ui->messageHistory->ensureCursorVisible();
}

void MessagesWindow::checkNewMessage()
{
    ClientConnect * client = ClientConnect::getInstance();
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
    for (int i (0); i < (int)result_4->messages.messages_len; ++i)
    {
        if (result_4->messages.messages_val[i].recipient.id == client->getUser()->id)
            counter++;
    }
    qDebug () << "check" << client->getUser()->id;
    if (counter)
    {
        m_forcedCheck = FORCEDCHECK;
        updateMessageHistory();
    }
    else
    {
        m_forcedCheck--;
        if (!m_forcedCheck)
        {
            m_forcedCheck = FORCEDCHECK;
            updateMessageHistory();
        }
    }
}

void MessagesWindow::updateMessageHistory()
{
    ui->messageHistory->clear();

    ClientConnect * client = ClientConnect::getInstance();

    Result_getMessagesHistory * result_3 = getmessageshistory_3(client->getSession(), client->getClient());
    if (result_3 == (Result_getMessagesHistory *) NULL) {
        clnt_perror (client->getClient(), "call failed");
    }

    if (!result_3->status.stat)
    {
        QString text = result_3->status.descr;
        QMessageBox::critical(0, "Client Error", text);
        return;
    }

    QTextCursor cursor = ui->messageHistory->textCursor();
    for (int i(0); i < (int)result_3->messages.messages_len; ++i)
    {
        if ((result_3->messages.messages_val[i].sender.id != client->getUser()->id && result_3->messages.messages_val[i].sender.id != m_item->id)
                || (result_3->messages.messages_val[i].recipient.id != client->getUser()->id && result_3->messages.messages_val[i].recipient.id != m_item->id))
            continue;
        QString text = "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">";
        text += "<span style=\" font-weight:600; font-style:italic; color:";
        if(result_3->messages.messages_val[i].sender.id == m_item->id)
        {
            text += "#0000ff;\">";
            text += m_item->name;
        }
        else
        {
            text += "#ff0000;\">";
            text += client->getUser()->name;
        }
        text += "</span><span style=\" font-style:italic; color:#7d7d7d;\">                 [";
        QDateTime time;
        time.setTime_t(result_3->messages.messages_val[i].date);
        text += time.toString("hh:mm:ss d MMMM yyyy");
        text += "]</span></p>";
        text += "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#000000;\">";
        text += result_3->messages.messages_val[i].msg;
        text += "</span></p><br>";
        cursor.insertHtml(text);
    }
    ui->messageHistory->ensureCursorVisible();
}

MessagesWindow::~MessagesWindow()
{
    delete ui;
}
