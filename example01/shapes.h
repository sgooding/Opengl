#ifndef _SHAPES_H_
#define _SHAPES_H_

#include <iostream>

struct Point3D
{
    Point3D()
    {
    }

    Point3D(float _x,float _y,float _z) :
        x(_x),
        y(_y),
        z(_z)
    {
    }
    float x;
    float y;
    float z;
};

std::ostream& operator<<(std::ostream& ss, const Point3D& point )
{
    ss << point.x << ":" << point.y << ":" << point.x; 
    return ss;
}

struct Line3D
{
    Line3D( Point3D s, Point3D e ) :
        start(s),
        end(e)
    {
    }
    Point3D start;
    Point3D end;
};

std::ostream& operator<<(std::ostream& ss, const Line3D& line)
{
    ss << line.start << "->" << line.end;
    return ss;
}

struct BoundingBox
{
    void Start(Point3D o)
    {
        origin = o;
    }
    void End(Point3D e)
    {
        end = e;
    }
    void GetTL(Point3D& rt) const
    {
        rt.x = std::min(origin.x,end.x);
        rt.y = std::min(origin.y,end.y);
        rt.z = std::min(origin.z,end.z);
    }
    void GetBR(Point3D& rt) const
    {
        rt.x = std::max(origin.x,end.x);
        rt.y = std::max(origin.y,end.y);
        rt.z = std::max(origin.z,end.z);
    }

    Point3D origin;
    Point3D end;
};

std::ostream& operator<<(std::ostream& ss, const BoundingBox& bb)
{
    Point3D tl;
    bb.GetTL(tl);
    Point3D br;
    bb.GetBR(br);
    ss << "(" << tl << ") (" << br << ")";
    
    return ss;
}



void draw_line( const Line3D& line )
{
    glBegin(GL_LINES);
        glVertex3f(line.start.x,line.start.y,line.start.z);
        glVertex3f(line.end.x,line.end.y,line.end.z);
    glEnd();
}

void draw_pyramid()
{
    // draw a triangle
    glBegin(GL_TRIANGLES);              // start drawing a polygon
        glVertex3f( 0.0f, 1.0f, 0.0f);  // Front Top
        glVertex3f( 1.0f,-1.0f, -1.0f); // Front Bottom Right
        glVertex3f(-1.0f,-1.0f, -1.0f); // Front Bottom Left	
        glColor3f(0.0,0.0,1.0);
        glVertex3f( 0.0f, 1.0f, 0.0f);  // Front Top
        glVertex3f( 1.0f,-1.0f, 1.0f);  // Back Bottom Right
        glVertex3f(-1.0f,-1.0f, 1.0f);  // Back Bottom Left	
        glColor3f(1.0,0.0,1.0);
        glVertex3f( 0.0f, 1.0f, 0.0f);  // Left Top
        glVertex3f(-1.0f,-1.0f, 1.0f);  // Left Back 
        glVertex3f(-1.0f,-1.0f, -1.0f); // Left Front
        glColor3f(1.0,1.0,0.0);
        glVertex3f( 0.0f, 1.0f, 0.0f);  // Right Top
        glVertex3f( 1.0f,-1.0f, 1.0f);  // Right Back 
        glVertex3f( 1.0f,-1.0f, -1.0f); // Right Front
    glEnd();                            // we're done with the polygon
}

void draw_square()
{
    // draw a square (quadrilateral)
    glBegin(GL_QUADS);				// start drawing a polygon (4 sided)
        glVertex3f(-1.0f, 1.0f, 0.0f); // Top Left
        glVertex3f( 1.0f, 1.0f, 0.0f); // Top Right
        glVertex3f( 1.0f,-1.0f, 0.0f); // Bottom Right
        glVertex3f(-1.0f,-1.0f, 0.0f); // Bottom Left	
    glEnd();					// done with the polygon
}

void draw_textured_square()
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


void draw_BB( const BoundingBox& bb )
{
    Point3D bl, br, tl, tr;
    bb.GetTL(tl);
    bb.GetBR(br);
    bl.x = tl.x;
    bl.y = br.y;

    tr.x = br.x;
    tr.y = tl.y;

    //std::cout << "bl->br = " << Line3D(bl,br) << std::endl;
    //std::cout << "br->tr = " << Line3D(br,tr) << std::endl;
    //std::cout << "tr->tl = " << Line3D(tr,tl) << std::endl;
    //std::cout << "tl->br = " << Line3D(tl,br) << std::endl;

    draw_line(Line3D(bl,br));
    draw_line(Line3D(br,tr));
    draw_line(Line3D(tr,tl));
    draw_line(Line3D(tl,bl));
}

#endif
