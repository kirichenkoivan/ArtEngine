#version 300 es

precision mediump float;

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in int  a_TexIndex;
layout(location = 4) in int  a_MeshID;

struct ModelTransform{
        mat4 modelMatrix;
        mat4 modelUvmatrix;
    };

layout(std140) uniform ModelMatrices {
    ModelTransform u_ModelTransforms[128];
};

uniform mat4 u_ViewProjection;
out vec4 v_Color;
out mat4 v_ModelUvMatrix;

void main(){
    mat4 modelMatrix = u_ModelTransforms[a_MeshID].modelMatrix;

    v_Color = a_Color;
    gl_Position = u_ViewProjection * modelMatrix * vec4(a_Position, 1.0);
}