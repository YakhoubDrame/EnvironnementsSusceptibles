#ifndef MAINVIEWER_H
#define MAINVIEWER_H
#include <GL/glew.h>

//#include "glext.h"
#include <QGLViewer/qglviewer.h>

#include <gl.h>                                // Header File For The OpenGL32 Library
#include <glu.h>                               // Header File For The GLu32 Library
#include <QKeyEvent>
//#include <vector>
#include <QVector>
#include "face.h"
#include "light.h"
#include <stdlib.h>
#include "glext.h"
#include <QColor>
#include "Librairies/glm-0.9.3.4/glm/glm.hpp"
#include "../../Librairies/SOIL/src/SOIL.h"

using namespace qglviewer;

struct Trace_coord{

    glm::vec3 coord;
    int num_mur;
    int num_text;
    float rayon;
    float circle;

};
class MainViewer : public QGLViewer
{
public:
    MainViewer();
    void initgl();
    int test_drawEnvironnment();
    int test_draw_env_quad_base(const bool specialColor, double position_x,  double position_y);
    int test_draw_env_quad_wall(const bool specialColor, double position_x,  double position_z);
    int LoadGLTextures(QString filename);
    void computeLocalSpace(Face &face);
    void normalise(qglviewer::Vec &a);
    qglviewer::Vec cross(qglviewer::Vec &a, qglviewer::Vec &b);
    void config_norm();
    GLvoid draw_circle(bool flat, int num_text, int mur, const GLfloat radius, float x_point, float y_point, float z_point  ,const GLuint num_vertex);
    void computeLocalSpace();

protected :
    virtual void draw();
    virtual void init();
    virtual QString helpString() const;
    virtual void postSelection(const QPoint& point);
    void keyPressEvent(QKeyEvent * event);
    virtual void drawWithNames();
    void remplissage_face();
    void drawLocalSpace();
    void generateSphereMap(int size);
    void init_Lighting(const QColor &ambiant);
    void draw_trace(bool flat, const int mur);
    void draw_prev_trace();
    void compute_trace(bool flat, int num_text, int mur, float rayon, float x_point, float y_point, float z_point );
    void indexVBO_TBN2(
        std::vector<glm::vec3> & in_vertices,
        std::vector<glm::vec2> & in_uvs,
        std::vector<glm::vec3> & in_normals,
        std::vector<glm::vec3> & in_tangents,
        std::vector<glm::vec3> & in_bitangents,

        std::vector<unsigned short> & out_indices,
        std::vector<glm::vec3> & out_vertices,
        std::vector<glm::vec2> & out_uvs,
        std::vector<glm::vec3> & out_normals,
        std::vector<glm::vec3> & out_tangents,
        std::vector<glm::vec3> & out_bitangents
    );
    private:
    int transDir;
    int rotDir;
    GLuint		texture[5], lightmap;
    GLfloat     rtri;                       // Angle For The Triangle ( NEW )
    GLfloat     rquad;                      // Angle For The Quad     ( NEW )
    GLfloat     xrot;                               // X Rotation ( NEW )
    GLfloat     yrot;                               // Y Rotation ( NEW )
    GLfloat     zrot;                               // Z Rotation ( NEW )
    int count_text;
    int count_names;
    int count_vertex;
    int nbvertex;
    int nbfaces;
    qglviewer::Vec orig, dir, selectedPoint;
    QList<qglviewer::Vec> vertex, textcoord;
    QVector<qglviewer::Vec> normals, tangents, bitangents ;
    QList<Face> faces;
    QColor ambiant;
    qglviewer::AxisPlaneConstraint* constraints[2];
    QVector<Trace_coord> traces;
    int current_trace_text;
    bool drawspace;
    bool circle2;

};


#endif // MAINVIEWER_H
