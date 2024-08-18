#version 100
precision mediump float;

uniform float uSize;
uniform vec2 uPosition;
uniform float uRotation;

attribute vec3 aPosition;
attribute vec2 aTexCoord;

varying vec2 vTexCoord;

void main() {
    vec3 position = aPosition * uSize;
    float cosAngle = cos(uRotation);
    float sinAngle = sin(uRotation);

    vec2 center = vec2(0.0, 0.0);
    position.xy -= center;
    position.x = position.x * cosAngle - position.y * sinAngle;
    position.y = position.x * sinAngle + position.y * cosAngle;
    position.xy += center;

    position.xy += uPosition;
    gl_Position = vec4(position, 1.0);

    vTexCoord = aTexCoord;
}