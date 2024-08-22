#version 100
precision mediump float;

uniform float uSizeX;
uniform float uSizeY;
uniform vec3 uPosition; // Изменено на vec3 для поддержки Z-координаты
uniform float uRotation;

attribute vec3 aPosition;
attribute vec2 aTexCoord;

varying vec2 vTexCoord;

void main() {
    vec3 position = aPosition;
    position.x *= uSizeX;
    position.y *= uSizeY;
    
    float cosAngle = cos(uRotation);
    float sinAngle = sin(uRotation);

    vec2 center = vec2(0.0, 0.0);
    position.xy -= center;
    position.x = position.x * cosAngle - position.y * sinAngle;
    position.y = position.x * sinAngle + position.y * cosAngle;
    position.xy += center;

    position += uPosition; // Теперь учитываем Z-координату

    gl_Position = vec4(position, 1.0);
    vTexCoord = aTexCoord;
}
