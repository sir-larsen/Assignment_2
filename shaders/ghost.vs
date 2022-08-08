#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 u_TransformationMat;
uniform mat4 u_ViewMat;
uniform mat4 u_ProjectionMat;

void main()
{
    FragPos = vec3(u_TransformationMat * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(u_TransformationMat))) * aNormal;  
    TexCoords = aTexCoords;
    
    gl_Position = u_ProjectionMat * u_ViewMat * vec4(FragPos, 1.0);
}