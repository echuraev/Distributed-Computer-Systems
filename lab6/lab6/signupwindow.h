#ifndef SIGNUPWINDOW_H
#define SIGNUPWINDOW_H

#include <QWidget>

namespace Ui {
class SignUpWindow;
}

class SignUpWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SignUpWindow(QWidget *parent = 0);
    ~SignUpWindow();

private slots:
    void onSignUpButton();
signals:
    void rosterWindow();
private:
    Ui::SignUpWindow *ui;
};

#endif // SIGNUPWINDOW_H
