#version 410
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec4 TexRange;

out VERTEX{
	vec2 uv;
	vec4 range;
}vertex;

void main()
{
	vertex.uv = VertexCoord;
	vertex.range = TexRange;
	gl_Position = P * V * M * vec4(VertexPosition, 1.0);
}