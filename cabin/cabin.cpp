#include <glad/glad.h>   //������ָ��OpenGL�ĺ���ָ�롣OpenGLֻ��һ����׼/�淶�������ʵ��������������������ض��Կ�ʵ�ֵġ�����OpenGL�����汾�ڶ࣬�������������λ�ö��޷��ڱ���ʱȷ����������Ҫ������ʱ��ѯ��
#include <GLFW/glfw3.h>  //����OpenGL�����ģ����崰�ڲ����Լ������û�����
#include "Shader.h"      //��ɫ���࣬�򻯴�����ɫ���Ĺ���
#include "stb_image.h"   //�������裬��ͼƬת��OpenGL���Զ�ȡ�ĸ�ʽ

#include <iostream>      

void framebuffer_size_callback(GLFWwindow* window, int width, int height);  //�ӿ����ã���ⴰ�ڸı��ӿ�Ҳ���Ÿı�
void processInput(GLFWwindow *window);   

// settings
const unsigned int SCR_WIDTH = 800;    //���ڿ��
const unsigned int SCR_HEIGHT = 600;   //���ڸ߶�

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();  //��ʼ��GLFW��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //����GLFW���汾Ϊ3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  //����GLFW�˰汾Ϊ3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //ʹ�ú���ģʽ����ζ������Ҫ�Լ�д��ɫ��

#ifdef __APPLE__  //����ƻ��ϵͳ�����
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL); //����һ�����ڣ�Ҫ��������ͼ��viewport��
	if (window == NULL)  //�������ɹ��᷵��NULL���������
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();  //����ǰ���øú�����������������������ڴ�
		return -1;
	}
	glfwMakeContextCurrent(window);  //�󶨵�ǰ����Ϊ�����Ĵ��ڣ�Ҳ�����Ժ�Ļ��ƶ����ڸô�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //GLFW�Ĵ��ڻص������������ڸı�ʱ����Ӧ�����������Щ����Ӧ���ڴ��ڴ�������Ⱦ��ʼǰע��

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //�����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader shader("vertex.glsl", "fragment.glsl");

	float vertices[] = {  //����һ�������ꡢ��ɫ�������������������Ѿ��������ˣ��Ժ�������
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	unsigned int VBO, VAO, EBO; 
	glGenVertexArrays(1, &VAO);  //����һ�������������
	glGenBuffers(1, &VBO);       //����һ�����㻺�����
	glGenBuffers(1, &EBO);       //����һ�������������
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);      //�󶨶����������

	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //�󶨶��㻺�����
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //������Ҫ���������

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);    //����������
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //�����������棬GL_STATIC_DRAW˵���ǲ��������

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);  //��������Ϊ0��һ������Ϊ3��float�Ļ���������ɣ�����Ҫ��һ������СΪ3��float�Ĵ�С
	glEnableVertexAttribArray(0);  //������������Ϊ0�Ķ�����Ⱦ��OpenGLĬ�Ϲص����ж�������

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);  //��ɫ����Ϊ1����ɫΪ3��float�Ļ���������ɣ�����Ҫ��һ������СΪ3��float�Ĵ�С
	glEnableVertexAttribArray(1);  //����������ɫ����Ϊ1�Ķ�����Ⱦ��OpenGLĬ�Ϲص����ж�������

	// texture coord attribute ��������������Ϊ2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //��������Ϊ2��float
	glEnableVertexAttribArray(2);

	// load and create a texture 
   // -------------------------
	unsigned int texture1, texture2;
	// texture 1
	// ---------
	glGenTextures(1, &texture1);   //��������1
	glBindTexture(GL_TEXTURE_2D, texture1); //�󶨵�ǰ����Ϊ����1
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // ʵ��ͼƬ������OpenGL������y���Ƿ��ģ���������һ�¾���
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// texture 2
	// ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	shader.use(); // don't forget to activate/use the shader before setting uniforms!
	// either set it manually like so:
	glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);  //texture1����Ϊ0����һ�������ȥ�ҵ������Ԫ��Ⱦ
	// or set it via the texture class
	shader.setInt("texture2", 1);
	
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))  //���window��Ӧ�ñ��رգ��ͳ�����Ⱦ
	{
		// input
		// -----
		processInput(window);  //ÿ��ѭ�������������Ƿ�Ҫ���ر�

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //�����������ɫ����Ҫ�����ʱ���������ɫ���������״̬���ú���
		glClear(GL_COLOR_BUFFER_BIT);         //�����ɫ���棬�Ժ������Ȼ���ȵȣ�����״̬ʹ�ú�����ʹ��ǰ�������
		
		 // bind textures on corresponding texture units����ȾǰҪ��������Ԫ
		glActiveTexture(GL_TEXTURE0); //���õ�ǰ���ԪΪGL_TEXTURE0
		glBindTexture(GL_TEXTURE_2D, texture1);  //ǰ�������ˣ��������ͻ�󶨵�ǰ��ļ��Ԫ��
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		//����������
		shader.use();                         //ʹ�����ǵ���ɫ��������ģʽ�ǲ��ṩ��
		shader.setFloat("xOffset", 0.2f);      //���ʹ��uniform
		glBindVertexArray(VAO);               //ʹ��VAO���������������ֻ��һ��������ж����ÿ��ֻ�ܰ�һ��
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);    //������ɫ���棬��������Ⱦ�ٶȿ쵽���ǿ��ú����������
		glfwPollEvents();     //�����Ƿ񴥷��¼����������롢����ƶ��������´��ڵȣ���ִ����Ӧ�Ļص�����
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();  //��Ⱦѭ��������������Ҫ��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ��
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //ÿ��ѭ���Ŀ�ʼǰ���һ��GLFW�Ƿ�Ҫ���˳�����Esc��
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);  //OpenGLʹ���ӿڵĿ�͸߽���2D����ת��
	std::cout << "���ڴ�С�����˸ı䣡" << std::endl;
}