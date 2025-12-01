#version 430

layout (location = 0) in vec2 iPosition;
layout (location = 1) in vec3 iColor;

layout( location = 1 ) uniform mat2 uRotation;

out vec3 v2fColor;

void main()
{
    v2fColor = iColor;

    vec2 rotatedPosition = uRotation * iPosition;
    gl_Position = vec4(rotatedPosition.xy, 0.0,1.0);

}
