#version 300 es

precision mediump float;

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[2];

void main() {
    o_Color = v_Color;
}
