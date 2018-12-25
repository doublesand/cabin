#include <glad/glad.h>   //管理函数指针OpenGL的函数指针。OpenGL只是一个标准/规范，具体的实现是由驱动开发商针对特定显卡实现的。由于OpenGL驱动版本众多，它大多数函数的位置都无法在编译时确定下来，需要在运行时查询。
#include <GLFW/glfw3.h>  //创建OpenGL上下文，定义窗口参数以及处理用户输入

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