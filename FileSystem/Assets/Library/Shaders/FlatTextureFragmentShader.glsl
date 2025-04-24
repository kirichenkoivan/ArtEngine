#version 300 es

precision mediump float;

struct Light {
    vec4 position;
    vec4 direction;
    vec4 color;
    vec4 intencivity;
    float cutoff;
    float exponent;
};

struct MaterialInfo {
    vec4 Ka;
    vec4 Kd;
    vec4 Ks;
    float Shiness;
};

vec3 ambientColor = vec3(0.5f, 0.5f, 0.5f);

layout(location = 0) out vec4 o_Color;

layout(std140) uniform Lights {
    Light u_Lights[128];
};
layout(std140) uniform Materials {
    MaterialInfo u_Materials[128];
};

in vec4 v_Color;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

flat in int v_MeshID;
flat in int v_TexIndex;

uniform sampler2D u_Textures[16];
uniform int u_LightsCount;

vec3 AdsForSpotLight(Light light, float La, float Ld, float Ls, vec3 Ka, vec3 Kd, vec3 Ks) {
    vec3 s = normalize(vec3(light.position) - v_FragPos);
    vec3 normal = normalize(v_Normal);
    float dot_s_dir = dot(-s, normalize(vec3(light.direction)));
    float angle = acos(clamp(dot_s_dir, -1.0f, 1.0f));
    float cutOff = radians(clamp(light.cutoff, 0.0f, 90.0f));

    vec3 ambient = La * Ka;

    if(angle < cutOff) {
        float spotFactor = pow(max(dot_s_dir, 0.0f), light.exponent);
        vec3 v = normalize(-v_FragPos);
        vec3 h = normalize(v + s);
        float spotIntencivity = light.color.a;
        return ambient + spotFactor * spotIntencivity * (Kd * Ld * max(dot(s, normal), 0.0f) + Ks * Ls * pow(max(dot(h, normal), 0.0f), u_Materials[v_MeshID].Shiness));
    } else {
        return ambient;
    }
}

vec3 ads(Light light) {

    float La = light.intencivity.x;
    float Ld = light.intencivity.y;
    float Ls = light.intencivity.z;

    vec3 Ka = vec3(u_Materials[v_MeshID].Ka);
    vec3 Kd = vec3(u_Materials[v_MeshID].Kd);
    vec3 Ks = vec3(u_Materials[v_MeshID].Ks);
    float shininess = u_Materials[v_MeshID].Shiness;

    vec3 n = normalize(v_Normal);
    vec3 s;

    if(light.position.w == 0.0f) {
        s = normalize(vec3(vec3(light.position) - v_FragPos));
    }

    if(light.position.w == 1.0f) {
        s = normalize(vec3(light.position));
    } else {
        return AdsForSpotLight(light, La, Ld, Ls, Ka, Kd, Ks);
    }

    vec3 v = normalize(-v_FragPos);
    vec3 h = normalize(v + s);

    vec3 ambient = La * Ka;
    float sDotN = max(dot(s, n), 0.0f);
    vec3 diffuse = Ld * Kd * sDotN;
    vec3 spec = vec3(0.0f);

    if(sDotN > 0.0f) {
        spec = Ls * Ks * pow(max(dot(h, n), 0.0f), shininess);
    }

    return (ambient + diffuse + spec) * light.color.a;
}

void main() {

    vec3 light = vec3(0.2f, 0.2f, 0.2f);

    for(int i = 0; i < u_LightsCount; ++i) {
        light += ads(u_Lights[i]);
    }

    switch(v_TexIndex) {
        case 0:
            o_Color = texture(u_Textures[0], v_TexCoord) * vec4(light, 1.0f);
            break;
        case 1:
            o_Color = texture(u_Textures[1], v_TexCoord) * vec4(light, 1.0f);
            break;
        default:
            o_Color = v_Color;
            break;
    }

    light = vec3(0.0f);
}
