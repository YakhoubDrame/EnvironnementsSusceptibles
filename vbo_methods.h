#ifndef VBO_METHODS_H
#define VBO_METHODS_H
#include "Librairies/glm-0.9.3.4/glm/glm.hpp"
#include <vector>
using namespace std;
using namespace glm;


bool is_near(
        float v1,
        float v2
        );

void computeLocalSpace(
        vector<vec3> & vertex_sol,
        vector<vec2> & textcoord_sol,
        vector<vec3> & normal_sol,
        vector<vec3> & tangent_sol,
        vector<vec3> & bitangent_sol);

bool getSimilarVertexIndex(
        vec3 & in_vertex,
        vec2 & in_uv,
        vec3 & in_normal,
        vector<vec3> & out_vertices,
        vector<vec2> & out_uvs,
        vector<vec3> & out_normals,
        unsigned short & result
        );

void indexVBO_TBN(
        vector<vec3> & in_vertices,
        vector<vec2> & in_uvs,
        vector<vec3> & in_normals,
        vector<vec3> & in_tangents,
        vector<vec3> & in_bitangents,

        vector<vec3> &in_colors,
        vector<unsigned short> &out_indices,
        vector<vec3> &out_vertices,
        vector<vec2> &out_uvs,
        vector<vec3> & out_normals,
        vector<vec3> & out_tangents,
        vector<vec3> &out_bitangents,
        vector<vec3> &out_colors
        );

//float unpackFloat(const void *buf, int *i);
//int packFloat(void *buf, float x);

#endif // VBO_METHODS_H
