#ifndef LIGHT_SOURCE
#define LIGHT_SOURCE

#include "../../Core/ArtCore.h"
#include "../../Core/ArtCoreUtils.h"

enum LightType
{
    POINT_LIGHT = 0,
    DIRECTIONAL_LIGHT,
    SPOT_LIGHT,
    SUN
};

struct alignas(16) LightInfo
{
    glm::vec4 position = glm::vec4(1.0f);
    glm::vec4 direction = glm::vec4(1.0f);
    glm::vec4 color = glm::vec4(1.0f);
    glm::vec4 intencivity = glm::vec4(1.0f);
    float cutOff;
    float exponent;
    float _padding[2];
};

class LightSource
{

public:
    LightSource(const glm::vec4 color, const glm::vec3 position) : m_Color(color), m_Position(position) {};
    LightSource(const glm::vec3 color) {};

    glm::vec3 GetPosition() const { return m_Position; }
    float GetPositionX() const { return m_Position.x; }
    float GetPositionY() const { return m_Position.y; }
    float GetPositionZ() const { return m_Position.z; }

    glm::vec3 GetRotation() const { return m_Rotation; }
    float GetRotationX() const { return m_Rotation.x; }
    float GetRotationY() const { return m_Rotation.y; }
    float GetRotationZ() const { return m_Rotation.z; }

    glm::vec4 GetColor() const { return m_Color; }
    float GetColorR() const { return m_Color.x; }
    float GetColorG() const { return m_Color.y; }
    float GetColorB() const { return m_Color.z; }
    float GetColorA() const { return m_Color.w; }

    glm::vec3 GetIntencity() const { return m_Intencivity; }

    LightType GetType() const { return m_Type; }

    void SetPosition(float x, float y, float z)
    {
        m_Position = {x, y, z};
        isLightSourceChanged = true;
    }
    void SetPositionX(float x)
    {
        m_Position.x = x;
        isLightSourceChanged = true;
    }
    void SetPositionY(float y)
    {
        m_Position.y = y;
        isLightSourceChanged = true;
    }
    void SetPositionZ(float z)
    {
        m_Position.z = z;
        isLightSourceChanged = true;
    }

    void SetColor(float r, float g, float b, float a)
    {
        m_Color = {r, g, b, a};
        isLightSourceChanged = true;
    }
    void SetColorR(float r)
    {
        m_Color.x = r;
        isLightSourceChanged = true;
    }
    void SetColorG(float g)
    {
        m_Color.y = g;
        isLightSourceChanged = true;
    }
    void SetColorB(float b)
    {
        m_Color.z = b;
        isLightSourceChanged = true;
    }
    void SetColorA(float a)
    {
        m_Color.w = a;
        isLightSourceChanged = true;
    }

    void SetIntencivity(glm::vec3 intencivity)
    {
        m_Intencivity = intencivity;
        isLightSourceChanged = true;
    }

    void SetType(LightType type)
    {
        m_Type = type;
        isLightSourceChanged = true;
    }

    virtual ~LightSource() = default;
    virtual bool GetIsLightSourceStateChanged()
    {
        Logger::GetInstance().Warning("Graphics/Light/LightSource", "Abstract method called");
        return false;
    }

    virtual void SetDirection(glm::vec3 direction) { Logger::GetInstance().Warning("Graphics/Light/LightSource", "Abstract method called"); }
    virtual void SetDirectionX(float x) { Logger::GetInstance().Warning("Graphics/Light/LightSource", "Abstract method called"); }
    virtual void SetDirectionY(float y) { Logger::GetInstance().Warning("Graphics/Light/LightSource", "Abstract method called"); }
    virtual void SetDirectionZ(float z) { Logger::GetInstance().Warning("Graphics/Light/LightSource", "Abstract method called"); }
    virtual void SetCutOff(float cutOff) { Logger::GetInstance().Warning("Graphics/Light/LightSource", "Abstract method called"); }
    virtual void SetExponent(float exponent) { Logger::GetInstance().Warning("Graphics/Light/LightSource", "Abstract method called"); }

    virtual glm::vec3 GetDirection() const
    {
        Logger::GetInstance().Warning("Graphics/Light/LightSource", "Abstract method called");
        return glm::vec3(1.0f);
    }
    virtual float GetDirectionX() const
    {
        Logger::GetInstance().Warning("Graphics/Light/LightSource", "Abstract method called");
        return 1.0f;
    }
    virtual float GetDirectionY() const
    {
        Logger::GetInstance().Warning("Graphics/Light/LightSource", "Abstract method called");
        return 1.0f;
    }
    virtual float GetDirectionZ() const
    {
        Logger::GetInstance().Warning("Graphics/Light/LightSource", "Abstract method called");
        return 1.0f;
    }

    virtual float GetCutOff() const
    {
        Logger::GetInstance().Warning("Graphics/Light/LightSource", "Abstract method called");
        return 0.0f;
    }
    virtual float GetExponent() const
    {
        Logger::GetInstance().Warning("Graphics/Light/LightSource", "Abstract method called");
        return 1.0f;
    }

    virtual LightInfo &GetLightInfo()
    {
        Logger::GetInstance().Warning("Graphics/Light/LightSource", "Abstract method called");
        return m_LightInfo;
    };

protected:
    glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_Rotation = {0.0f, 0.0f, 0.0f};
    glm::vec4 m_Color = {1.0f, 1.0f, 1.0f, 1.0f};
    glm::vec3 m_Intencivity = {1.0f, 1.0f, 1.0f};
    LightType m_Type;

    bool isLightSourceChanged = false;

    LightInfo m_LightInfo;
};

#endif