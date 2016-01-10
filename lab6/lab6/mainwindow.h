#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "loginwindow.h"
#include "signupwindow.h"
#include "rosterwindow.h"
#include "searchwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void closeEvent(QCloseEvent *event);
    ~MainWindow();

private slots:
    void onSignUp();
    void onLogIn();
    void onSearch();
    void onSearchBack();
    void rosterWindow();
    void friendshipRequests();
signals:
    void updateRoster();
    void updateSearch();

private:
    Ui::MainWindow *ui;
    LoginWindow * m_loginWindow;
    SignUpWindow * m_signUpWindow;
    RosterWindow * m_rosterWindow;
    SearchWindow * m_searchWindow;
};

#endif // MAINWINDOW_H
