#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;   //供纹理对象使用的内建数据类型,uniform可以理解为全局变量

void main()
{
	FragColor = texture(texture1, TexCoord);  //GLSL内建的texture函数来采样纹理的颜色，参数为纹理采样器和纹理坐标
}