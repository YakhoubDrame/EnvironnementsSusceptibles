#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "test.h"
//#include "interface.h"



class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);
    //void init(Test* qgl, Interface* inter);
    
signals:
    
public slots:
    
private:
    Test* qgl;
    //Interface* inter;

};

#endif // CONTROLLER_H
