#ifndef LIGHT_H
#define LIGHT_H
#include <QGLViewer/qglviewer.h>
#include "../../Librairies/SOIL/src/SOIL.h"

class Light
{

private:
    float radius;
    QColor color;
    qglviewer::Vec position;
public:
    Light(void);
    virtual ~Light(void);
    void setRadius(float r);
    float getRadius();
    void setColor(float r, float g, float b);
    QColor getColor();
    void setPosition(float x, float y, float z);
    qglviewer::Vec getPosition();
    qglviewer::Vec computeLighting(const qglviewer::Vec & position);
};

#endif // LIGHT_H
