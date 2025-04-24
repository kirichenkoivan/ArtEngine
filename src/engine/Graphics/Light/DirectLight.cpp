#include "../include/engine/Graphics/Light/DirectLight.h"

static const std::string CATEGORY = "Graphics/Light/DirectLight";

std::shared_ptr<DirectLight> DirectLight::CreateDirectLight(glm::vec3 direction, glm::vec4 color)
{
    std::shared_ptr<DirectLight> light(new DirectLight(color));
    light->m_Type = DIRECTIONAL_LIGHT;
    light->m_Direction = direction;

#ifdef DEBUG
    Logger::GetInstance().Info("Graphics/Light/DirectLight", "Direct Light Created!");
#endif

    return light;
}

DirectLight::DirectLight(glm::vec4 color) : LightSource(color)
{
}

LightInfo &DirectLight::GetLightInfo()
{
    m_LightInfo.position = glm::vec4(m_Position, 1.0f);
    m_LightInfo.intencivity = glm::vec4(m_Intencivity, 0.0f);
    m_LightInfo.color = m_Color;
    return m_LightInfo;
}

bool DirectLight::GetIsLightSourceStateChanged()
{
    bool isChanged = isLightSourceChanged;
    isLightSourceChanged = false;
    return isChanged;
}
