#version 460

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec3 g_position;
out vec3 g_normal;
noperspective out vec3 g_edge_distance;

in vec3 position[];
in vec3 normal[];

uniform mat4 viewport_matrix;

void main() {
    vec2 p0 = vec2(viewport_matrix * (gl_in[0].gl_Position / gl_in[0].gl_Position.w));
    vec2 p1 = vec2(viewport_matrix * (gl_in[1].gl_Position / gl_in[1].gl_Position.w));
    vec2 p2 = vec2(viewport_matrix * (gl_in[2].gl_Position / gl_in[2].gl_Position.w));

    float a = length(p1 - p2);
    float b = length(p2 - p0);
    float c = length(p1 - p0);

    float alpha = acos((b * b + c * c - a * a) / (2 * b * c));
    float beta = acos((a * a + c * c - b * b) / (2 * a * c));

    float ha = abs(c * sin(beta));
    float hb = abs(c * sin(alpha));
    float hc = abs(b * sin(alpha));

    g_edge_distance = vec3(ha, 0, 0);
    g_position = position[0];
    g_normal = normal[0];
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    g_edge_distance = vec3(0, hb, 0);
    g_position = position[1];
    g_normal = normal[1];
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    g_edge_distance = vec3(0, 0, hc);
    g_position = position[2];
    g_normal = normal[2];
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}
