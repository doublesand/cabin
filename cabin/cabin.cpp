#include <glad/glad.h>   //管理函数指针OpenGL的函数指针。OpenGL只是一个标准/规范，具体的实现是由驱动开发商针对特定显卡实现的。由于OpenGL驱动版本众多，它大多数函数的位置都无法在编译时确定下来，需要在运行时查询。
#include <GLFW/glfw3.h>  //创建OpenGL上下文，定义窗口参数以及处理用户输入

#include <iostream>      

void framebuffer_size_callback(GLFWwindow* window, int width, int height);  //视口设置，检测窗口改变视口也跟着改变
void processInput(GLFWwindow *window);   

// settings
const unsigned int SCR_WIDTH = 800;    //窗口宽度
const unsigned int SCR_HEIGHT = 600;   //窗口高度

const char *vertexShaderSource = "#version 330 core\n" //顶点着色器
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 color;\n"
"out vec3 fragcolor;"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"	fragcolor = color;\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"  //片元着色器
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
	glfwInit();  //初始化GLFW库
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //告诉GLFW主版本为3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  //告诉GLFW此版本为3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //使用核心模式，意味着我们要自己写着色器

#ifdef __APPLE__  //兼容苹果系统的语句
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL); //创建一个窗口，要区分于视图（viewport）
	if (window == NULL)  //创建不成功会返回NULL，程序结束
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();  //结束前调用该函数清除所有这个程序申请的内存
		return -1;
	}
	glfwMakeContextCurrent(window);  //绑定当前窗口为上下文窗口，也就是以后的绘制都是在该窗口上
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //GLFW的窗口回调函数，当窗口改变时会相应这个函数，这些函数应该在窗口创建后，渲染开始前注册

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //调用任何OpenGL的函数之前我们需要初始化GLAD
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//初始化顶点着色器
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);            //创建顶点着色器
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);     //传入源码
	glCompileShader(vertexShader);                                  //编译顶点着色器
	//检查顶点着色器编译是否成功
	int success;                     //标记位
	char infoLog[512];               //异常记录
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);  //传出错误标记位
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// 片元着色器
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);  //创建片元着色器
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); //传入片元着色器源码
	glCompileShader(fragmentShader);                              //编译片元着色器
	// 检查是否编译有误
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// 链接着色器
	int shaderProgram = glCreateProgram();             //创建小程序
	glAttachShader(shaderProgram, vertexShader);       //链接顶点着色器
	glAttachShader(shaderProgram, fragmentShader);     //链接片元着色器
	glLinkProgram(shaderProgram);                      //链接小程序
	// 检查链接是否有误
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);   //链接后顶点着色器源码可以删了
	glDeleteShader(fragmentShader); //链接后片元着色器源码可以删了

	float vertices[] = {
	   0.5f,  0.5f, 0.0f,  // top right
	   0.5f,  0.5f, 0.0f,  //颜色
	   0.5f, -0.5f, 0.0f,  // bottom right
	   0.5f,  0.6f, 0.3f,  //颜色
	  -0.5f, -0.5f, 0.0f,  // bottom left
	   0.2f,  0.5f, 0.7f,  //颜色 
	  -0.5f,  0.5f, 0.0f,  // top left 
	   0.5f,  0.1f, 0.5f   //颜色
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	unsigned int VBO, VAO, EBO; 
	glGenVertexArrays(1, &VAO);  //生成一个顶点数组对象
	glGenBuffers(1, &VBO);       //生成一个顶点缓存对象
	glGenBuffers(1, &EBO);       //生成一个索引缓存对象
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);      //绑定顶点数组对象

	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //绑定顶点缓存对象
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //传入需要缓存的数据

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);    //绑定索引缓存
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //传入索引缓存，GL_STATIC_DRAW说明是不变的数据

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);  //顶点属性为0，一个顶点为3个float的基本类型组成，不需要归一化，大小为3个float的大小
	glEnableVertexAttribArray(0);  //开启顶点属性为0的顶点渲染，OpenGL默认关掉所有顶点属性

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);  //顶点属性为0，一个顶点为3个float的基本类型组成，不需要归一化，大小为3个float的大小
	glEnableVertexAttribArray(1);  //开启顶点属性为0的顶点渲染，OpenGL默认关掉所有顶点属性


	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);  //注意，类型为GL_ARRAY_BUFFER的时候VAO是不记录的，但是如果是GL_ARRAY_ELEMENT_BUFFER，VAO是记录的，也就是如果你在绑定VAO前解绑了EBO，那很可能你将不能按索引渲染

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//之后您可以解除VAO的绑定，这样其他VAO调用就不会意外地修改这个VAO，但这很少发生。无论如何，修改其他VAO都需要调用glBindVertexArray，因此在不需要直接绑定VAO（或VBO）时，我们通常不释放VAO
	glBindVertexArray(0);
	
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))  //如果window不应该被关闭，就持续渲染
	{
		// input
		// -----
		processInput(window);  //每次循环都监听窗口是否要被关闭

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //清除背景的颜色，当要清除的时候用这个颜色清除，这是状态设置函数
		glClear(GL_COLOR_BUFFER_BIT);         //清除颜色缓存，以后会有深度缓存等等，这是状态使用函数，使用前面的设置
		
		//绘制正方形
		glUseProgram(shaderProgram);  //使用我们的着色器，核心模式是不提供的
		glBindVertexArray(VAO);       //使用VAO顶点数组对象，这里只有一个，如果有多个，每次只能绑定一个
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);    //交换颜色缓存，以至于渲染速度快到我们看得很流畅，舒服
		glfwPollEvents();     //监听是否触发事件（键盘输入、鼠标移动）、更新窗口等，并执行相应的回调函数
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();  //渲染循环结束后我们需要正确释放/删除之前的分配的所有资源。
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //每次循环的开始前检查一次GLFW是否被要求退出，按Esc键
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);  //OpenGL使用视口的宽和高进行2D坐标转换
	std::cout << "窗口大小发生了改变！" << std::endl;
}