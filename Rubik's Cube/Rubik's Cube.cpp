#include "GLEW/glew.h"
#include "GL/freeglut.h"
#include "glm.hpp"
#include "ReadFile.h"
#include "Pipeline.h"
#include "PerspectiveInfo.h"
#include "Texture.h"
#include "Camera.h"
#include <iostream>
#include <vector>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

using namespace std;
using namespace glm;

struct Vertex {
	vec3 m_pos;
	vec2 m_tex;

	Vertex() {}

	Vertex(vec3 pos, vec2 tex)
	{
		m_pos = pos;
		m_tex = tex;
	}
};

GLuint VBO;
GLuint IBO;
GLuint gWorldLocation;
GLuint controls;
GLuint gSampler;
Camera* pCamera = NULL;
Texture* pTexture = NULL;
PersProjInfo persProjInfo;

const char* pVSFileName = "shader.vs";
const char* pFSFileName = "shader.fs";

static void RenderSceneCB()
{
	pCamera->OnRender();

	glClear(GL_COLOR_BUFFER_BIT);

	static float Scale = 0.0f;
	Scale += 0.001f;

	Pipeline p;
	p.Scale(1.0f);
	p.Rotate(0.0f, 0.0f, 0.0f);
	p.WorldPos(0.0f, 0.0f, 0.0f);
	p.setCamera(*pCamera);
	p.setPersProj(persProjInfo);

	Pipeline c;
	c.Scale(1.0f);
	c.Rotate(0.0f, 90.0f, 0.0f);
	c.WorldPos(0.0f, 0.0f, 0.0f);
	c.setCamera(*pCamera);
	c.setPersProj(persProjInfo);


	glUniformMatrix4fv(gWorldLocation, 1, GL_FALSE, (const GLfloat*)p.getWVPTrans());
	glUniformMatrix4fv(controls, 1, GL_FALSE, (const GLfloat*)c.getWVPTrans());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	pTexture->Bind(GL_TEXTURE0);

	glDrawElements(GL_TRIANGLES, 144, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glutSwapBuffers();
}

static void SpecialKeyboardCB(int Key, int x, int y)
{
	pCamera->OnKeyboard(Key);
}

static void KeyboardCB(unsigned char Key, int x, int y)
{
	switch (Key) {
	case 'q':
		exit(0);
	}
}

static void PassiveMouseCB(int x, int y)
{
	pCamera->OnMouse(x, y);
}

static void InitializeGlutCallbacks() 
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);
	glutSpecialFunc(SpecialKeyboardCB);
	glutPassiveMotionFunc(PassiveMouseCB);
	glutKeyboardFunc(KeyboardCB);
}

static void CreateVertexBuffer()
{
	float bWidth = 0.0025;

	Vertex vertices[56];

	vertices[0] = { vec3(-1.0f, -1.0f, -1.0f), vec2(0.125f, 0.25f) };
	vertices[1] = { vec3(-bWidth, -1.0, -1.0f), vec2(0.25f, 0.25f) };
	vertices[2] = { vec3(-1.0f, -bWidth, -1.0f), vec2(0.125f, 0.5f) };
	vertices[3] = { vec3(-1.0f, -1.0f, -bWidth), vec2(0.125f, 0.0f) };
	vertices[4] = { vec3(-1.0f, -bWidth, -bWidth), vec2(0.0f, 0.25f) };
	vertices[5] = { vec3(-bWidth, -1.0f, -bWidth), vec2(0.25f, 0.0f) };
	vertices[6] = { vec3(-bWidth, -bWidth, -1.0f), vec2(0.25f, 0.5f) };
	vertices[7] = { vec3(-1.0f, -1.0f, 1.0f), vec2(0.375f, 0.25f) };
	vertices[8] = { vec3(-bWidth, -1.0f, 1.0f), vec2(0.5f, 0.25f) };
	vertices[9] = { vec3(-1.0f, -bWidth, 1.0f), vec2(0.375f, 0.5f) };
	vertices[10] = { vec3(-1.0f, -1.0f, bWidth), vec2(0.375f, 0.0f) };
	vertices[11] = { vec3(-1.0f, -bWidth, bWidth), vec2(0.25f, 0.25f) };
	vertices[12] = { vec3(-bWidth, -1.0f, bWidth), vec2(0.5f, 0.0f) };
	vertices[13] = { vec3(-bWidth, -bWidth, 1.0f), vec2(0.5f, 0.5f) };
	vertices[14] = { vec3(-1.0f, 1.0f, -1.0f), vec2(0.625f, 0.25f) };
	vertices[15] = { vec3(-bWidth, 1.0f, -1.0f), vec2(0.75f, 0.25f) };
	vertices[16] = { vec3(-1.0, bWidth, -1.0f), vec2(0.625f, 0.5f) };
	vertices[17] = { vec3(-1.0f, 1.0, -bWidth), vec2(0.625f, 0.0f) };
	vertices[18] = { vec3(-1.0f, bWidth, -bWidth), vec2(0.5f, 0.25f) };
	vertices[19] = { vec3(-bWidth, 1.0f, -bWidth), vec2(0.75f, 0.0f) };
	vertices[20] = { vec3(-bWidth, bWidth, -1.0f), vec2(0.75f, 0.5f) };
	vertices[21] = { vec3(-1.0, 1.0f, 1.0f), vec2(0.875f, 0.25f) };
	vertices[22] = { vec3(-bWidth, 1.0f, 1.0f), vec2(1.0f, 0.25f) };
	vertices[23] = { vec3(-1.0f, bWidth, 1.0f), vec2(0.875f, 0.5f) };
	vertices[24] = { vec3(-1.0f, 1.0f, bWidth), vec2(0.875f, 0.0f) };
	vertices[25] = { vec3(-1.0f, bWidth, bWidth), vec2(0.75f, 0.25f) };
	vertices[26] = { vec3(-bWidth, 1.0f, bWidth), vec2(1.0f, 0.0f) };
	vertices[27] = { vec3(-bWidth, bWidth, 1.0f), vec2(1.0f, 0.5f) };
	vertices[28] = { vec3(1.0f, -1.0f, -1.0f), vec2(0.125f, 0.75f) };
	vertices[29] = { vec3(bWidth, -1.0, -1.0f), vec2(0.25f, 0.75f) };
	vertices[30] = { vec3(1.0f, -bWidth, -1.0f), vec2(0.125f, 1.0f) };
	vertices[31] = { vec3(1.0f, -1.0f, -bWidth), vec2(0.125f, 0.5f) };
	vertices[32] = { vec3(1.0f, -bWidth, -bWidth), vec2(0.0f, 0.75f) };
	vertices[33] = { vec3(bWidth, -1.0f, -bWidth), vec2(0.25f, 0.5f) };
	vertices[34] = { vec3(bWidth, -bWidth, -1.0f), vec2(0.25f, 1.0f) };
	vertices[35] = { vec3(1.0f, -1.0f, 1.0f), vec2(0.375f, 0.75f) };
	vertices[36] = { vec3(bWidth, -1.0f, 1.0f), vec2(0.5f, 0.75f) };
	vertices[37] = { vec3(1.0f, -bWidth, 1.0f), vec2(0.375f, 1.0f) };
	vertices[38] = { vec3(1.0f, -1.0f, bWidth), vec2(0.375f, 0.5f) };
	vertices[39] = { vec3(1.0f, -bWidth, bWidth), vec2(0.25f, 0.75f) };
	vertices[40] = { vec3(bWidth, -1.0f, bWidth), vec2(0.5f, 0.5f) };
	vertices[41] = { vec3(bWidth, -bWidth, 1.0f), vec2(0.5f, 1.0f) };
	vertices[42] = { vec3(1.0f, 1.0f, -1.0f), vec2(0.625f, 0.75f) };
	vertices[43] = { vec3(bWidth, 1.0f, -1.0f), vec2(0.75f, 0.75f) };
	vertices[44] = { vec3(1.0, bWidth, -1.0f), vec2(0.625f, 1.0f) };
	vertices[45] = { vec3(1.0f, 1.0, -bWidth), vec2(0.625f, 0.5f) };
	vertices[46] = { vec3(1.0f, bWidth, -bWidth), vec2(0.5f, 0.75f) };
	vertices[47] = { vec3(bWidth, 1.0f, -bWidth), vec2(0.75f, 0.5f) };
	vertices[48] = { vec3(bWidth, bWidth, -1.0f), vec2(0.75f, 1.0f) };
	vertices[49] = { vec3(1.0, 1.0f, 1.0f), vec2(0.875f, 0.75f) };
	vertices[50] = { vec3(bWidth, 1.0f, 1.0f), vec2(1.0f, 0.75f) };
	vertices[51] = { vec3(1.0f, bWidth, 1.0f), vec2(0.875f, 1.0f) };
	vertices[52] = { vec3(1.0f, 1.0f, bWidth), vec2(0.875f, 0.5f) };
	vertices[53] = { vec3(1.0f, bWidth, bWidth), vec2(0.75f, 0.75f) };
	vertices[54] = { vec3(bWidth, 1.0f, bWidth), vec2(1.0f, 0.5f) };
	vertices[55] = { vec3(bWidth, bWidth, 1.0f), vec2(1.0f, 1.0f) };

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

static void CreateIndexBuffer()
{
	unsigned int indices[] = { 0, 3, 4,
							   4, 2, 0,
							   0, 1, 5,
							   5, 3, 0,
							   0, 2, 6,
							   6, 1, 0,
							   7, 9, 11,
							   11, 10, 7,
							   7, 10, 12,
							   12, 8, 7,
							   7, 8, 13,
							   13, 9, 7,
							   14, 16, 18,
							   18, 17, 14,
							   14, 17, 19,
							   19, 15, 14,
							   14, 15, 20,
							   20, 16, 14,
							   21, 24, 25,
							   25, 23, 21,
							   21, 22, 26,
							   26, 24, 21,
							   21, 23, 27,
							   27, 22, 21,
							   28, 30, 32,
							   32, 31, 28,
							   28, 31, 33,
							   33, 29, 28,
							   28, 29, 34,
							   34, 30, 28,
							   35, 38, 39,
							   39, 37, 35,
							   35, 36, 40,
							   40, 38, 35,
							   35, 37, 41,
							   41, 36, 35,
							   42, 45, 46,
							   46, 44, 42,
							   42, 43, 47,
							   47, 45, 42,
							   42, 44, 48,
							   48, 43, 42,
							   49, 51, 53,
							   53, 52, 49,
							   49, 52, 54,
							   54, 50, 49,
							   49, 50, 55,
							   55, 51, 49
	};

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
	}

	glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders()
{
	GLuint ShaderProgram = glCreateProgram();

	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	string vs, fs;

	if (!ReadFile(pVSFileName, vs)) {
		exit(1);
	}

	if (!ReadFile(pFSFileName, fs)) {
		exit(1);
	}

	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glUseProgram(ShaderProgram);

	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
	assert(gWorldLocation != 0xFFFFFFFF);
	controls = glGetUniformLocation(ShaderProgram, "rotateSide");
	assert(controls != 0xFFFFFFFF);
	gSampler = glGetUniformLocation(ShaderProgram, "gSampler");
	assert(gSampler != 0xFFFFFFFF);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Rubik's Cube");
	glutFullScreenToggle();

	InitializeGlutCallbacks();

	pCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);
	
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	printf("GL version: %s\n", glGetString(GL_VERSION));

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	CreateVertexBuffer();
	CreateIndexBuffer();

	CompileShaders();	

	glUniform1i(gSampler, 0);

	pTexture = new Texture(GL_TEXTURE_2D, "../Content/RubiksCube.png");

	if (!pTexture->Load()) {
		return 1;
	}


	persProjInfo.FOV = 90.0f;
	persProjInfo.Height = WINDOW_HEIGHT;
	persProjInfo.Width = WINDOW_WIDTH;
	persProjInfo.zNear = 0.1f;
	persProjInfo.zFar = 60.0f;


	glutMainLoop(); 

	return 0;
}

