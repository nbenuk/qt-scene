
#include <iostream>
#include <QSlider>
#include <cmath>
#include <GL/glu.h>
#include <GL/glut.h>
#include <QGLWidget>
#include "SolidCubeWidget.h"

// Setting up material properties
typedef struct materialStruct {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat shininess;
} materialStruct;


static materialStruct whiteShinyMaterials = {
  { 1.0, 1.0, 1.0, 1.0},
  { 1.0, 1.0, 1.0, 1.0},
  { 1.0, 1.0, 1.0, 1.0},
  100.0
};


// constructor
SolidCubeWidget::SolidCubeWidget(QWidget *parent):
  QGLWidget(parent),
  angle(0.0),
  lightAngle(0),
  colour(0.0),
  speed(0),
  _earth("./earth.ppm"),
  _wall("./wall.ppm"),
  _bar("./bar.ppm"),
  _floor("./floor.ppm"),
  _oak("./oak.ppm"),
  _metal("./metal.ppm"),
  _black("./black.jpj"),
  _ball("./dball.ppm"),
  _time(0.0)
	{ // constructor
    pperson = gluNewQuadric();
    plight  = gluNewQuadric();
    pbar = gluNewQuadric();
	} // constructor


// called when OpenGL context is set up
void SolidCubeWidget::initializeGL()
	{ // initializeGL()
	// set the widget background colour
	glClearColor(0.0, 0.0, 0.0, 1.0);
	} // initializeGL()

  SolidCubeWidget::~SolidCubeWidget()
  {
    gluDeleteQuadric(pperson);
    gluDeleteQuadric(plight);
    gluDeleteQuadric(pbar);
  }


  //For rotating objects with time
  void SolidCubeWidget::updateAngle(){
    angle += 2.5;
    if (lights){
    animLight();}
    this->repaint();
  }

  //For adjusting dancer speed with slider
  void SolidCubeWidget::updateDancer(int i){
    speed = i;
    this->repaint();
  }
void SolidCubeWidget::updateColour(){
    if (lights){
    if (colour==0){
        colour = 1;
        light_ambient[0] = 1; //change party light colour
        light_ambient[1] = 0;
        light_ambient[2] = 0;
        light_ambient[3] = 0;
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    }else if (colour==1) {
        colour = 2;
        light_ambient[0] = 0;
        light_ambient[1] = 1;
        light_ambient[2] = 0;
        light_ambient[3] = 0;
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
       } else if (colour==2){
        colour =0;
        light_ambient[0] = 0;
        light_ambient[1] = 0;
        light_ambient[2] = 1;
        light_ambient[3] = 0;
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    }

}}



// called every time the widget is resized
void SolidCubeWidget::resizeGL(int w, int h)
	{ // resizeGL()
	// set the viewport to the entire widget
	glViewport(0, 0, w, h);
	GLfloat light_pos[] = {0., 0., 1., 0.};

	glEnable(GL_LIGHTING); // enable lighting in general
    glEnable(GL_LIGHT0);   // each light source must also be enabled
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    //Inital light colour
    light_ambient[3] = 1;

    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightf (GL_LIGHT0, GL_SPOT_CUTOFF,180.);


  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho(-3.0, 3.0, -3.0, 3.0, -5.0, 99.0);
//    glOrtho(-4.0, 4.0, -4.0, 4.0, -4.0, 4.0);

	} // resizeGL()


//for rendering a model speaker object
  void SolidCubeWidget::speaker(){
      glEnable(GL_TEXTURE_2D);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _metal.Width(), _metal.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, _metal.imageField());

      //box
      glTranslatef(0.,0.,-1.);
      this->cube();
      glTranslatef(0.,1.,0.);
      this->cube();
      //speaker cone
      glTranslatef(0,0.3,0.1);
      circle(0,0,0.3);
      //to animate spaker moving
      if (std::fmod(angle/5 , 2.0) >= 1){
      glTranslatef(0,-1.2,0.1);
      }
      else {
          glTranslatef(0,-1.2,0.2);
      }
      circle(0,0,0.8);
      circle(0,0,0.5);
      glDisable(GL_TEXTURE_2D);
  }

  //draw a circle from triangles
  void SolidCubeWidget::circle(GLfloat x, GLfloat y, GLfloat radius){
      int i;
      int nTriangles = 30;
      glEnable(GL_TEXTURE_2D);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _floor.Width(), _floor.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, _floor.imageField());
      GLfloat Pi =  3.141256;
      GLfloat normals[][3] = { {1., 0. ,0.}, {-1., 0., 0.}, {0., 0., 1.}, {0., 0., -1.}, {0, 1, 0}, {0, -1, 0} };
      glNormal3fv(normals[4]);

      glBegin(GL_TRIANGLE_FAN);
      glVertex3f(x, y,1);
          for(i = 0; i <= nTriangles;i++) {
              glVertex3f(
                      x + (radius * cos(i * 2.0f *Pi / nTriangles)),
                  y + (radius * sin(i * 2.0f *Pi / nTriangles)),
                          1
              );
          }
      glEnd();
      glDisable(GL_TEXTURE_2D);
  }

//draw floor and simulate colour changes
void SolidCubeWidget::floor(){
  GLfloat normals[][3] = { {1., 0. ,0.}, {-1., 0., 0.}, {0., 0., 1.}, {0., 0., -1.}, {0, 1, 0}, {0, -1, 0} };
  glEnable(GL_TEXTURE_2D);
  if (lights){
    // to change dancefloor colours
      if (colour==0){
         glRotatef(90,0,1,0);
      } else if (colour == 1) {
          glRotatef(180,0,1,0);
      }
    }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _floor.Width(), _floor.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, _floor.imageField());

  //construct plane horizontal
  glNormal3fv(normals[4]);
  glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(  1.0,  -1.0,  1.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(  1.0,  -1.0, -1.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f( -1.0,  -1.0, -1.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f( -1.0,  -1.0,  1.0);


  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void SolidCubeWidget::wall(){
    glEnable(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _bar.Width(), _bar.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, _bar.imageField());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glNormal3f(0.0,0.0,1.0);
    glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-1.0, -1.0, -1.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f( 1.0, -1.0, -1.0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f( 1.0,  1.0, -1.0);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(-1.0,  1.0, -1.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

}
void SolidCubeWidget::wall2(){
    glEnable(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _wall.Width(), _wall.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, _wall.imageField());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glNormal3f(0.0,0.0,1.0);
    glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 0.0);
        glVertex3f( -1.0, -1.0,  1.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f( -1.0, -1.0, -1.0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f( -1.0,  1.0, -1.0);
        glTexCoord2f(0.0, 1.0);
        glVertex3f( -1.0,  1.0,  1.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

}
void SolidCubeWidget::cube(){
      GLfloat normals[][3] = {  {-1., 0., 0.}, {0., 0., 1.}, {1., 0. ,0.}, {0., 0., -1.} };
    materialStruct* p_front = &whiteShinyMaterials;

    glMaterialfv(GL_FRONT, GL_AMBIENT,    p_front->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    p_front->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   p_front->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   p_front->shininess);

    glNormal3fv(normals[0]);
    glBegin(GL_POLYGON);
      glVertex3f( 1.0, -1.0,  1.0);
      glVertex3f( 1.0, -1.0, -1.0);
      glVertex3f( 0.0,  1.0, -1.0);
      glVertex3f( 1.0,  1.0,  1.0);
    glEnd();

    glNormal3fv(normals[3]);
    glBegin(GL_POLYGON);
      glVertex3f(-1.0, -1.0, -1.0);
      glVertex3f( 1.0, -1.0, -1.0);
      glVertex3f( 1.0,  1.0, -1.0);
      glVertex3f(-1.0,  1.0, -1.0);
    glEnd();

    glNormal3fv(normals[2]);
    glBegin(GL_POLYGON);
      glVertex3f(-1.0, -1.0, 1.0);
      glVertex3f( 1.0, -1.0, 1.0);
      glVertex3f( 1.0,  1.0, 1.0);
      glVertex3f(-1.0,  1.0, 1.0);
    glEnd();

    glNormal3fv(normals[1]);
    glBegin(GL_POLYGON);
      glVertex3f( -1.0, -1.0,  1.0);
      glVertex3f( -1.0, -1.0, -1.0);
      glVertex3f( -1.0,  1.0, -1.0);
      glVertex3f( -1.0,  1.0,  1.0);
    glEnd();

    glNormal3fv(normals[4]);
    glBegin(GL_POLYGON);
      glVertex3f(  1.0,  1.0,  1.0);
      glVertex3f(  1.0,  1.0, -1.0);
      glVertex3f( -1.0,  1.0, -1.0);
      glVertex3f( -1.0,  1.0,  1.0);
    glEnd();

    glNormal3fv(normals[5]);
    glBegin(GL_POLYGON);
      glVertex3f(  1.0,  -1.0,  1.0);
      glVertex3f(  1.0,  -1.0, -1.0);
      glVertex3f( -1.0,  -1.0, -1.0);
      glVertex3f( -1.0,  -1.0,  1.0);
    glEnd();
}

void SolidCubeWidget::bar(){
    glEnable(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _oak.Width(), _oak.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, _oak.imageField());
    gluQuadricDrawStyle(pbar, GLU_FILL);
    gluQuadricTexture(pbar, true);
    gluQuadricNormals(pbar, GLU_SMOOTH);
    glTranslatef(-4.,0.,-1.);
    glScalef(5,1,1);
    this->cube();
    glDisable(GL_TEXTURE_2D);
}

void SolidCubeWidget::dj(){
    glEnable(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _metal.Width(), _metal.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, _metal.imageField());

    glTranslatef(-4.,0.,-1.);
    glTranslatef(-4.,0.,-1.);
    this->cube();
    glTranslatef(1.,0.,0.);
    this->cube();
    glTranslatef(1.,0.,0.);
    this->cube();
    glTranslatef(1.,0.,0.);
    this->cube();
    glTranslatef(1.,0.,0.);
    this->cube();
    glTranslatef(1.,0.,0.);
    this->cube();
    glTranslatef(1.,0.,0.);
    this->cube();
    glTranslatef(1.,0.,0.);
    this->cube();
    glTranslatef(1.,0.,0.);
    this->cube();

    glDisable(GL_TEXTURE_2D);
    glRotatef(-90,1,0,0);
    glPushMatrix();
    glTranslatef(-3,0.1,0.1);
    glRotatef(angle,0,0,1);
    circle(0,0,0.8);
    glPopMatrix();
    glTranslatef(-5,0.1,0.1);
    glRotatef(angle,0,0,1);
    circle(0,0,0.8);
}


void SolidCubeWidget::lighting(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _black.Width(), _black.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, _black.imageField());

    glTranslatef(0.,1.,0.);
    glScalef(0.5,0.5,0.5);
    this->cube();

    glPushMatrix();
    glRotatef((double)lightAngle,0.,0.,1.);
    glRotatef(90,1.,0.,0.);
    this->lightLense();
    glPopMatrix();
}

void SolidCubeWidget::lightLense(){
    glBindTexture(GL_TEXTURE_2D, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _metal.Width(), _metal.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, _metal.imageField());
    glColor3f(0.0,1.0,0.0);
    gluQuadricDrawStyle(plight, GLU_FILL);
    gluQuadricTexture(plight, true);
    gluQuadricNormals(plight,GL_SMOOTH);

    glTranslatef(0.0,0.0,0.5);
    gluCylinder(plight,      0.5,0.5,   2,10,4);

    glColor3f(0.0,1.0,0.0);
    gluQuadricDrawStyle(plight, GLU_FILL);
    gluQuadricNormals(plight,GL_SMOOTH);

    gluQuadricTexture(plight, true);
    gluCylinder(plight,      1,1,    1,10,4);

}
void SolidCubeWidget::person(){
    //body
    glTranslatef(0,1,0);
    glPushMatrix();
    glRotatef(90,1,0,0);
    gluQuadricNormals(pperson,GL_SMOOTH);
    gluCylinder(pperson, 1,1,1.5,100,5);
    glPopMatrix();
    //
    glPushMatrix();
    glTranslatef(0.5,-1.5,0);
    glRotatef(90,1,0,0);
    gluCylinder(pperson,0.2,0.2,1,100,10);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.5,-1.5,0);
    glRotatef(90,1,0,0);
    gluCylinder(pperson,0.2,0.2,1,100,10);
    glPopMatrix();

    //arm
    glPushMatrix();
    glTranslatef(1.2,0,0);
    glRotatef(angle,1,0,0);
    gluCylinder(pperson, 0.2,0.2,0.8,100,5);
    glPopMatrix();
    //arm
    glPushMatrix();
    glTranslatef(-1.2,0,0);
    glRotatef(-angle,1,0,0);
    gluCylinder(pperson, 0.2,0.2,0.8,100,5);
//    glPopMatrix();
    glPopMatrix();
    glTranslatef(0,1,0);

    glEnable(GL_TEXTURE_2D);
    if (head == 0) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _earth.Width(), _earth.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, _earth.imageField());

    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _ball.Width(), _ball.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, _ball.imageField());
    }
    gluQuadricTexture(pperson, GL_TRUE);
    gluQuadricNormals(pperson, GL_SMOOTH);
    gluSphere(pperson,0.7,100,10);
    glDisable(GL_TEXTURE_2D);


    glPopMatrix();
}


void SolidCubeWidget::animLight(){
    int speed = 5;
    lightAngle = (dir) ? lightAngle+speed : lightAngle-speed;

    if (lightAngle>30 && dir){
        dir=false;
    } else if (lightAngle<-30 && !dir){
        dir=true;
    }
}

void SolidCubeWidget::partyLightSwitch(){
    if (lights )
    {
        lights = false;
    } else {
        lights = true;
    }
}
void SolidCubeWidget::changeHead(){
    if (head==0){
        head = 1;
    }else {
        head = 0;
       }
}
// called every time the widget needs painting
void SolidCubeWidget::paintGL()
{ // paintGL()
	// clear the widget
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_FLAT);
    glMatrixMode(GL_MODELVIEW);

    //Floor
    glPushMatrix();
    glTranslatef(2.0,2.0,2.0);
    glScalef(2.0,2.0,2.0);
    this->floor();
    glPopMatrix();

    //Party light
    glPushMatrix();
    glTranslatef(2.2, 3.8, 2);
    if (lights) {
    glRotatef(-(double)angle,0.,1.,0.);
    }
    this->lighting();
    glPopMatrix();
    //Party light
    glPushMatrix();
    glTranslatef(0.2, 3.8, 3);
    if (lights) {

    glRotatef((double)angle,0.,1.,0.);}
    this->lighting();
    glPopMatrix();
    //Party light
    glPushMatrix();
    glTranslatef(3, 3.8, 0.2);
    if (lights) {

    glRotatef((double)angle,0.,1.,0.);}
    this->lighting();
    glPopMatrix();
    //speaker R
    glPushMatrix();
    glTranslatef(1.4,3,2);
    glRotatef(90,0,1,0);
    glScalef(0.25,0.25,0.25);
    this->speaker();
    glPopMatrix();
    //speaker L
    glPushMatrix();
    glTranslatef(1.4,3,4.5);
    glRotatef(90,0,1,0);
    glScalef(0.25,0.25,0.25);
    this->speaker();
    glPopMatrix();

    //wall1
    glPushMatrix();
    glTranslatef(2.0,2.0,2.0);
    glScalef(2.5,2.0,2.0);
    this->wall();
    glPopMatrix();

//    //wall2
    glPushMatrix();
    glTranslatef(2.0,2.0,2.0);
    glScalef(2.0,2.0,2.5);
    this->wall2();
    glPopMatrix();

    //bar
    glPushMatrix();
    glTranslatef(4.4,1.0,1.5);
    glScalef(.25,.5,.25);
    this->bar();
    glPopMatrix();

    //dj booth
    glPushMatrix();
    glTranslatef(1.5,1.0,3.1);
    glScalef(.25,.5,.12);
    glRotatef(90,0,1,0);
    this->dj();
    glPopMatrix();

//People
    //diskjockey
    glPushMatrix();
    glTranslatef(0.5,1.5,3.1);
    glScalef(.25,.5,.12);
    glRotatef(90,0,1,0);

    this->person();

    glPopMatrix();

    //bartender
    glPushMatrix();
    glTranslatef(3.1,1.5,0.5);
    glScalef(.25,.5,.12);
    this->person();
    glPopMatrix();

    //dancer
    glPushMatrix();
    if (std::fmod(angle/10 , 7.0) == 0){
        glTranslatef(3.5,1.2,1.8);
    } else {
    glTranslatef(3.5,1.0,1.8);
    }//
    glScalef(.25,.5,.12);
    this->person();

    glPopMatrix();
    //dancer
    glPushMatrix();
    if (std::fmod(angle/10 , 7.0) == 0){
      glTranslatef(1.5,1.2,3.4);


    } else {
    glTranslatef(1.5,1.0,3.4);}//
    glScalef(.25,.5,.12);
    this->person();


    glPopMatrix();

    //spin dancer
           //animate...
            //hierarchymla

    glPushMatrix();//xhange
    glTranslatef(2.5,0.0,2.5);//
    glRotatef((double)angle*speed,0.,1.,0.);
    glTranslatef(-2.5,-0.0,-2.0);//
    glPushMatrix();

    if (std::fmod(angle/10 , 7.0) == 0){
        glTranslatef(3.1,1.2,2.1);
    } else {
      glTranslatef(3.1,1.0,2.1);}//
    glScalef(.25,.5,.12);
    this->person();

    //
    glPopMatrix();
    glPopMatrix();


//Camera
    glLoadIdentity();
    gluLookAt(1.,0.75,1., 0.0,0.0,0.0, 0.0,1.0,0.0);

	// flush to screen
	glFlush();

	} // paintGL()
