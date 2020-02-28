#ifndef PRINT_DELEGATE_H
#define PRINT_DELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QPainter>
#include <QSize>
#include <QPalette>
#include <Qcolor>

using namespace std;

class Print_Delegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit Print_Delegate(int heigth, QObject *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QPixmap* selected(const QPixmap &pixmap, const QPalette &palette, bool enabled) const;

signals:
    
public slots:

    private:
    QPixmap* image;
    int height;
    
};

#endif // PRINT_DELEGATE_H
