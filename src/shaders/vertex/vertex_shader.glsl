#version 100
precision mediump float;
uniform float uSize;
uniform vec2 uPosition;
uniform float uRotation;

attribute vec3 aPosition;

void main() {
    vec3 position = aPosition * uSize;
    float cosAngle = cos(uRotation);
    float sinAngle = sin(uRotation);

    // Translate to origin, rotate, then translate back
    vec2 center = vec2(0.0, 0.0); // Assuming the center of rotation is at (0.0, 0.0)
    position.xy -= center;
    position.x = position.x * cosAngle - position.y * sinAngle;
    position.y = position.x * sinAngle + position.y * cosAngle;
    position.xy += center;

    position.xy += uPosition;
    gl_Position = vec4(position, 1.0);
}