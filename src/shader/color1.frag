#version 460 core
out vec4 FragColor;
in vec4 color1;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float colorAlt;
uniform float opacity;

vec4 acolor;
void main() {
    acolor = vec4(colorAlt / 1.2, 0.3, colorAlt, 1.0);
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), opacity);
}
