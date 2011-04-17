#include <GL/glut.h>
#include <glui.h>
#include <iostream>

#include "image_functions.h"
////////////////////////////////////////////////////////

class Star
{
public:
    int r,g,b;
    float dist, angle;
};

////////////////////////////////////////////////////////
// Global Variables

int gMainWindow;

GLUI *glui(NULL);

int gWidth(300), gHeight(300);

const char* gTexFilename("Star.bmp");

unsigned int gTexture[1]; // storage for one texture

float gZTranslation(-15.0);

int gTwinkle(0);

const int gNumStars(50);

Star gStars[gNumStars];

float gSpin(0.0);

float gTilt(90.0);

////////////////////////////////////////////////////////
// Prototypes

void InitGL();

void Resize(int w, int h);

void DrawGL();
    
void DrawStars();

void LoadGLTextures(); 

////////////////////////////////////////////////////////
//
int main(int argc, char* argv[])
{
    ////////////////////////////////////////////////////////
    // Initialize GLUT
    
    glutInit(&argc,argv);

    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowPosition(0,0);
    glutInitWindowSize(gWidth,gHeight);

    gMainWindow = glutCreateWindow("Example 03 - Stars");
    glutDisplayFunc(&DrawGL);
    glutReshapeFunc(&Resize);

    InitGL();

    ////////////////////////////////////////////////////////
    // Initialize GLUI
    glui = GLUI_Master.create_glui("Toolbar",0,350,0);
    glui->add_statictext("Example 03 - Stars");
    glui->add_checkbox("EnabelTwinkle",&gTwinkle);

    // Add Z Position
    glui->add_translation("Tilt",GLUI_TRANSLATION_Z,&gTilt);

    glui->set_main_gfx_window(gMainWindow);
    GLUI_Master.set_glutIdleFunc(&DrawGL);
 
    ////////////////////////////////////////////////////////
    glutMainLoop();

    return 0;
}

void InitGL()
{
    // Load Texture
    LoadGLTextures();

    glEnable(GL_TEXTURE_2D); // Enable texture mapping
    glShadeModel(GL_SMOOTH); // Enables Smooth Color Shading

    // This Will Clear The Background Color To Black
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);		
    glClearDepth(1.0);       // Enables Clearing Of The Depth Buffer
    //glDepthFunc(GL_LESS);    // The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST); // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);    // The Type Of Depth Test To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();        // Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    float aspect = (GLfloat)gWidth/(GLfloat)gHeight;
    gluPerspective(45.0,aspect,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);


    for( int i = 0; i < gNumStars; i++ )
    {
        gStars[i].angle = 0.0;
        gStars[i].dist = ((float)i/(float)gNumStars)*5.0;
        gStars[i].r = rand()%256;
        gStars[i].g = rand()%256;
        gStars[i].b = rand()%256;
    }

}

void Resize(int w, int h)
{
    gWidth  = w;
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

    float aspect = (GLfloat)gWidth/(GLfloat)gHeight;
    gluPerspective(45.0,aspect,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);

    glui->sync_live();
}

void DrawGL()
{
    glutSetWindow(gMainWindow);
    glutPostRedisplay();

    // Clear The Screen And The Depth Buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (GLfloat)gWidth/(GLfloat)gHeight;
    gluPerspective(45.0,aspect,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);

    // choose the texture to use.
    glColor4f(1.0,1.0,1.0,0.5);
    glBindTexture(GL_TEXTURE_2D, gTexture[0]);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glEnable(GL_BLEND);

    DrawStars();

    // swap buffers to display, since we're double buffered.
    glutSwapBuffers();

    glui->sync_live();
}

void DrawStars()
{
    float tilt = gTilt;
    for( int i = 0; i < gNumStars; i++ )
    {
        glLoadIdentity();

        glTranslatef( 0.0, 0.0, -15.0);
        glRotatef(tilt,1.0,0.0,0.0);

        glRotatef(gStars[i].angle, 0.0, 1.0, 0.0); // rotate in stars dir
        glTranslatef(gStars[i].dist,0.0,0.0); // Move forward in stars dir

        glRotatef(-gStars[i].angle, 0.0, 1.0, 0.0); // unrotate in stars dir
        glRotatef(-tilt,1.0,0.0,0.0);

        // Begin Drawing This Star
 
        if(1== gTwinkle)
        {
            int inx( gNumStars-i );
            //Assign colors using bytes
            glColor4ub(gStars[inx-1].r,gStars[inx-1].g,gStars[inx-1].b,255);
            glBegin(GL_QUADS);
                glTexCoord2f(0.0,0.0);glVertex3f(-1.0,-1.0,0.0);
                glTexCoord2f(1.0,0.0);glVertex3f( 1.0,-1.0,0.0);
                glTexCoord2f(1.0,1.0);glVertex3f( 1.0, 1.0,0.0);
                glTexCoord2f(0.0,1.0);glVertex3f(-1.0, 1.0,0.0);
            glEnd();
        }

        // Draw Main Star
        glRotatef(gSpin,0.0,0.0,1.0); // Rotate the star
        glColor4ub(gStars[i].r,gStars[i].g,gStars[i].b,255);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0,0.0);glVertex3f(-1.0,-1.0,0.0);
            glTexCoord2f(1.0,0.0);glVertex3f( 1.0,-1.0,0.0);
            glTexCoord2f(1.0,1.0);glVertex3f( 1.0, 1.0,0.0);
            glTexCoord2f(0.0,1.0);glVertex3f(-1.0, 1.0,0.0);
        glEnd();

        gSpin+=0.01;
        gStars[i].angle += float(i)/gNumStars;
        gStars[i].dist  -= 0.01;

        if( gStars[i].dist<0.0 )
        {
            gStars[i].dist+=5.0;
            gStars[i].r=rand()%256;
            gStars[i].g=rand()%256;
            gStars[i].b=rand()%256;
        }
    }
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

    if (!ReadBMPImage(gTexFilename, image1)) 
    {
        exit(1);
    }        

    // Create Texture	
    glGenTextures(1, &gTexture[0]);

    {
        glBindTexture(GL_TEXTURE_2D, gTexture[0]);   // 2d texture (x and y size)

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

        // 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image, 
        // border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
        glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
    }
    

    if( image1 && image1->data)
    {
        free(image1->data);
    }
}




