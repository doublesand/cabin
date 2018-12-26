#include <glad/glad.h>   //������ָ��OpenGL�ĺ���ָ�롣OpenGLֻ��һ����׼/�淶�������ʵ��������������������ض��Կ�ʵ�ֵġ�����OpenGL�����汾�ڶ࣬�������������λ�ö��޷��ڱ���ʱȷ����������Ҫ������ʱ��ѯ��
#include <GLFW/glfw3.h>  //����OpenGL�����ģ����崰�ڲ����Լ������û�����

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"   //�������裬��ͼƬת��OpenGL���Զ�ȡ�ĸ�ʽ

#include "Shader.h"      //��ɫ���࣬�򻯴�����ɫ���Ĺ���
#include <iostream>      

#define ASSERT(x) if (!(x)) __debugbreak();   //���Դ���
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << "): " << function <<
			" " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}



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

	glEnable(GL_DEPTH_TEST);

	Shader shader("vertex.glsl", "fragment.glsl");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {                       //��������
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	unsigned int VBO, VAO; 
	glGenVertexArrays(1, &VAO);  //����һ�������������
	glGenBuffers(1, &VBO);       //����һ�����㻺�����
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);      //�󶨶����������

	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //�󶨶��㻺�����
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //������Ҫ���������

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);  //��������Ϊ0��һ������Ϊ3��float�Ļ���������ɣ�����Ҫ��һ������СΪ3��float�Ĵ�С
	glEnableVertexAttribArray(0);  //������������Ϊ0�Ķ�����Ⱦ��OpenGLĬ�Ϲص����ж�������

	// texture coord attribute ��������������Ϊ2
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); //��������Ϊ2��float
	glEnableVertexAttribArray(1);

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
	shader.setInt("texture2", 0);
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         //�����ɫ���棬�Ժ������Ȼ���ȵȣ�����״̬ʹ�ú�����ʹ��ǰ�������
		
		 // bind textures on corresponding texture units����ȾǰҪ��������Ԫ
		glActiveTexture(GL_TEXTURE0); //���õ�ǰ���ԪΪGL_TEXTURE0
		glBindTexture(GL_TEXTURE_2D, texture1);  //ǰ�������ˣ��������ͻ�󶨵�ǰ��ļ��Ԫ��
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// activate shader
		shader.use();

		// create transformations
		glm::mat4 model(1.0f);            //ģ�;���
		glm::mat4 view(1.0f);             //�۲����
		glm::mat4 projection(1.0f);       //͸�Ӿ���
		//�ú�����modol��һ������ת�ĽǶ�ΪglfwGetTime()��glm::radians���Խ��Ƕ�ת��Ϊ����
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));   
		//��ÿ���������ŵı���
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		//������Z��ĸ������ƶ������������ǿ��Կ���
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		//���������ֱ�����Ұ��ͨ����45.0f���ڶ��������ǿ�߱ȣ����ӿڵĿ���Ը����á��������͵��ĸ�������ƽ��ͷ��Ľ���Զƽ�棬ͨ�����ý�����Ϊ0.1f,Զ����Ϊ100.0f
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		// retrieve the matrix uniform locations
		unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
		unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
		// pass them to the shaders (3 different ways)
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		shader.setMat4("projection", projection);


		//����������
		glBindVertexArray(VAO);               //ʹ��VAO���������������ֻ��һ��������ж����ÿ��ֻ�ܰ�һ��
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);    //������ɫ���棬��������Ⱦ�ٶȿ쵽���ǿ��ú����������
		glfwPollEvents();     //�����Ƿ񴥷��¼����������롢����ƶ��������´��ڵȣ���ִ����Ӧ�Ļص�����
	}

	// optional: de-allocate all resources once they've outlived their purpose:
   // ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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