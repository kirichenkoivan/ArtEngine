#ifndef DIRECT_LIGHT
#define DIRECT_LIGHT

#include "LightSource.h"

class DirectLight : public LightSource
{
public:
    ~DirectLight() = default;
    static std::shared_ptr<DirectLight> CreateDirectLight(glm::vec3 direction, glm::vec4 color);

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

    virtual glm::vec3 GetDirection() const override { return m_Direction; }
    virtual float GetDirectionX() const override { return m_Direction.x; }
    virtual float GetDirectionY() const override { return m_Direction.y; }
    virtual float GetDirectionZ() const override { return m_Direction.z; }

    virtual LightInfo &GetLightInfo() override;

private:
    DirectLight(glm::vec4 color);

private:
    glm::vec3 m_Direction = {0.0f, 0.0f, 1.0f};
};

#endif