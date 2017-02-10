#include"Star.h"
#define _USE_MATH_DEFINES
#include <math.h>

Star::Star(int ID, double radius, double rotation_period, double rotation_angle,
	double revolution_radius, double revolution_period, double revolution_angle,double angle)
{
	Star();
	this->ID = ID;
	this->radius = radius;
	this->rotation_period = rotation_period;
	this->rotation_angle = rotation_angle;
	this->revolution_radius = revolution_radius;
	this->revolution_period = revolution_period;
	this->revolution_angle = revolution_angle;
	this->angle = angle;
}

Star::Star()
{
	SetValue(rotaVector[SUN], 0.0f, 0.0f, -1.0f);	
	SetValue(rotaVector[MERCURY], 1.0f, 0.0f, 1.0f);
	SetValue(rotaVector[VENUS], -1.0f, 0.0f, 1.0f);
	SetValue(rotaVector[EARTH], 2.0f, 0.0f, -1.0f);
	SetValue(rotaVector[MARS], -1.0f, 0.0f, -2.0f);
	SetValue(rotaVector[JUPITER], 3.0f, 0.0f, 2.0f);
	SetValue(rotaVector[STATURN], -2.0f, 0.0f, 3.0f);
	SetValue(rotaVector[URANUS], 1.0f, 0.0f, -2.0f);
	SetValue(rotaVector[NEPTUNE], -2.0f, 0.0f, -1.0f);
	SetValue(rotaVector[MOON], -2.0f, 0.0f, -1.0f);
	SetValue(rotaVector[LO], -1.0f, 0.0f, 1.0f);
	SetValue(rotaVector[EUROPA], 2.0f, 0.0f, -1.0f);
	SetValue(rotaVector[GANYMEDA], 3.0f, 0.0f, 2.0f);
	SetValue(rotaVector[CALLISO], -2.0f, 0.0f, 3.0f);

}

void Star::Mertial(GLfloat* emission,GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat shininess)
{
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);			//自己发光
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);			//环境光
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);			//漫反射
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);			//镜面反射
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);			//镜面指数
}

void Star::DrawStar(bool run)
{
	if (run)
	{
		//计算公转角度
		if (this->revolution_period != 0)
			this->angle += (float)(1 / this->revolution_period);
		while (this->angle >= 360)
			this->angle = this->angle - 360;

		//计算自转角度
		bool flag = (rotation_period < 0) ? false : true;
		if (this->rotation_period != 0)
			this->rotationAngle += (float)(1 / this->rotation_period * flag);
		while (this->rotationAngle >= 360)
			this->rotationAngle = this->rotationAngle - 360;
	}

	glPushMatrix();

	//公转轨道倾角
	glRotatef(this->revolution_angle, rotaVector[this->ID][0], rotaVector[this->ID][1], rotaVector[this->ID][2]);
	glRotatef(angle, 0.0f, 1.0f, 0.0f);						//公转
	glTranslatef(this->revolution_radius, 0.0f, 0.0f);		//公转半径
	//glRotatef(rotationAngle, 0.0f, 1.0f * flag, 0.0f);	//自转
	glutSolidSphere(this->radius, 80.0f, 80.0f);
	
	glPopMatrix();

}

//绘制卫星时必须在行星后面，因为要用到行星的参数
void Star::DrawStaellite(Star& Planet,bool run)
{
	if (run)
	{
		//计算卫星公转角度
		if (this->revolution_period != 0)
			this->angle += (float)(1 / this->revolution_period);
		while (this->angle >= 360)
			this->angle = this->angle - 360;

		//计算卫星自转角度
		bool flag = (rotation_period < 0) ? false : true;
		if (this->rotation_period != 0)
			this->rotationAngle += (float)(1 / this->rotation_period * flag);
		while (this->rotationAngle >= 360)
			this->rotationAngle = this->rotationAngle - 360;
	}

	glPushMatrix();

	//行星公转轨道倾角
	glRotatef(Planet.revolution_angle, rotaVector[Planet.ID][0], rotaVector[Planet.ID][1], rotaVector[Planet.ID][2]);
	glRotatef(Planet.angle, 0.0f, 1.0f, 0.0f);						//行星公转
	glTranslatef(Planet.revolution_radius, 0.0f, 0.0f);				//行星公转半径
	//glRotatef(rotationAngle, 0.0f, 1.0f * flag, 0.0f);			//行星自转

	//卫星公转轨道倾角
	glRotatef(this->revolution_angle, rotaVector[this->ID][0], rotaVector[this->ID][1], rotaVector[this->ID][2]);
	glRotatef(angle, 0.0f, 1.0f, 0.0f);								//卫星公转
	glTranslatef(this->revolution_radius, 0.0f, 0.0f);				//卫星公转半径
	glutSolidSphere(this->radius, 80.0f, 80.0f);

	glPopMatrix();
}

//绘制公转轨道
void Star::DrawOrbit()
{                  
	glPushMatrix();
	//公转轨道倾角
	glRotatef(this->revolution_angle, rotaVector[this->ID][0], rotaVector[this->ID][1], rotaVector[this->ID][2]);
	glDisable(GL_LIGHTING);
	glColor3f(1.0f,1.0f, 1.0f);                      //设置线段绘制颜色  
	GLfloat r = this->revolution_radius;
	GLfloat x1, z1, x2, z2, y = 0;
	int NUM = (((int)r / 8000) + 1 ) * 360;
	for (int i = 0; i < NUM; i += 2)
	{
		x1 = cos(1.0 * i / NUM * 2 * M_PI) * r;
		z1 = sin(1.0 * i / NUM * 2 * M_PI) * r;
		x2 = cos(1.0 * (i+1) /NUM * 2 * M_PI) * r;
		z2 = sin(1.0 * (i+1) / NUM * 2 * M_PI) * r;
		glBegin(GL_LINES);
		glVertex3f(x1, y, z1);
		glVertex3f(x2, y, z2);
		glEnd();
	}
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

void Star::DrawSatelliteOrbit(Star& Planet)
{
	glPushMatrix();

	//行星公转轨道倾角
	glRotatef(Planet.revolution_angle, rotaVector[Planet.ID][0], rotaVector[Planet.ID][1], rotaVector[Planet.ID][2]);
	glRotatef(Planet.angle, 0.0f, 1.0f, 0.0f);						//行星公转
	glTranslatef(Planet.revolution_radius, 0.0f, 0.0f);				//行星公转半径

	//公转轨道倾角
	glRotatef(this->revolution_angle, rotaVector[this->ID][0], rotaVector[this->ID][1], rotaVector[this->ID][2]);
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);                      //设置线段绘制颜色  
	GLfloat r = this->revolution_radius;
	GLfloat x1, z1, x2, z2, y = 0;
	int NUM = (((int)r / 8000) + 1) * 360;
	for (int i = 0; i < NUM; i += 2)
	{
		x1 = cos(1.0 * i / NUM * 2 * M_PI) * r;
		z1 = sin(1.0 * i / NUM * 2 * M_PI) * r;
		x2 = cos(1.0 * (i + 1) / NUM * 2 * M_PI) * r;
		z2 = sin(1.0 * (i + 1) / NUM * 2 * M_PI) * r;
		glBegin(GL_LINES);
		glVertex3f(x1, y, z1);
		glVertex3f(x2, y, z2);
		glEnd();
	}
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

void Star::SetValue(GLfloat* Str, GLfloat v1, GLfloat v2, GLfloat v3)
{
	Str[0] = v1;
	Str[1] = v2;
	Str[2] = v3;
}

void Star::DrawRing(GLfloat R, GLfloat Width)
{
	glPushMatrix();
	//公转轨道倾角
	glRotatef(this->revolution_angle, rotaVector[this->ID][0], rotaVector[this->ID][1], rotaVector[this->ID][2]);
	glRotatef(angle, 0.0f, 1.0f, 0.0f);						//公转
	glTranslatef(this->revolution_radius, 0.0f, 0.0f);		//公转半径

	//轨道倾角
	glRotatef(this->rotation_angle, rotaVector[this->ID][0], rotaVector[this->ID][1], rotaVector[this->ID][2]);
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);                      //设置线段绘制颜色  
	GLfloat r = R;
	GLfloat x1, z1, x2, z2, y = 0;
	int NUM = (((int)r / 8000) + 1) * 180;
	for (; r < R + Width; r += 10.0f) {
		for (int i = 0; i < NUM; i += 2)
		{
			x1 = cos(1.0 * i / NUM * 2 * M_PI) * r;
			z1 = sin(1.0 * i / NUM * 2 * M_PI) * r;
			x2 = cos(1.0 * (i + 1) / NUM * 2 * M_PI) * r;
			z2 = sin(1.0 * (i + 1) / NUM * 2 * M_PI) * r;
			glBegin(GL_LINES);
			glVertex3f(x1, y, z1);
			glVertex3f(x2, y, z2);
			glEnd();
		}
	}
	glEnable(GL_LIGHTING);

	glPopMatrix();
}