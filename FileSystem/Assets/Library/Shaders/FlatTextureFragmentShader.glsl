#version 300 es

precision mediump float;

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
flat in int v_TexIndex;

uniform sampler2D u_Textures[16];

void main() {
    switch (v_TexIndex) {
        case 0:
            o_Color = texture(u_Textures[0], v_TexCoord);
            break;
        case 1:
            o_Color = texture(u_Textures[1], v_TexCoord);
            break;
        default:
            o_Color = v_Color;
            break;
    }
}
