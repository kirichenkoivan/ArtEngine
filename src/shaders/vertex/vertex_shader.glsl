#version 100
precision mediump float;

uniform float uSizeX;
uniform float uSizeY;
uniform vec2 uPosition; // uniform для позиции теперь vec2
uniform float uRotation;

attribute vec2 aPosition; // attribute для позиции теперь vec2
attribute vec2 aTexCoord;

varying vec2 vTexCoord;

void main() {
    vec2 position = aPosition;
    position.x *= uSizeX;
    position.y *= uSizeY;
    float cosAngle = cos(uRotation);
    float sinAngle = sin(uRotation);

    vec2 center = vec2(0.0, 0.0);
    position -= center;
    position = vec2(position.x * cosAngle - position.y * sinAngle, position.x * sinAngle + position.y * cosAngle);
    position += center;

    position += uPosition;
    gl_Position = vec4(position, 0.0, 1.0); // Устанавливаем Z в 0.0

    vTexCoord = aTexCoord;
}