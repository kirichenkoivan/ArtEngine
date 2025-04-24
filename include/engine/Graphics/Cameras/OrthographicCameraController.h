/****************************************************************
 *                        --- Desc ---                           *
 *                                                               *
 *          Controller class that handles all events             *
 *              related to the orthographic camera               *
 *                                                               *
 *****************************************************************/

#ifndef ORTHOGRAPHIC_CAMERA_CONTROLLER
#define ORTHOGRAPHIC_CAMERA_CONTROLLER

#include "../../Core/ArtCore.h"
#include "OrthographicCamera.h"

class OrthographicCameraController
{

public:
    OrthographicCameraController(float aspectRatio, bool rotation = false);

    void OnUpdate(Timestep ts);

    OrthographicCamera &GetCamera() { return m_Camera; }
    const OrthographicCamera &GetCamera() const { return m_Camera; }

private:
    const std::string CATEGORY = "Graphics/Cameras/OrthographicCameraController";

    float m_AspectRatio;
    float m_ZoomLevel = 1.0f;
    OrthographicCamera m_Camera;

    bool m_Rotation;

    glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_CameraRotation = {0.0f, 0.0f, 0.0f};
    float m_CameraTranslatedSpeed = 5.0f;
    float m_CameraRotationSpeed = 180.0f;
};

#endif