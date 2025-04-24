#version 300 es

precision mediump float;

layout(location = 0) out vec4 o_Color;

struct Light{
    int type;
    mat4 lightMatrix;
    vec4 direction;
    vec4 color;
};

struct MaterialInfo{
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float shiness;
};

layout(std140) uniform Lights {
    Light u_Lights[128];
};

layout(std140) uniform Material {
    MaterialInfo matInfo;
};

in vec4 v_Color;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;
flat in int v_TexIndex;

uniform sampler2D u_Textures[16];
uniform int u_LightsCount;

float Kd = 5.7;
float Ks = 5.7; 

float ambientEntencity = 1.0;

vec3 CalculatePointLight(Light light, vec3 normal ){
    vec4 lightPosition = light.lightMatrix[3];
    vec3 lightSource = vec3(lightPosition);

    vec3 lightDir = normalize(lightSource - v_FragPos);
    float diffuseStrength = light.color.a * max(dot(normal, lightDir), 0.0);

    vec3 diffuse = diffuseStrength * vec3(light.color.rgb);
    float distance = length(lightSource - v_FragPos);

    float constant = 1.0; 
    float linear = 0.09; 
    float quadratic = 0.032; 

    float attenuation = 1.0;
    diffuse *= attenuation;
    return diffuse;
}

vec3 CalculateDirectLight(Light light, vec3 normal){
    vec3 lightDir = normalize(vec3(-light.direction));
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * vec3(light.color.rgb);
    diffuse *= light.color.a;
    return diffuse;
}

vec3 CalculateLight() {
    vec3 ambient = vec3(0.1, 0.1, 0.1) * Kd * ambientEntencity;
    vec3 normal = normalize(v_Normal);
    vec3 totalLight = vec3(0.0);

    for (int i = 0; i < u_LightsCount; ++i) {
        if (u_Lights[i].type == 0){
            totalLight += CalculatePointLight(u_Lights[i], normal);
        }
        if (u_Lights[i].type == 1){
            totalLight += CalculateDirectLight(u_Lights[i], normal);
        }
    }

    return ambient + totalLight;
}

void main() {

    vec3 light = CalculateLight();

    switch (v_TexIndex) {
        case 0:
            o_Color = texture(u_Textures[0], v_TexCoord) * vec4(light, 1.0);
            break;
        case 1:
            o_Color = texture(u_Textures[1], v_TexCoord) * vec4(light, 1.0);
            break;
        default:
            o_Color = v_Color;
            break;
    }

    light = vec3(0.0);
}
