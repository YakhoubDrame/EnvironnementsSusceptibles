#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H
#include "GL/glew.h"
#include "Librairies/glm-0.9.3.4/glm/glm.hpp"
#include "Librairies/glm-0.9.3.4/glm/gtc/matrix_transform.hpp"
#include <deque>
#include <QObject>
#include <QGLViewer/qglviewer.h>


using namespace std;
using namespace glm;
class Memory_manager : public QObject
{
    Q_OBJECT
public:
    explicit Memory_manager(int width_, int height_, int nbre_partition_ = 0, QObject *parent = 0);
    void init_Partitions(deque<vec3>& list);
    void add_print(float* data, int size);
    void add_print_partition(float* data, int partition);
    void erase_tableau();
    void erase_tableau_partition();
    float* get_tableau_pointer();
    float* get_tableau_partition_pointer();
    int get_prints_number();
    int get_prints_total_number();
    int get_prints_by_partition_number(int partition);
    int get_max_prints_by_partition();

signals:
    
public slots:

private:
    float* tableau,* tableau_partitions;
    int width_print, height;
    int nbre_partition, position_in_tableau, *pos_part;

    
};

#endif // MEMORY_MANAGER_H
