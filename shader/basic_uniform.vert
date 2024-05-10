#version 460

// Inputs to the shader, position and normal of this vertex
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_tex_coord;

// Output of the shader, light intensity of this vertex
out vec3 position;
out vec3 normal;
out vec2 tex_coord;

uniform float time;
uniform float freq = 4.5;
uniform float velocity = 3.5;
uniform float amp = 0.03;

uniform mat4 model_view_matrix;
uniform mat4 model_view_projection;
uniform mat3 normal_matrix;

void main() {
    vec4 vpos = vec4(vertex_position, 1.0);
    float off = amp * sin(freq * vpos.z - velocity * time);
    vpos += vec4(normalize(vertex_normal), 1.0) * off;

    position = (model_view_matrix * vpos).xyz;
    normal = normalize(normal_matrix * vertex_normal);
    tex_coord = vertex_tex_coord;
    gl_Position = model_view_projection * vpos;
}
