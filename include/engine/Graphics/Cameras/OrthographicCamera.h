/****************************************************************
*                        --- Desc ---                           * 
*                                                               *
*          Camera class with orthographic projection,           *
*                    used by default in scenes                  *
*                                                               *
*****************************************************************/


#ifndef ORTHOGRAPHIC_CAMERA
#define ORTHOGRAPHIC_CAMERA

#include "../../Core/ArtCore.h"
#include "../../Core/ArtCoreUtils.h"

class OrthographicCamera{
    public:
        OrthographicCamera(float left, float right, float bottom, float top);

        void SetProjection(float left, float right, float bottom, float top);

        const glm::vec3& GetPosition() const { return m_Position; }
        void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
        
        float GetRotation() const { return m_Rotation; }
        void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

    private:
        void RecalculateViewMatrix();

    private:
        const std::string CATEGORY = "Graphics/Cameras/OrthographicCamera";

        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectionMatrix;

        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
        float m_Rotation = 0.0f;
};

#endif