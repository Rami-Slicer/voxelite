#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
// layout(location = 3) in vec3 aInstancePos;

layout(location = 3) in mat4 aInstanceMatrix;

out vec4 vertexColor;
out vec3 vertexNormal;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vertexColor = vec4(1.0);
    texCoord = aTexCoord;
    vertexNormal = aNormal;

    gl_Position = projection * view * model * aInstanceMatrix * vec4(aPos, 1.0);
    // gl_Position = projection * view * model * vec4(aPos+aInstancePos, 1.0);

}
