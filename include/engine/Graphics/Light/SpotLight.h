#ifndef SPOT_LIGHT_SOURCE
#define SPOT_LIGHT_SOURCE

#include "LightSource.h"

class SpotLight : public LightSource
{
public:
    ~SpotLight() = default;
    static std::shared_ptr<SpotLight> CreateSpotLight(const glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f}, const glm::vec3 position = {0.0f, 0.0f, 1.0f}, const glm::vec3 direction = {0.0f, 0.0f, 0.0f}, const float exponent = 1.0f, const float cutOff = 0.5f);

    virtual bool GetIsLightSourceStateChanged() override;

    virtual void SetDirection(glm::vec3 direction) override { m_Direction = direction; }
    virtual void SetDirectionX(float x) override
    {
        m_Direction.x = x;
        isLightSourceChanged = true;
    }
    virtual void SetDirectionY(float y) override
    {
        m_Direction.y = y;
        isLightSourceChanged = true;
    }
    virtual void SetDirectionZ(float z) override
    {
        m_Direction.z = z;
        isLightSourceChanged = true;
    }
    virtual void SetCutOff(float cutOff) override
    {
        m_CutOff = cutOff;
        isLightSourceChanged = true;
    }
    virtual void SetExponent(float exponent) override
    {
        m_Exponent = exponent;
        isLightSourceChanged = true;
    }

    virtual glm::vec3 GetDirection() const override { return m_Direction; }
    virtual float GetDirectionX() const override { return m_Direction.x; }
    virtual float GetDirectionY() const override { return m_Direction.y; }
    virtual float GetDirectionZ() const override { return m_Direction.z; }

    virtual float GetCutOff() const override { return m_CutOff; }
    virtual float GetExponent() const override { return m_Exponent; }

    virtual LightInfo &GetLightInfo() override;

private:
    SpotLight(const glm::vec4 color, const glm::vec3 position);

private:
    glm::vec3 m_Direction = {0.0f, 0.0f, 0.0f};
    float m_CutOff = 0.0f;
    float m_Exponent = 1.0f;
};

#endif