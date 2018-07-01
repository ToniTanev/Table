#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <matrix_transform.hpp>
#include <type_ptr.hpp>
#include <matrix_inverse.hpp>

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm> //std::max

using std::istream;
using std::ostream;

#define OUT  //mark out parameters

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const float pi = 3.1415f;

struct Point
{
	float x;
	float y;
	float z;

	Point() { x = y = z = 0.0; }
	Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

typedef enum
{
	RECTANGLE,
	OVAL,
	CIRCLE,
	TRIANGLE,
	SQUARE
}Shape;
istream& operator >> (istream& is, Shape& shape);
ostream& operator << (ostream& os, Shape& shape);

class PlotShape;
class LegShape;

void init();
void createWindow(OUT GLFWwindow*& window);
void createShaderProgram(OUT int& shaderProgram);
void configureBuffersAndDraw(float* _vertices, size_t verticesSize, unsigned int* _indices, size_t indicesSize);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void input(OUT PlotShape*& plot, OUT LegShape*& leg);
void render(GLFWwindow* window, int shaderProgram);
void end();

void drawTetragon(Point p1, Point p2, Point p3, Point p4);
std::vector<Point> drawParallelepiped(float width, float length, float height, Point center = Point(0, 0, 0));
std::vector<Point> drawPartialCircle(float r, Point center = Point(0, 0, 0), float drawAngle = 2 * pi, float startAngle = 0.0);
std::vector<Point> drawOval(float width, float length, Point center = Point(0, 0, 0));
std::vector<Point> drawOvalPlot(float width, float length, float height, Point center = Point(0, 0, 0));
std::vector<Point> drawCylinder(float radius, float height, Point center = Point(0, 0, 0));
void drawTable(PlotShape& plot, LegShape& leg);

class PlotShape
{
protected:
	float width;
	float length;
	float height;
	Point center;
public:
	virtual Shape getShape() const = 0;
	virtual float getWidth() const = 0;
	virtual float getLength() const = 0;
	virtual float getHeight() const = 0;
	virtual Point getCenter() const = 0;
	virtual void draw() const = 0;
};

class RectPlot : public PlotShape
{
public:
	RectPlot(float _width, float _length, float _height, Point _center = Point(0, 0, 0))
	{
		width = _width;
		length = _length;
		height = _height;
		center = _center;
	}
	Shape getShape() const { return RECTANGLE; }
	float getWidth() const { return width; }
	float getLength() const { return length; }
	float getHeight() const { return height; }
	Point getCenter() const { return center; }
	void draw() const { drawParallelepiped(width, length, height, center); }
};

class OvalPlot : public PlotShape
{
public:
	OvalPlot(float _width, float _length, float _height, Point _center = Point(0, 0, 0))
	{
		width = _width;
		length = _length;
		if (width > 1.3*length)
			width = 1.3*length;
		height = _height;
		center = _center;
	}
	Shape getShape() const { return OVAL; }
	float getWidth() const { return width; }
	float getLength() const { return length; }
	float getHeight() const { return height; }
	Point getCenter() const { return center; }
	void draw() const { drawOvalPlot(width, length, height, center); }
};

class LegShape
{
protected:
	float height;
	Point center;
public:
	virtual float getHeight() const = 0;
	virtual Shape getShape() const = 0;
	virtual float maxDist() const = 0;
	virtual void setCenter(Point) = 0;
	virtual void draw() const = 0;
};

class RectLeg : public LegShape
{
private:
	float width;
	float length;
public:
	RectLeg(float _width, float _length, float _height, Point _center = Point(0, 0, 0))
	{
		width = _width;
		length = _length;
		height = _height;
		center = _center;
	}
	float getHeight() const { return height; }
	Shape getShape() const { return RECTANGLE; }
	float maxDist() const {
		return //sqrt(pow((width / 2), 2) + pow((length / 2), 2));}
			std::max(width / 2, length / 2);
	}
	void setCenter(Point _center) { center = _center; }
	void draw() const { drawParallelepiped(width, length, height, center); }
};

class CircleLeg : public LegShape
{
private:
	float radius;
public:
	CircleLeg(float _radius, float _height, Point _center = Point(0, 0, 0))
	{
		radius = _radius;
		height = _height;
		center = _center;
	}
	float getHeight() const { return height; }
	Shape getShape() const { return CIRCLE; }
	float maxDist() const { return radius; }
	void setCenter(Point _center) { center = _center; }
	void draw() const { drawCylinder(radius, height, center); }
};