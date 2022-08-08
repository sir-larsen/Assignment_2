#version 430 core

layout (location = 0) in vec3 aPos;

//We specify our uniforms. We do not need to specify locations manually, but it helps with knowing what is bound where.
layout(location=0) uniform mat4 u_TransformationMat = mat4(1);
layout(location=1) uniform mat4 u_ViewMat           = mat4(1);
layout(location=2) uniform mat4 u_ProjectionMat     = mat4(1);
layout(location=3) uniform mat4 u_Scale				= mat4(1);


void main()
{
//We multiply our matrices with our position to change the positions of vertices to their final destinations.
gl_Position = u_ProjectionMat * u_ViewMat * u_TransformationMat * u_Scale * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}


