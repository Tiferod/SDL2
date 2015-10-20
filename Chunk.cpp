#ifndef BUFFER_OFFSET

    #define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

#endif

#include "Chunk.h"
#include "SceneOpenGL.h"

using namespace glm;

/*Chunk::Chunk(): Chunk(0, 0, "Shaders/texture.vert", "Shaders/texture.frag")
{
    m_loaded = false;
}*/
Chunk::Chunk(int pX, int pZ, std::string const vertexShader, std::string const fragmentShader) : m_shader(vertexShader, fragmentShader), m_herbe("TextureHerbe.jpg"), m_terre("TextureTerre.jpg"), m_vboID(0), m_tailleVerticesBytes(46080 * sizeof(float)), m_tailleCoordTexturesBytes(30720 * sizeof(float)), m_vaoID(0), m_nbrCubes(0), m_loaded(true)
{
    m_herbe.charger();
    m_terre.charger();

    m_shader.charger();

    float verticesTmp1[] = {1, 0, 1, 1, 0, 0, 1, 1, 0,//DROITE
                           1, 0, 1, 1, 1, 1, 1, 1, 0,//DROITE
                           0, 0, 0, 1, 0, 0, 1, 1, 0,//DEVANT
                           0, 0, 0, 0, 1, 0, 1, 1, 0,//DEVANT
                           0, 0, 0, 0, 0, 1, 0, 1, 1,//GAUCHE
                           0, 0, 0, 0, 1, 0, 0, 1, 1,//GAUCHE
                           0, 0, 1, 1, 0, 1, 1, 1, 1,//DERRIERE
                           0, 0, 1, 0, 1, 1, 1, 1, 1,//DERRIERE
                           0, 1, 1, 1, 1, 1, 1, 1, 0,//HAUT
                           0, 1, 1, 0, 1, 0, 1, 1, 0};//HAUT

    float coordTexture1[] = {0, 1, 0, 0, 1, 0,//DROITE
                            0, 1, 1, 1, 1, 0,//DROITE
                            0, 0, 1, 0, 1, 1,//DEVANT
                            0, 0, 0, 1, 1, 1,//DEVANT
                            0, 0, 0, 1, 1, 1,//GAUCHE
                            0, 0, 1, 0, 1, 1,//GAUCHE
                            0, 0, 1, 0, 1, 1,//DERRIERE
                            0, 0, 0, 1, 1, 1,//DERRIERE
                            0, 1, 1, 1, 1, 0,//HAUT
                            0, 1, 0, 0, 1, 0};//HAUT

    for (int i = pX ; i < pX + 16 ; i++)
    {
        for (int j = pZ ; j < pZ + 16 ; j++)
        {
            m_nbrCubes = m_nbrCubes + SceneOpenGL::m_dHeight[i+1000][j+1000];
        }
    }
    m_tailleVerticesBytes = 90 * m_nbrCubes * sizeof(float);
    m_tailleCoordTexturesBytes = 60 * m_nbrCubes * sizeof(float);

    float verticesTmp [m_nbrCubes*90];
    float coordTexture [m_nbrCubes*60];

    int p1 = 0, q1 = m_nbrCubes*72, p2 = 0, q2 = m_nbrCubes*48;

    for (int i = pX ; i < pX + 16 ; i++)
    {
        for (int j = pZ ; j < pZ + 16 ; j++)
        {
            for (int k = 0 ; k < SceneOpenGL::m_dHeight[i+1000][j+1000] ; k++)
            {
                for (int l = 0 ; l < 24 ; l++)
                {
                    verticesTmp[p1] = verticesTmp1[3*l] + i;
                    verticesTmp[p1+1] = verticesTmp1[3*l+1] + SceneOpenGL::m_height[i+1000][j+1000] - k;
                    verticesTmp[p1+2] = verticesTmp1[3*l+2] + j;
                    p1 += 3;
                    coordTexture[p2] = coordTexture1[2*l];
                    coordTexture[p2+1] = coordTexture1[2*l+1];
                    p2 += 2;
                }
                for (int l = 24 ; l < 30 ; l++)
                {
                    verticesTmp[q1] = verticesTmp1[3*l] + i;
                    verticesTmp[q1+1] = verticesTmp1[3*l+1] + SceneOpenGL::m_height[i+1000][j+1000] - k;
                    verticesTmp[q1+2] = verticesTmp1[3*l+2] + j;
                    q1 += 3;
                    coordTexture[q2] = coordTexture1[2*l];
                    coordTexture[q2+1] = coordTexture1[2*l+1];
                    q2 += 2;
                }
            }
        }
    }

    for (int i = 0 ; i < m_nbrCubes ; i++)
    {
        for (int j = 0 ; j < 90  ; j++)
        {
            m_vertices[i*90+j] = verticesTmp[i*90+j];
            if (j < 60)
                m_coordTexture[i*60+j] = coordTexture[i*60+j];
        }
    }
}

Chunk::~Chunk()
{
    glDeleteBuffers(1, &m_vboID);
    glDeleteVertexArrays(1, &m_vaoID);
}

void Chunk::afficher(glm::mat4 &projection, glm::mat4 &modelview)
{
    glUseProgram(m_shader.getProgramID());
        glBindVertexArray(m_vaoID);
            glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));
            glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "modelview"), 1, GL_FALSE, value_ptr(modelview));
            glBindTexture(GL_TEXTURE_2D, m_terre.getID());
            glDrawArrays(GL_TRIANGLES, 0, 24*m_nbrCubes);
            glBindTexture(GL_TEXTURE_2D, m_herbe.getID());
            glDrawArrays(GL_TRIANGLES, 24*m_nbrCubes, 30*m_nbrCubes);
            glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
    glUseProgram(0);
}

void Chunk::charger()
{
    if(glIsBuffer(m_vboID) == GL_TRUE)
        glDeleteBuffers(1, &m_vboID);
    glGenBuffers(1, &m_vboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

        glBufferData(GL_ARRAY_BUFFER, m_tailleVerticesBytes + m_tailleCoordTexturesBytes, 0, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_tailleVerticesBytes, m_vertices);
        glBufferSubData(GL_ARRAY_BUFFER, m_tailleVerticesBytes, m_tailleCoordTexturesBytes, m_coordTexture);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if(glIsVertexArray(m_vaoID) == GL_TRUE)
        glDeleteVertexArrays(1, &m_vaoID);
    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(m_tailleVerticesBytes));
            glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

bool Chunk::isLoaded()
{
    std::cout<<"Bonjour"<<std::endl;
    return m_loaded;
}
