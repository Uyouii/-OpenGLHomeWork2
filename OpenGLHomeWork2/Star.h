#ifndef STAR_H
#define STAR_H
#include<string>
#include<glut.h>
#include<iostream>

#define NUMBER		14
#define SUN			0
#define MERCURY		1
#define VENUS		2
#define EARTH		3
#define MARS		4
#define JUPITER		5
#define STATURN		6
#define URANUS		7
#define NEPTUNE		8
#define MOON		9
#define LO			10
#define EUROPA		11
#define GANYMEDA	12
#define CALLISO		13


class Star {
private:
	int ID;
	double radius;				//半径
	double rotation_period;		
	double rotation_angle;
	double revolution_radius;	
	double revolution_period;
	double revolution_angle;
	double angle = 0;
	double rotationAngle = 0;

public:
	GLfloat rotaVector[NUMBER][3];
public:
	//名称，半径，自转周期，自转角度，公转半径，公转周期，公转角度
	Star(int, double, double, double, double, double, double,double);
	Star();
	//设置行星的材质
	void Mertial(GLfloat*, GLfloat*, GLfloat*, GLfloat*, GLfloat);

	void DrawStar(bool run);						//绘制行星
	void DrawStaellite(Star& Planet,bool run);		//绘制卫星
	void DrawOrbit();								//绘制轨道
	void DrawSatelliteOrbit(Star& Planet);			//绘制卫星轨道
	void SetValue(GLfloat* , GLfloat , GLfloat , GLfloat );

	void DrawRing(GLfloat R,GLfloat Width);			//绘制行星环
};

#endif // !STAR_H
