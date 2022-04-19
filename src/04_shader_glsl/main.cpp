// https://learnopengl-cn.github.io/01%20Getting%20started/03%20Hello%20Window/

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

//定义一个顶点着色器
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 aColor;\n" // 颜色变量的属性位置值为 1
                                 "out vec3 ourColor;\n"                    // 向片段着色器输出一个颜色
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
                                 "   ourColor=aColor;\n"
                                 "}\0";

//定义一个片元着色器
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "in vec3 ourColor;" // 在OpenGL程序代码中设定这个变量
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(ourColor,1);\n"
                                   "}\0";

int main()
{
    glfwInit(); //初始化GLFW
    //设置主要版本和次要版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfwCreateWindow函数需要窗口的宽和高作为它的前两个参数。第三个参数表示这个窗口的名称（标题）
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //设置视口 前两个是控制左下角的位置
    glViewport(0, 0, 800, 600);
    glEnable(GL_PROGRAM_POINT_SIZE);

    //注册窗口变化监听
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //定义顶点数组
    float vertices[] = {
        // 位置              // 颜色
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // 右下
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 左下
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // 顶部
    };

    //创建VBO,VAO,EBO
    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &EBO);

    //绑定VAO
    glBindVertexArray(VAO);

    //绑定VBO缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //填充VBO数据

    //设置顶点位置属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    //设置顶点颜色属性指针
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    //创建顶点着色器
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //附加着色器源码
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //创建片元着色器
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //附加着色器源码
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //创建着色器程序
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    //附加着色器
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    //链接着色器程序
    glLinkProgram(shaderProgram);

    //删除着色器
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //激活着色器程序
    glUseProgram(shaderProgram);

    //让窗口保持显示
    while (!glfwWindowShouldClose(window))
    {
        // 输入
        processInput(window);

        // 渲染指令
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //设置清空屏幕所用的颜色
        glClear(GL_COLOR_BUFFER_BIT);         //清除颜色缓冲之后 设置上面的颜色

        //随时间变化的颜色值
        // float timeValue = glfwGetTime();
        // //设置顶点着色器变量
        // float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        // int ourColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); //获取地址
        // glUseProgram(shaderProgram);
        // glUniform4f(ourColorLocation, 0.0f, greenValue, 0.0f, 1.0f); //设置vec4 值

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawArrays(GL_LINE_LOOP, 0, 3);
        // glDrawArrays(GL_POINTS, 0, 3);

        // glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);//EBO
        glBindVertexArray(0); //解绑VAO

        // 检查并调用事件，交换缓冲
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    //资源释放
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();

    return 0;
}

//当窗口大小改变的时候 视口大小同样也需要改变
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//输入监听函数
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //按下ESC 退出窗口
        glfwSetWindowShouldClose(window, true);
}