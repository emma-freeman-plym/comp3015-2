#version 460

// Inputs to the shader, position and normal of this vertex
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;

// Output of the shader, light intensity of this vertex
out vec3 position;
out vec3 normal;

uniform mat4 model_view_matrix;
uniform mat4 model_view_projection;
uniform mat3 normal_matrix;

void main() {
    position = (model_view_matrix * vec4(vertex_position, 1.0)).xyz;
    normal = normalize(normal_matrix * vertex_normal);
    gl_Position = model_view_projection * vec4(vertex_position, 1.0);
}
