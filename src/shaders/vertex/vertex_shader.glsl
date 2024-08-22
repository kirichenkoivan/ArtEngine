#version 100
precision mediump float;

uniform mat4 uView;
uniform mat4 uProjection;

uniform float uSizeX;
uniform float uSizeY;
uniform vec3 uPosition;
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

    position += uPosition;

    gl_Position = uView * vec4(position, 1.0);
    vTexCoord = aTexCoord;
}
