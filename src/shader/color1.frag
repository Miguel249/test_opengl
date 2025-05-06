#version 460 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float opacity;

vec4 acolor;
void main() {
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), opacity);
}
