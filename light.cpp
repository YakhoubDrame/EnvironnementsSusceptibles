#include "light.h"

#include <math.h>

Light::Light(void)
{
    radius = 100;
    color.setRgb(255,255,255);
    position.x = 0;
    position.y = 0;
    position.z = 0;

}

Light::~Light(void)
{
}

void Light::setRadius(float r)
{
    radius = fabs(r);
}

float Light::getRadius()
{
    return radius;
}

void Light::setColor(float r, float g, float b)
{
    color.setRgb(r*255,g*255,b*255);
}

QColor Light::getColor()
{
    return color;
}

void Light::setPosition(float x, float y, float z)
{
    position.x = x;
    position.y = y;
    position.z = z;
}

qglviewer::Vec Light::getPosition()
{
    return position;
}

qglviewer::Vec Light::computeLighting(const qglviewer::Vec & position)
{
    qglviewer::Vec ret;
    ret.x = (this->position.x - position.x) / (radius) + 0.5f;
    ret.y = (this->position.y - position.y) / (radius) + 0.5f;
    ret.z = (this->position.z - position.z) / (radius) + 0.5f;
    return ret;
}
