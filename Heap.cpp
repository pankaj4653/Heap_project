#include <OpenGL/gl.h> 
 #include <OpenGL/glu.h>
 #include <GLUT/glut.h>
#include <stdio.h>                   
//#include<GLUT/glut.h>
//#include<GL/glut.h>
#include <stdlib.h>
#include <math.h> 
#include<string>
#include<sstream>
#include<cstring>
#include<string.h>
#include<unistd.h>

using namespace std;
typedef struct treenode {
	int data, id;
	struct treenode* left;
	struct treenode* right;
}TREENODE;
int p = 0, h[100];
TREENODE* root = NULL;
int globalcount = 0, globalcounti;
void myparent(TREENODE* tree, int myid, TREENODE** parent)
{
	if (tree->id == (myid / 2))
		*parent = tree;
	if (tree->left)
		myparent(tree->left, myid, parent);
	if (tree->right)
		myparent(tree->right, myid, parent);

}
void add_node(TREENODE** t_root, TREENODE* current) {
	TREENODE* parent;
	if (current->id == 1) {
		*t_root = current;
	}
	else
	{
		myparent(*t_root, current->id, &parent);
		if ((current->id) % 2)
		{
			parent->right = current;
		}
		else
			parent->left = current;
	}
}
void maxheapify(int vec[],int i);



int deleteh(int vec[])
{
	int temp=vec[0];
	

	vec[0]=vec[p-1];
	p=p-1;
	

	maxheapify(vec,0);

	return temp;
}

void maxheapify(int vec[],int i)
{
	int l=(2*i)+1;
	int r=(2*i)+2;
	int largest;

	int s=p;

	if(l<s && vec[l] > vec[i])
	{
		largest=l;
	}
	else
		largest=i;

	if(r<s && vec[r] > vec[largest])
	{
		largest=r;
	}

	if(largest!=i)
	{
		int temp;
		temp=vec[largest];
		vec[largest]=vec[i];
		vec[i]=temp;

		maxheapify(vec,largest);
	}

	return;

}

void heapsort()
{
	printf("heap sort : ");
	int t=p;
	while(t--)
	{
		int x=deleteh(h);
		printf("%d  ",x);
	}
}
void insert(int element)
{
	int childpo, parentpo;
	childpo = p;
	parentpo = (childpo - 1) / 2;
	h[childpo] = element;
	while (childpo != 0 && h[childpo]>h[parentpo])
	{
		h[childpo] = h[parentpo];
		h[parentpo] = element;
		childpo = parentpo;
		parentpo = (childpo - 1) / 2;
	}
	h[childpo] = element;
	p = p + 1;
}
void accept_tree()
{
	int i,idcount = 1, k;
	
	printf("\n Enter the element to be inserted : ");
	scanf("%d",&k);
	insert(k);

	i = 0;
	while (i < 1) {
		fflush(stdin);
		if (p == 0) {
			break;
		}
		TREENODE* current = new TREENODE;
		current->data = h[i];
		current->id = idcount++;
		current->right = NULL;
		current->left = NULL;
		i++;
		add_node(&root, current);
	}
}
void inorder(TREENODE* root) {
	if (root != NULL) {
		inorder(root->left);
		printf("%d  ",root->data);
		inorder(root->right);
	}
}
void preorder(TREENODE* root) {
	if (root != NULL) {
		printf("%d  ", root->data);
		preorder(root->left);
		preorder(root->right);
	}
}
void postorder(TREENODE* root) {
	if (root != NULL) {
		postorder(root->left);
		postorder(root->right);
		printf("%d  ", root->data);
	}
}
void drawCircle(float segments, float radius, float sx, float sy)
{
	float theta = 2 * 3.1415926 / segments;
	float tan_factor = tanf(theta);
	float radial_factor = cosf(theta);
	float x = radius;
	float y = 0;

	int cache_pt = 0;
	double cache_x;
	double cache_y;

	glBegin(GL_LINES);
	glColor3f(0.0,0.0,0.0);
	
	for (int ii = 0; ii < segments; ii++) {
		if (!cache_pt) {
			cache_x = x + sx;
			cache_y = y + sy;
			cache_pt = 1;
		}
		else {
			//glVertex2f(cache_x,cache_y); 
			glVertex2f(x + sx, y + sy);
			cache_x = x + sx;
			cache_y = y + sy;
		}
		float tx = -y;
		float ty = x;
		x += tx * tan_factor;
		y += ty * tan_factor;
		x *= radial_factor;
		y *= radial_factor;
	}
	glEnd();
}
void draw_line(float x1, float y1, float x2, float y2) {

	float tempX = 0;
	float tempY = 0;
	float slope = (y2 - y1) / (x2 - x1);
	float i;
	//printf("y1 : %f y2 : %f", y1, y2);
	float m;
	if(slope > 0)
		m=1.25;
	else
		m=0.50;
	for(i=y1; i>=y2+m; i-=0.1) {
		glLineWidth(3.0);
		glBegin(GL_LINES);
		glColor3f(0.0,0.0,0.0);
		glVertex2f(x1, y1);
		tempY = i;
		tempX = x1 + ((tempY - y1) / slope);
		glVertex2f(tempX, tempY);
		glEnd();
		glFlush();
		usleep(10000);
		//printf("\nHere %f\n", i);
		// glutSwapBuffers();				
		
	}

	
}
void draw_text(char* text, float x, float y) {
	GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
	glRasterPos3f(x, y, 0);
	for (int i = 0; text[i] != '\0'; i++){
		glutBitmapCharacter(font_style, text[i]);
	}
}
void drawNode(TREENODE* t_root, float x1, float y1, int level) {
	printf("Drawing node\n");
	if (t_root == NULL) {
		return;
	}
	float segments = 30*sqrtf(2.0);
	float radius =  2.0;
	float left_angle = 245;
	float right_angle = 115;
	float branch_length = 12 - level*2.5;
	float angle_change = 20;

	// Draw the current circle 

	drawCircle(segments, radius, x1+1, y1+0.5);

	char buff[5];
	string str=to_string(t_root->data);
	strcpy(buff,str.c_str());
	//itoa(t_root->data, buff, 10);
	
	draw_text(buff, x1, y1);

	if (t_root->left) {
		// Draw the Left circle 
		float angle = left_angle - level*angle_change;
		double radian = angle*3.14 / 180;
		float m = (double)tan((double)radian);
		float x2 = x1 + branch_length * sin((double)radian);
		float y2 = y1 + branch_length * cos((double)radian);
		drawNode(t_root->left, x2, y2, level + 1);

		printf("\nDrawing (%f, %f) (%f, %f)\n", x1, y1, x2, y2);
		draw_line(x1, y1, x2, y2);
	}
	if (t_root->right) {
		// Draw the Right circle 
		float angle = right_angle + level*angle_change;
		float radian = angle*3.14 / 180;
		float m = (double)tan((double)radian);
		float x2 = x1 + branch_length * sin((double)radian);
		float y2 = y1 + branch_length * cos((double)radian);
		drawNode(t_root->right, x2, y2, level + 1);
		draw_line(x1, y1, x2, y2);
	}
}
void display(void) {
	
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0, 0.0, 0.0, 1.0);
	//glutFullScreen();
	glLoadIdentity();
	glTranslatef(0, 10, -30);
	glColor3f(1, 1, 1);
	drawNode(root, 0, 0, 0);
	glFlush();
	// glutSwapBuffers();
}
void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	}
}
void menu(int id1)
{
	switch (id1)
	{
	case 11:exit(0);
		break;
	}
}
void bg_menu(int id3)
{
	switch (id3)
	{
	case 5:glClearColor(0.0, 1.0, 1.0, 1.0);
		break;
	case 6:glClearColor(0.5, 0.5, 0.5, 1.0);
		break;
	case 7:glClearColor(0.0, 0.0, 1.0, 1.0);
		break;
	case 8:glClearColor(1.0, 0.0, 0.0, 1.0);
		break;
	case 9:glClearColor(0.0, 1.0, 0.0, 1.0);
		break;
	case 10:glClearColor(0.0, 0.0, 0.0, 1.0);
		break;
	}
	glutPostRedisplay();
}
void printletter(int id)
{

	int n, i = 0, idcount = 1;

	switch (id)
	{
	case 1:
		printf("\n Enter the new element : ");
		scanf("%d", &n);
		insert(n);
		break;
	case 2:
		deleteh(h);
		break;
	case 3:
	    heapsort();
	 	break;



	default:
		break;
	}
	i = 0;
	while (i < p) {
		fflush(stdin);
		if (p == 0) {
			break;
		}
		TREENODE* current = new TREENODE;
		current->data = h[i];
		current->id = idcount++;
		current->right = NULL;
		current->left = NULL;
		i++;
		add_node(&root, current);
	}
	display();
}
void order(int id)
{
	switch(id)
	{
		case 1 :
			printf("\nInorder  : ");
			inorder(root);
			printf("\n");
			break;
		case 2 :
			printf("\nPostorder  : "); 
			postorder(root);
			printf("\n");
			break;
		case 3 :
			printf("\nPreorder  : "); 
			preorder(root);
			printf("\n");
			break;
	}
}
int main(int argc, char **argv) {
	int submenu3, submenu2,submenu4;
	accept_tree();

	// OPENGL Drawing functions 
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800,800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Heap Implementation");

	// Register function pointers to the drawing framework 
	glClearColor(1.0, 0.0, 0.0, 1.0);
	glutDisplayFunc(display);
	// glutIdleFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	submenu2 = glutCreateMenu(printletter);
	glutAddMenuEntry("Insertion", 1);
	glutAddMenuEntry("Deletion", 2);
	glutAddMenuEntry("Heap sort", 3);


	submenu3 = glutCreateMenu(bg_menu);
	glutAddMenuEntry("Navy Blue", 5);
	glutAddMenuEntry("Gray", 6);
	glutAddMenuEntry("Blue", 7);
	glutAddMenuEntry("Red ", 8);
	glutAddMenuEntry("Green", 9);
	glutAddMenuEntry("reset color", 10);
	
	submenu4=glutCreateMenu(order);
	glutAddMenuEntry("Inorder",1);
	glutAddMenuEntry("Postorder",2);
	glutAddMenuEntry("Preorder",3);

	glutCreateMenu(menu);

	glutAddSubMenu("Operation", submenu2);
	glutAddSubMenu("Background", submenu3);
	glutAddSubMenu("Order",submenu4);
	glutAddMenuEntry("Quit", 11);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
	return 0;
}