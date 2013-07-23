#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <texture.h>

#include <png.h>
#include <SDL/SDL.h>
#include <GL/glut.h>

#define ESCAPE 27

#ifndef M_PI
#define M_PI 3.141592654f
#endif

#ifndef TO_RADIANS
#define TO_RADIANS M_PI / 180.0f
#endif

#ifndef WINDOW_W
#define WINDOW_W 1024
#endif

#ifndef WINDOW_H
#define WINDOW_H 768
#endif

typedef enum { X=0, Y=1, Z=2 } dimension;

GLint T0 = 0, t;
GLint Frames = 1;
GLfloat view_rotx = 20.0, view_roty = 30.0, view_rotz = 0.0;
GLint gear1, gear2, gear3;
GLint grid;
GLfloat angle = 0.0, seconds = 0.0, fps = 1.0;
int done;
int screenWidth, screenHeight;
int windowWidth = WINDOW_W, windowHeight = WINDOW_H;
int midWindowX, midWindowY;
int bpp;
int flags;
bool _drawGUI = true;

float campos[3];
float camspeed[3];

int fieldOfView = 90;
float near = 0.01f;
float far = 1024.0f;

bool holdingForward = false;
bool holdingBackward = false;
bool holdingRight = false;
bool holdingLeft = false;

bool fullscreen = false;
const SDL_VideoInfo *info;

void reshape(void);

float toRads(float deg)
{
	return deg * TO_RADIANS;
}

void resetTimer(void)
{
	T0 = t;
	Frames = 1;
}

void prepare2d(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, windowWidth, windowHeight, 0.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
}

void prepare3d(void)
{
	glViewport(0, 0, windowWidth, windowHeight);

	reshape();
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
}

void draw3dScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glRotatef(view_rotx, 1.0, 0.0, 0.0);
	glRotatef(view_roty, 0.0, 1.0, 0.0);
	glRotatef(view_rotz, 0.0, 0.0, 1.0);
	glTranslatef(-campos[X], -campos[Y], -campos[Z]);

	glPushMatrix();
	glTranslatef(-10.0, 0.0, 0.0);
	glCallList(grid);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.0, -2.0, 0.0);
	glRotatef(angle, 0.0, 0.0, 1.0);
	glCallList(gear1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.1, -2.0, 0.0);
	glRotatef(-2.0 * angle - 9.0, 0.0, 0.0, 1.0);
	glCallList(gear2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.1, 4.2, 0.0);
	glRotatef(-2.0 * angle - 25.0, 0.0, 0.0, 1.0);
	glCallList(gear3);
	glPopMatrix();

	glPopMatrix();
}

void draw3d(void)
{
	prepare3d();
	draw3dScene();
}

void drawGUI(void)
{
	glColor3ub(0xff, 0xff, 0xff);
	// draw cross hairs
	glBegin(GL_LINES); {
		glVertex2i(midWindowX - 8, midWindowY);
		glVertex2i(midWindowX + 8, midWindowY);
		glVertex2i(midWindowX, midWindowY - 8);
		glVertex2i(midWindowX, midWindowY + 8);
	} glEnd();
}

void draw2d(void)
{
	prepare2d();
	glDisable(GL_LIGHTING);
	if(_drawGUI) {
		drawGUI();
	}
	glEnable(GL_LIGHTING);
}

void switchScreenMode(void)
{
	if(fullscreen) {
		windowWidth = screenWidth;
		windowHeight = screenHeight;
		midWindowX = windowWidth / 2;
		midWindowY = windowHeight / 2;
		flags = SDL_OPENGL | SDL_FULLSCREEN;
	} else {
		windowWidth = WINDOW_W;
		windowHeight = WINDOW_H;
		midWindowX = windowWidth / 2;
		midWindowY = windowHeight / 2;
		flags = SDL_OPENGL;
	}
	bpp = info->vfmt->BitsPerPixel;

	if(SDL_SetVideoMode(windowWidth, windowHeight, bpp, flags) == 0) {
		exit(1);
	}
	reshape();
}

/*

  Since this does a LOT of trig, one should probably build
  a display list with this rather than call it each frame

  Input:  inner_radius - radius of hole at center
  outer_radius - radius at center of teeth
  width - width of gear
  teeth - number of teeth
  tooth_depth - depth of tooth

**/

void gear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width, GLint teeth, GLfloat tooth_depth)
{
	GLint i;
	GLfloat r0, r1, r2;
	GLfloat angle, da;
	GLfloat u, v, len;

	r0 = inner_radius;
	r1 = outer_radius - tooth_depth / 2.0;
	r2 = outer_radius + tooth_depth / 2.0;

	da = 2.0 * M_PI / teeth / 4.0;

	glShadeModel(GL_FLAT);

	glNormal3f(0.0, 0.0, 1.0);

	/* draw front face */
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= teeth; i++) {
		angle = i * 2.0 * M_PI / teeth;
		glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
		glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
		if (i < teeth) {
			glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
			glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
		}
	}
	glEnd();

	/* draw front sides of teeth */
	glBegin(GL_QUADS);
	da = 2.0 * M_PI / teeth / 4.0;
	for (i = 0; i < teeth; i++) {
		angle = i * 2.0 * M_PI / teeth;

		glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
		glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
		glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
		glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
	}
	glEnd();

	glNormal3f(0.0, 0.0, -1.0);

	/* draw back face */
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= teeth; i++) {
		angle = i * 2.0 * M_PI / teeth;
		glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
		glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
		if (i < teeth) {
			glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
			glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
		}
	}
	glEnd();

	/* draw back sides of teeth */
	glBegin(GL_QUADS);
	da = 2.0 * M_PI / teeth / 4.0;
	for (i = 0; i < teeth; i++) {
		angle = i * 2.0 * M_PI / teeth;

		glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
		glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
		glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
		glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
	}
	glEnd();

	/* draw outward faces of teeth */
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i < teeth; i++) {
		angle = i * 2.0 * M_PI / teeth;

		glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
		glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
		u = r2 * cos(angle + da) - r1 * cos(angle);
		v = r2 * sin(angle + da) - r1 * sin(angle);
		len = sqrt(u * u + v * v);
		u /= len;
		v /= len;
		glNormal3f(v, -u, 0.0);
		glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
		glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
		glNormal3f(cos(angle), sin(angle), 0.0);
		glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
		glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
		u = r1 * cos(angle + 3 * da) - r2 * cos(angle + 2 * da);
		v = r1 * sin(angle + 3 * da) - r2 * sin(angle + 2 * da);
		glNormal3f(v, -u, 0.0);
		glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
		glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
		glNormal3f(cos(angle), sin(angle), 0.0);
	}

	glVertex3f(r1 * cos(0), r1 * sin(0), width * 0.5);
	glVertex3f(r1 * cos(0), r1 * sin(0), -width * 0.5);

	glEnd();

	glShadeModel(GL_SMOOTH);

	/* draw inside radius cylinder */
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= teeth; i++) {
		angle = i * 2.0 * M_PI / teeth;
		glNormal3f(-cos(angle), -sin(angle), 0.0);
		glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
		glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
	}
	glEnd();
}

void calculateCameraMovement(float f)
{
	float movementSpeedFactor = 1.0f;
	float camMovementComponent[3] = {0.0, 0.0, 0.0};
	float mF = movementSpeedFactor * f;

	if(holdingForward) {
		float pitchFactor = cos(toRads(view_rotx));
		camMovementComponent[X] += (mF * (float)sin(toRads(view_roty))) * pitchFactor;
		camMovementComponent[Y] += mF * (float)sin(toRads(view_rotx)) * -1.0f;

		float yawFactor = (float)cos(toRads(view_rotx));
		camMovementComponent[Z] += ( mF * (float)cos(toRads(view_roty)) * -1.0f) * yawFactor;
	}

	if(holdingBackward) {
		float pitchFactor = cos(toRads(view_rotx));
		camMovementComponent[X] += (mF * (float)sin(toRads(view_roty)) * -1.0f) * pitchFactor;
		camMovementComponent[Y] += mF * (float)sin(toRads(view_rotx));
    
		float yawFactor = (float)cos(toRads(view_rotx));
		camMovementComponent[Z] += (mF * (float)cos(toRads(view_roty))) * yawFactor;
	}

	if(holdingLeft) {
		float yRotRad = toRads(view_roty);

		camMovementComponent[X] += -mF * (float)cos(yRotRad);
		camMovementComponent[Z] += -mF * (float)sin(yRotRad);
	}

	if(holdingRight) {
		float yRotRad = toRads(view_roty);

		camMovementComponent[X] += mF * (float)cos(yRotRad);
		camMovementComponent[Z] += mF * (float)sin(yRotRad);
	}

	camspeed[X] = camMovementComponent[X];
	camspeed[Y] = camMovementComponent[Y];
	camspeed[Z] = camMovementComponent[Z];
}

void moveCamera(void)
{
	campos[X] += camspeed[X];
	campos[Y] += camspeed[Y];
	campos[Z] += camspeed[Z];
}

void draw(void)
{
	draw3d();
	draw2d();

	SDL_GL_SwapBuffers();

	t = SDL_GetTicks();
	seconds = (t - T0) / 1000.0;
	fps = Frames / seconds;

	Frames++;
	{
		if(t - T0 >= 4000) {
			printf("%d frames in %g seconds = %g FPS\n", Frames, seconds, fps);
			resetTimer();
		}
	}
}


void idle(void)
{
	angle += (100 / fps);
	calculateCameraMovement(100 / fps);
	moveCamera();
}

void buildDisplayLists(void)
{
	static GLfloat red[4] =
		{0.8, 0.1, 0.0, 1.0};
	static GLfloat green[4] =
		{0.0, 0.8, 0.2, 1.0};
	static GLfloat blue[4] =
		{0.2, 0.2, 1.0, 1.0};
	static GLfloat white[4] =
		{1.0, 1.0, 1.0, 1.0};
  
	/* make the gears */
	gear1 = glGenLists(1);
	glNewList(gear1, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
	gear(1.0, 4.0, 1.0, 20, 0.7);
	glEndList();

	gear2 = glGenLists(1);
	glNewList(gear2, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
	gear(0.5, 2.0, 2.0, 10, 0.7);
	glEndList();

	gear3 = glGenLists(1);
	glNewList(gear3, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
	gear(1.3, 2.0, 0.5, 10, 0.7);
	glEndList();
  
	grid = glGenLists(1);
	glNewList(grid, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glBegin(GL_LINES);
	glVertex3f(-256.0f, 0.0f, -256.0f);
	glVertex3f( 256.0f, 0.0f, -256.0f);
	glVertex3f( 256.0f, 0.0f, -256.0f);
	glVertex3f( 256.0f, 0.0f,  256.0f);
	glVertex3f( 256.0f, 0.0f,  256.0f);
	glVertex3f(-256.0f, 0.0f,  256.0f);
	glVertex3f(-256.0f, 0.0f,  256.0f);
	glVertex3f(-256.0f, 0.0f, -256.0f);
	glEnd();
	glEndList();
}

/* new window size or exposure */
void reshape(void)
{
	GLfloat aspectRatio =
		(windowWidth > windowHeight) ?
		(GLfloat)windowWidth / (GLfloat)windowHeight :
		(GLfloat)windowHeight / (GLfloat)windowWidth;

	GLfloat fH = tan((GLfloat)fieldOfView / 360.0f * M_PI) * near;
	GLfloat fW = fH * aspectRatio;

	glViewport(0, 0, (GLint)windowWidth, (GLint)windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glFrustum(-fW, fW, -fH, fH, near, far);
  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -40.0);
}

void init(int argc, char *argv[])
{
	SDL_ShowCursor(SDL_DISABLE);
	SDL_WM_SetCaption(argv[0], "gears");
  
	reshape();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	static GLfloat pos[4] =
		{5.0, 5.0, 10.0, 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	glLineWidth(5.0f);

	buildDisplayLists();

	glEnable(GL_NORMALIZE);
	
	if(argc > 1 && strcmp(argv[1], "-info") == 0) {
		printf("GL_RENDERER   = %s\n", (char *) glGetString(GL_RENDERER));
		printf("GL_VERSION    = %s\n", (char *) glGetString(GL_VERSION));
		printf("GL_VENDOR     = %s\n", (char *) glGetString(GL_VENDOR));
		printf("GL_EXTENSIONS = %s\n", (char *) glGetString(GL_EXTENSIONS));
	}
}

void handleMouseMove(int mouseX, int mouseY) {
	int horizMovement = mouseX - midWindowX;
	int vertMovement = mouseY - midWindowY;

	view_rotx += vertMovement / 10.0f;
	view_roty += horizMovement / 10.0f;

	SDL_WarpMouse(midWindowX, midWindowY);
}

void checkKeys(Uint8 *keys) {
	if(keys[SDLK_ESCAPE]) {
		done = 1;
	}
	if(keys[SDLK_q]) {
		done = 1;
	}
	if(keys[SDLK_UP]) {
		view_rotx += (100.0 / fps);
	}
	if(keys[SDLK_DOWN]) {
		view_rotx -= (100.0 / fps);
	}
	if(keys[SDLK_LEFT]) {
		view_roty += (100.0 / fps);
	}
	if(keys[SDLK_RIGHT]) {
		view_roty -= (100.0 / fps);
	}
	if(keys[SDLK_p]) {
		fieldOfView = 10;
		reshape();
	} else {
		fieldOfView = 90;
		reshape();
	}
	holdingForward = keys[SDLK_w];
	holdingBackward = keys[SDLK_s];
	holdingLeft = keys[SDLK_a];
	holdingRight = keys[SDLK_d];
  
	if(keys[SDLK_F11]) {
		fullscreen = !fullscreen;
		switchScreenMode();
		resetTimer();
	}
}

void checkEvents(void) {
	Uint8 *keys;
	SDL_Event event;
  
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_QUIT:
			done = 1;
			break;
		case SDL_MOUSEMOTION:
			handleMouseMove(event.motion.x, event.motion.y);
			break;
		}
	}
	keys = SDL_GetKeyState(NULL);

	checkKeys(keys);
}

int omain(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	glutInit(&argc, argv);

	info = SDL_GetVideoInfo();
	if(!info) {
		done = 1;
	}

	screenWidth = info->current_w;
	screenHeight = info->current_h;

	switchScreenMode();


	init(argc, argv);
	done = 0;
	while(!done) {
		idle();
    
		checkEvents();

		draw();
	}
	SDL_Quit();
	return 0;
}
