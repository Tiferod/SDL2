#include "Camera.h"

using namespace glm;

Camera::Camera() : m_phi(0.0), m_theta(0.0), m_orientation(), m_axeVertical(0, 0, 1), m_deplacementLateral(), m_position(), m_pointCible(), m_sensibilite(0.0), m_vitesse(0.0)
{

}

Camera::Camera(glm::vec3 position, glm::vec3 pointCible, glm::vec3 axeVertical, float sensibilite, float vitesse) : m_phi(0.0), m_theta(0.0), m_orientation(),
                                                                                                                    m_axeVertical(axeVertical), m_deplacementLateral(),
                                                                                                                    m_position(position), m_pointCible(pointCible),
                                                                                                                    m_sensibilite(sensibilite), m_vitesse(vitesse)
{

}

Camera::~Camera()
{

}

void Camera::orienter(int xRel, int yRel)
{
    m_phi += -yRel * m_sensibilite;
    m_theta += -xRel * m_sensibilite;

    if(m_phi > 89.0)
        m_phi = 89.0;

    else if(m_phi < -89.0)
        m_phi = -89.0;

    float phiRadian = m_phi * M_PI / 180;
    float thetaRadian = m_theta * M_PI / 180;

    if(m_axeVertical.x == 1.0)
    {
        m_orientation.x = sin(phiRadian);
        m_orientation.y = cos(phiRadian) * cos(thetaRadian);
        m_orientation.z = cos(phiRadian) * sin(thetaRadian);
    }
    else if(m_axeVertical.y == 1.0)
    {
        m_orientation.x = cos(phiRadian) * sin(thetaRadian);
        m_orientation.y = sin(phiRadian);
        m_orientation.z = cos(phiRadian) * cos(thetaRadian);
    }
    else
    {
        m_orientation.x = cos(phiRadian) * cos(thetaRadian);
        m_orientation.y = cos(phiRadian) * sin(thetaRadian);
        m_orientation.z = sin(phiRadian);
    }
    m_deplacementLateral = cross(m_axeVertical, m_orientation);
    m_deplacementLateral = normalize(m_deplacementLateral);
    m_pointCible = m_position + m_orientation;
}

void Camera::deplacer(Input const &input)
{
    if(input.mouvementSouris())
        orienter(input.getXRel(), input.getYRel());

    if(input.getTouche(SDL_SCANCODE_W))
    {
        m_position = m_position + m_orientation * m_vitesse;
        m_pointCible = m_position + m_orientation;
    }

    if(input.getTouche(SDL_SCANCODE_S))
    {
        m_position = m_position - m_orientation * m_vitesse;
        m_pointCible = m_position + m_orientation;
    }

    if(input.getTouche(SDL_SCANCODE_A))
    {
        m_position = m_position + m_deplacementLateral * m_vitesse;
        m_pointCible = m_position + m_orientation;
    }

    if(input.getTouche(SDL_SCANCODE_D))
    {
        m_position = m_position - m_deplacementLateral * m_vitesse;
        m_pointCible = m_position + m_orientation;
    }
}

void Camera::lookAt(glm::mat4 &modelview)
{
    modelview = glm::lookAt(m_position, m_pointCible, m_axeVertical);
}

void Camera::setPointCible(glm::vec3 pointCible)
{
    m_orientation = m_pointCible - m_position;
    m_orientation = normalize(m_orientation);
    if(m_axeVertical.x == 1.0)
    {
        m_phi = asin(m_orientation.x);
        m_theta = acos(m_orientation.y / cos(m_phi));
        if(m_orientation.y < 0)
            m_theta *= -1;
    }
    else if(m_axeVertical.y == 1.0)
    {
        m_phi = asin(m_orientation.y);
        m_theta = acos(m_orientation.z / cos(m_phi));
        if(m_orientation.z < 0)
            m_theta *= -1;
    }
    else
    {
        m_phi = asin(m_orientation.x);
        m_theta = acos(m_orientation.z / cos(m_phi));
        if(m_orientation.z < 0)
            m_theta *= -1;
    }
    m_phi = m_phi * 180 / M_PI;
    m_theta = m_theta * 180 / M_PI;
}

void Camera::setPosition(glm::vec3 position)
{
    m_position = position;
    m_pointCible = m_position + m_orientation;
}

float Camera::getSensibilite() const
{
    return m_vitesse;
}

float Camera::getVitesse() const
{
    return m_vitesse;
}

void Camera::setSensibilite(float sensibilite)
{
    m_sensibilite = sensibilite;
}

void Camera::setVitesse(float vitesse)
{
    m_vitesse = vitesse;
}

vec3 Camera::getPosition()
{
    return m_position;
}

vec3 Camera::getPointCible()
{
    return m_pointCible;
}
