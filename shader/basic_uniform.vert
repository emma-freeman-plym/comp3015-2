#version 460

// Inputs to the shader, position and normal of this vertex
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_tex_coord;

// Output of the shader, light intensity of this vertex
out vec3 position;
out vec3 normal;
out vec2 tex_coord;

uniform mat4 model_view_matrix;
uniform mat4 model_view_projection;
uniform mat3 normal_matrix;

void main() {
    position = (model_view_matrix * vec4(vertex_position, 1.0)).xyz;
    normal = normalize(normal_matrix * vertex_normal);
    tex_coord = vertex_tex_coord;
    gl_Position = model_view_projection * vec4(vertex_position, 1.0);
}
