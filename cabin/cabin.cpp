#include <glad/glad.h>   //管理函数指针OpenGL的函数指针。OpenGL只是一个标准/规范，具体的实现是由驱动开发商针对特定显卡实现的。由于OpenGL驱动版本众多，它大多数函数的位置都无法在编译时确定下来，需要在运行时查询。
#include <GLFW/glfw3.h>  //创建OpenGL上下文，定义窗口参数以及处理用户输入
#include "Shader.h"      //着色器类，简化创建着色器的过程
#include "stb_image.h"   //纹理所需，将图片转成OpenGL可以读取的格式

#include <iostream>      

void framebuffer_size_callback(GLFWwindow* window, int width, int height);  //视口设置，检测窗口改变视口也跟着改变
void processInput(GLFWwindow *window);   

// settings
const unsigned int SCR_WIDTH = 800;    //窗口宽度
const unsigned int SCR_HEIGHT = 600;   //窗口高度

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

	Shader shader("vertex.glsl", "fragment.glsl");

	float vertices[] = {  //顶点一般有坐标、颜色、纹理、法向量，这里已经有三个了，以后继续添加
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
	glGenVertexArrays(1, &VAO);  //生成一个顶点数组对象
	glGenBuffers(1, &VBO);       //生成一个顶点缓存对象
	glGenBuffers(1, &EBO);       //生成一个索引缓存对象
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);      //绑定顶点数组对象

	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //绑定顶点缓存对象
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //传入需要缓存的数据

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);    //绑定索引缓存
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //传入索引缓存，GL_STATIC_DRAW说明是不变的数据

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);  //顶点属性为0，一个顶点为3个float的基本类型组成，不需要归一化，大小为3个float的大小
	glEnableVertexAttribArray(0);  //开启顶点属性为0的顶点渲染，OpenGL默认关掉所有顶点属性

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);  //颜色属性为1，颜色为3个float的基本类型组成，不需要归一化，大小为3个float的大小
	glEnableVertexAttribArray(1);  //开启顶点颜色属性为1的顶点渲染，OpenGL默认关掉所有顶点属性

	// texture coord attribute ，纹理属性设置为2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //纹理坐标为2个float
	glEnableVertexAttribArray(2);

	// load and create a texture 
	// -------------------------
	unsigned int texture;  
	glGenTextures(1, &texture);  //申请一个纹理对象
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// 设置视觉效果（4种，边缘方式的话还需要设置边缘颜色），分别为纹理目标、纹理方向（单独设置每个方向，和x、y轴类似）以及环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 设置纹理过滤方式（邻近过滤和线性过滤），我们使用线性过滤会比较光滑好看，其实就是浮点坐标到像素阵列的转换
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// 下面参数分别是图片路径、图片宽度、高度以及图片的颜色通道个数
	unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
	if (data)  //如果读取到文件，生成一个纹理
	{
		//参数为纹理目标、纹理级别（可以用来渐远设置）、纹理格式（一般都是颜色）、以及读到的宽和高，后面的0没有什么用，接下来是源图的格式和数据类型，最后是图像数据
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//为当前绑定的纹理自动生成所有需要的多级渐远纹理，远处物体不会失真
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	//生成了纹理和相应的多级渐远纹理后，释放图像的内存是一个很好的习惯。
	stbi_image_free(data);
	
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
		
		//绑定纹理到texture对象中
		glBindTexture(GL_TEXTURE_2D, texture);

		//绘制正方形
		shader.use();                         //使用我们的着色器，核心模式是不提供的
		shader.setFloat("xOffset", 0.2);      //灵活使用uniform
		glBindVertexArray(VAO);               //使用VAO顶点数组对象，这里只有一个，如果有多个，每次只能绑定一个
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