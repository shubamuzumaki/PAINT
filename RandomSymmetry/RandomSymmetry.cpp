#include "pch.h"
#include<gl/freeglut.h>
#include<iostream>
#include<vector>
#include "shape.h"

using namespace std;

//global variables
int width = 600;
int height = 600;

//color
char r, g, b;

//state
bool isLeftPressed = false;
bool isShapeComplete = false;

//tools
bool isLineToolActive = false;
bool isRectangleToolActive = false;
bool isCircleToolActive = false;
	bool diameterTool = false;

//GLut display
void init2D();
void display();

//ansillary
void clear();
void Undo();
void Redo();
void SetColor(GLubyte, GLubyte, GLubyte);
void ToggleTools(bool&);

//menu
void MainMenu(int);
void ToolSelectorMenu(int);
void ColorSelectorMenu(int);

//controller
void mouseFunc(int, int, int, int);
void motionFunc(int,int);
void RecordPoints(int,int);
void keyboard(unsigned char, int, int);

//drawing board
vector<Shape*> canvas;
vector<Shape*> redo;

void reshape(int w, int h)
{
	
	width = w;
	height = h;
}


//----------------------------[ CONTROLLER _START]------------------------------------------
void mouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			isLeftPressed = true;
			isShapeComplete = false;
			RecordPoints(x,-y+height);
		}
		else if (state == GLUT_UP)
		{
			
			isShapeComplete = true;//record the shape
			RecordPoints(x, -y + height);

			isLeftPressed = false;
			RecordPoints(-1,-1);//reset the function
		}
	}
}
void motionFunc(int x, int y)
{
	if (isLeftPressed)
	{
		RecordPoints(x,-y+height);
	}
}

void keyboard(unsigned char key, int x, int y)				//-----KEYBOARD-----
{
	switch (key)
	{
	case 'd'://clear the screen
		glClear(GL_COLOR_BUFFER_BIT);						//clear the buffer of the screen
		glFlush();
		canvas.clear();
		break;
	case 's'://set the color
		cout << "ENTER COLOR" << endl;
		double r, b, g;
		cin >> r >> b >> g;
		//SetColor(r, b, g);
		break;

	case 'l'://toggle LineTool on/off
		ToggleTools(isLineToolActive);
		break;

	case 'r'://toggle Rectangle tool
		ToggleTools(isRectangleToolActive);
		break;

	case 'c'://toggle Circle tool
		ToggleTools(isCircleToolActive);
		break;
	case 8://backspace
		Undo();
		break;
	case 13://enter
		Redo();
		break;
	case '1'://multifunction
		if (isCircleToolActive)
		{
			diameterTool = !diameterTool;
			cout << "DIAMETER CIRCLE: " << diameterTool << endl;
		}
		break;
	}
}
//----------------------------[ CONTROLLER_END]------------------------------------------

void RecordPoints(int x, int y)													//-----RECORD POINTS-----
{
	static int count = 0;
	if (x == -1 && y == -1)//lifting the pen up
	{
		count = 0;
		return;
	}

	static int x_old, y_old;
	Shape *c = NULL;
	if (count == 0)
	{
		x_old = x;
		y_old = y;
		count++;
	}
	else
	{
		clear();//clear the canvas before drawing anything

		if (isCircleToolActive)
		{
			double radius = sqrt((x - x_old)*(x - x_old) + (y - y_old)*(y - y_old));
			c = new Circle(x_old, y_old, radius);
		}
		else if (isRectangleToolActive)
		{
			c = new RectAngle(x_old,y_old,x,y);
		}
		else if (isLineToolActive)
		{
			c = new Line(x_old, y_old, x, y);
		}
		if (c)
		{
			c->SetColor(r,g,b);
			c->Draw();
		}
		if (isShapeComplete && c)
		{
			//cout << "pushed" << endl;
			cout << canvas.size() << endl;
			canvas.push_back(c);
			redo.clear();
		}
		if (c && !isShapeComplete)
		{
			delete c;
		}
	}

	
}

//----------------------------[ ANSILLARY_START]------------------------------------------

void clear()										//-----CLEAR-----
{
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < canvas.size(); i++)
	{
		canvas[i]->Draw();
	}
}

void ToggleTools(bool &current)							    //ToggleTools
{
	//system("clear");
	system("CLS");
	bool temp = current;
	isCircleToolActive = isRectangleToolActive  = isLineToolActive = false;
	current = !temp;

	cout << "LINETOOL : " << isLineToolActive << endl;
	cout << "RECTANGLE: " << isRectangleToolActive << endl;
	cout << "CIRCLE   : " << isCircleToolActive << endl;
}


void Undo()													//UNDO
{
	if (canvas.size() == 0) return;
	redo.push_back(canvas[canvas.size()-1]);
	
	canvas.pop_back();
	clear();
}

void Redo()													//redo
{
	if (redo.size() == 0)  return;
	canvas.push_back(redo[redo.size()-1]);
	redo.pop_back();
	clear();
}
void SetColor(GLubyte R, GLubyte B, GLubyte G)			    //SetColor
{
	r = R;
	b = B;
	g = G;
	//glColor3ub(r, b, g);
}
//----------------------------[ ANSILLARY_END]------------------------------------------

//----------------------------[ MENU_START]------------------------------------------

void MainMenu(int  choice)					    //-----Simple_MENU-----
{
	switch (choice)
	{
	case 1:
		Undo();
		break;
	case 2:
		Redo();
		break;
	case 3://EXIT
		exit(0);
		break;
	}
}
void ToolSelectorMenu(int choice)				//-----Tool_Selector_Menu-----
{
	switch (choice)
	{
	case 1:
		ToggleTools(isLineToolActive);
		break;
	case 2:
		ToggleTools(isRectangleToolActive);
		break;
	case 3:
		ToggleTools(isCircleToolActive);
		break;
	}
}

void ColorSelectorMenu(int choice)					//-----ColorSelector-----
{
	switch (choice)
	{
	case 1://RED
		SetColor(255, 0, 0);
		break;
	case 2://BLUE
		SetColor(0, 0, 255);
		break;
	case 3://BLACK
		SetColor(0, 0, 0);
		break;
	case 4://WHITE
		SetColor(255, 255, 255);
		break;
	case 5://YELLOW
		SetColor(255, 255, 0);
		break;
	case 6://GREEN
		SetColor(0, 128, 0);
		break;
	case 7://SPRINGGREEN
		SetColor(0, 255, 127);
		break;
	case 8://DEEPPINK
		SetColor(255, 20, 147);
		break;
	case 9://LAWNGREEN
		SetColor(124, 252, 0);
		break;
	case 10://AQUAMARINE
		SetColor(127, 255, 212);
		break;
	}
}


//----------------------------[ MENU_END]------------------------------------------

void CreateMyMenu()
{
	//Creating submenus
	int subMenuTool = glutCreateMenu(ToolSelectorMenu);	    //Tool_selctor_tool
	//glutAddMenuEntry("PEN TOOL", 1);
	glutAddMenuEntry("LINE TOOL", 1);
	glutAddMenuEntry("RECTANGLE TOOL", 2);
	glutAddMenuEntry("CIRCLE TOOL", 3);

	int subMenuColor = glutCreateMenu(ColorSelectorMenu);	//Color_Menu
	glutAddMenuEntry("RED", 1);
	glutAddMenuEntry("BLUE", 2);
	glutAddMenuEntry("BLACK", 3);
	glutAddMenuEntry("WHITE", 4);
	glutAddMenuEntry("YELLOW", 5);
	glutAddMenuEntry("GREEN", 6);
	glutAddMenuEntry("SPRING_GREEN", 7);
	glutAddMenuEntry("DEEP_PINK", 8);
	glutAddMenuEntry("LAWNGREEN", 9);
	glutAddMenuEntry("AQUAMARINE", 10);


	//Creating Main Menu
	glutCreateMenu(MainMenu);							//Tool_selctor_tool
	glutAddSubMenu("TOOLS", subMenuTool);
	glutAddSubMenu("COLORS", subMenuColor);
	glutAddMenuEntry("UNDO",1);
	glutAddMenuEntry("REDO",2);
	glutAddMenuEntry("EXIT", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

void init2D()													//-----INIT_2D-----
{
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, width, 0, height);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3ub(255, 0, 0);
}

void display()
{
	//cout << "hello" << endl;
	glFlush();
}

int main(int argc, char *argv[])									//======[ MAIN ]=====
{
	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("RADIAL");
	init2D();

	CreateMyMenu();
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}