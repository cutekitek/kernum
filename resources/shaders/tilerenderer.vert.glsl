#version 430 core

layout (location = 0) in vec2 aPos;

layout (location = 1) in vec4 aInstanceUVs;
layout (location = 2) in vec4 aInstanceTransform;

out vec2 TexCoord;

uniform mat4 projection;

void main()
{
    vec2 instanceWorldPos = aInstanceTransform.xy;
    vec2 instanceSize = aInstanceTransform.zw;

    vec2 finalPos = (aPos * instanceSize) + instanceWorldPos;

    gl_Position = projection * vec4(finalPos, 0.0, 1.0);

    TexCoord.x = mix(aInstanceUVs.x, aInstanceUVs.z, aPos.x);
    TexCoord.y = mix(aInstanceUVs.y, aInstanceUVs.w, aPos.y);
}