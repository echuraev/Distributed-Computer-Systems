#ifndef LISTROASTERWIDGET_H
#define LISTROASTERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QItemDelegate>
#include "rosteritem.h"
#include <QPainter>
#include <QPushButton>

class ListRosterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ListRosterWidget(QWidget *parent = 0);
    QLabel *logo;
    QLabel *name;
    QLabel *unread;
    QPushButton *button;
    QHBoxLayout *layout;
};

class ListRosterModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ListRosterModel(QObject *parent = 0);
    QList<RosterItem *> list;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
};

class RosterItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    RosterItemDelegate();
    ListRosterWidget *wdg;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
signals:
    void buttonClicked(const QModelIndex &index);
};

#endif // LISTROASTERWIDGET_H
