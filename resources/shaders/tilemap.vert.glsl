#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aInstanceUVs;

out vec2 TexCoord;

uniform mat4 projection;
uniform float tileSize;
uniform int cols;

void main()
{
    gl_Position = projection * vec4(aPos.x + float(gl_InstanceID % cols) * tileSize, aPos.y + float(gl_InstanceID / cols) * tileSize, 0.0, 1.0);
    TexCoord.x = mix(aInstanceUVs.x, aInstanceUVs.z, aPos.x);
    TexCoord.y = mix(aInstanceUVs.y, aInstanceUVs.w, aPos.y);
}