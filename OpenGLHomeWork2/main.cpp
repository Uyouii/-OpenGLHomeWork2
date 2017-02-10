#include <glut.h>
#include "Star.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define RIGHT	0
#define LEFT	1
#define UP		2
#define DOWN	3

#define MOUSEBUFFERSIZE 5

Star Planet[NUMBER];
GLfloat planetAmbient[NUMBER][4];
GLfloat planetDiffuse[NUMBER][4];
GLfloat planetSpecular[NUMBER][4];
GLfloat planetEmission[NUMBER][4];
GLfloat noEmission[4];
GLfloat defaultShin = 30.0f;
GLfloat speedRate = 2.0f;			//公转速度设置
GLfloat rsRate = 0.3f;				//自转速度设置
GLfloat sizeRate = 0.5f;			//画面大小设置
GLfloat eyeAngle1 = 0.0f, eyeAngle2 = 0.0f;
GLfloat eyeR = 40000.0f;
GLfloat eyeX = 0.0f, eyeY = 0.0f, eyeZ = 40000.0f;
GLfloat eyeXV = 0.0f, eyeYV= 1.0f, eyeZV = 0.0f;
GLfloat eyeRStep = 50.0f;
int MouseX[MOUSEBUFFERSIZE], MouseY[MOUSEBUFFERSIZE], MouseBuffer = 0;
int Move[4];
bool Run = true;

void reshape(int , int );
void redraw();
void idle();
void Initial();					//初始化全局变量并设置各个行星的参数
void EnvironmentLight();		//设置环境光
template <class T> void SetValue(T* Str, T v1, T v2, T v3, T v4);
void ShowStar(int num);			//显示行星
void ShowSatellite(int satellite,int planet);		//显示卫星
void ActiveMotion(int, int);	//鼠标控制（按下键之后）
void SetCerma();				//重新设置相机位置
void GetCermaLocation();		//得到此时相机位置
void NormalKey(unsigned char key,int x,int y);	//普通按键回调函数
void SpecialKey(int key, int x, int y);			//特殊按键回调函数

int main(int argc, char *argv[])
{
 	Initial();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(1080, 720);
	int windowHandle = glutCreateWindow("Solar System");

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	//glutMouseFunc(MouseFunc);
	glutMotionFunc(ActiveMotion);
	glutKeyboardFunc(NormalKey);
	glutSpecialFunc(SpecialKey);

	glutMainLoop();

	return 0;
}

void reshape(int width, int height)
{
	if (height == 0)									
		height = 1;			

	glViewport(0, 0, width, height);					
	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();									

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100000.0f * sizeRate);

	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();								
}

//屏幕向里z轴负方向，右边x轴正方向，上边y轴正方向
void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();						// Reset The Current Modelview Matrix

	gluLookAt(eyeX * sizeRate , eyeY * sizeRate,eyeZ * sizeRate,					//相机在世界坐标的位置
		0.0f, 0.0f, 0.0f,															//相机对准的物体在世界坐标的位置
		0.0f, 1.0f, 0.0f);															//相机的向上的方向在

	EnvironmentLight();

	ShowStar(SUN);
	ShowStar(MERCURY);
	ShowStar(VENUS);
	ShowStar(EARTH);
	ShowSatellite(MOON,EARTH);
	ShowStar(MARS);
	ShowStar(JUPITER);
	ShowSatellite(LO,JUPITER);
	ShowSatellite(EUROPA, JUPITER);
	ShowSatellite(GANYMEDA, JUPITER);
	ShowSatellite(CALLISO, JUPITER);
	ShowStar(STATURN);
	ShowStar(URANUS);
	ShowStar(NEPTUNE);

	glutSwapBuffers();
}

void ShowStar(int num)
{
	Planet[num].Mertial(noEmission, planetAmbient[num], planetDiffuse[num], planetSpecular[num], defaultShin);
	Planet[num].DrawStar(Run);
	Planet[num].DrawOrbit();
	if (num == STATURN)
		Planet[num].DrawRing((690.0f + 320.0f)*sizeRate, 200.0f);
}

void ShowSatellite(int satellite,int planet)
{
	Planet[satellite].Mertial(noEmission, planetAmbient[satellite], planetDiffuse[satellite], planetSpecular[satellite], defaultShin);
	Planet[satellite].DrawStaellite(Planet[planet], Run);
	Planet[satellite].DrawSatelliteOrbit(Planet[planet]);
}

void ActiveMotion(int x, int y)
{
	if (MouseX == 0 || MouseY == 0)
	{
		MouseX[MouseBuffer] = x;
		MouseY[MouseBuffer] = y;
		return;
	}

	Move[RIGHT] = x - MouseX[MouseBuffer] - 4;
	Move[LEFT] = x - MouseX[MouseBuffer] + 4;
	Move[UP] = y - MouseY[MouseBuffer] - 4;
	Move[DOWN] = y - MouseY[MouseBuffer] + 4;

	//std::cout << x << " " << y << std::endl;

	MouseBuffer = (MouseBuffer + 1) % MOUSEBUFFERSIZE;

	MouseX[MouseBuffer] = x;
	MouseY[MouseBuffer] = y;
	SetCerma();
}

void SetCerma()
{
	if (Move[RIGHT] > 0)
	{
		eyeAngle1 += 2;
		while (eyeAngle1 >= 360)
			eyeAngle1 -= 360;
	}
	if (Move[LEFT] < 0)
	{
		eyeAngle1 -= 2;
		while (eyeAngle1 <= 360)
			eyeAngle1 += 360;
	}
	if (Move[UP] > 0)
	{
		if(eyeAngle2 <= 89)
			eyeAngle2 += 2;
	}
	if (Move[DOWN] < 0)
	{
		if(eyeAngle2 >= -89)
			eyeAngle2 -= 2;
	}
	GetCermaLocation();
}

void NormalKey(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'w':
		{
			if (eyeR >= 2500.0f * sizeRate)
				eyeR -= eyeR / 50;
		} break;
		case 's':
		{
			if (eyeR <= 80000.0f * sizeRate)
				eyeR += eyeR / 50;
		}break;
		case 'r':
		{
			Run = !Run;
		}break;
		default:
			break;
	}
	GetCermaLocation();
}

void SpecialKey(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_LEFT:
			eyeAngle1 -= 2; break;
		case GLUT_KEY_RIGHT:
			eyeAngle1 += 2; break;
		case GLUT_KEY_UP:
			if(eyeAngle2 <= 89)
				eyeAngle2 += 2;
			break;
		case GLUT_KEY_DOWN:
			if(eyeAngle2 >= -89)
				eyeAngle2 -= 2; 
			break;
		default:
			break;
	}

	GetCermaLocation();
}

void GetCermaLocation()
{
	eyeY = eyeR * sin(eyeAngle2 / 360 * 2 * M_PI);
	eyeZ = eyeR * cos(eyeAngle2 / 360 * 2 * M_PI) * cos(eyeAngle1 / 360 * 2 * M_PI);
	eyeX = eyeR * cos(eyeAngle2 / 360 * 2 * M_PI) * sin(eyeAngle1 / 360 * 2 * M_PI);
}

void idle()
{
	glutPostRedisplay();
}

void EnvironmentLight()
{
	// 光源1
	GLfloat sun_light_position1[] = { 0.0f, 20000.0f * sizeRate, 0.0f, 1.0f };
	GLfloat sun_light_ambient1[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat sun_light_diffuse1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat sun_light_specular1[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position1);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular1);

	// 光源2
	GLfloat sun_light_position2[] = { 0.0f, -20000.0f * sizeRate, 0.0f, 1.0f };
	GLfloat sun_light_ambient2[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat sun_light_diffuse2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat sun_light_specular2[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT1, GL_POSITION, sun_light_position2);
	glLightfv(GL_LIGHT1, GL_AMBIENT, sun_light_ambient2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, sun_light_diffuse2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, sun_light_specular2);

	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_COLOR_MATERIAL);
}

void Initial()
{
	SetValue(noEmission, 0.0f, 0.0f, 0.0f, 1.0f);

	Planet[SUN] = Star(SUN, 2000.0f*sizeRate, 12.56f / rsRate, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	SetValue(planetAmbient[SUN], 0.8f, 0.3f, 0.0f, 1.0f);
	SetValue(planetDiffuse[SUN], 0.8f, 0.3f, 0.0f, 1.0f);
	SetValue(planetSpecular[SUN], 0.8f, 0.3f, 0.0f, 1.0f);

	Planet[MERCURY] = Star(MERCURY, 58.0*sizeRate, 29.32f / rsRate, 0, 4257.0f*sizeRate, 2.38f / speedRate, 7.0f, 45);
	SetValue(planetAmbient[MERCURY], 0.0f, 0.8f, 0.5f, 1.0f);
	SetValue(planetDiffuse[MERCURY], 0.0f, 0.8f, 0.5f, 1.0f);
	SetValue(planetSpecular[MERCURY], 0.0f, 0.8f, 0.5f, 1.0f);

	Planet[VENUS] = Star(VENUS, 146.0f * sizeRate, -121.5f / rsRate, 0, 4980.0f*sizeRate, 4.62f / speedRate, 3.4f, 60);
	SetValue(planetAmbient[VENUS], 0.8f, 0.8f, 0.0f, 1.0f);
	SetValue(planetDiffuse[VENUS], 0.8f, 0.8f, 0.0f, 1.0f);
	SetValue(planetSpecular[VENUS], 0.8f, 0.8f, 0.0f, 1.0f);

	Planet[EARTH] = Star(EARTH, 154.0f*sizeRate, 0.5f / rsRate, 0, 5664.0f*sizeRate, 7.5 / speedRate, 0.0f, 75);
	SetValue(planetAmbient[EARTH], 0.1f, 0.5f, 0.5f, 1.0f);
	SetValue(planetDiffuse[EARTH], 0.1f, 0.5f, 0.5f, 1.0f);
	SetValue(planetSpecular[EARTH], 0.1f, 0.5f, 0.5f, 1.0f);

	Planet[MARS] = Star(MARS, 82.0f*sizeRate, 0.52 / rsRate, 0, 6412.0f*sizeRate, 14.1 / speedRate, 1.85f, 90);
	SetValue(planetAmbient[MARS], 0.8f, 0.2f, 0.0f, 1.0f);
	SetValue(planetDiffuse[MARS], 0.8f, 0.2f, 0.0f, 1.0f);
	SetValue(planetSpecular[MARS], 0.8f, 0.8f, 0.0f, 1.0f);

	Planet[JUPITER] = Star(JUPITER, 750.0f*sizeRate, 0.27f / rsRate, 0, 9000.0f*sizeRate, 58.96 / speedRate, 1.3f, 120);
	SetValue(planetAmbient[JUPITER], 0.0f, 0.5f, 0.0f, 1.0f);
	SetValue(planetDiffuse[JUPITER], 0.0f, 0.5f, 0.0f, 1.0f);
	SetValue(planetSpecular[JUPITER], 0.0f, 0.5f, 0.0f, 1.0f);

	Planet[STATURN] = Star(STATURN, 690.0f*sizeRate, 0.23f / rsRate, 26.73f, 13000.0f*sizeRate, 224.2 / speedRate, 2.5f, 150);
	SetValue(planetAmbient[STATURN], 0.5f, 0.4f, 0.2f, 1.0f);
	SetValue(planetDiffuse[STATURN], 0.5f, 0.4f, 0.2f, 1.0f);
	SetValue(planetSpecular[STATURN], 0.5f, 0.4f, 0.2f, 1.0f);

	Planet[URANUS] = Star(URANUS, 309.0f*sizeRate, -0.36f / rsRate, 0, 20000.0f*sizeRate, 632.41 / speedRate, 0.77f, 200);
	SetValue(planetAmbient[URANUS], 0.4f, 0.1f, 0.5f, 1.0f);
	SetValue(planetDiffuse[URANUS], 0.4f, 0.1f, 0.5f, 1.0f);
	SetValue(planetSpecular[URANUS], 0.4f, 0.1f, 0.5f, 1.0f);

	Planet[NEPTUNE] = Star(NEPTUNE, 300.0f*sizeRate, 0.3f / rsRate, 0, 30000.0f*sizeRate, 1238.73 / speedRate, 1.77f, 0);
	SetValue(planetAmbient[NEPTUNE], 0.0f, 0.0f, 0.4f, 1.0f);
	SetValue(planetDiffuse[NEPTUNE], 0.0f, 0.0f, 0.5f, 1.0f);
	SetValue(planetSpecular[NEPTUNE], 0.0f, 0.0f, 0.5f, 1.0f);

	Planet[MOON] = Star(MOON, 41.94f *sizeRate, 13.65f / rsRate, 0.0f, 280.0f*sizeRate, 0.63f / speedRate, 0.0f, 0.0f);
	SetValue(planetAmbient[MOON], 0.8f, 0.8f, 0.7f, 1.0f);
	SetValue(planetDiffuse[MOON], 0.8f, 0.8f, 0.7f, 1.0f);
	SetValue(planetSpecular[MOON], 0.8f, 0.8f, 0.7f, 1.0f);

	Planet[LO] = Star(LO, 88.37f*sizeRate, 0.0f / rsRate, 0.0f, 1000.0f*sizeRate, 1.77f / speedRate, 0.05f, 0.0f);
	SetValue(planetAmbient[LO], 0.8f, 0.4f, 0.1f, 1.0f);
	SetValue(planetDiffuse[LO], 0.8f, 0.4f, 0.1f, 1.0f);
	SetValue(planetSpecular[LO], 0.8f, 0.4f, 0.1f, 1.0f);

	Planet[EUROPA] = Star(EUROPA, 75.65f*sizeRate, 0.0f / rsRate, 0.0f, 1200.0f*sizeRate, 3.55f / speedRate, 0.47f, 60.0f);
	SetValue(planetAmbient[EUROPA], 0.8f, 0.8f, 0.6f, 1.0f);
	SetValue(planetDiffuse[EUROPA], 0.8f, 0.8f, 0.6f, 1.0f);
	SetValue(planetSpecular[EUROPA], 0.8f, 0.8f, 0.6f, 1.0f);

	Planet[GANYMEDA] = Star(GANYMEDA, 127.06f*sizeRate, 0.0f / rsRate, 0.0f, 1450.0f*sizeRate, 7.15f / speedRate, 0.204f, 150.0f);
	SetValue(planetAmbient[GANYMEDA], 0.9f, 0.9f, 0.9f, 1.0f);
	SetValue(planetDiffuse[GANYMEDA], 0.9f, 0.9f, 0.9f, 1.0f);
	SetValue(planetSpecular[GANYMEDA], 0.9f, 0.9f, 0.9f, 1.0f);

	Planet[CALLISO] = Star(CALLISO, 116.39f*sizeRate, 0.0f / rsRate, 0.0f, 1800.0f*sizeRate, 16.69f / speedRate, 0.205f,240.0f);
	SetValue(planetAmbient[CALLISO], 0.4f, 0.3f, 0.05f, 1.0f);
	SetValue(planetDiffuse[CALLISO], 0.4f, 0.3f, 0.05f, 1.0f);
	SetValue(planetSpecular[CALLISO], 0.4f, 0.3f, 0.05f, 1.0f);


	SetValue(Move, 0, 0, 0, 0);
	for (int i = 0; i < MOUSEBUFFERSIZE; i++)
	{
		MouseX[i] = 0;
		MouseY[i] = 0;
	}

}

template <class T>
void SetValue(T* Str, T v1, T v2, T v3, T v4)
{
	Str[0] = v1;
	Str[1] = v2;
	Str[2] = v3;
	Str[3] = v4;
}
