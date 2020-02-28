#include "memory_manager.h"

Memory_manager::Memory_manager(int width_, int height_, int nbre_partition_, QObject *parent) :
    QObject(parent)
{


    this->width_print = width_;
    this->height = height_;
    this->nbre_partition = nbre_partition_;

    tableau = new float[height_*width_];
    tableau_partitions = new float[height_* nbre_partition_ *width_];
    pos_part = new int[nbre_partition_];
    for(int i = 0; i<nbre_partition_;++i){
        pos_part[i] = 0;
    }

    //float* tabl1 = new float[height*width];
    //float* tabl2 = new float[height*4*width];
    //int* pos_part2 = new int[4];
    //pos_part2 = {0};


    // tableau = (float*)tabl1;
    //tableau_partitions = (float*)tabl2;
    //pos_part = (int*) pos_part2;
    position_in_tableau = 0;

    /*
    void* a = (void*)(&tableau[0]);
    GLfloat* b = (GLfloat*)a;
    int counter = 0;
    for(int i = 0; i<height;++i){
        cout << "///////// Objet N° : "<< i+1 << " et le counter " << counter/8  << " //////////" << endl;
        cout << "Centre du point : " << b[counter++] << " en x et " << b[counter++] << " en y "  << endl;
        cout << "Rayon : " << b[counter++] << endl;
        cout << "Orientation : " << b[counter++] << endl;
        cout << "Sens : " << b[counter++] << endl;
        cout << "Force : " << b[counter++] << endl;
        cout << "Modif Forme : " << b[counter++] << " en x et " << b[counter++]<< " en y "<< endl;
    }
    */

}

void Memory_manager::init_Partitions(deque<vec3>& list){

    for(int i = 0; i<nbre_partition; ++i){
        vec3 coord = (vec3)list.at(i);
        tableau_partitions[i*width_print] =coord.x;
        tableau_partitions[i*width_print+1] =coord.y;
        tableau_partitions[i*width_print+2] =coord.z;
        //++pos_part[i];
        tableau_partitions[i*width_print+3] = pos_part[i];

    }




}


void Memory_manager::add_print(float* data, int size){
    for(int i = 0; i < size; ++i){

        tableau[position_in_tableau + i] = data[i];

    }
    position_in_tableau += size;
    /*
    int counter = 0;
    cout << "///// IN THE MEMORY MANAGER" << endl;
        cout << "///////// Objet N° : "<< position_in_tableau/width_print +1 << " et le counter " << " //////////" << endl;
        cout << "Centre du point : " << data[counter++] << " en x et " << data[counter++] << " en y "  << endl;
        cout << "Rayon : " << data[counter++] << endl;
        cout << "Orientation : " << data[counter++] << endl;
        cout << "Sens : " << data[counter++] << endl;
        cout << "Force : " << data[counter++] << endl;
        cout << "Modif Forme : " << data[counter++] << " en x et " << data[counter++]<< " en y "<< endl;


    for(int i = 0; i < size; ++i){

        tableau[position_in_tableau + i] = data[i];

    }
    position_in_tableau += size;
*/
}
void Memory_manager::add_print_partition(float* data, int partition){
    int pos_dans_tableau = nbre_partition *width_print + partition*width_print;
    pos_dans_tableau += pos_part[partition]*nbre_partition*width_print;

    for(int i = 0; i < width_print; ++i){

        tableau_partitions[pos_dans_tableau + i] = data[i];

    }
    ++pos_part[partition];
    tableau_partitions[partition*width_print+3] = pos_part[partition];


}

void Memory_manager::erase_tableau(){

    for(int i = 0; i< position_in_tableau; ++i){
        tableau[i] = 0;
    }
    position_in_tableau = 0;

}

void Memory_manager::erase_tableau_partition(){
    for(int i = 0; i < nbre_partition; ++i){

        for(int j = 0; j < pos_part[i]; ++j){

            int pos_dans_tableau = i*width_print + nbre_partition*width_print + j*nbre_partition*width_print;

            for(int k = 0; k < width_print; ++k){

                tableau_partitions[pos_dans_tableau + k] = 0;

            }
        }
        pos_part[i] = 0;
        tableau_partitions[i*width_print+3] = pos_part[i];
    }

}


float* Memory_manager::get_tableau_pointer(){

    return tableau;


}

float* Memory_manager::get_tableau_partition_pointer(){

    return tableau_partitions;

}

int Memory_manager::get_prints_number(){

    return position_in_tableau/width_print;

}

int Memory_manager::get_prints_total_number(){
    int total = 0;

    for(int i = 0; i< nbre_partition;++i){
        total += pos_part[i];
    }

    return total;

}

int Memory_manager::get_prints_by_partition_number(int partition){

    return pos_part[partition];

}

int Memory_manager::get_max_prints_by_partition(){
    int max=0;
    for(int i=0; i<nbre_partition; ++i){
        if(max<pos_part[i])
            max=pos_part[i];
    }
    return max;

}

