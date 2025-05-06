#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
uniform float time;
uniform vec3 cordMove;

mat4 rotate2D(float angle);
mat4 translateF(vec3 scalar);

void main()
{
    float angle = time;
    mat4 translate = translateF(cordMove);
    mat4 rotate = rotate2D(angle);
//    mat4 trans = rotate * translate;
    mat4 trans = translate * rotate;
    gl_Position = trans * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}

mat4 rotate2D(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return mat4(
    c, -s, 0.0, 0.0,
    s,  c, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
    );
}

mat4 translateF(vec3 scalar) {
    return mat4(
    1.0,            0.0,          0.0,       0.0,
    0.0,            1.0,          0.0,       0.0,
    0.0,            0.0,          1.0,       0.0,
    scalar.x,      scalar.y,    scalar.z,    1.0
    );
}