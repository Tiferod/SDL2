#ifdef WIN32
#include <GL/glew.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#endif

#include <SDL2/SDL.h>
#include <iostream>
#include "SceneOpenGL.h"

int SceneOpenGL::m_height[2000][2000];
int SceneOpenGL::m_dHeight[2000][2000];
Chunk *SceneOpenGL::m_tableauChunk[10][10];

int main(int argc, char **argv)
{
    glewExperimental=true;
    GLenum err=glewInit();
    if(err!=GLEW_OK)
    {
        std::cout<<"glewInit failed, aborting."<<std::endl;
    }
    SceneOpenGL scene("Procedural", 1280, 720);

    if(scene.initialiserFenetre() == false)
        return -1;

    if(scene.initGL() == false)
        return -1;

    scene.bouclePrincipale();

    return 0;
}
