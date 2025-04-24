#include "../include/engine/Graphics/Light/PointLight.h"

std::shared_ptr<PointLight> PointLight::CreatePointLight(const glm::vec4 color, const glm::vec3 position)
{
    std::shared_ptr<PointLight> pointLight(new PointLight(color, position));
    pointLight->m_Type = POINT_LIGHT;

#ifdef DEBUG
    Logger::GetInstance().Info("Graphics/Light/PointLight", "Point light Created");
#endif

    return pointLight;
}

PointLight::PointLight(const glm::vec4 color, const glm::vec3 position)
    : LightSource(color, position) {}

LightInfo &PointLight::GetLightInfo()
{
    m_LightInfo.position = glm::vec4(m_Position, 0.0f);
    m_LightInfo.color = m_Color;

    return m_LightInfo;
}

bool PointLight::GetIsLightSourceStateChanged()
{
    bool isChanged = isLightSourceChanged;
    isLightSourceChanged = false;
    return isChanged;
}