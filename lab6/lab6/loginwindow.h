#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

private slots:
    void onSignIn();
signals:
    void rosterWindow();
private:
    Ui::LoginWindow *ui;
};

#endif // LOGIN_H
