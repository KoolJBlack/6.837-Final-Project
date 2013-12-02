// modelerview.h

// This is the base class for your view window.  It contains
// a camera control for your use.  The draw() function will 
// set up default lighting and apply the projection.
  
#ifndef MODELERVIEW_H
#define MODELERVIEW_H

#include <FL/Fl_Gl_Window.H>
#include "extra.h"
class Camera;
class ModelerView;

#include "SkeletalModel.h"

using namespace std;

class ModelerView : public Fl_Gl_Window 
{
public:

    ModelerView(int x, int y, int w, int h, const char *label = 0);

    void loadModel(int argc, char* argv[]);
    
    virtual ~ModelerView ();
    
    virtual int handle(int event);
	virtual void update();
    virtual void draw();

	void updateBlendShapes();
	void updateJoints();
	void drawAxes();

    Camera *m_camera;
	SkeletalModel model;

	bool m_drawAxes;
	bool m_drawSkeleton;		// if false, the mesh is drawn instead.

	Mesh m_mesh;
};


#endif	/*  */
