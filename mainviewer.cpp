#include "mainviewer.h"
#include <QGLViewer/vec.h>
#include <QString>
#include <QDir>
using namespace std;
using namespace qglviewer;

MainViewer::MainViewer()
{
    count_text = 0;
    current_trace_text = 5;
    ambiant.setRgb(25,25,25);
    drawspace = false;
    circle2 = false;

}

//Initialisations
void MainViewer::init()
{

    if (glewInit() != GLEW_OK) {
        cout << "Failed to initialize GLEW" << endl;
        exit (0);
    }else{
        cout << "Works like a charm" << endl;
    }
    glClearColor(0.1, 0.1, 0.3, 0.0);

    // Light setup
    glDisable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    // Light default parameters
    const GLfloat light_ambient[4]  = {1.0, 1.0, 1.0, 1.0};
    const GLfloat light_specular[4] = {1.0, 1.0, 1.0, 1.0};
    const GLfloat light_diffuse[4]  = {1.0, 1.0, 1.0, 1.0};
    const GLfloat light_pos[3]  = {16.0, 16.0, 1.0};

    glLightf( GL_LIGHT1, GL_SPOT_EXPONENT, 200.0);
    glLightf( GL_LIGHT1, GL_SPOT_CUTOFF,   10.0);
    glLightf( GL_LIGHT1, GL_CONSTANT_ATTENUATION,  0.1f);
    glLightf( GL_LIGHT1, GL_LINEAR_ATTENUATION,    0.3f);
    glLightf( GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.3f);
    glLightfv(GL_LIGHT1, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light_diffuse);
    //glLightfv(GL_LIGHT1, GL_POSITION, light_pos);


    initgl();

    setSceneRadius(100.0);          // scene has a 100 OpenGL units radius
    setSceneCenter( Vec(16.0 , 16.0, 5.0 ) );
    // Move camera according to viewer type (on X, Y or Z axis)

    camera()->setPosition(Vec( 16.0 , 16.0, 5.0 ));
    camera()->lookAt(Vec(16.0, 32.0, 5.0 ));
    this->setGridIsDrawn(false);
    //Setting de la caméra
    constraints[0] = new WorldConstraint();
    constraints[1] = new LocalConstraint();

    constraints[1]->setTranslationConstraintType(AxisPlaneConstraint::AXIS);
    constraints[1]->setRotationConstraintType(AxisPlaneConstraint::AXIS);

    Vec dir(0.0, 0.0, 1.0);
    constraints[1]->setTranslationConstraintDirection(dir);
    constraints[1]->setRotationConstraintDirection(Vec(0.0, 1.0, 0.0));
    camera()->frame()->setConstraint(constraints[1]);



}

// Initialisation des textures
void MainViewer::initgl(){
    glEnable(GL_TEXTURE_2D);
    if (!LoadGLTextures(QString("../../../Textures/pave.jpg")))                         // Jump To Texture Loading Routine ( NEW )
    {
        //cout << "current Path" << QDir::current().absolutePath().toStdString() << endl;
        cout << " pave Not loaded" << endl;
    }
    if (!LoadGLTextures(QString("../../../Textures/brique.jpg")))                       // Jump To Texture Loading Routine ( NEW )
    {

        cout << "  brique Not loaded" << endl;
    }

    if (!LoadGLTextures(QString("../../../Textures/marbre.jpg")))                   // Jump To Texture Loading Routine ( NEW )
    {
        cout << " marbre Not loaded" << endl;
    }
    if (!LoadGLTextures(QString("../../../Textures/pierre.jpg")))                          // Jump To Texture Loading Routine ( NEW )
    {
        cout << " pierre Not loaded" << endl;

    }

    if (!LoadGLTextures(QString("../../../Textures/rock.jpg")))                      // Jump To Texture Loading Routine ( NEW )
    {
        cout << "rock Not loaded" << endl;

    }

    if (!LoadGLTextures(QString("../../../Textures/rouille.jpg")))                      // Jump To Texture Loading Routine ( NEW )
    {
        cout << " rouille Not loaded" << endl;

    }
    if (!LoadGLTextures(QString("../../../Textures/fissure.jpg")))                      // Jump To Texture Loading Routine ( NEW )
    {
        cout << " fissure Not loaded" << endl;

    }
    if (!LoadGLTextures(QString("../../../Textures/mars.jpg")))                      // Jump To Texture Loading Routine ( NEW )
    {
        cout << " mars Not loaded" << endl;

    }
    if (!LoadGLTextures(QString("../../../Textures/mur.jpg")))                      // Jump To Texture Loading Routine ( NEW )
    {
        cout << " mur Not loaded" << endl;
    }


}


// ////////////MULTITEXTURING//////////
void MainViewer::config_norm(){


    // on configure la premiere unité de texture : elle contient la texture de la scene
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    // on effectue le rendu de la lumiere ambiante
    init_Lighting(ambiant);

    // on doit reconfigurer les unites de textures
    // la premiere unite contient la normal map
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    // on configure la normal map pour qu'elle effectue la passe de DOT3
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE_ARB);
    glTexEnvf(GL_TEXTURE_ENV,GL_RGB_SCALE_ARB,1);
    glTexEnvf(GL_TEXTURE_ENV,GL_COMBINE_RGB_ARB,GL_DOT3_RGB_ARB);
    glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE0_RGB_ARB,GL_PREVIOUS_ARB);
    glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE1_RGB_ARB,GL_TEXTURE);
    glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND0_RGB_ARB,GL_SRC_COLOR);
    glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND1_RGB_ARB,GL_SRC_COLOR);


    // la troisieme contient la texture de diffuse
    glActiveTexture(GL_TEXTURE2);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture[3] );
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    // on doit activer le blending additif pour pouvoir utiliser ce mode d'eclairage
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE,GL_ONE);
    glDepthMask(GL_FALSE);
    // on desactive la troisieme unitÈ de texture
    glActiveTextureARB(GL_TEXTURE2);
    glDisable(GL_TEXTURE_2D);

    // on desactive la premiere unitÈ de texture
    glActiveTextureARB(GL_TEXTURE0);
    glDisable(GL_TEXTURE_2D);

    glDisable(GL_BLEND);

    glFlush();


}

/////////// Méthodes draw //////////////
void MainViewer::draw()
{
    test_drawEnvironnment();

}

int MainViewer::test_drawEnvironnment(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    drawWithNames();

}
//Dessin de l'environnement avec les indices pour chaque polygone////////
void MainViewer::drawWithNames(){
    //Initialisations
    glColor3f(1.0f, 1.0f, 1.0f);
    nbvertex = 0;
    count_names= 1;
    int nbx = 16;
    int nby = 16;
    int nbz = 6;
    int count_buffer = count_names;
    nbfaces= nbx *nby;

    //Init du tableau de sommets, normales et tangentes
    normals.clear();
    tangents.clear();
    bitangents.clear();


    glPushMatrix();
    for(int i = 0 ; i < nbx ; ++i){
        for(int j = 0 ; j < nby ; ++j){
            glPushMatrix();
            glPushName( count_names );
            test_draw_env_quad_base(count_names == selectedName(), (i*2.0+1.0), (j*2.0+1.0));
            glPopName();
            glPopMatrix();
            ++ count_names;
        }
    }
    if (selectedName() >= count_buffer && selectedName()< count_names ){
        draw_trace(true, false);
        cout << "Sol" << endl;
    }
    glPopMatrix();
    if(drawspace) remplissage_face();


    count_buffer = count_names;
    glPushMatrix();


    glBindTexture(GL_TEXTURE_2D, texture[1]);               // Select Our Texture
    glRotatef(90.0, 0.0, 0.0, 1.0);
    for(int i = 0 ; i < nbx ; ++i){
        for(int j = 0 ; j < nbz ; ++j){
            glPushMatrix();
            glPushName( count_names );
            test_draw_env_quad_wall(count_names == selectedName(),(i*2.0+1.0), (j*2.0+1.0));
            glPopName();
            glPopMatrix();
            ++ count_names;
        }
    }


    glPopMatrix();
    if (selectedName() >= count_buffer && selectedName()< count_names ){
        draw_trace(false,1);
        cout << "Mur gauche" << endl;

    }
    count_buffer = count_names;
    glBindTexture(GL_TEXTURE_2D, texture[2]);               // Select Our Texture

    glPushMatrix();
    glTranslatef(0.0 ,nby*2.0, 0.0 );
    for(int i = 0 ; i < nby ; ++i){
        for(int j = 0 ; j < nbz ; ++j){
            glPushMatrix();
            glPushName(count_names );
            test_draw_env_quad_wall(count_names == selectedName(),(i*2.0+1.0), (j*2.0+1.0));
            glPopName();
            glPopMatrix();
            ++ count_names;
        }
    }

    glPopMatrix();
    if (selectedName() >= count_buffer && selectedName()< count_names ){
        draw_trace(false,2);
        cout << "Mur front" << endl;

    }
    count_buffer = count_names;

    glBindTexture(GL_TEXTURE_2D, texture[3]);               // Select Our Texture
    glPushMatrix();
    glTranslatef(nbx*2.0 ,nby*2.0, 0.0 );
    glRotatef(-90.0, 0.0, 0.0, 1.0);
    for(int i = 0 ; i < nbx ; ++i){
        for(int j = 0 ; j < nbz ; ++j){
            glPushMatrix();
            glPushName( count_names );
            test_draw_env_quad_wall(count_names == selectedName(),(i*2.0+1.0), (j*2.0+1.0));
            glPopName();
            glPopMatrix();
            ++ count_names;
        }
    }
    glPopMatrix();
    if (selectedName() >= count_buffer && selectedName()< count_names ){
        draw_trace(false, 3);
        cout << "Mur droite" << endl;

    }
    count_buffer = count_names;
    glBindTexture(GL_TEXTURE_2D, texture[4]);               // Select Our Texture
    glPushMatrix();
    for(int i = 0 ; i < nbx ; ++i){
        for(int j = 0 ; j < nbz ; ++j){
            glPushMatrix();
            glPushName( count_names );
            test_draw_env_quad_wall(count_names == selectedName(),(i*2.0+1.0), (j*2.0+1.0));
            glPopName();
            glPopMatrix();
            ++ count_names;
        }
    }
    glPopMatrix();
    if (selectedName() >= count_buffer && selectedName()< count_names ){
        draw_trace(false,4);
        cout << "Mur back" << endl;

    }

    if(traces.size() > 0){
        draw_prev_trace();
    }
}
// Dessin des dégradation précédentes///
void MainViewer::draw_prev_trace(){

    for(int i = 0; i < traces.size(); ++i){
        glBindTexture(GL_TEXTURE_2D, texture[traces[i].num_text]);               // Select Our Texture

        if(traces[i].circle ) draw_circle(traces[i].num_mur == 0, traces[i].num_text, traces[i].num_mur, traces[i].rayon, traces[i].coord.x, traces[i].coord.y, traces[i].coord.z,20);
        else compute_trace(traces[i].num_mur == 0, traces[i].num_text, traces[i].num_mur, traces[i].rayon, traces[i].coord.x, traces[i].coord.y, traces[i].coord.z);
    }


}
//Dessin de la dégradation///////
void MainViewer::draw_trace(bool flat, const int mur = -1){

    float x_point = selectedPoint.x;
    float y_point = selectedPoint.y;
    float z_point = selectedPoint.z;
    float rayon = 1.0;
    cout << "Point sélectionné x :  " << x_point << " en y ; " << y_point << "en z : " << z_point << endl;
    Trace_coord t;
    t.coord = glm::vec3(x_point, y_point, z_point);
    t.num_mur = mur;
    t.num_text = current_trace_text;
    t.rayon = rayon;
    t.circle = circle2;
    if(t.circle) draw_circle(flat,current_trace_text, mur, rayon, x_point, y_point, z_point,20);
    else     compute_trace(flat,current_trace_text, mur, rayon, x_point, y_point, z_point);
    traces.append(t);

}

//Dessin de dégradation en forme de cercle cercle ///////////
void MainViewer::draw_circle(bool flat, int num_text, int mur, const GLfloat radius, float x_point, float y_point, float z_point ,const GLuint num_vertex)
{
    GLfloat vertex[4];
    GLfloat texcoord[2];

    const GLfloat delta_angle = 2.0*M_PI/num_vertex;

    glBindTexture(GL_TEXTURE_2D,texture[num_text]);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    glBegin(GL_TRIANGLE_FAN);

    //draw the vertex at the center of the circle
    texcoord[0] = 0.5;
    texcoord[1] = 0.5;
    glTexCoord2fv(texcoord);

    vertex[0] = x_point;
    vertex[1] = y_point;
    vertex[2] = z_point;
    vertex[3] = 1.0;
    glVertex4fv(vertex);

    if(!flat){

        if(mur ==1 ){
            for(int i = 0; i < num_vertex ; i++)
            {
                texcoord[0] = (std::cos(delta_angle*i) + 1.0)*0.5;
                texcoord[1] = (std::sin(delta_angle*i) + 1.0)*0.5;
                glTexCoord2fv(texcoord);

                vertex[0] = 0.0+x_point;
                vertex[1] = std::cos(delta_angle*i) * radius+ y_point;
                vertex[2] = std::sin(delta_angle*i) * radius +z_point;
                vertex[3] = 1.0;
                glVertex4fv(vertex);
            }

            texcoord[0] = (1.0 + 1.0)*0.5;
            texcoord[1] = (0.0 + 1.0)*0.5;
            glTexCoord2fv(texcoord);

            vertex[0] = 0.0 + x_point;
            vertex[1] = 1.0 * radius + y_point;
            vertex[2] = 0.0 * radius + z_point;
            vertex[3] = 1.0;
            glVertex4fv(vertex);
        }
        if(mur == 3){
            for(int i = 0; i < num_vertex ; i++)
            {
                texcoord[0] = (std::sin(delta_angle*i) + 1.0)*0.5;
                texcoord[1] = (std::cos(delta_angle*i) + 1.0)*0.5;
                glTexCoord2fv(texcoord);

                vertex[0] = 0.0+x_point;
                vertex[1] = std::sin(delta_angle*i) * radius + y_point;
                vertex[2] = std::cos(delta_angle*i) * radius + z_point;
                vertex[3] = 1.0;
                glVertex4fv(vertex);
            }

            texcoord[0] = (1.0 + 1.0)*0.5;
            texcoord[1] = (0.0 + 1.0)*0.5;
            glTexCoord2fv(texcoord);

            vertex[0] = 0.0 + x_point;
            vertex[1] = 0.0 * radius + y_point;
            vertex[2] = 1.0 * radius + z_point;
            vertex[3] = 1.0;
            glVertex4fv(vertex);
        }
        if (mur == 2){
            for(int i = 0; i < num_vertex ; i++)
            {
                texcoord[0] = (std::cos(delta_angle*i) + 1.0)*0.5;
                texcoord[1] = (std::sin(delta_angle*i) + 1.0)*0.5;
                glTexCoord2fv(texcoord);

                vertex[0] = std::cos(delta_angle*i) * radius + x_point;
                vertex[1] = 0.0+ y_point;
                vertex[2] = std::sin(delta_angle*i) * radius+ z_point;
                vertex[3] = 1.0;
                glVertex4fv(vertex);
            }

            texcoord[0] = (1.0 + 1.0)*0.5;
            texcoord[1] = (0.0 + 1.0)*0.5;
            glTexCoord2fv(texcoord);

            vertex[0] = 1.0 * radius + x_point;
            vertex[1] = 0.0 + y_point;
            vertex[2] = 0.0 * radius + z_point;
            vertex[3] = 1.0;
            glVertex4fv(vertex);
        }
        if (mur == 4){
            for(int i = 0; i < num_vertex ; i++)
            {
                texcoord[0] = (std::sin(delta_angle*i) + 1.0)*0.5;
                texcoord[1] = (std::cos(delta_angle*i) + 1.0)*0.5;
                glTexCoord2fv(texcoord);

                vertex[0] = std::sin(delta_angle*i) * radius + x_point;
                vertex[1] = 0.0+ y_point;
                vertex[2] = std::cos(delta_angle*i) * radius+ z_point;
                vertex[3] = 1.0;
                glVertex4fv(vertex);
            }

            texcoord[0] = (1.0 + 1.0)*0.5;
            texcoord[1] = (0.0 + 1.0)*0.5;
            glTexCoord2fv(texcoord);

            vertex[0] = 0.0 * radius + x_point;
            vertex[1] = 0.0 + y_point;
            vertex[2] = 1.0 * radius + z_point;
            vertex[3] = 1.0;
            glVertex4fv(vertex);
        }
    }else{

        for(int i = 0; i < num_vertex ; i++)
        {
            texcoord[0] = (std::cos(delta_angle*i) + 1.0)*0.5;
            texcoord[1] = (std::sin(delta_angle*i) + 1.0)*0.5;
            glTexCoord2fv(texcoord);

            vertex[0] = std::cos(delta_angle*i) * radius + x_point;
            vertex[1] = std::sin(delta_angle*i) * radius+ y_point;
            vertex[2] = 0.0+ z_point;
            vertex[3] = 1.0;
            glVertex4fv(vertex);
        }

        texcoord[0] = (1.0 + 1.0)*0.5;
        texcoord[1] = (0.0 + 1.0)*0.5;
        glTexCoord2fv(texcoord);

        vertex[0] = 1.0 * radius +x_point;
        vertex[1] = 0.0 * radius + y_point;
        vertex[2] = 0.0 + z_point;
        vertex[3] = 1.0;
        glVertex4fv(vertex);

    }

    glEnd();


}


//Méthode de dessin de dégradation en forme de carré//////
void MainViewer::compute_trace(bool flat, int num_text, int mur, float rayon, float x_point, float y_point, float z_point ){
    glBindTexture(GL_TEXTURE_2D, texture[num_text]);               // Select Our Texture

    glBegin(GL_QUADS);

    if(!flat){

        if(mur ==2 ){

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f( x_point - rayon,  y_point,  z_point - rayon);  // Bottom Left Of The Texture and Quad
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f( x_point + rayon,  y_point,  z_point - rayon);  // Bottom Right Of The Texture and Quad
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f( x_point + rayon,  y_point,  z_point + rayon);  // Top Right Of The Texture and Quad
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f( x_point - rayon,  y_point,  z_point + rayon);  // Top Left Of The Texture and Quad

        }
        if(mur == 4){

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f( x_point + rayon,  y_point,  z_point - rayon);  // Bottom Right Of The Texture and Quad
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f( x_point - rayon,  y_point,  z_point - rayon);  // Bottom Left Of The Texture and Quad
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f( x_point - rayon,  y_point,  z_point + rayon);  // Top Left Of The Texture and Quad
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f( x_point + rayon,  y_point,  z_point + rayon);  // Top Right Of The Texture and Quad

        }
        if (mur == 1){

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f( x_point,  y_point - rayon,  z_point - rayon);  // Bottom Left Of The Texture and Quad
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f( x_point,  y_point + rayon,  z_point - rayon);  // Bottom Right Of The Texture and Quad
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f( x_point,  y_point + rayon,  z_point + rayon);  // Top Right Of The Texture and Quad
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f( x_point,  y_point - rayon,  z_point + rayon);  // Top Left Of The Texture and Quad

        }
        if (mur == 3){

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f( x_point,  y_point + rayon,  z_point - rayon);  // Bottom Right Of The Texture and Quad
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f( x_point,  y_point - rayon,  z_point - rayon);  // Bottom Left Of The Texture and Quad
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f( x_point,  y_point - rayon,  z_point + rayon);  // Top Left Of The Texture and Quad
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f( x_point,  y_point + rayon,  z_point + rayon);  // Top Right Of The Texture and Quad

        }
    }else{
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f( x_point - rayon,  y_point - rayon,  z_point);  // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f( x_point + rayon,  y_point - rayon,  z_point);  // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f( x_point + rayon,  y_point + rayon,  z_point);  // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f( x_point - rayon,  y_point + rayon,  z_point);  // Top Left Of The Texture and Quad
    }
    glEnd();
}


//Méthode de remplissage d'informations pour chaque faces
//(sommets, coordonnées de textures, normales)
void MainViewer::remplissage_face(){
    int counter = 0;
    cout << nbvertex << endl;
    for (int i = 0; i < nbvertex; ++i)
    {
        normals.append(Vec(0.0, 0.0, 0.0));
        tangents.append(Vec(0.0, 0.0, 0.0));
        bitangents.append(Vec(0.0, 0.0, 0.0));
    }


    for (int i = 0; i < nbfaces; ++i)
    {
        QVector<Vec> vertex1;
        vertex1.append(Vec(vertex.at(counter)));
        vertex1.append(Vec(vertex.at(counter +1)));
        vertex1.append(Vec(vertex.at(counter +2)));
        vertex1.append(Vec(vertex.at(counter +3)));
        QVector<qglviewer::Vec> normal;
        normal.append(Vec(normals.at(counter)));
        normal.append(Vec(normals.at(counter +1)));
        normal.append(Vec(normals.at(counter +2)));
        normal.append(Vec(normals.at(counter +3)));
        QVector<qglviewer::Vec> texCoord1;
        texCoord1.append(Vec(textcoord.at(counter)));
        texCoord1.append(Vec(textcoord.at(counter +1)));
        texCoord1.append(Vec(textcoord.at(counter +2)));
        texCoord1.append(Vec(textcoord.at(counter +3)));
        Face a(vertex1,normal,texCoord1);
        faces.append(a);
        counter += 4;

    }

    // maintenant que nous avons toutes les informations sur les faces,
    // on peut calculer l'espace local
    count_vertex = 0;
    for (int i = 0; i <  nbfaces; ++i)
    {
        computeLocalSpace(faces[nbfaces-1]);

    }



    // il ne faut pas oublier de renormaliser les tangentes et binormales
    for (int i = 0; i < nbvertex; ++i)
    {
        bitangents[i].normalize();
        tangents[i].normalize();
        normals[i].normalize();
    }

    drawLocalSpace();


}


//Méthode de génération de normales, tangentes, binormales/////
void MainViewer::computeLocalSpace(Face &face){

    // on calcule 2 vecteurs formant les bords du carré
    // le vecteur side0 est celui allant du vertex 0 au vertex 1
    Vec side0 = Vec(face.vertexIndex[1]) - Vec(face.vertexIndex[0]);

    // le vecteur side1 est celui allant du vertex 0 au vertex 2
    Vec side1 = Vec(face.vertexIndex[2]) - Vec(face.vertexIndex[0]);

    // ici on calcule la normale à la face
    Vec normal1 = cross(side0, side1);
    normalise(normal1);


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
    tmpTangente.normalize();

    // on calcule la binormale temporaire

    Vec tmpBinormal = ((side0*(-deltaB1)) + (side1*deltaB0))*scale;
    tmpBinormal.normalize();

    // nous avons donc maintenant les tangentes, binormales et normales de la face.
    // on les additionne a celles deja presentes dans les tableaux
    tangents[count_vertex].operator +=(tmpTangente);
    tangents[count_vertex+1].operator +=( tmpTangente);
    tangents[count_vertex+2].operator +=( tmpTangente);
    tangents[count_vertex+3].operator +=( tmpTangente);

    bitangents[count_vertex].operator +=(  tmpBinormal);
    bitangents[count_vertex+1].operator +=( tmpBinormal);
    bitangents[count_vertex+2].operator +=( tmpBinormal);
    bitangents[count_vertex+3].operator +=( tmpBinormal);

    normals[count_vertex].operator +=( normal1);
    normals[count_vertex+1].operator +=( normal1);
    normals[count_vertex+2].operator +=( normal1);
    normals[count_vertex+3].operator +=( normal1);

    count_vertex +=4;
}


//Méthode de dessin d'espace tangent/////
void MainViewer::drawLocalSpace()
{
    glBegin(GL_LINES);
    for(int i = 0;i < nbfaces; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Vec &v = faces[i].vertexIndex[j];
            // tangente en rouge
            Vec tangent = tangents[nbfaces+j] + v;
            glColor3f(1,0,0);
            glVertex3f(v.x,v.y,v.z);
            glVertex3f(tangent.x,tangent.y,tangent.z);
            // binormale en vert
            Vec binormal = bitangents[nbfaces+j] + v;

            glColor3f(0,1,0);
            glVertex3f(v.x,v.y,v.z);
            glVertex3f(binormal.x,binormal.y,binormal.z);
            // normale en bleu
            Vec normal = normals[nbfaces+j] + v;

            glColor3f(0,0,1);
            glVertex3f(v.x,v.y,v.z);
            glVertex3f(normal.x,normal.y,normal.z);
        }
    }
    glEnd();
    glColor3f(1,1,1);

}


//Méthode de normalisation////////
void MainViewer::normalise(Vec &a){


    float magnitude = sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
    a.x /= magnitude;
    a.y /= magnitude;
    a.z /= magnitude;
    cout << a.x << "   " << a.y << "  " << a.z << endl;


}

//Méthode de produit vectoriel////////
Vec MainViewer::cross(Vec &a, Vec &b){
    Vec ret;
    ret.x = a.y*b.z - a.z*b.y;
    ret.y = a.z*b.x - a.x*b.z;
    ret.z = a.x*b.y - a.y*b.x;
    return ret;
}
//Méthode de dessin de polygone au sol///////
int MainViewer::test_draw_env_quad_base(const bool specialColor = false, double pos_x = 0, double pos_y = 0){
    glBindTexture(GL_TEXTURE_2D, texture[0]);               // Select Our Texture

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( pos_x - 1.0f,  pos_y - 1.0f,  0.0f);  // Bottom Left Of The Texture and Quad
    vertex.append(Vec(pos_x - 1.0f,  pos_y - 1.0f,  0.0f));
    textcoord.append(Vec(0.0f, 0.0f,  0.0f));
    ++nbvertex;
    glTexCoord2f(1.0f, 0.0f); glVertex3f( pos_x + 1.0f,  pos_y - 1.0f,  0.0f);  // Bottom Right Of The Texture and Quad
    vertex.append(Vec( pos_x + 1.0f,  pos_y - 1.0f,  0.0f ));
    textcoord.append(Vec(1.0f, 0.0f,  0.0f));
    ++nbvertex;
    glTexCoord2f(1.0f, 1.0f); glVertex3f( pos_x + 1.0f,  pos_y + 1.0f,  0.0f);  // Top Right Of The Texture and Quad
    vertex.append(Vec( pos_x + 1.0f,  pos_y + 1.0f,  0.0f ));
    textcoord.append(Vec(1.0f, 1.0f,  0.0f));
    ++nbvertex;
    glTexCoord2f(0.0f, 1.0f); glVertex3f( pos_x - 1.0f,  pos_y + 1.0f,  0.0f);  // Top Left Of The Texture and Quad
    vertex.append(Vec( pos_x - 1.0f,  pos_y + 1.0f,  0.0f ));
    textcoord.append(Vec(0.0f, 1.0f,  0.0f));
    ++nbvertex;
    glEnd();



}
// Méthode de dessin de polygone mural////
int MainViewer::test_draw_env_quad_wall(const bool specialColor = false, double pos_x = 0, double pos_z = 0){

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f( pos_x - 1.0f,  0.0f,  pos_z - 1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( pos_x + 1.0f,  0.0f,  pos_z - 1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( pos_x + 1.0f,  0.0f,  pos_z + 1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( pos_x - 1.0f,  0.0f,  pos_z + 1.0f);  // Top Left Of The Texture and Quad
    glEnd();
}


// Méthode pour afficher les coordonnées du point sélectionné en console/////
void MainViewer::postSelection(const QPoint& point)
{
    // Compute orig and dir, used to draw a representation of the intersecting line
    camera()->convertClickToLine(point, orig, dir);
    // Find the selectedPoint coordinates, using camera()->pointUnderPixel().
    bool found;
    selectedPoint = camera()->pointUnderPixel(point, found);
    selectedPoint -= 0.01f*dir; // Small offset to make point clearly visible.
    cout << "Name selected is :"  << point.x() << "   " << point.y() << endl;
    cout << "The object " << selectedName() << " has been selected" << endl;
}


QString MainViewer::helpString() const
{

}
// Méthode de chagement des textures/////////
int MainViewer::LoadGLTextures(QString filename){

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
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering
        glTexParameteri(GL_TEXTURE_2D,
                        GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        ++ count_text;
    }
    return status;
}



// Méthode pour un éclérage ambiant des polygones///////
void MainViewer::init_Lighting(const QColor& ambiant)
{
    glColor3f(ambiant.red(), ambiant.green(), ambiant.blue());
    glBegin(GL_QUADS);
    for(int i = 0;i < vertex.size(); i++){

        Vec tc = textcoord[i];
        Vec &v = vertex[i];
        glTexCoord2f(tc.x,tc.y);
        glVertex3f(v.x,v.y,v.z);

    }
    glEnd();
}

// Gestion des évènements clavier//////
void MainViewer::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_G : ++current_trace_text;
        if(current_trace_text == 9) current_trace_text = 5;
        break;

    case Qt::Key_D : if(drawspace == false)
            drawspace = true; else
            drawspace = false;
        updateGL();
        break;


    case Qt::Key_Space :
        if(circle2 == false) circle2 = true;
        else circle2= false;
        if(circle2) cout << "changement en circle" << endl;


        break;


    case Qt::Key_Up :
        camera()->frame()->translate(camera()->viewDirection());
        updateGL();
        break;

    case Qt::Key_Down :
        camera()->frame()->translate(-(camera()->viewDirection()));
        updateGL();
        break;

    case Qt::Key_Left :
        camera()->frame()->rotate(Quaternion(Vec(0,1,0),0.1*M_PI));
        updateGL();
        break;

    case Qt::Key_Right :
        camera()->frame()->rotate(Quaternion(Vec(0,1,0),-0.1*M_PI));
        updateGL();
        break;


    default:
        QGLViewer::keyPressEvent(e);

    }


}


void MainViewer::indexVBO_TBN2(
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
        ){

}
