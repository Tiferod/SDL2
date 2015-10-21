#include "SceneOpenGL.h"
#include "Shader.h"
#include <math.h>
#include "Chunk.h"

SceneOpenGL::SceneOpenGL(std::string titreFenetre, int largeurFenetre, int hauteurFenetre) : m_titreFenetre(titreFenetre), m_largeurFenetre(largeurFenetre), m_hauteurFenetre(hauteurFenetre), m_fenetre(0), m_contexteOpenGL(0), m_input(), m_dAffichage(10), m_dChunk(25)
{
    int m = 0;
    for (int i = 0 ; i < 2000 ; i++)
    {
        for (int j = 0 ; j < 2000 ; j++)
        {
            m_height[i][j]= int(50*Get2DPerlinNoiseValue(i, j, 50));
        }
    }
    for (int i = 0 ; i < 2000 ; i++)
    {
        for (int j = 0 ; j < 2000 ; j++)
        {
            m_dHeight[i][j]= 1;
        }
    }
    for (int i = 1 ; i < 1999 ; i++)
    {
        for (int j = 1 ; j < 1999 ; j++)
        {
            m = min(min(min(m_height[i+1][j],m_height[i-1][j]),m_height[i][j-1]),m_height[i][j+1]);
            if (m+1 < m_height[i][j])
            {
                m_dHeight[i][j] = m_height[i][j] - m;
            }
        }
    }
}

SceneOpenGL::~SceneOpenGL()
{
    SDL_GL_DeleteContext(m_contexteOpenGL);
    SDL_DestroyWindow(m_fenetre);
    SDL_Quit();
}

bool SceneOpenGL::initialiserFenetre()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    m_fenetre = SDL_CreateWindow(m_titreFenetre.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_largeurFenetre, m_hauteurFenetre, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if(m_fenetre == 0)
    {
        std::cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    m_contexteOpenGL = SDL_GL_CreateContext(m_fenetre);
    if(m_contexteOpenGL == 0)
    {
        std::cout << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_fenetre);
        SDL_Quit();
        return false;
    }
    return true;
}

bool SceneOpenGL::initGL()
{
    #ifdef WIN32

        GLenum initialisationGLEW( glewInit() );
        if(initialisationGLEW != GLEW_OK)
        {
            std::cout << "Erreur d'initialisation de GLEW : " << glewGetErrorString(initialisationGLEW) << std::endl;
            SDL_GL_DeleteContext(m_contexteOpenGL);
            SDL_DestroyWindow(m_fenetre);
            SDL_Quit();
            return false;
        }

    #endif

    glEnable(GL_DEPTH_TEST);

    return true;
}

void SceneOpenGL::bouclePrincipale()
{
    Uint32 frameRate (1000 / 60);
    Uint32 debutBoucle(0), finBoucle(0), tempsEcoule(0);

    //SDL_EnableKeyRepeat(10,10);

    bool terminer(false);
    int positionX = 0;
    int positionZ = 0;

    mat4 projection;
    mat4 modelview;

    projection = perspective(70.0, (double) m_largeurFenetre / m_hauteurFenetre, 1.0, 1000.0);
    modelview = mat4(1.0);

    Camera camera(vec3(0, 5, 0), vec3(0, 2, 0), vec3(0, 1, 0), 5, 1);
    m_input.afficherPointeur(false);
    m_input.capturerPointeur(true);

    for (int i = -m_dAffichage ; i < m_dAffichage ; i++)
        {
            for (int j = -m_dAffichage ; j < m_dAffichage ; j++)
            {
                if (!m_tableauChunk[i+m_dAffichage][j+m_dAffichage])
                {
                    Chunk *chunk = new Chunk((i+positionX/16)*16, (j+positionZ/16)*16, "Shaders/texture.vert", "Shaders/texture.frag");
                    chunk->charger();
                    m_tableauChunk[i+m_dAffichage][j+m_dAffichage] = chunk;
                }
            }
        }

    /*Cube cubeLF(0, 0, "Shaders/texture.vert", "Shaders/texture.frag");
    cubeLF.charger();
    Cube cubeLB(0, 1, "Shaders/texture.vert", "Shaders/texture.frag");
    cubeLB.charger();
    Cube cubeRF(1, 0, "Shaders/texture.vert", "Shaders/texture.frag");
    cubeRF.charger();
    Cube cubeRB(1, 1, "Shaders/texture.vert", "Shaders/texture.frag");
    cubeRB.charger();*/

    while(!m_input.terminer())
    {
        debutBoucle = SDL_GetTicks();
        m_input.updateEvenements();
        if(m_input.getTouche(SDL_SCANCODE_ESCAPE))
            break;

        camera.deplacer(m_input);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        modelview = lookAt(vec3(0, 5, 0), vec3(0, 5, 5), vec3(0, 1, 0));
        camera.lookAt(modelview);

        if (positionX / 16 < int(camera.getPosition().x) / 16)
        {
            for (int j = -m_dAffichage ; j < m_dAffichage ; j++)
            {
                delete m_tableauChunk[0][j+m_dAffichage];
                for (int i = -m_dAffichage ; i < m_dAffichage - 1 ; i++)
                {
                    m_tableauChunk[i+m_dAffichage][j+m_dAffichage] = m_tableauChunk[i+1+m_dAffichage][j+m_dAffichage];
                }
                Chunk *chunk = new Chunk((m_dAffichage+positionX/16)*16, (j+positionZ/16)*16, "Shaders/texture.vert", "Shaders/texture.frag");
                chunk->charger();
                m_tableauChunk[2*m_dAffichage-1][j+m_dAffichage] = chunk;
            }
        }
        if (positionX / 16 > int(camera.getPosition().x) / 16)
        {
            for (int j = -m_dAffichage ; j < m_dAffichage ; j++)
            {
                delete m_tableauChunk[2*m_dAffichage-1][j+m_dAffichage];
                for (int i = m_dAffichage-1 ; i > -m_dAffichage ; i--)
                {
                    m_tableauChunk[i+m_dAffichage][j+m_dAffichage] = m_tableauChunk[i-1+m_dAffichage][j+m_dAffichage];
                }
                Chunk *chunk = new Chunk((-m_dAffichage+positionX/16)*16, (j+positionZ/16)*16, "Shaders/texture.vert", "Shaders/texture.frag");
                chunk->charger();
                m_tableauChunk[0][j+m_dAffichage] = chunk;
            }
        }
        if (positionZ / 16 < int(camera.getPosition().z) / 16)
        {
            for (int i = -m_dAffichage ; i < m_dAffichage ; i++)
            {
                delete m_tableauChunk[i+m_dAffichage][0];
                for (int j = -m_dAffichage ; j < m_dAffichage-1 ; j++)
                {
                    m_tableauChunk[i+m_dAffichage][j+m_dAffichage] = m_tableauChunk[i+m_dAffichage][j+1+m_dAffichage];
                }
                Chunk *chunk = new Chunk((i+positionX/16)*16, (m_dAffichage+positionZ/16)*16, "Shaders/texture.vert", "Shaders/texture.frag");
                chunk->charger();
                m_tableauChunk[i+m_dAffichage][2*m_dAffichage-1] = chunk;
            }
        }
        if (positionZ / 16 > int(camera.getPosition().z) / 16)
        {
            for (int i = -m_dAffichage ; i < m_dAffichage ; i++)
            {
                delete m_tableauChunk[i+m_dAffichage][2*m_dAffichage-1];
                for (int j = m_dAffichage-1 ; j > -m_dAffichage ; j--)
                {
                    m_tableauChunk[i+m_dAffichage][j+m_dAffichage] = m_tableauChunk[i+m_dAffichage][j-1+m_dAffichage];
                }
                Chunk *chunk = new Chunk((i+positionX/16)*16, (-m_dAffichage+positionZ/16)*16, "Shaders/texture.vert", "Shaders/texture.frag");
                chunk->charger();
                m_tableauChunk[i+m_dAffichage][0] = chunk;
            }
        }

        positionX = int(camera.getPosition().x);
        positionZ = int(camera.getPosition().z);

        for (int i = -m_dAffichage ; i < m_dAffichage ; i++)
        {
            for (int j = -m_dAffichage ; j < m_dAffichage ; j++)
            {
                m_tableauChunk[i+m_dAffichage][j+m_dAffichage]->afficher(projection, modelview);
            }
        }

        /*for (int j = -m_dAffichage ; j < 0 ; j++)
        {
            for (int i = -m_dAffichage ; i < 0 ; i++)
            {
                if ((i*i+j*j < m_dAffichage*m_dAffichage) && (i+int(positionX) < 2000) && (i+int(positionX) > 0) && (j+int(positionZ) < 2000) && (j+int(positionZ) > 0))
                {
                    for (int k = 0 ; k < m_dHeight[i+int(positionX)+m_dAffichage][j+int(positionZ)+m_dAffichage] ; k++)
                    {
                        cubeRB.afficher(i+int(positionX),m_height[i+int(positionX)+m_dAffichage][j+int(positionZ)+m_dAffichage]-k,j+int(positionZ), projection, modelview);
                    }
                }
            }
            for (int i = 0 ; i < m_dAffichage ; i++)
            {
                if ((i*i+j*j < m_dAffichage*m_dAffichage) && (i+int(positionX) < 2000) && (i+int(positionX) > 0) && (j+int(positionZ) < 2000) && (j+int(positionZ) > 0))
                {
                    for (int k = 0 ; k < m_dHeight[i+int(positionX)+m_dAffichage][j+int(positionZ)+m_dAffichage] ; k++)
                    {
                        cubeLB.afficher(i+int(positionX),m_height[i+int(positionX)+m_dAffichage][j+int(positionZ)+m_dAffichage]-k,j+int(positionZ), projection, modelview);
                    }
                }
            }
        }
        for (int j = 0 ; j < m_dAffichage ; j++)
        {
            for (int i = -m_dAffichage ; i < 0 ; i++)
            {
                if ((i*i+j*j < m_dAffichage*m_dAffichage) && (i+int(positionX) < 2000) && (i+int(positionX) > 0) && (j+int(positionZ) < 2000) && (j+int(positionZ) > 0))
                {
                    for (int k = 0 ; k < m_dHeight[i+int(positionX)+m_dAffichage][j+int(positionZ)+m_dAffichage] ; k++)
                    {
                        cubeRF.afficher(i+int(positionX),m_height[i+int(positionX)+m_dAffichage][j+int(positionZ)+m_dAffichage]-k,j+int(positionZ), projection, modelview);
                    }
                }
            }
            for (int i = 0 ; i < m_dAffichage ; i++)
            {
                if ((i*i+j*j < m_dAffichage*m_dAffichage) && (i+int(positionX) < 2000) && (i+int(positionX) > 0) && (j+int(positionZ) < 2000) && (j+int(positionZ) > 0))
                {
                    for (int k = 0 ; k < m_dHeight[i+int(positionX)+m_dAffichage][j+int(positionZ)+m_dAffichage] ; k++)
                    {
                        cubeLF.afficher(i+int(positionX),m_height[i+int(positionX)+m_dAffichage][j+int(positionZ)+m_dAffichage]-k,j+int(positionZ), projection, modelview);
                    }
                }
            }
        }*/
        SDL_GL_SwapWindow(m_fenetre);

        finBoucle = SDL_GetTicks();
        tempsEcoule = finBoucle - debutBoucle;
        if(tempsEcoule < frameRate)
            SDL_Delay(frameRate - tempsEcoule);
    }
}

float SceneOpenGL::Get2DPerlinNoiseValue(float x, float y, float res)
{

    float tempX,tempY;

    int x0,y0,ii,jj,gi0,gi1,gi2,gi3;

    float unit = 1.0f/sqrt(2);

    float tmp,s,t,u,v,Cx,Cy,Li1,Li2;

    float gradient2[][2] = {{unit,unit},{-unit,unit},{unit,-unit},{-unit,-unit},{1,0},{-1,0},{0,1},{0,-1}};


    unsigned int perm[] =

       {151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,

        142,8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,

        203,117,35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,

        74,165,71,134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,

        105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,

        187,208,89,18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,

        64,52,217,226,250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,

        47,16,58,17,182,189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,

        153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,

        112,104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,

        235,249,14,239,107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,

        127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,

        156,180};


    //Adapter pour la résolution

    x /= res;

    y /= res;


    //On récupère les positions de la grille associée à (x,y)

    x0 = (int)(x);

    y0 = (int)(y);


    //Masquage

    ii = x0 & 255;

    jj = y0 & 255;


    //Pour récupérer les vecteurs

    gi0 = perm[ii + perm[jj]] % 8;

    gi1 = perm[ii + 1 + perm[jj]] % 8;

    gi2 = perm[ii + perm[jj + 1]] % 8;

    gi3 = perm[ii + 1 + perm[jj + 1]] % 8;


    //on récupère les vecteurs et on pondère

    tempX = x-x0;

    tempY = y-y0;

    s = gradient2[gi0][0]*tempX + gradient2[gi0][1]*tempY;


    tempX = x-(x0+1);

    tempY = y-y0;

    t = gradient2[gi1][0]*tempX + gradient2[gi1][1]*tempY;


    tempX = x-x0;

    tempY = y-(y0+1);

    u = gradient2[gi2][0]*tempX + gradient2[gi2][1]*tempY;


    tempX = x-(x0+1);

    tempY = y-(y0+1);

    v = gradient2[gi3][0]*tempX + gradient2[gi3][1]*tempY;



    //Lissage

    tmp = x-x0;

    Cx = 3 * tmp * tmp - 2 * tmp * tmp * tmp;


    Li1 = s + Cx*(t-s);

    Li2 = u + Cx*(v-u);


    tmp = y - y0;

    Cy = 3 * tmp * tmp - 2 * tmp * tmp * tmp;


    return Li1 + Cy*(Li2-Li1);

}
