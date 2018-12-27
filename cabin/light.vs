#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
	//法向量变换矩阵应该在CPU上处理，这里为了方便，搭建框架时注意
    Normal = mat3(transpose(inverse(view * model))) * aNormal; 
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}