#include "../include/engine/Graphics/Light/SpotLight.h"

std::shared_ptr<SpotLight> SpotLight::CreateSpotLight(const glm::vec4 color, const glm::vec3 position, const glm::vec3 direction, const float exponent, const float cutOff)
{
    std::shared_ptr<SpotLight> light(new SpotLight(color, position));
    light->m_CutOff = cutOff;
    light->m_Exponent = exponent;
    light->m_Direction = direction;
    light->m_Type = SPOT_LIGHT;

#ifdef DEBUG
    Logger::GetInstance().Info("Graphics/Light/SpotLight", "Spot light Created");
#endif

    return light;
}

bool SpotLight::GetIsLightSourceStateChanged()
{
    bool isChanged = isLightSourceChanged;
    isLightSourceChanged = false;
    return isChanged;
}

SpotLight::SpotLight(const glm::vec4 color, const glm::vec3 position) : LightSource(color, position)
{
}

LightInfo &SpotLight::GetLightInfo()
{
    m_LightInfo.position = glm::vec4(m_Position, 2.0f);
    m_LightInfo.color = m_Color;
    m_LightInfo.direction = glm::vec4(m_Direction, 0.0f);
    m_LightInfo.cutOff = m_CutOff;
    m_LightInfo.exponent = m_Exponent;

    return m_LightInfo;
}
