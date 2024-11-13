#version 300 es

precision mediump float;

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[2];

void main() {
    vec4 color;
    if (int(v_TexIndex) == 0) {
        color = texture(u_Textures[0], v_TexCoord);
    } else {
        color = texture(u_Textures[1], v_TexCoord);
    }
    o_Color = color;
}
