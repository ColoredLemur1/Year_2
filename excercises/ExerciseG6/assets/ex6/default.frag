#version 430

in vec2 v2fTexCoord;

layout ( location = 0 ) out vec3 oColor;
layout ( binding = 0 ) uniform sampler2D uTexture;

void main()
{
    oColor = texture( uTexture, v2fTexCoord ).rgb;
}