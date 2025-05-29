#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aOffset;

out vec2 TexCoord;
uniform vec3 cordMove = vec3(0.0, 0.0, 0.0);
uniform vec3 scalar = vec3(1.0, 1.0, 1.0);
uniform vec2 direction = vec2(0.0, 1.0);
uniform bool rotated = false;

mat4 rotate2D(float angle);
mat4 translateF(vec3 scalar);
mat4 scaleF(vec3 scale);

void main()
{
    float angle = 0.0;
    if (rotated) {
        angle = atan(direction.y, -direction.x) - radians(90.0);
    }
    mat4 translate = translateF(aOffset + cordMove);
    mat4 rotate = rotate2D(angle);
    mat4 trans = translate * rotate * scaleF(scalar);
    gl_Position = trans * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}

mat4 rotate2D(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return mat4(
    c, -s, 0.0, 0.0,
    s, c, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
    );
}

mat4 translateF(vec3 cords) {
    return mat4(
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    cords.x, cords.y, cords.z, 1.0
    );
}

mat4 scaleF(vec3 scale) {
    return mat4(
    scale.x, 0.0, 0.0, 0.0,
    0.0, scale.y, 0.0, 0.0,
    0.0, 0.0, scale.z, 0.0,
    0.0, 0.0, 0.0, 1.0
    );
}