#ifndef _SHAPE_
#define _SHAPE_
#include<algorithm>
void DrawLine(int x1,int y1,int x2,int y2);
double Max(double a, double b)
{
	return a > b ? a : b;
}

class Shape
{
protected:
	char r, g, b;
	int width;
public:
	virtual void Draw() = 0;
	void SetColor(char R, char G, char B)
	{
		r = R; g = G; b = B;
	}
	void SetWidth(int w) { width = w; }
};


class Circle:public Shape							//------[ CLASS_CIRCLE ]------
{
	int h, k;
	int radius;
//ansillary
	double CalculateEpsilon(int r);
	void _circle(int);
public:
//constructors
	Circle()
	{
		h = k = radius = 0;
	}
	Circle(int h, int k, int r)
	{
		this->h = h;
		this->k = k;
		radius = r;
	}
//overridden methods
	void Draw();
};

class RectAngle:public Shape							//------[ CLASS_RECTANGLE ]------
{
	int topLeft_X, topLeft_Y;
	int bottomRight_X, bottomRight_Y;
	void _rectangle();
public:
//constructor
	RectAngle()
	{
		topLeft_X = topLeft_Y = bottomRight_X = bottomRight_Y = 0;
	}
	RectAngle(int x1,int y1,int x2,int y2)
	{
		topLeft_X = x1;
		topLeft_Y = y1;
		bottomRight_X = x2;
		bottomRight_Y = y2;
	}
//overridden methods
	void Draw();
};

class Line :public Shape							//------[ CLASS_LINE ]------
{
	int x1, y1;
	int x2, y2;
public:
	Line()
	{
		x1 = x2 = y1 = y2 = 0;
	}
	Line(int x1,int y1,int x2,int y2)
	{
		this->x1 = x1;
		this->y1 = y1;
		this->x2 = x2;
		this->y2 = y2;
	}
//overridden methods
	void Draw();
};

//-----------------------------------------------------------------------------------------

double Circle::CalculateEpsilon(int r)							//_____epsilon_____
{
	int n;
	for (n = 0; pow(2, n) < r; n++);
	return (double)1 / pow(2, n);
}

void Circle::Draw()												//-----CIRCLE_DRAW-----
{
	glColor3ub(r,b,g);
	_circle(radius);	
}
void Circle::_circle(int r)
{
	//std::cerr << "~";
	double x = r,
		   y = 0;
	double eps = CalculateEpsilon(r);
	//cerr << "eps: " << eps << endl;

	while ((y - 0) < eps || (r - x) > eps)
	{
		glBegin(GL_POINTS);
		glVertex2f(x + h, y + k);
		glEnd();
		//cerr << "(" << x +h<< "," << y+k << ")" << endl;
		x += y * eps;
		y -= x * eps;
	}
}
void RectAngle::Draw()										//-----RECTANGLE_DRAW-----
{
	glColor3ub(r, b, g);
	_rectangle();
	
}

void RectAngle::_rectangle()							//_____rectangle_____
{
	DrawLine(topLeft_X, topLeft_Y, topLeft_X, bottomRight_Y);
	DrawLine(topLeft_X, topLeft_Y, bottomRight_X, topLeft_Y);
	DrawLine(bottomRight_X, bottomRight_Y, topLeft_X, bottomRight_Y);
	DrawLine(bottomRight_X, bottomRight_Y, bottomRight_X, topLeft_Y);
}

void Line::Draw()											//-----LINE_DRAW-----
{
	glColor3ub(r, b, g);

	DrawLine(x1,y1,x2,y2);
}

//-----------------------------------------------------------------------------------------------
void DrawLine(int x1, int y1, int x2, int y2)				//-----DDA-----
{
	double dx = x2 - x1,
		   dy = y2 - y1;
	int step = Max(abs(dx), abs(dy));
	double xinc = dx / step,
		   yinc = dy / step;
	double x = x1,
		   y = y1;

	while (step--)
	{
		x += xinc;
		y += yinc;
		glBegin(GL_POINTS);
			glVertex2d(x, y);
		glEnd();
	}
}

#endif