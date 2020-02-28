#include "print_delegate.h"

Print_Delegate::Print_Delegate(int heigth, QObject *parent) :
    QItemDelegate(parent)
{
    this->height = heigth;

}

QWidget* Print_Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{

}

void Print_Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const{

}

void Print_Delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{

}

void Print_Delegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const{

    editor->setGeometry(option.rect);
}

void Print_Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{

    QString filename = index.model()->data(index,Qt::DisplayRole).toString();
    QPixmap image(filename);
    painter->drawPixmap(option.rect, image);


    if (option.showDecorationSelected && (option.state & QStyle::State_Selected)){
        if (option.state & QStyle::State_Active){
            // Mourad place here whatever you would like to see when the row is selected and active
            painter->fillRect(option.rect, QColor(119,136,153));
            painter->drawPixmap(option.rect, image);

        }else {
            // Mourad place here whatever you would like to see when the row is selected but not active
            QPalette p= option.palette;
            painter->fillRect(option.rect, p.color(QPalette::Inactive, QPalette::Background));
        }
    }
}


QSize Print_Delegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const{

    return QSize(height,height);
}


