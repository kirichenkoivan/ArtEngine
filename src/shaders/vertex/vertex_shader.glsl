#version 100

uniform float uSize;
uniform vec2 uPosition;

attribute vec3 aPosition;

void main() {
    vec3 position = aPosition * uSize;
    position.xy += uPosition;
    gl_Position = vec4(position, 1.0);
}