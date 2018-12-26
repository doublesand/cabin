#version 330 core
layout (location = 0) in vec3 aPos;       //顶点属性为0表示顶点坐标
layout (location = 1) in vec2 aTexCoord;  //顶点属性为0表示顶点纹理

out vec2 TexCoord;        //输出纹理到片元着色器

uniform mat4 model;       //模型矩阵
uniform mat4 view;        //观察矩阵
uniform mat4 projection;  //投影矩阵

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);    //模视矩阵到透视矩阵
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);                     //传纹理到
}