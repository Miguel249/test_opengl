#version 460 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;

vec4 acolor;
void main() {
    vec4 texColor = texture(texture1, TexCoord);
    if (texColor.a < 0.1) {
        discard;
    }
    FragColor = texColor;
}
