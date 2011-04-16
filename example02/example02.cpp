#include <GL/glut.h>
#include <glui.h>
#include <iostream>

#include "image_functions.h"

////////////////////////////////////////////////////////
// Global Variables

int gMainWindow;

GLUI *glui(NULL);

int gWidth(300), gHeight(300);

const char* gTexFilename("Evie.bmp");

unsigned int gTexture[1]; // storage for one texture

////////////////////////////////////////////////////////
// Prototypes

void InitGL();

void Resize(int w, int h);

void DrawGL();

void LoadGLTextures(); 

void DrawTextureCube();

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

    gMainWindow = glutCreateWindow("Example 02 - Shading Tests");
    glutDisplayFunc(&DrawGL);
    glutReshapeFunc(&Resize);

    InitGL();

    ////////////////////////////////////////////////////////
    // Initialize GLUI
    glui = GLUI_Master.create_glui("Toolbar",0,350,0);
    glui->add_statictext("Example 02 - Shading tests");
    //glui->add_checkbox("Click Me",&state);
    //glui->add_checkbox("FullScreen",&enable_full,0,&FullScreen);
    //glui->add_edittext("",GLUI_EDITTEXT_TEXT,text);
    
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

    // This Will Clear The Background Color To Black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		
    glClearDepth(1.0);       // Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);    // The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST); // Enables Depth Testing
    glShadeModel(GL_SMOOTH); // Enables Smooth Color Shading

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();        // Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    float aspect = (GLfloat)gWidth/(GLfloat)gHeight;
    gluPerspective(45.0,aspect,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);
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

    // Reset The View
    glLoadIdentity();
    
    glTranslatef(0.0f,0.0f,-6.0f);
    static float angle(0);
    glRotatef(angle++, 0.0, 1.0, 0.0 );
    glRotatef(15.0, 1.0, 0.0, 0.0 );

    // choose the texture to use.
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D, gTexture[0]);
    DrawTextureCube();

    // swap buffers to display, since we're double buffered.
    glutSwapBuffers();

    glui->sync_live();
}

void DrawTextureCube()
{
    //   y
    //   ^ 
    //   |
    //   *-->x
    //  /
    // z
    //
    // 0,1  <-   1,0
    //            ^
    //            |
    // 0,0  ->   1,0
    glBegin(GL_QUADS);		                // begin drawing a cube

    // Front Face (note that the texture's corners have to match the quad's corners)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Top Left Of The Texture and Quad

    // Right Face
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Top Left Of The Texture and Quad


    // Back Face
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad


    // Left Face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad

    // Top Face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f, -1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f,  1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f,  1.0f);	// Top Left Of The Texture and Quad

    // Bottom Face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Top Left Of The Texture and Quad

    glEnd();

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
    glBindTexture(GL_TEXTURE_2D, gTexture[0]);   // 2d texture (x and y size)

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

    // 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image, 
    // border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
}

