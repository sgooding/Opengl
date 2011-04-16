
#include <GL/glut.h>
#include <glui.h>
#include <iostream>

#include "shapes.h"
#include "image_functions.h"

int main_window;
int state(0);
int live_state(0);
GLUI *glui(NULL);
char text[sizeof(GLUI_String)];
const char* desc("Window : %d:%d");
GLUI_StaticText* label(NULL);

float trans[2]  = { 0.0, 0.0 }; 
float ztrans[1] = { -6.0f };
float rot[16]   = { 1.0, 0.0, 0.0, 0.0, 
                    0.0, 1.0, 0.0, 0.0, 
                    0.0, 0.0, 1.0, 0.0, 
                    0.0, 0.0, 0.0, 1.0 }; 
float focal[1] = { 45.0 };
float aspect;
float trirot(0.0);
int enable_full(0);

GLUI_StaticText* mouselabel(NULL);
char mousetext[sizeof(GLUI_String)];
const char* mousedesc("Mouse: %i:%i:%i:%i");

GLUI_StaticText* motionlabel(NULL);
char motiontext[sizeof(GLUI_String)];
const char* motiondesc("Motion: %i:%i");

/* storage for one texture  */
unsigned int texture[1];

BoundingBox gBox;

int gWidth(300), gHeight(300);

void draw_2d()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(  0,      //left
                 gWidth, //Right
                 gHeight,      // bottom
                 0); // top

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    
    glEnable (GL_BLEND); 
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0,0.0,1.0,0.3);
    glBegin(GL_TRIANGLES);
        glVertex2f(0.0,0.0);
        glVertex2f(200.0,0.0);
        glVertex2f(100.0,200.0);
    glEnd();
    glDisable(GL_BLEND);

    draw_BB(gBox);
}

void mousebutton(int button, int state, int x, int y)
{
    sprintf(mousetext,mousedesc,button,state,x,y);
    mouselabel->set_text(mousetext);

    if( button == 0 && state == 0 )
    {
        gBox.Start(Point3D(x,y,0.0));
        gBox.End(Point3D(x+1,y+1,0.0));
        glutPostRedisplay();
    } 
}

void mousemotion(int x, int y)
{
    // Drag Motion

    sprintf(motiontext,motiondesc,x,y);
    motionlabel->set_text(motiontext);

    gBox.End(Point3D(x,y,0.0));
}
   
// Load Bitmaps And Convert To Textures
void LoadGLTextures() 
{	
    // Load Texture
    Image *image1;
    
    // allocate space for texture
    image1 = (Image *) malloc(sizeof(Image));
    if (image1 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }

    if (!ReadBMPImage("Evie.bmp", image1)) {
	exit(1);
    }        

    // Create Texture	
    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);   // 2d texture (x and y size)

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

    // 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image, 
    // border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
}

void draw()
{
    glutSetWindow(main_window);
    glutPostRedisplay();

    // Clear The Screen And The Depth Buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(focal[0],aspect,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 

    // Reset The View
    //glLoadIdentity();
    
    glMultMatrixf(rot);

    glTranslatef(trans[0],trans[1],ztrans[0]);

    // Move Left 1.5 Units And Into The Screen 6.0
    glTranslatef(-1.5f,0.0f,-6.0f);
    glPushMatrix();

    glRotatef(trirot, 0.0, 1.0, 0.0 );

    (state==0?glColor3f(1.0,0.0,0.0):glColor3f(0.0,1.0,0.0));

    draw_pyramid();

    glPopMatrix();

    glTranslatef(3.0f,0.0f,0.0f);  // Move Right 3 Units

    (state==0?glColor3f(0.0,1.0,0.0):glColor3f(1.0,0.0,0.0));

    glPushMatrix();

    glRotatef(trirot,1.0,0.0,0.0);
    glRotatef(trirot,0.0,1.0,0.0);
    glRotatef(trirot,0.0,0.0,1.0);
    //draw_square();
    
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);   // choose the texture to use.
    draw_textured_square();

    glPopMatrix();

    // Draw Ground
    glColor3f(0.0,1.0,0.0);
    glLineWidth(1.0);
    double l_h(50.0);
    double z_h(-5.0);
    for( double x = -l_h; x <= l_h; x += 1.0)
    { 
        Line3D lineh( Point3D(x,z_h,l_h),Point3D(x,z_h,-l_h) ); 
        Line3D linew( Point3D(l_h,z_h,x),Point3D(-l_h,z_h,x) ); 
        draw_line(lineh);
        draw_line(linew);
    }




    live_state = state;
    
    if( state == 1 )
    {
        trirot = (trirot > 360.0?0.0:trirot+1.0);
    }

    draw_2d();

    // swap buffers to display, since we're double buffered.
    glutSwapBuffers();

    glui->sync_live();
}

void resize(int w, int h)
{
    gWidth = w;
    gHeight = h;

    // Prevent A Divide By Zero If The Window Is Too Small
    if (gHeight==0)
    {
        gHeight=1;
    }
    
    // Reset The Current Viewport And Perspective Transformation
    glViewport(0, 0, gWidth, gHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    aspect = (GLfloat)gWidth/(GLfloat)gHeight;
    gluPerspective(focal[0],aspect,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);

    sprintf(text,desc,gWidth,gHeight);
    label->set_text(text);

    glui->sync_live();
}

void FullScreen(int)
{
    if( enable_full == 1 )
    {
        glutFullScreen();
    } else
    {
        glutReshapeWindow(300,300);
    }
}

void init(int w, int h)
{
    gWidth  = w;
    gHeight = h;

    // Load Texture
    LoadGLTextures();
    glEnable(GL_TEXTURE_2D); // Enable texture mapping

    // This Will Clear The Background Color To Black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		
    glClearDepth(1.0);       // Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);    // The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST); // Enables Depth Testing
    glShadeModel(GL_SMOOTH); // Enables Smooth Color Shading

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();        // Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    aspect = (GLfloat)gWidth/(GLfloat)gHeight;
    gluPerspective(focal[0],aspect,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char* argv[])
{
    ////////////////////////////////////////////////////////
    // Initialize GLUT
    
    glutInit(&argc,argv);

    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowPosition(0,0);
    glutInitWindowSize(gWidth,gHeight);

    main_window = glutCreateWindow("MainWindow");
    glutDisplayFunc(&draw);
    glutReshapeFunc(&resize);

    init(gWidth,gHeight);
    sprintf(text,desc,gWidth,gHeight);


    ////////////////////////////////////////////////////////
    // Initialize GLUI
    glui = GLUI_Master.create_glui("GLUI",0,350,0);
    glui->add_statictext("Simple GLUI Example");
    glui->add_checkbox("Click Me",&state);
    glui->add_checkbox("Live Variable",&live_state);
    glui->add_checkbox("FullScreen",&enable_full,0,&FullScreen);
    //glui->add_edittext("",GLUI_EDITTEXT_TEXT,text);
    label = glui->add_statictext(text);
    
    sprintf(mousetext,mousedesc,0,0,0,0);
    mouselabel = glui->add_statictext(mousetext);

    sprintf(motiontext,motiondesc,0,0,0,0);
    motionlabel = glui->add_statictext(motiontext);


    GLUI_Panel* pos_panel = glui->add_panel("Position");
    glui->add_translation_to_panel(pos_panel,"POS",GLUI_TRANSLATION_XY,trans);
    glui->add_column_to_panel(pos_panel,true);
    glui->add_translation_to_panel(pos_panel,"Z",GLUI_TRANSLATION_Z,ztrans);
    glui->add_rotation("Rot",rot);
    glui->add_translation("Zoom",GLUI_TRANSLATION_Z,focal);

    glui->set_main_gfx_window(main_window);
    GLUI_Master.set_glutIdleFunc(&draw);
    GLUI_Master.set_glutMouseFunc(&mousebutton);
    glutMotionFunc(&mousemotion);


    ////////////////////////////////////////////////////////
    glutMainLoop();

    return 0;
}
