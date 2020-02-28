#ifndef VIEWER_H
#define VIEWER_H
#include "GL/glew.h"
//#include "glext.h"
#include <QGLViewer/qglviewer.h>
#include <gl.h>                                // Header File For The OpenGL32 Library
#include <glu.h>                               // Header File For The GLu32 Library
#include "light.h"
#include <vector>
#include <map>
#include <QList>
#include "face.h"
#include <QGLShader>
#include <QGLShaderProgram>
#include <QFileInfo>
#include "Librairies/glm-0.9.3.4/glm/glm.hpp"
#include "Librairies/glm-0.9.3.4/glm/gtc/matrix_transform.hpp"
using namespace std;
using namespace glm;
using namespace qglviewer;

class Viewer : public QGLViewer
{
private:
    GLuint		texture[5], VertexArrayID;
    QVector<Vec> vertex_sol, textcoord_sol, normal_sol, bitangent_sol, tangent_sol;
    std::vector<glm::vec3> vertex_sol1,  normal_sol1, bitangent_sol1, tangent_sol1;
    std::vector<glm::vec2>  textcoord_sol1;
    int nbfaces_x, nbfaces_y, nbfaces, count_vertex_sol, count_text;
    QList<Face> faces;
    struct PackedVertex{
        glm::vec3 position;
        glm::vec2 uv;
        glm::vec3 normal;
        bool operator<(const PackedVertex that) const{
            return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
        }
    };
    QGLShader *VertexShader, *FragmentShader;
    QGLShaderProgram* ShaderProgram;

public:
    Viewer();
    void init();
    void draw();
    void fill_sol();
    void computeLocalSpace(Face &face);
    void transform();
    int LoadGLTextures(QString filename);
    void drawLocalSpace();
    void indexVBO_TBN2(
            vector<vec3> & in_vertices,
            vector<vec2> & in_uvs,
            vector<vec3> & in_normals,
            vector<vec3> & in_tangents,
            vector<vec3> & in_bitangents,

            vector<unsigned short> & out_indices,
            vector<vec3> & out_vertices,
            vector<vec2> & out_uvs,
            vector<vec3> & out_normals,
            vector<vec3> & out_tangents,
            vector<vec3> & out_bitangents
            );
    bool getSimilarVertexIndex_fast(
            PackedVertex & packed,
            std::map<PackedVertex,unsigned short> & VertexToOutIndex,
            unsigned short & result
            );
    bool getSimilarVertexIndex(
        glm::vec3 & in_vertex,
        glm::vec2 & in_uv,
        glm::vec3 & in_normal,
        std::vector<glm::vec3> & out_vertices,
        std::vector<glm::vec2> & out_uvs,
        std::vector<glm::vec3> & out_normals,
        unsigned short & result
    );
    bool is_near(float v1, float v2);
    void LoadShader(QString vshader, QString fshader);

};

#endif // VIEWER_H
