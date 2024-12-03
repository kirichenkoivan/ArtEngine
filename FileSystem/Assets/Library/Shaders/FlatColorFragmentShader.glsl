#version 300 es

precision mediump float;

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;
in mat3 v_ModelUvMatrix;

void main() {
    o_Color = v_Color;        
}
