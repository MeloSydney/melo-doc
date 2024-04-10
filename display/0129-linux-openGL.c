

//TODO - openGL 画一个三角形


float vertices[] = {
	-0.5f, 0.0f, 0.0f, // 左顶点
	0.5f, 0.0f, 0.0f, // 右顶点
	0.0f,  0.5f, 0.0f  // 上顶点
};
float vertices1[] = {
	-0.5f, 0.0f, 0.0f, // 左顶点
	0.5f, 0.0f, 0.0f, // 右顶点
	0.0f,  -0.25f, 0.0f  // 下顶点
};

......
unsigned int VBO, VBO1, VAO, VAO1;   /* 1 */

glGenVertexArrays(1, &VAO);          /* 2 */
glGenBuffers(1, &VBO);
glBindVertexArray(VAO);              /* 3 */
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);    /* 4 */

glGenVertexArrays(1, &VAO1);
glGenBuffers(1, &VBO1);
glBindVertexArray(VAO1);
glBindBuffer(GL_ARRAY_BUFFER, VBO1);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

while (!glfwWindowShouldClose(window))
{
    ......
    glBindVertexArray(VAO);              /* 5 */
    glDrawArrays(GL_TRIANGLES, 0, 3);    /* 6 */

    glBindVertexArray(VAO1);             /* 7 */
    glDrawArrays(GL_TRIANGLES, 0, 3);
  ......
}



1 声明顶点缓冲对象和顶点数组对象
2 glGen 分配顶点数组对象和顶点缓冲对象，
	mesa分别维护了顶点数组对象和缓冲对象的hash表(ctx->Shared->BufferObjects)，
	分配实际上就是从hash表中查找可用的key
3 glBind 绑定数组对象和缓冲对象，mesa维护了OpenGL渲染的上下文 gl_context(mtypes.h )，
	这是一个全局的变量，绑定的主要动作就是让上下文中的Array数组指向新配备的数组对象，缓冲对象同理
4 glBufferData 将用户定义的顶点数据拷贝到缓冲对象中。
	核心动作都是将顶点数据拷贝到VBO关联的内存buffer中，
	而该buffer通过顶点数组VAO就可以找到
5 glBind 将当前上下文与第一个顶点数组VAO关联，通过VAO可以找到顶点数据vertices
6 下发绘制命令给GPU，实质就是将顶点数据准备好，然后组装DRAW_INDEX渲染命令，将其提交给GPU执行，
	GPU执行命令时将CPU准备的顶点数据拷贝到GPU硬件单元，然后处理顶点，绘制图元
7 将当前上下文与第一个顶点数组VAO1关联，通过VAO1可以找到顶点数据vertices1




//TODO - openGL 画一个三角形 利用 index buffer

float vertices[] = {
	-0.5f, 0.0f, 0.0f,  // 左顶点
	0.5f, 0.0f, 0.0f,   // 右顶点
	0.0f, 0.5f, 0.0f,   // 上顶点
	0.0f, -0.25f, 0.0f  // 下顶点
};

unsigned int indices[] = {
	0, 1, 2,       /* 第一个三角形 */
	0, 1, 3        /* 第二个三角形 */
};


......
unsigned int VBO, VAO, EBO;   /* 1 */
glGenVertexArrays(1, &VAO);   /* 2 */
glGenBuffers(1, &VBO);
glGenBuffers(1, &EBO);

glBindVertexArray(VAO);       /* 3 */
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); /* 4 */

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);                                /* 5 */
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); /* 6 */
while (!glfwWindowShouldClose(window))
{
	......
	glBindVertexArray(VAO);                              /* 7 */
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); /* 8 */
	......
}

1 声明顶点缓冲对象，顶点数组和索引缓冲对象，我们需要用索引缓冲对象来向GPU指明绘制图元时需要用的顶点
2 glGen 分配顶点各个对象，对于每类对象，mesa中都维护有对应的hash表，分配对象就是查找可用的key
3 glBind 绑定顶点数组对象和顶点缓冲对象，后续所有的VBO和EBO缓存都会被顶点数组对象记录
4 glBufferData 将顶点数据传递给缓冲对象
5 绑定索引缓冲对象
6 将索引数据传递给缓冲对象
7 再次绑定顶点数组到当前上下文，让mesa可以通过VAO找到对应的VBO和EBO
8 glDraw 索引绘制三角形，glDrawElements 会将当前上下文绑定的EBO作为顶点索引，
  VBO作为顶点数据，传递给GPU，然后执行DRAW_INDEX命令


//TODO - openGL keyword 关键字

gl同步
	glFlush  glFinish  glFenceSync  glClientWaitSync  glWaitSync


server gpu  client cpu
commandQUeue
	cpu和gpu分别有一个queue

GLContext


//TODO - openGL VBO VAO EBO


VBO  在显存上开辟的一片buf  每个VBO有**唯一标识符**
	存放定点
VAO  组织多个VBO  记录一次绘制需要的信息
	vertax array context句柄
	glBindBuffer: 数据位置
	glVertexAttribPointer: OpenGL怎样解释数据格式
	glEnableVertexAttribArray: shader-attribute的location的启用

EBO
	存放定点


//TODO - openGL API

// 通过 glVertex 定义三角形定点坐标 告诉GPU //
glBegin(GL_TRIANGLES);
glVertex(0, 0);
glVertex(1, 1);
glVertex(2, 2);
glEnd();

// 分配VBO //
glGenBuffers(1, &vboId);   /* 分配id */
glBindBuffer(GL_ARRAY_BUFFER, vboId);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); /* 将ram数据传给vram */
glEnableVertexAttribArray(0);   /* 激活Attrib0  之后才可以使用 */
glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    // 参数说明 1 属性idx  2 属性大小  3 数据类型  4  5 步长(0 让openGL决定)  6 offset //


// 分配VAO //
glGenVertexArrays(1, &vaoId);  分配id
glBindVertexArray(vaoId);
glDrawArrays (GLenum mode, GLint first, GLsizei count);  绘制
	// 1 绘制三角形的模式  2   3 绘制定点个数 //
	https://blog.csdn.net/u012169524/article/details/113383015  对mode的说明



//TODO - openGL 查看版本

#include <stdio.h>
#include <GL/glut.h>

int main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);    /* 1 */
	glutInitWindowSize(300,300);                           /* 2 */
	glutInitWindowPosition(100,100);              /* 3 */
	glutCreateWindow("OpenGL Version");
	const GLubyte* OpenGLVersion =glGetString(GL_VERSION); /* 4 */
	printf("OpenGL实现的版本号：%s\n",OpenGLVersion);
	return 0;
}

makefile

shifan@shifan-VirtualBox:~/0523-opengl$ cat Makefile
version: version.c
  gcc version.c -o version -lglut -lGL -lGLU

环境

sudo apt install freeglut3-dev



//TODO - openGl virgl

virglrenderer是一个开源项目virgil3d提供的开源库
主要功能是针对虚拟化场景，为QEMU提供一个具有3D图形处理的显卡，其使用方式就是为QEMU提供一组3D图形处理的接口。
QEMU通过调用virglrenderer的库接口实现主机侧的3D图形加速处理

virgl 主要工作是将Opengl state编码成virgl command

virgl_encoder_write_cmd_dword()  //生成command header
virgl_encoder_write_dword();    //followed dword



1 letting the guest applications speak unmodified OpenGL to the Mesa.
	用户应用使用 原生 opengl 给到 mesa
2 But instead of Mesa handing commands over to the hardware
	it is channeled through virtio-gpu on the guest to QEMU on the host.
	mesa 不处理命令 而是通过 virtio-gpu 将命令传递给 host
3 QEMU then receives the raw graphics stack state (Gallium state)
	qemu 收到原始的 graphics stack state
4 interprets it using virglrenderer from the raw state into an OpenGL form,
	which can be executed as entirely normal OpenGL on the host machine.
	使用 virglrenderer 翻译原始语句 >> opengl 语句

The host OpenGL stack does not even have to be Mesa,
and could for example be the proprietary nvidia stack.




//TODO - OpenGL ES

早先定义OpenGL ES是OpenGL的嵌入式设备版本，但由于嵌入式设备要求的是高性能，
所以一些其它纯追求高性能的设备也开始用这种API方式，比如 SONY PlayStation 3。

openGL ES是openGL的嵌入式子集 (OpenGL for Embedded System)



//TODO - OpenVG

OpenVG是针对诸如Flash和SVG的矢量图形算法库提供底层硬件加速界面的免授权费、跨平台应用程序接口API。



//TODO - WGL

Windows平台下的


//TODO - Mesa

mesa version
glxinfo | grep -iE "version|vendor|device"
glxinfo|grep direct          /* 是否支持 3d加速 */
sudo glxinfo | grep "OpenGL" /* 查询opengl版本 */

Linux下的, Mesa是Linux下的OpenGL实现, Mesa已经是任何一个Linux版本首选的OpenGL实现



