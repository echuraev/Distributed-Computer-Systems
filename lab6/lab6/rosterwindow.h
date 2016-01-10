#ifndef ROSTERWINDOW_H
#define ROSTERWINDOW_H

#include <QWidget>
#include <QModelIndex>
#include "loginwindow.h"
#include "signupwindow.h"
#include "rosterwindow.h"
#include <QLabel>
#include "threadcheck.h"

namespace Ui {
class RosterWindow;
class IconLabel;
}

class IconLabel : public QLabel
{
    Q_OBJECT
public:
    IconLabel(QWidget * parent = 0);
    ~IconLabel(){}

signals:
    void clicked();

protected:
    void mousePressEvent ( QMouseEvent * event ) ;
};

class RosterWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RosterWindow(QWidget *parent = 0);
    QString getSearchQuery();
    ~RosterWindow();

private slots:
    void clearSearch();
    void selectUser(QModelIndex);
    void updateRoster();
    void onDeleteUser(QModelIndex);
    void setSearchQuery(QString);

private:
    Ui::RosterWindow *ui;
    IconLabel * m_friendIconLabel;
    QString m_searchQuery;
    ThreadCheck * m_thread;
};

#endif // ROSTERWINDOW_H
