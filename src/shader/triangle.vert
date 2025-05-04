#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor1;
layout (location = 2) in vec2 aTexCoord;

out vec4 color1;
out vec2 TexCoord;
uniform float horizontalOffset;
uniform float verticalOffset;

void main()
{
    gl_Position = vec4(aPos.x + horizontalOffset, aPos.y + verticalOffset, aPos.z, 1.0);
    color1 = vec4(aColor1, 1.0);
    TexCoord = aTexCoord;
}