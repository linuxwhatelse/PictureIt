R"(
#version 150

in vec3 position;
in vec3 color;
in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 trans;

void main() {
    gl_Position = trans * vec4(position, 1.0f);

    // We swap the y-axis by substracing our coordinates
    // This is done because most images have the top y-axis inversed
    // with OpenGL's top y-axis.
    TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}
)"
