#include <glad/glad.h>   //������ָ��OpenGL�ĺ���ָ�롣OpenGLֻ��һ����׼/�淶�������ʵ��������������������ض��Կ�ʵ�ֵġ�����OpenGL�����汾�ڶ࣬�������������λ�ö��޷��ڱ���ʱȷ����������Ҫ������ʱ��ѯ��
#include <GLFW/glfw3.h>  //����OpenGL�����ģ����崰�ڲ����Լ������û�����

#include <iostream>      

void framebuffer_size_callback(GLFWwindow* window, int width, int height);  //�ӿ����ã���ⴰ�ڸı��ӿ�Ҳ���Ÿı�
void processInput(GLFWwindow *window);   

// settings
const unsigned int SCR_WIDTH = 800;    //���ڿ��
const unsigned int SCR_HEIGHT = 600;   //���ڸ߶�

const char *vertexShaderSource = "#version 330 core\n" //������ɫ��
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 color;\n"
"out vec3 fragcolor;"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"	fragcolor = color;\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"  //ƬԪ��ɫ��
"in  vec3 fragcolor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(fragcolor, 1.0f);\n"
"}\n\0";

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

	//��ʼ��������ɫ��
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);            //����������ɫ��
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);     //����Դ��
	glCompileShader(vertexShader);                                  //���붥����ɫ��
	//��鶥����ɫ�������Ƿ�ɹ�
	int success;                     //���λ
	char infoLog[512];               //�쳣��¼
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);  //����������λ
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// ƬԪ��ɫ��
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);  //����ƬԪ��ɫ��
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); //����ƬԪ��ɫ��Դ��
	glCompileShader(fragmentShader);                              //����ƬԪ��ɫ��
	// ����Ƿ��������
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// ������ɫ��
	int shaderProgram = glCreateProgram();             //����С����
	glAttachShader(shaderProgram, vertexShader);       //���Ӷ�����ɫ��
	glAttachShader(shaderProgram, fragmentShader);     //����ƬԪ��ɫ��
	glLinkProgram(shaderProgram);                      //����С����
	// ��������Ƿ�����
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);   //���Ӻ󶥵���ɫ��Դ�����ɾ��
	glDeleteShader(fragmentShader); //���Ӻ�ƬԪ��ɫ��Դ�����ɾ��

	float vertices[] = {
	   0.5f,  0.5f, 0.0f,  // top right
	   0.5f,  0.5f, 0.0f,  //��ɫ
	   0.5f, -0.5f, 0.0f,  // bottom right
	   0.5f,  0.6f, 0.3f,  //��ɫ
	  -0.5f, -0.5f, 0.0f,  // bottom left
	   0.2f,  0.5f, 0.7f,  //��ɫ 
	  -0.5f,  0.5f, 0.0f,  // top left 
	   0.5f,  0.1f, 0.5f   //��ɫ
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);  //��������Ϊ0��һ������Ϊ3��float�Ļ���������ɣ�����Ҫ��һ������СΪ3��float�Ĵ�С
	glEnableVertexAttribArray(0);  //������������Ϊ0�Ķ�����Ⱦ��OpenGLĬ�Ϲص����ж�������

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);  //��������Ϊ0��һ������Ϊ3��float�Ļ���������ɣ�����Ҫ��һ������СΪ3��float�Ĵ�С
	glEnableVertexAttribArray(1);  //������������Ϊ0�Ķ�����Ⱦ��OpenGLĬ�Ϲص����ж�������


	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);  //ע�⣬����ΪGL_ARRAY_BUFFER��ʱ��VAO�ǲ���¼�ģ����������GL_ARRAY_ELEMENT_BUFFER��VAO�Ǽ�¼�ģ�Ҳ����������ڰ�VAOǰ�����EBO���Ǻܿ����㽫���ܰ�������Ⱦ

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//֮�������Խ��VAO�İ󶨣���������VAO���þͲ���������޸����VAO��������ٷ�����������Σ��޸�����VAO����Ҫ����glBindVertexArray������ڲ���Ҫֱ�Ӱ�VAO����VBO��ʱ������ͨ�����ͷ�VAO
	glBindVertexArray(0);
	
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
		
		//����������
		glUseProgram(shaderProgram);  //ʹ�����ǵ���ɫ��������ģʽ�ǲ��ṩ��
		glBindVertexArray(VAO);       //ʹ��VAO���������������ֻ��һ��������ж����ÿ��ֻ�ܰ�һ��
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