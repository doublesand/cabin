#version 330 core
layout (location = 0) in vec3 aPos;       //��������Ϊ0��ʾ��������
layout (location = 1) in vec2 aTexCoord;  //��������Ϊ0��ʾ��������

out vec2 TexCoord;        //�������ƬԪ��ɫ��

uniform mat4 model;       //ģ�;���
uniform mat4 view;        //�۲����
uniform mat4 projection;  //ͶӰ����

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);    //ģ�Ӿ���͸�Ӿ���
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);                     //������
}