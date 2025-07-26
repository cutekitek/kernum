#version 430 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aInstanceUVs;

out vec2 TexCoord;

uniform mat4 projection;
uniform float tileSize;
uniform float cols;
uniform vec2 offset;

void main()
{
    float x = mod(float(gl_InstanceID), cols);
    float y = floor(float(gl_InstanceID) / cols);

    vec2 worldPos = vec2(x, y) * tileSize + offset;
    vec2 pos = worldPos + aPos * tileSize;

    gl_Position = projection * vec4(pos, 0.0, 1.0);

    TexCoord = aInstanceUVs.xy + aPos * aInstanceUVs.zw;
}