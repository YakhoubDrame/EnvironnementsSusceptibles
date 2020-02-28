#include "vbo_methods.h"

// Returns true iif v1 can be considered equal to v2
bool is_near(float v1, float v2){
    return fabs( v1-v2 ) < 0.01f;
}

void computeLocalSpace(vector<vec3> & vertex_sol,
                       vector<vec2> & textcoord_sol,
                       vector<vec3> & normal_sol,
                       vector<vec3> & tangent_sol,
                       vector<vec3> & bitangent_sol){
    int counter = 0 ;
    while(counter< vertex_sol.size()){

        /*Calcul de la normale d'un triangle*/
        // on calcule 2 vecteurs formant les bords du carré
        // le vecteur side0 est celui allant du vertex 0 au vertex 1
        vec3 side0 = vertex_sol[counter+1] - vertex_sol[counter];
        vec3 side1 = vertex_sol[counter+2] - vertex_sol[counter];
        // ici on calcule la normale à la face
        vec3 normal = cross(side0, side1);
        normalize(normal);

        /*Calcul des tangentes*/
        // maintenant on calcule les coefficients des tangentes
        // ces coefficients sont calcules en fonction des
        // coordonnées de textures du polygone
        float deltaT0 = textcoord_sol[counter+1].y -
                textcoord_sol[counter].y;
        float deltaT1 = textcoord_sol[counter+2].y -
                textcoord_sol[counter].y;

        // on effectue la meme chose pour la binormale
        float deltaB0 = textcoord_sol[counter+1].x -
                textcoord_sol[counter].x;

        float deltaB1 = textcoord_sol[counter+2].x -
                textcoord_sol[counter].x;

        /*
        cout << "Tangente : " << deltaT0 << " et " << deltaT1 <<
                endl << "Binormale : "<< deltaB0 << " et " << deltaB1 << endl;
*/
        // le facteur permettant de rendre les tangentes et binormales
        // proportionnelles aux coordonnees de textures.
        // nous aurons donc des vecteurs dont la norme depend des coordonnees
        // de textures.
        float scale = 1/ ((deltaB0 * deltaT1) - (deltaB1 * deltaT0));

        // on calcule la tangente temporaire
        vec3 tmpTangente = ((side0*deltaT1) - (side1*deltaT0))*scale;
        //cout << "Tangente non norm " << tmpTangente << " plz "<< endl;
        normalize(tmpTangente);
        //cout << "Tangente norm " << tmpTangente << " plz "<< endl;

        // on calcule la binormale temporaire

        vec3 tmpBinormal = ((side0*(-deltaB1)) + (side1*deltaB0))*scale;
        normalize(tmpBinormal);

        // nous avons donc maintenant les tangentes, binormales et normales de la face.
        // on les additionne a celles deja presentes dans les tableaux
        tangent_sol.push_back(tmpTangente);
        tangent_sol.push_back(tmpTangente);
        tangent_sol.push_back(tmpTangente);

        bitangent_sol.push_back(tmpBinormal);
        bitangent_sol.push_back(tmpBinormal);
        bitangent_sol.push_back(tmpBinormal);

        normal_sol.push_back(normal);
        normal_sol.push_back(normal);
        normal_sol.push_back(normal);

        counter +=3;

    }

}

// Searches through all already-exported vertices
// for a similar one.
// Similar = same position + same UVs + same normal
bool getSimilarVertexIndex(
        vec3 & in_vertex,
        vec2 & in_uv,
        vec3 & in_normal,
        vector<vec3> & out_vertices,
        vector<vec2> & out_uvs,
        vector<vec3> & out_normals,
        unsigned short & result
        ){
    // Lame linear search
    for ( unsigned int i=0; i<out_vertices.size(); ++i ){
        if (
                is_near( in_vertex.x , out_vertices[i].x ) &&
                is_near( in_vertex.y , out_vertices[i].y ) &&
                is_near( in_vertex.z , out_vertices[i].z ) &&
                is_near( in_uv.x     , out_uvs[i].x ) &&
                is_near( in_uv.y     , out_uvs[i].y ) &&
                is_near( in_normal.x , out_normals [i].x ) &&
                is_near( in_normal.y , out_normals [i].y ) &&
                is_near( in_normal.z , out_normals [i].z )
                ){

            result = i;
            return true;
        }
    }
    // No other vertex could be used instead.
    // Looks like we'll have to add it to the VBO.
    return false;
}

void indexVBO_TBN(
        vector<vec3> & in_vertices,
        vector<vec2> & in_uvs,
        vector<vec3> & in_normals,
        vector<vec3> & in_tangents,
        vector<vec3> & in_bitangents,
        vector<vec3> & in_colors,
        vector<unsigned short> & out_indices,
        vector<vec3> & out_vertices,
        vector<vec2> & out_uvs,
        vector<vec3> & out_normals,
        vector<vec3> & out_tangents,
        vector<vec3> & out_bitangents,
        vector<vec3> & out_colors
        ){
    // For each input vertex
    for ( unsigned int i=0; i<in_vertices.size(); ++i ){

        // Try to find a similar vertex in out_XXXX
        unsigned short index;
        bool found = getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i], out_vertices, out_uvs, out_normals, index);

        if ( found ){ // A similar vertex is already in the VBO, use it instead !
            out_indices.push_back( index );

            // Average the tangents and the bitangents
            out_tangents[index] += in_tangents[i];
            out_bitangents[index] += in_bitangents[i];
        }else{ // If not, it needs to be added in the output data.
            out_vertices.push_back( in_vertices[i]);
            out_uvs     .push_back( in_uvs[i]);
            out_normals .push_back( in_normals[i]);
            out_tangents .push_back( in_tangents[i]);
            out_bitangents .push_back( in_bitangents[i]);
            out_indices .push_back( (unsigned short)out_vertices.size() - 1 );
            out_colors.push_back(in_colors[i]);
        }
    }
}

/*
float unpackFloat(const void *buf, int *i) {
    const unsigned char *b = (const unsigned char *)buf;
    uint32_t temp = 0;
    *i += 4;
    temp = ((b[0] << 24) |
            (b[1] << 16) |
            (b[2] <<  8) |
             b[3]);
    return *((float *) temp);
}


// pack method for storing data in network,
//   big endian, byte order (MSB first)
// returns number of bytes packed
// usage:
//   float x, y, z;
//   int i = 0;
//   i += packFloat(&buffer[i], x);
//   i += packFloat(&buffer[i], y);
//   i += packFloat(&buffer[i], z);
int packFloat(void *buf, float x) {
    unsigned char *b = (unsigned char *)buf;
    unsigned char *p = (unsigned char *) &x;
#if defined (_M_IX86) || (defined (CPU_FAMILY) && (CPU_FAMILY == I80X86))
    b[0] = p[3];
    b[1] = p[2];
    b[2] = p[1];
    b[3] = p[0];
#else
    b[0] = p[0];
    b[1] = p[1];
    b[2] = p[2];
    b[3] = p[3];
#endif
    return 4;
}
*/
