R"(
#version 150

in vec2 TexCoord;

out vec4 color;

uniform sampler2D tex;
uniform vec4 texColor = vec4(0.0, 0.0, 0.0, 1.0);

void main() {
    color = texture2D(tex, TexCoord);
    color = color + texColor;
    color.a = texColor.a;
}
)"
