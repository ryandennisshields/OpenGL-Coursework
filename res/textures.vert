#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 TextCoords;
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Bitangent;

out vec2 tC;
out vec3 fP;
out mat3 TBN;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    tC = TextCoords;
    fP = vec3(model * vec4(VertexPosition, 1.0));
	gl_Position = projection * view * model * vec4(VertexPosition, 1.0);

    vec3 T = normalize(mat3(model) * Tangent);
    vec3 B = normalize(mat3(model) * Bitangent);
    vec3 N = normalize(mat3(model) * Normal);

    TBN = transpose(mat3(T, B, N));

    TangentLightPos = TBN * lightPos;
    TangentViewPos = TBN * viewPos;
    TangentFragPos = TBN * fP;
}