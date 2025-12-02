#version 430

in vec3 v2fColor;
in vec3 v2fNormal;

layout ( location = 0 ) out vec3 oColor;
layout ( location = 2 ) uniform vec3 uLightDirection;
layout ( location = 3 ) uniform vec3 uLightDiffuse;
layout ( location = 4 ) uniform vec3 uSceneAmbient;

void main()
{
    vec3 normal = normalize(v2fNormal);
    float nDotL = max(0.0, dot(normal, uLightDirection));
    oColor = (uSceneAmbient + nDotL * uLightDiffuse) * v2fColor;
}
