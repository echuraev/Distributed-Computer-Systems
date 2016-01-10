#include "listrosterwidget.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QDebug>

ListRosterWidget::ListRosterWidget(QWidget *parent) :
    QWidget(parent)
{
    logo = new QLabel();
    name = new QLabel();
    unread = new QLabel();
    layout = new QHBoxLayout();
    button = new QPushButton("");
    button->setMaximumWidth(50);

    logo->setMaximumSize(32,32);
    layout->addWidget(logo);
    layout->addWidget(name);
    layout->addWidget(unread);
    layout->addWidget(button);
    button->setVisible(false);
    QFont font;
    font.setBold(true);
    name->setFont(font);
    this->setLayout(layout);
}

ListRosterModel::ListRosterModel(QObject *parent) :
    QAbstractListModel(parent)
{}

int ListRosterModel::rowCount(const QModelIndex &) const
{
    return list.size();
}

QVariant ListRosterModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= list.size())
        return QVariant();
    if (role == Qt::DisplayRole)
    {
        return QVariant::fromValue(list.at(index.row()));
    }
    else
    {
        return QVariant();
    }
}

Qt::ItemFlags ListRosterModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEnabled ;
}

RosterItemDelegate::RosterItemDelegate()
{
    wdg = new ListRosterWidget();
}

void RosterItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString messagePic = "/home/echuraev/discributed_computing_system/lab6/lab6/message.png";
    RosterItem *my_item = index.data().value<RosterItem *>();
    QPixmap user_icon;
    user_icon.load(my_item->logo);
    wdg->logo->setPixmap(user_icon.scaled(32, 32, Qt::KeepAspectRatio));
    wdg->name->setText(my_item->name);
    wdg->button->setText(my_item->buttonText);
    if (my_item->unread)
    {
        QPixmap msg_icon;
        msg_icon.load(messagePic);
        wdg->unread->setPixmap(msg_icon.scaled(32, 32, Qt::KeepAspectRatio));
        wdg->unread->setVisible(true);
    }
    else
    {
        wdg->unread->setVisible(false);
    }

    QPalette pal;
    QLinearGradient gradient(0, 0, 0, 65);

    if ((option.state & QStyle::State_Selected) == QStyle::State_Selected){
        gradient.setColorAt(0.0, QColor(0,0,0));
        gradient.setColorAt(0.01, QColor(255,255,255));
        gradient.setColorAt(0.4, QColor(192,155,4));
        gradient.setColorAt(0.6, QColor(192,155,4));
        gradient.setColorAt(0.99, QColor(255,255,255));
        gradient.setColorAt(1.0, QColor(0,0,0));
        pal.setBrush(QPalette::Window, QBrush(QColor(Qt::transparent)));
        pal.setBrush(QPalette::Window, QBrush(gradient));
        wdg->button->setVisible(true);
    }else{
        gradient.setColorAt(0.0, QColor(0,0,0));
        gradient.setColorAt(0.01, QColor(255,255,255));
        gradient.setColorAt(0.99, QColor(255,255,255));
        gradient.setColorAt(1.0, QColor(0,0,0));
        pal.setBrush(QPalette::Window, QBrush(QColor(Qt::transparent)));
        pal.setBrush(QPalette::Window, QBrush(gradient));
        wdg->button->setVisible(false);
    }
    wdg->setPalette(pal);
    wdg->resize(option.rect.size());
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->translate(option.rect.topLeft());
    wdg->render(painter);
    painter->restore();
}

bool RosterItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if( event->type() == QEvent::MouseButtonPress ||
        event->type() == QEvent::MouseButtonRelease ) {
    } else {
        return false;
    }

    QRect buttonRect( option.rect);
    buttonRect.setY(option.rect.y() + 20);
    buttonRect.setX(option.rect.x() + 330);
    buttonRect.setHeight(30);

    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
    if( !buttonRect.contains( mouseEvent->pos()) ) {
        return false;
    }

    if( event->type() == QEvent::MouseButtonPress) {
    } else if( event->type() == QEvent::MouseButtonRelease) {
        emit buttonClicked(index);
    }
    return false;
}
QSize RosterItemDelegate::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    int width = 100;
    int height = 65;
    return QSize(width, height);
}
