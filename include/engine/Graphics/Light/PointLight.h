#include "LightSource.h"

class PointLight : public LightSource
{

public:
    ~PointLight() = default;
    static std::shared_ptr<PointLight> CreatePointLight(const glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f}, const glm::vec3 position = {0.0f, 0.0f, 1.0f});

    virtual bool GetIsLightSourceStateChanged() override;
    virtual LightInfo &GetLightInfo() override;

private:
    PointLight(const glm::vec4 color, const glm::vec3 position);

private:
    static std::string CATEGORY;
};