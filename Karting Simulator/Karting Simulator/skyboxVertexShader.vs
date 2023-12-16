#version 400
layout(location = 0) in vec3 position; // 3D position data

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 pos = projection * view * vec4(position, 1.0);
    gl_Position = pos.xyww; // Render at the depth of the far plane
    TexCoords = position; // Pass the position to the fragment shader for the texture lookup
}
