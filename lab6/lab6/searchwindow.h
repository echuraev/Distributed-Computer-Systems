#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QWidget>
#include <QModelIndex>

namespace Ui {
class SearchWindow;
}

class SearchWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SearchWindow(QWidget *parent = 0);
    ~SearchWindow();
    void setSearchQuery(QString str);
private slots:
    void updateSearch();
    void buttonClicked(QModelIndex);

private:
    Ui::SearchWindow *ui;
    QString m_searchQuery;
};

#endif // SEARCHWINDOW_H
