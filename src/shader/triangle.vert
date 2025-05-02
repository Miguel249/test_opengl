#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor1;
layout (location = 2) in vec3 aColor2;
out vec4 color1;
out vec4 color2;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    color1 = vec4(aColor1, 1.0);
    color2 = vec4(aColor2, 1.0);
}