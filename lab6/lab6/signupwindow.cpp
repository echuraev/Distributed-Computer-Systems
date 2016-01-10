#include "signupwindow.h"
#include "ui_signupwindow.h"
#include "clientconnect.h"
#include <QDebug>
#include <QMessageBox>

SignUpWindow::SignUpWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignUpWindow)
{
    ui->setupUi(this);

    ui->passwd->setEchoMode(QLineEdit::Password);
    ui->passwd2->setEchoMode(QLineEdit::Password);

    connect(ui->signUpButton, SIGNAL(clicked()), SLOT(onSignUpButton()));
    connect(this, SIGNAL(rosterWindow()), parent, SLOT(rosterWindow()));
}

void SignUpWindow::onSignUpButton()
{
    ClientConnect * client = ClientConnect::getInstance();
    if (ui->name->text().length() < 3)
    {
        QMessageBox::critical(0, "Error!", "The name must be greater than two characters!");
        return;
    }
    if (ui->nickName->text().length() < 3)
    {
        QMessageBox::critical(0, "Error!", "The nick name must be greater than two characters!");
        return;
    }
    if (ui->passwd->text().length() < 3)
    {
        QMessageBox::critical(0, "Error!", "The password must be greater than two characters!");
        return;
    }
    if (ui->passwd->text() != ui->passwd2->text())
    {
        QMessageBox::critical(0, "Error!", "Passwords do not match!");
        return;
    }
    char * cname = new char [ui->name->text().length() + 1];
    char * cnick = new char [ui->nickName->text().length() + 1];
    char * cpasswd = new char [ui->passwd->text().length() + 1];
    strcpy(cname, ui->name->text().toStdString().c_str());
    strcpy(cnick, ui->nickName->text().toStdString().c_str());
    strcpy(cpasswd, ui->passwd->text().toStdString().c_str());

    Args_register  signup_3_arg;
    signup_3_arg.name = cname;
    signup_3_arg.nick = cnick;
    signup_3_arg.passwd = cpasswd;
    qDebug() << signup_3_arg.name << '\t' << signup_3_arg.nick << '\t' << signup_3_arg.passwd;

    Status * result = signup_3(&signup_3_arg, client->getClient());
    if (result == (Status *) NULL)
    {
        clnt_perror (client->getClient(), "call failed");
    }
    if (!result->stat)
    {
        QString text = result->descr;
        QMessageBox::critical(0, "Client Error", text);
        return;
    }

    Args_login  login_3_arg;
    login_3_arg.name  = cnick;
    login_3_arg.passwd = cpasswd;
    Result_login  * result2 = login_3(&login_3_arg, client->getClient());
    if (result2 == (Result_login *) NULL)
    {
        clnt_perror (client->getClient(), "call failed");
    }

    if (!result2->status.stat)
    {
        QString text = result2->status.descr;
        QMessageBox::critical(0, "Client Error", text);
        return;
    }
    client->setSession(&result2->session);
    client->setUser(&result2->user);

    emit rosterWindow();
}

SignUpWindow::~SignUpWindow()
{
    delete ui;
}
