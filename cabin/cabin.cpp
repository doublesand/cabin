#include <glad/glad.h>   //������ָ��OpenGL�ĺ���ָ�롣OpenGLֻ��һ����׼/�淶�������ʵ��������������������ض��Կ�ʵ�ֵġ�����OpenGL�����汾�ڶ࣬�������������λ�ö��޷��ڱ���ʱȷ����������Ҫ������ʱ��ѯ��
#include <GLFW/glfw3.h>  //����OpenGL�����ģ����崰�ڲ����Լ������û�����

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