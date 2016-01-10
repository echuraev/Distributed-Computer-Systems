#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_loginWindow = new LoginWindow(this);
    m_signUpWindow = new SignUpWindow(this);
    m_rosterWindow = new RosterWindow(this);
    m_searchWindow = new SearchWindow(this);

    ui->mainLayout->addWidget(m_loginWindow);
    ui->mainLayout->addWidget(m_signUpWindow);
    ui->mainLayout->addWidget(m_rosterWindow);
    ui->mainLayout->addWidget(m_searchWindow);
    m_signUpWindow->setVisible(false);
    m_rosterWindow->setVisible(false);
    m_searchWindow->setVisible(false);
    // icon
    QIcon icon("/home/echuraev/discributed_computing_system/lab6/lab6/logo.ico");
    this->setWindowIcon(icon);
    // connect
    connect(ui->menuExit, SIGNAL(triggered()),SLOT(close()));
    connect(ui->menuLogIn, SIGNAL(triggered()), SLOT(onLogIn()));
    connect(ui->menuSignUp, SIGNAL(triggered()), SLOT(onSignUp()));
    connect(this, SIGNAL(updateRoster()), m_rosterWindow, SLOT(updateRoster()));
}

void MainWindow::closeEvent(QCloseEvent * e)
 {
    e->accept();
    if (this->sender() == NULL)
    {
        this->showMinimized();
        e->ignore();
    }
    else
    {
        qApp->quit();
    }
 }

void MainWindow::onSignUp()
{
    m_signUpWindow->setVisible(true);
    m_loginWindow->setVisible(false);
    m_rosterWindow->setVisible(false);
    m_searchWindow->setVisible(false);
}
void MainWindow::onLogIn()
{
    m_loginWindow->setVisible(true);
    m_signUpWindow->setVisible(false);
    m_rosterWindow->setVisible(false);
    m_searchWindow->setVisible(false);
}
void MainWindow::onSearch()
{
    m_searchWindow->setVisible(true);
    m_signUpWindow->setVisible(false);
    m_rosterWindow->setVisible(false);
    m_loginWindow->setVisible(false);
    m_searchWindow->setSearchQuery(m_rosterWindow->getSearchQuery());
    emit updateSearch();
}
void MainWindow::onSearchBack()
{
    m_searchWindow->setVisible(false);
    m_signUpWindow->setVisible(false);
    m_rosterWindow->setVisible(true);
    m_loginWindow->setVisible(false);
    emit updateRoster();
}
void MainWindow::friendshipRequests()
{
    m_searchWindow->setVisible(true);
    m_signUpWindow->setVisible(false);
    m_rosterWindow->setVisible(false);
    m_loginWindow->setVisible(false);
    m_searchWindow->setSearchQuery("Friendship Requests");
    emit updateSearch();
}
void MainWindow::rosterWindow()
{
    m_searchWindow->setVisible(false);
    m_signUpWindow->setVisible(false);
    m_rosterWindow->setVisible(true);
    m_loginWindow->setVisible(false);
    ui->menuLogIn->setVisible(false);
    ui->menuSignUp->setVisible(false);

    emit updateRoster();
}

MainWindow::~MainWindow()
{
    delete ui;
}
