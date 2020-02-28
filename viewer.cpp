#include "viewer.h"
#include "QGLViewer/vec.h"
#include "common/vboindexer.hpp"
Viewer::Viewer()
{

    nbfaces_x = nbfaces_y = 8;
    nbfaces = nbfaces_x * nbfaces_y;
    count_text = 0;

}

void Viewer::init(){
cout << "haha" << endl;
glewExperimental = GL_TRUE;
if (glewInit() != GLEW_OK) {
        cout << "Failed to initialize GLEW" << endl;
        exit (0);
}else{
    cout << "Works like a charm" << endl;
}
glEnable(GL_TEXTURE_2D);
if (!LoadGLTextures(QString("Textures/dirt")))                         // Jump To Texture Loading Routine ( NEW )
{
    cout << "Erreur chargement texture sol" << endl;                           // If Texture Didn't Load Return FALSE ( NEW )
}else{
    cout << " chargement texture sol réussi" << endl;                           // If Texture Didn't Load Return FALSE ( NEW )

}

if (!LoadGLTextures(QString("Textures/dirt_NRM.jpg")))                      // Jump To Texture Loading Routine ( NEW )
{
    cout << "Erreur chargement normale sol" << endl;
    // If Texture Didn't Load Return FALSE ( NEW )
}else{
    cout << " chargement normale sol réussi" << endl;

}

if (!LoadGLTextures(QString("Textures/dirt_SPEC.jpg")))                      // Jump To Texture Loading Routine ( NEW )
{
    cout << "Erreur chargement spéculaire sol" << endl;
    // If Texture Didn't Load Return FALSE ( NEW )
}else{
    cout << "chargement spéculaire sol réussi" << endl;

}

// Dark blue background
        //glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);

        // Cull triangles which normal is not towards the camera
        glEnable(GL_CULL_FACE);
        setSceneRadius(100.0);          // scene has a 100 OpenGL units radius
        setSceneCenter( Vec(16.0 , 16.0, 5.0 ) );

}

void Viewer::draw(){
    fill_sol();


}

void Viewer::fill_sol(){




    vertex_sol.clear();
    textcoord_sol.clear();
    normal_sol.clear();
    bitangent_sol.clear();
    tangent_sol.clear();

    for (float i = 0.0; i < nbfaces_x; ++i){
        for (float j = 0.0; j < nbfaces_y; ++j){

            vertex_sol.append(Vec(i,j,0.0));
            vertex_sol.append(Vec(i+2.0,j,0.0));
            vertex_sol.append(Vec(i+2.0,j+2.0,0.0));
            vertex_sol.append(Vec(i,j,0.0));
            vertex_sol.append(Vec(i+2.0,j,0.0));
            vertex_sol.append(Vec(i,j+2.0,0.0));

            textcoord_sol.append(Vec(0.0, 0.0, 0.0));
            textcoord_sol.append(Vec(1.0, 0.0, 0.0));
            textcoord_sol.append(Vec(1.0, 1.0, 0.0));
            textcoord_sol.append(Vec(0.0, 0.0, 0.0));
            textcoord_sol.append(Vec(1.0, 1.0, 0.0));
            textcoord_sol.append(Vec(0.0, 1.0, 0.0));

        }


    }

    for(int i = 0; i< vertex_sol.size(); ++i){

        normal_sol.append(Vec(0.0, 0.0, 0.0));
        tangent_sol.append(Vec(0.0, 0.0, 0.0));
        bitangent_sol.append(Vec(0.0, 0.0, 0.0));

    }

    int counter = 0;
    int nb = nbfaces *2;


    for (int i = 0; i < nb; ++i)
    {

        QVector<Vec> vertex1;
        vertex1.append(Vec(vertex_sol.at(counter)));
        vertex1.append(Vec(vertex_sol.at(counter +1)));
        vertex1.append(Vec(vertex_sol.at(counter +2)));

        QVector<Vec> normal;
        normal.append(Vec(normal_sol.at(counter)));
        normal.append(Vec(normal_sol.at(counter +1)));
        normal.append(Vec(normal_sol.at(counter +2)));

        QVector<Vec> texCoord1;
        texCoord1.append(Vec(textcoord_sol.at(counter)));
        texCoord1.append(Vec(textcoord_sol.at(counter +1)));
        texCoord1.append(Vec(textcoord_sol.at(counter +2)));
        Face a(vertex1,normal,texCoord1);
        faces.append(a);
        counter += 3;

    }

    count_vertex_sol = 0;

    for (int i = 0; i <  nbfaces*2; ++i)
    {
        computeLocalSpace(faces[i]);

    }

    // il ne faut pas oublier de renormaliser les tangentes et binormales
    for (int i = 0; i < count_vertex_sol; ++i)
    {
        bitangent_sol[i].normalize();
        tangent_sol[i].normalize();
        normal_sol[i].normalize();
    }


    transform();


    glGenVertexArrays(1, &VertexArrayID);

     glBindVertexArray(VertexArrayID);
     LoadShader("Shader/normalmapv.vert", "Shader/normalmapf.frag");


    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(ShaderProgram->programId(), "MVP");
    GLuint ViewMatrixID = glGetUniformLocation(ShaderProgram->programId(), "V");
    GLuint ModelMatrixID = glGetUniformLocation(ShaderProgram->programId(), "M");
    GLuint ModelView3x3MatrixID = glGetUniformLocation(ShaderProgram->programId(), "MV3x3");



    // Get a handle for our "myTextureSampler" uniform
    GLuint DiffuseTextureID  = glGetUniformLocation(ShaderProgram->programId(), "DiffuseTextureSampler");
    GLuint NormalTextureID  = glGetUniformLocation(ShaderProgram->programId(), "NormalTextureSampler");
    GLuint SpecularTextureID  = glGetUniformLocation(ShaderProgram->programId(), "SpecularTextureSampler");


    vector<unsigned short> indices;
    vector<glm::vec3> indexed_vertices;
    vector<glm::vec2> indexed_uvs;
    vector<glm::vec3> indexed_normals;
    vector<glm::vec3> indexed_tangents;
    vector<glm::vec3> indexed_bitangents;

    indexVBO_TBN2(
                vertex_sol1, textcoord_sol1, normal_sol1, tangent_sol1, bitangent_sol1,
                indices, indexed_vertices, indexed_uvs, indexed_normals, indexed_tangents, indexed_bitangents
                );
    // Load it into a VBO

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

    GLuint normalbuffer;
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

    GLuint tangentbuffer;
    glGenBuffers(1, &tangentbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_tangents.size() * sizeof(glm::vec3), &indexed_tangents[0], GL_STATIC_DRAW);

    GLuint bitangentbuffer;
    glGenBuffers(1, &bitangentbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_bitangents.size() * sizeof(glm::vec3), &indexed_bitangents[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well
    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

    // Get a handle for our "LightPosition" uniform
    glUseProgram(ShaderProgram->programId());
    GLuint LightID = glGetUniformLocation(ShaderProgram->programId(), "LightPosition_worldspace");

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Compute the MVP matrix from keyboard and mouse input
    glm::mat4 Proj;

    GLfloat ProjectionMatrix[16];
    camera()->getProjectionMatrix(ProjectionMatrix);
    int proj = 0;
    for(int i = 0; i< 4; ++i){
        for(int j = 0; j< 4; ++j){
            Proj[i][j] = ProjectionMatrix[proj];
            ++proj;
        }
    }
    mat4 ViewMatrix = glm::lookAt(vec3(camera()->position().x,
                                       camera()->position().y,
                                       camera()->position().z),
                                  vec3(camera()->position().x + camera()->viewDirection().x,
                                       camera()->position().y + camera()->viewDirection().y,
                                       camera()->position().z + camera()->viewDirection().z),
                                  vec3(camera()->upVector().x,
                                       camera()->upVector().y,
                                       camera()->upVector().z)
                                  );
    glm::mat4 ModelMatrix = glm::mat4(1.0);
    glm::mat4 ModelViewMatrix = ViewMatrix * ModelMatrix;
    glm::mat3 ModelView3x3Matrix = glm::mat3(ModelViewMatrix);
    glm::mat4 MVP = Proj * ViewMatrix * ModelMatrix;


    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
    glUniformMatrix3fv(ModelView3x3MatrixID, 1, GL_FALSE, &ModelView3x3Matrix[0][0]);


    glm::vec3 lightPos = glm::vec3(0,0,0);

    glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);



    // Bind our diffuse texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    // Set our "DiffuseTextureSampler" sampler to user Texture Unit 0
    glUniform1i(DiffuseTextureID, 0);


    // Bind our normal texture in Texture Unit 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    // Set our "Normal      TextureSampler" sampler to user Texture Unit 0
    glUniform1i(NormalTextureID, 1);

    // Bind our specular texture in Texture Unit 2
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    // Set our "Normal      TextureSampler" sampler to user Texture Unit 0
    glUniform1i(SpecularTextureID, 2);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
                0,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(
                1,                                // attribute
                2,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
                );

    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(
                2,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
                );

    // 4th attribute buffer : tangents
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
    glVertexAttribPointer(
                3,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
                );

    // 5th attribute buffer : bitangents
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
    glVertexAttribPointer(
                4,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
                );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

    // Draw the triangles !
    glDrawElements(
                GL_TRIANGLES,      // mode
                indices.size(),    // count
                GL_UNSIGNED_SHORT, // type
                (void*)0           // element array buffer offset
                );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteBuffers(1, &normalbuffer);
    glDeleteBuffers(1, &tangentbuffer);
    glDeleteBuffers(1, &bitangentbuffer);
    glDeleteBuffers(1, &elementbuffer);
    //glDeleteProgram(ShaderProgram->programId());
    glDeleteTextures(1, &texture[0]);
    glDeleteTextures(1, &texture[1]);
    glDeleteTextures(1, &texture[2]);

    // Swap buffers
    glDeleteVertexArrays(1, &VertexArrayID);


}

void Viewer::computeLocalSpace(Face &face){

    // on calcule 2 vecteurs formant les bords du carré
    // le vecteur side0 est celui allant du vertex 0 au vertex 1
    Vec side0 = Vec(face.vertexIndex[1]) - Vec(face.vertexIndex[0]);

    // le vecteur side1 est celui allant du vertex 0 au vertex 2
    Vec side1 = Vec(face.vertexIndex[2]) - Vec(face.vertexIndex[0]);

    // ici on calcule la normale à la face
    Vec normal1 = cross(side0, side1);
    normal1.normalize();


    // maintenant on calcule les coefficients des tangentes
    // ces coefficients sont calcules en fonction des
    // coordonnées de textures du polygone

    float deltaT0 = face.texCoordIndex[1].y -
            face.texCoordIndex[0].y;

    float deltaT1 = face.texCoordIndex[2].y -
            face.texCoordIndex[0].y;

    // on effectue la meme chose pour la binormale
    float deltaB0 = face.texCoordIndex[1].x -
            face.texCoordIndex[0].x;

    float deltaB1 = face.texCoordIndex[2].x -
            face.texCoordIndex[0].x;

    // le facteur permettant de rendre les tangentes et binormales
    // proportionnelles aux coordonnees de textures.
    // nous aurons donc des vecteurs dont la norme depend des coordonnees
    // de textures.
    float scale = 1/ ((deltaB0 * deltaT1) - (deltaB1 * deltaT0));

    // on calcule la tangente temporaire
    Vec tmpTangente = ((side0*deltaT1) - (side1*deltaT0))*scale;
    //cout << "Tangente non norm " << tmpTangente << " plz "<< endl;
    tmpTangente.normalize();
    //cout << "Tangente norm " << tmpTangente << " plz "<< endl;

    // on calcule la binormale temporaire

    Vec tmpBinormal = ((side0*(-deltaB1)) + (side1*deltaB0))*scale;
    tmpBinormal.normalize();

    // nous avons donc maintenant les tangentes, binormales et normales de la face.
    // on les additionne a celles deja presentes dans les tableaux
    tangent_sol[count_vertex_sol].operator +=(tmpTangente);
    tangent_sol[count_vertex_sol+1].operator +=( tmpTangente);
    tangent_sol[count_vertex_sol+2].operator +=( tmpTangente);

    bitangent_sol[count_vertex_sol].operator +=(  tmpBinormal);
    bitangent_sol[count_vertex_sol+1].operator +=( tmpBinormal);
    bitangent_sol[count_vertex_sol+2].operator +=( tmpBinormal);

    normal_sol[count_vertex_sol].operator +=( normal1);
    normal_sol[count_vertex_sol+1].operator +=( normal1);
    normal_sol[count_vertex_sol+2].operator +=( normal1);

    // il ne faudra pas oublier de renormaliser les vecteurs par la suite.
    count_vertex_sol += 3;
}

void Viewer::drawLocalSpace()
{

    glBegin(GL_LINES);
    for(int i = 0;i < nbfaces; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            Vec &v = faces[i].vertexIndex[j];
            // tangente en rouge
            Vec tangent = tangent_sol[nbfaces+j] + v;
            glColor3f(1,0,0);
            glVertex3f(v.x,v.y,v.z);
            glVertex3f(tangent.x,tangent.y,tangent.z);
            // binormale en vert
            Vec binormal = bitangent_sol[nbfaces+j] + v;

            glColor3f(0,1,0);
            glVertex3f(v.x,v.y,v.z);
            glVertex3f(binormal.x,binormal.y,binormal.z);
            // normale en bleu
            Vec normal = normal_sol[nbfaces+j] + v;

            glColor3f(0,0,1);
            glVertex3f(v.x,v.y,v.z);
            glVertex3f(normal.x,normal.y,normal.z);
        }
    }
    glEnd();
    glColor3f(1,1,1);


}

int Viewer::LoadGLTextures(QString filename){
    int status = false;
    QImage image = QGLWidget::convertToGLFormat(QImage(filename));
    if(!image.isNull())
    {
        status = true;

        glGenTextures(1, &texture[count_text]);
        glBindTexture(GL_TEXTURE_2D, texture[count_text]);
        gluBuild2DMipmaps(GL_TEXTURE_2D,     // texture to specify
                          GL_RGBA,           // internal texture storage format
                          image.width(),     // texture width
                          image.height(),    // texture height
                          GL_RGBA,           // pixel format
                          GL_UNSIGNED_BYTE,  // color component format
                          image.bits());     // pointer to texture image
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
        ++ count_text;
    }
    return status;
}

void Viewer::transform(){

    for (int i = 0; i< vertex_sol.size(); ++i){
        vertex_sol1.push_back(vec3(vertex_sol.at(i).x,vertex_sol.at(i).y,vertex_sol.at(i).z));
        textcoord_sol1.push_back(vec2(textcoord_sol.at(i).x,textcoord_sol.at(i).y));
        normal_sol1.push_back(vec3(normal_sol.at(i).x,normal_sol.at(i).y,normal_sol.at(i).z));
        tangent_sol1.push_back(vec3(tangent_sol.at(i).x,tangent_sol.at(i).y,tangent_sol.at(i).z));
        bitangent_sol1.push_back(vec3(bitangent_sol.at(i).x,bitangent_sol.at(i).y,bitangent_sol.at(i).z));
    }


}

void Viewer::indexVBO_TBN2(
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
){

    // For each input vertex
    for ( unsigned int i=0; i<in_vertices.size(); i++ ){

        // Try to find a similar vertex in out_XXXX
        unsigned short index;
        bool found = this->getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i],     out_vertices, out_uvs, out_normals, index);

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
        }
    }

}

bool Viewer::getSimilarVertexIndex_fast(
    PackedVertex & packed,
    std::map<PackedVertex,unsigned short> & VertexToOutIndex,
    unsigned short & result
){
    std::map<PackedVertex,unsigned short>::iterator it = VertexToOutIndex.find(packed);
    if ( it == VertexToOutIndex.end() ){
        return false;
    }else{
        result = it->second;
        return true;
    }
}

bool Viewer::getSimilarVertexIndex(
    glm::vec3 & in_vertex,
    glm::vec2 & in_uv,
    glm::vec3 & in_normal,
    std::vector<glm::vec3> & out_vertices,
    std::vector<glm::vec2> & out_uvs,
    std::vector<glm::vec3> & out_normals,
    unsigned short & result
){
    // Lame linear search
    for ( unsigned int i=0; i<out_vertices.size(); i++ ){
        if (
            is_near( in_vertex.x , out_vertices[i].x ) &&
            is_near( in_vertex.y , out_vertices[i].y ) &&
            is_near( in_vertex.z , out_vertices[i].z ) &&
            is_near( in_uv.x     , out_uvs     [i].x ) &&
            is_near( in_uv.y     , out_uvs     [i].y ) &&
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
// Returns true iif v1 can be considered equal to v2
bool Viewer::is_near(float v1, float v2){
    return fabs( v1-v2 ) < 0.01f;
}

void Viewer::LoadShader(QString vshader, QString fshader)
{
    const GLubyte *renderer = glGetString( GL_RENDERER );
    const GLubyte *vendor = glGetString( GL_VENDOR );
    const GLubyte *version = glGetString( GL_VERSION );
    const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );

    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    printf("GL Vendor : %s\n", vendor);
    printf("GL Renderer : %s\n", renderer);
    printf("GL Version (string) : %s\n", version);
    printf("GL Version (integer) : %d.%d\n", major, minor);
    printf("GLSL Version : %s\n", glslVersion);

    GLint nExtensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);

    for( int i = 0; i < nExtensions; i++ )
       printf("%s\n", glGetStringi( GL_EXTENSIONS, i ) );

    if(ShaderProgram)
        {
        ShaderProgram->release();
        ShaderProgram->removeAllShaders();
        }
    else ShaderProgram = new QGLShaderProgram;

    if(VertexShader)
        {
        //delete VertexShader;
        VertexShader = NULL;
        }

    if(FragmentShader)
        {
        //delete FragmentShader;
        FragmentShader = NULL;
        }
    cout << "hoooooooh" << endl;

    // load and compile vertex shader
    QFileInfo vsh(vshader);
    if(vsh.exists())
        {
        VertexShader = new QGLShader(QGLShader::Vertex);
        if(VertexShader->compileSourceFile(vshader))
            ShaderProgram->addShader(VertexShader);
        else qWarning() << "Vertex Shader Error" << VertexShader->log();
        }
    else qWarning() << "Vertex Shader source file " << vshader << " not found.";


    // load and compile fragment shader
    QFileInfo fsh(fshader);
    if(fsh.exists())
        {
        FragmentShader = new QGLShader(QGLShader::Fragment);
        if(FragmentShader->compileSourceFile(fshader))
            ShaderProgram->addShader(FragmentShader);
        else qWarning() << "Fragment Shader Error" << FragmentShader->log();
        }
    else qWarning() << "Fragment Shader source file " << fshader << " not found.";

    if(!ShaderProgram->link())
        {
        qWarning() << "Shader Program Linker Error" << ShaderProgram->log();
        }
    else ShaderProgram->bind();

}
