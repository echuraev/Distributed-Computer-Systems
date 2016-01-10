#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QLineEdit>
#include <QMessageBox>
#include "clientconnect.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    QPixmap logo;
    logo.load("/home/echuraev/discributed_computing_system/lab6/lab6/logo.png");

    ui->logo->setPixmap(logo.scaled(400, 400, Qt::KeepAspectRatio));
    ui->logo->setMaximumSize(450,450);
    ui->verticalLayout->setAlignment(ui->logo, Qt::AlignCenter);

    ui->password->setEchoMode(QLineEdit::Password);
    connect(ui->signInButton, SIGNAL(clicked()), SLOT(onSignIn()));
    connect(this, SIGNAL(rosterWindow()), parent, SLOT(rosterWindow()));
}

void LoginWindow::onSignIn()
{
    if (ui->nickName->text().length() < 3)
    {
        QMessageBox::critical(0, "Error!", "The nick name must be greater than two characters!");
        return;
    }
    if (ui->password->text().length() < 3)
    {
        QMessageBox::critical(0, "Error!", "The password must be greater than two characters!");
        return;
    }
    char * name = new char [ui->nickName->text().length() + 1];
    char * passwd = new char [ui->password->text().length() + 1];
    strcpy(name, ui->nickName->text().toStdString().c_str());
    strcpy(passwd, ui->password->text().toStdString().c_str());

    ClientConnect * client = ClientConnect::getInstance();

    Args_login  login_3_arg;
    login_3_arg.name  = name;
    login_3_arg.passwd = passwd;
    Result_login  * result = login_3(&login_3_arg, client->getClient());
    if (result == (Result_login *) NULL)
    {
        clnt_perror (client->getClient(), "call failed");
    }

    if (!result->status.stat)
    {
        QString text = result->status.descr;
        QMessageBox::critical(0, "Client Error", text);
        return;
    }
    client->setSession(&result->session);
    client->setUser(&result->user);

    emit rosterWindow();
}

LoginWindow::~LoginWindow()
{
    delete ui;
}
