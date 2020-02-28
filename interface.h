#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMainWindow>
#include <QDesktopWidget>
#include "test.h"
#include "print_model.h"
#include <QStandardItemModel>
#include "print_delegate.h"
namespace Ui {
class Interface;
}

using namespace std;


class Interface : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Interface(QWidget *parent = 0);
    void initialisation_inputs();
    void init_Prints_Images();
    ~Interface();

signals:
    void send_datas_empreintes(Selection_Obj&);
    void erase_Prints();
    void mode_changed_signal(QString);
    void send_datas_empreintes_chemin(Selection_Obj);

public slots:
    void test_inputs(int value);
    void setText_lineEdit_taille(int value);
    void setText_lineEdit_orientation(int value);
    void setText_lineEdit_orientation_Chemin(int value);
    void setText_lineEdit_espacement_horizontal_chemin(int value);
    void setText_lineEdit_espacement_vertical_chemin(int value);
    void setValue_slider_taille(QString value);
    void setValue_slider_orientation(QString value);
    void setValue_dial_orientation_Chemin(QString value);
    void setValue_slider_espacement_horizontal_chemin(QString value);
    void setValue_slider_espacement_vertical_chemin(QString value);
    void mode_changed();
    void post_clic_Viewer(vec2 pos);
    void switch_orientation_value_mode();
    void change_Print(QModelIndex index);

protected:

private:
    Ui::Interface *ui;
    Test* fen;
    Print_Model* pr_model;
    QStandardItemModel* model;
    Print_Delegate* myDelegate;
    int print_selected;

};

#endif // INTERFACE_H
