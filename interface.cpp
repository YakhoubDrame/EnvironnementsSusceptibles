#include "interface.h"
#include "ui_interface.h"
#include "endian.h"
Interface::Interface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Interface)
{
    ui->setupUi(this);


    //////Positionner l'interface au centre de la fenêtre
    QDesktopWidget wid;
    int screenWidth = wid.screen()->width();
    int screenHeight = wid.screen()->height();

    QRect r = this->frameGeometry();
    r.moveCenter(QPoint(screenWidth/2,screenHeight/2));
    this->move(r.topLeft());

    fen = new Test(ui->Viewer);

    QRect rect = ui->tableView_Prints->geometry();
    model = new QStandardItemModel(1,7);
    myDelegate = new Print_Delegate(rect.height() - 18);
    /*
    for(int row = 0; row < 1; ++row){
        for(int column = 0; column < 6; ++column){
            QModelIndex index = model->index(row,column,QModelIndex());
            model->setData(index, 0);
        }
    }
    */
    ui->tableView_Prints->setModel(model);

    QModelIndex index ;
    index = model->index(0,0,QModelIndex());
    model->setData(index, "Textures/empreintes/empreinte2bis.png", Qt::DisplayRole);
    model->setData(index, 0, Qt::UserRole);

    index = model->index(0,1,QModelIndex());
    model->setData(index, "Textures/empreintes/left_footbis.png", Qt::DisplayRole);
    model->setData(index, 1, Qt::UserRole);

    index = model->index(0,2,QModelIndex());
    model->setData(index, "Textures/empreintes/empreinte6.png", Qt::DisplayRole);
    model->setData(index, 2, Qt::UserRole);

    index = model->index(0,3,QModelIndex());
    model->setData(index, "Textures/empreintes/empreinte7.png", Qt::DisplayRole);
    model->setData(index, 3, Qt::UserRole);

    index = model->index(0,4,QModelIndex());
    model->setData(index, "Textures/empreintes/empreinte8.png", Qt::DisplayRole);
    model->setData(index, 4, Qt::UserRole);

    index = model->index(0,5,QModelIndex());
    model->setData(index, "Textures/empreintes/empreinte10.png", Qt::DisplayRole);
    model->setData(index, 5, Qt::UserRole);

    index = model->index(0,6,QModelIndex());
    model->setData(index, "Textures/empreintes/empreinte11.png", Qt::DisplayRole);
    model->setData(index, 6, Qt::UserRole);

    ui->tableView_Prints->setItemDelegate(myDelegate);
    ui->tableView_Prints->resizeColumnsToContents();
    ui->tableView_Prints->resizeRowsToContents();

    ui->tableView_Prints->verticalHeader()->hide();
    ui->tableView_Prints->horizontalHeader()->hide();
    initialisation_inputs();


}

void Interface::initialisation_inputs(){

    //connect(ui->Slide_Orientation,SIGNAL(valueChanged(int)), this, SLOT(test_inputs(int)));
    connect(ui->Slide_Orientation, SIGNAL(valueChanged(int)), this, SLOT(setText_lineEdit_orientation(int)));
    connect(ui->Slide_taille, SIGNAL(valueChanged(int)), this, SLOT(setText_lineEdit_taille(int)));
    connect(ui->lineEdit_Orientation_valeur, SIGNAL(textChanged(QString)), this, SLOT(setValue_slider_orientation(QString)));
    connect(ui->lineEdit_Taille_valeur, SIGNAL(textChanged(QString)), this, SLOT(setValue_slider_taille(QString)));
    connect(ui->pushButton_Effacer, SIGNAL(clicked()), fen, SLOT(erase_Prints()));
    connect(ui->radioButton_orientation_dial,SIGNAL(clicked()), this, SLOT(switch_orientation_value_mode()));
    connect(ui->radioButton_orientation_camera,SIGNAL(clicked()), this, SLOT(switch_orientation_value_mode()));
    connect(ui->radioButton_orientation_camera, SIGNAL(clicked()), fen, SLOT(animation_angle_prints_dial()));

    connect(ui->tableView_Prints, SIGNAL(clicked(QModelIndex)), this, SLOT(change_Print(QModelIndex)));
    connect(fen, SIGNAL(clic_Viewer(vec2)), this, SLOT(post_clic_Viewer(vec2)));
    connect(this,SIGNAL(send_datas_empreintes(Selection_Obj&)), fen, SLOT(post_send_datas_Interface(Selection_Obj&)));
    connect(this, SIGNAL(send_datas_empreintes_chemin(Selection_Obj)), fen, SLOT(compute_chemin_datas(Selection_Obj)));

    connect(ui->radioButton_deplacement_par_clic, SIGNAL(clicked()), this, SLOT(mode_changed()));
    connect(ui->radioButton_deplacement_par_chemin, SIGNAL(clicked()), this, SLOT(mode_changed()));
    connect(this, SIGNAL(mode_changed_signal(QString)), fen, SLOT(change_Mode(QString)));

    connect(ui->dial_orientation_empreintes_chemin, SIGNAL(valueChanged(int)), this, SLOT(setText_lineEdit_orientation_Chemin(int)));
    connect(ui->lineEdit_Orientation_Chemin, SIGNAL(textChanged(QString)), this, SLOT(setValue_dial_orientation_Chemin(QString)));
    connect(ui->horizontalSlider_espacement_horizontal_chemin, SIGNAL(valueChanged(int)), this, SLOT(setText_lineEdit_espacement_horizontal_chemin(int)));
    connect(ui->lineEdit_espacement_horizontal_chemin, SIGNAL(textChanged(QString)), this, SLOT(setValue_slider_espacement_horizontal_chemin(QString)));
    connect(ui->horizontalSlider_espacement_vertical_chemin, SIGNAL(valueChanged(int)), this, SLOT(setText_lineEdit_espacement_vertical_chemin(int)));
    connect(ui->lineEdit_espacement_vertical_chemin, SIGNAL(textChanged(QString)), this, SLOT(setValue_slider_espacement_vertical_chemin(QString)));

    ui->toolBox->setStyleSheet("QToolBox::tab {"
                               "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                               "stop: 0 #63d3cf, stop: 0.4 #47f3ff,"
                               "stop: 0.5 #48d3bc, stop: 1.0 #1c86ff);"
                               "border-radius: 9px;"
                               "color: white;"
                               "}"
                               "QToolBox::tab:selected {" /* italicize selected tabs */
                               "font-size: 16px;"
                               "}"
                               );

    ui->Slide_Orientation->setValue(270);
    ui->Slide_taille->setValue(1);
    ui->lineEdit_Taille_valeur->setText(QString::number(ui->Slide_taille->value()));
    ui->lineEdit_Orientation_valeur->setText(QString::number(ui->Slide_Orientation->value()));
    ui->radioButton_orientation_dial->setChecked(true);
    ui->widget_options_deplacement_chemin->hide();
    ui->lineEdit_Orientation_Chemin->setText("45");
    ui->dial_orientation_empreintes_chemin->setValue(45);
    ui->horizontalSlider_espacement_vertical_chemin->setValue(10);
    ui->lineEdit_espacement_vertical_chemin->setText("1");
    print_selected = 0;



}
void Interface::init_Prints_Images()
{
    QWidget empreinte1;
    empreinte1.setStyleSheet("background-image: url(Textures/empreintes/empreintes.jpg)");


}

void Interface::test_inputs(int value){
    cout << "La valeur est du dial est "  << value << endl;


}

void Interface::setText_lineEdit_taille(int value){

    float val = (float)value/10;
    cout << "LA valeur en int est "<<  value << " et en float " << val << endl;
    ui->lineEdit_Taille_valeur->setText(QString::number(val));

}

void Interface::setText_lineEdit_orientation(int value){
    ui->lineEdit_Orientation_valeur->setText(QString::number(value));

}

void Interface::setValue_slider_taille(QString value){
    float val = (float)value.toFloat()*10;
    ui->Slide_taille->setValue(val);

}

void Interface::setValue_slider_orientation(QString value){
    ui->Slide_Orientation->setValue(value.toInt());
}

void Interface::setText_lineEdit_orientation_Chemin(int value){
    ui->lineEdit_Orientation_Chemin->setText(QString::number(value));

}

void Interface::setValue_dial_orientation_Chemin(QString value){
    ui->dial_orientation_empreintes_chemin->setValue(value.toInt());
}

void Interface::setValue_slider_espacement_horizontal_chemin(QString value){
    float val = (float)value.toFloat()*10;
    ui->horizontalSlider_espacement_horizontal_chemin->setValue(val);
}

void Interface::setValue_slider_espacement_vertical_chemin(QString value){
    float val = (float)value.toFloat()*10;
    ui->horizontalSlider_espacement_vertical_chemin->setValue(val);
}

void Interface::setText_lineEdit_espacement_horizontal_chemin(int value){
    float val = (float)value/10;
    ui->lineEdit_espacement_horizontal_chemin->setText(QString::number(val));
}
void Interface::setText_lineEdit_espacement_vertical_chemin(int value){
    float val = (float)value/10;
    ui->lineEdit_espacement_vertical_chemin->setText(QString::number(val));
}


void Interface::switch_orientation_value_mode(){
    if(ui->radioButton_orientation_dial->isChecked() && !ui->radioButton_orientation_camera->isChecked()){
        disconnect(fen, SIGNAL(animation_angle_dial_value(int)), this, SLOT(setText_lineEdit_orientation(int)));
        connect(ui->Slide_Orientation, SIGNAL(valueChanged(int)), this, SLOT(setText_lineEdit_orientation(int)));
    }else if(!ui->radioButton_orientation_dial->isChecked() && ui->radioButton_orientation_camera->isChecked()){
        connect(fen, SIGNAL(animation_angle_dial_value(int)), this, SLOT(setText_lineEdit_orientation(int)));
        disconnect(ui->Slide_Orientation, SIGNAL(valueChanged(int)), this, SLOT(setText_lineEdit_orientation(int)));
    }
}

void Interface::post_clic_Viewer(vec2 pos){

    if(ui->radioButton_deplacement_par_clic->isChecked() && !ui->radioButton_deplacement_par_chemin->isChecked()){
        cout << "In Interface, ready to process datas" << endl;
        Selection_Obj a;

        /*
        char* oricon = new char[8];
        oricon[0] = 10;
        oricon[1] = 20;
        oricon[2] = 30;
        oricon[3] = 40;
        oricon[4] = 50;
        oricon[5] = 60;
        oricon[6] = 70;
        oricon[7] = 80;

        int Int32;

        memcpy( &Int32, oricon, sizeof(Int32) );



        Int32 = (Int32 << 8) + oricon[3];
        Int32 = (Int32 << 8) + oricon[2];
        Int32 = (Int32 << 8) + oricon[1];
        Int32 = (Int32 << 8) + oricon[0];

        int test = *((char*)(&Int32) + 2);
        int test1;
        int test2;
        int test3;



        unsigned char* oricon2 = new unsigned char[8];
        oricon2[0] = 10;
        oricon2[1] = 20;
        oricon2[2] = 30;
        oricon2[3] = 40;
        oricon2[4] = 50;
        oricon2[5] = 60;
        oricon2[6] = 70;
        oricon2[7] = 80;
        float f;
        memcpy( &f, oricon2, sizeof(f) );

        int test = *((unsigned char*)(&f) + 7 );

        unsigned char EndianTest[2] = { 1, 0 };
        short x;

        x = *(short *) EndianTest;



        unsigned char EndianTest[2] = { 1, 0 };
        short x;

        x = *(short *) EndianTest;
        cout << "Orientation en unsigned char : " << x << endl;


        unsigned char* oricon2 = new unsigned char[4];
        oricon2[0] = 1;
        oricon2[1] = 1;
        oricon2[2] = 0;
        oricon2[3] = 0;

        float test = packFloat(oricon2);


        cout << "Float : " << test << endl;
        unsigned char* oricon_test;
        oricon_test = unpackFloat(test);

         cout << " Oricon : " << (int)oricon_test[0] << "  " << (int)oricon_test[1] << "  " << (int)oricon_test[2] << "  " << (int)oricon_test[3] << "  " << endl;



*/


        //cout << "Orientation en unsigned char : " << x << endl;

        a.center_select = pos;

        a.radius = (float)ui->Slide_taille->value()/10;

        a.orientation = ui->Slide_Orientation->value();
        int test = (int)a.orientation * 255.0/360.0;
        a.orientation = -M_PI * a.orientation / 180;

        if(ui->radioButton_sens_peid_gauche->isChecked())
            a.sens = 0;
        if(ui->radioButton_sens_pied_droite->isChecked())
            a.sens = 1;

        a.force = print_selected;

        a.modif_forme = vec2(1,1);

        if(ui->checkBox_scale_X->isChecked()){
            QString buf = ui->lineEdit_scale_X->text();
            a.modif_forme.x = buf.toFloat();
            cout << "X is checked, the value is :" << a.modif_forme.x << endl;
        }

        if(ui->checkBox_scale_Y->isChecked()){
            QString buf = ui->lineEdit_scale_Y->text();
            a.modif_forme.y = buf.toFloat();
            cout << "Y is checked, the value is :" << a.modif_forme.y << endl;

        }

        unsigned char EndianTest[2] = { 1, 0 };
        short x;
        int xbis;

        x = *(short *) EndianTest;
        xbis = *(int *) EndianTest;

        cout << "Endian Test : dans des unsigned char : " << (int)EndianTest[0] << " et " << (int)EndianTest[1] << endl;
        cout << "              dans un short          : " << x  << endl;
        cout << "              dans un int            : " << x  << endl;

        ////////// Pack 4 datas in a int ////////////
        unsigned char* oricon = new unsigned char[4];
        oricon[0] = test & 255;
        test = a.sens;
        oricon[1] = test & 255;
        test = a.force;
        oricon[2] = test & 255;

        oricon[3] = 128;
        cout << " Oricon : " << (int)oricon[0] << "  " << (int)oricon[1] << "  " << (int)oricon[2] << "  " << (int)oricon[3] << "  " << endl;


        int packed = pack_int(oricon);
        unsigned char* oricon2 = unpackint(packed);
        cout << " Oricon : " << (int)oricon2[0] << "  " << (int)oricon2[1] << "  " << (int)oricon2[2] << "  " << (int)oricon2[3] << "  " << endl;

        Selection_Obj_optim* obj2 = new Selection_Obj_optim;
        obj2->center_select = pos;
        obj2->radius = (float)ui->Slide_taille->value()/10;
        obj2->or_se_typ_form = (float)packed;


        emit send_datas_empreintes(a);
    }else if(ui->radioButton_deplacement_par_chemin->isChecked() && !ui->radioButton_deplacement_par_clic->isChecked()){
        cout << "SENDING THE PRINT " << (float)ui->Slide_taille->value()/10 << endl;
        //vec3 datas = vec3(pos, (float)ui->Slide_taille->value()/10);
        Selection_Obj obj;
        obj.center_select = pos;
        obj.radius = (float)ui->Slide_taille->value()/10;
        obj.orientation = ui->dial_orientation_empreintes_chemin->value();
        obj.orientation = ui->dial_orientation_empreintes_chemin->value();
        obj.orientation = M_PI * obj.orientation / 180;
        obj.modif_forme = vec2(1,1);

        if(ui->checkBox_scale_X->isChecked()){
            QString buf = ui->lineEdit_scale_X->text();
            obj.modif_forme.x = buf.toFloat();
        }

        if(ui->checkBox_scale_Y->isChecked()){
            QString buf = ui->lineEdit_scale_Y->text();
            obj.modif_forme.y = buf.toFloat();

        }

        obj.force = print_selected;

        obj.sens = ui->horizontalSlider_espacement_vertical_chemin->value()/10;

        emit send_datas_empreintes_chemin(obj);
    }

}

void Interface::mode_changed(){
    if(ui->radioButton_deplacement_par_chemin->isChecked() && !ui->radioButton_deplacement_par_clic->isChecked()){
        emit mode_changed_signal(QString("chemin"));
        ui->widget_options_deplacement_chemin->show();
    }else if(ui->radioButton_deplacement_par_clic->isChecked() && !ui->radioButton_deplacement_par_chemin->isChecked()){
        emit mode_changed_signal(QString("clic"));
        ui->widget_options_deplacement_chemin->hide();
    }
}

void Interface::change_Print(QModelIndex index){

    QString filename = index.model()->data(index,Qt::DisplayRole).toString();
    int number = index.model()->data(index,Qt::UserRole).toInt();
    print_selected = number;
    cout << "Le nom sélectionné est " << filename.toStdString() <<"and the userole is " << number << endl;

}


Interface::~Interface()
{
    delete ui;
}
