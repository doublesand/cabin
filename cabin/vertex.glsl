#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 transform;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = transform * vec4(aPos, 1.0);             //位置信息
	ourColor = aColor;                         //颜色
	TexCoord = vec2(aTexCoord.x, aTexCoord.y); //纹理给片元着色器处理，这里只是传递
}