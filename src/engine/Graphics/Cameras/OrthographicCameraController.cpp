/****************************************************************
 *                        --- Desc ---                           *
 *                                                               *
 *          Controller class that handles all events             *
 *              related to the orthographic camera               *
 *                                                               *
 *****************************************************************/

#include "../include/engine/Graphics/Cameras/OrthographicCameraController.h"

OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
    : m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
{
    Logger::GetInstance().Info(CATEGORY, "Camera Controller Created");
}

void OrthographicCameraController::OnUpdate(Timestep ts)
{
    if (InputManager::GetInstance().IsKeyPressed(KEYBOARD_BTN_A))
    {
        m_CameraPosition.x -= cos(glm::radians(m_CameraRotation.z)) * m_CameraTranslatedSpeed * ts;
        m_CameraPosition.y -= sin(glm::radians(m_CameraRotation.z)) * m_CameraTranslatedSpeed * ts;
    }
    else if (InputManager::GetInstance().IsKeyPressed(KEYBOARD_BTN_D))
    {
        m_CameraPosition.x += cos(glm::radians(m_CameraRotation.z)) * m_CameraTranslatedSpeed * ts;
        m_CameraPosition.y += sin(glm::radians(m_CameraRotation.z)) * m_CameraTranslatedSpeed * ts;
    }

    if (InputManager::GetInstance().IsKeyPressed(KEYBOARD_BTN_W))
    {
        m_CameraPosition.x += -sin(glm::radians(m_CameraRotation.z)) * m_CameraTranslatedSpeed * ts;
        m_CameraPosition.y += cos(glm::radians(m_CameraRotation.z)) * m_CameraTranslatedSpeed * ts;
    }
    else if (InputManager::GetInstance().IsKeyPressed(KEYBOARD_BTN_S))
    {
        m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation.z)) * m_CameraTranslatedSpeed * ts;
        m_CameraPosition.y -= cos(glm::radians(m_CameraRotation.z)) * m_CameraTranslatedSpeed * ts;
    }

    if (m_Rotation)
    {
        if (InputManager::GetInstance().IsKeyPressed(KEYBOARD_BTN_Q))
        {
            m_CameraRotation.z += m_CameraRotation.z * ts;
        }
        if (InputManager::GetInstance().IsKeyPressed(KEYBOARD_BTN_E))
        {
            m_CameraRotation.z -= m_CameraRotation.z * ts;
        }

        if (m_CameraRotation.z > 180.0f)
        {
            m_CameraRotation.z -= 360.0f;
        }
        else if (m_CameraRotation.z <= -180.0f)
        {
            m_CameraRotation.z += 360.0f;
        }

        m_Camera.SetRotation(m_CameraRotation);
    }

    m_Camera.SetPosition(m_CameraPosition);
    m_CameraTranslatedSpeed = m_ZoomLevel;
}
