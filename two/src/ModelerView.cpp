#include "ModelerView.h"
#include "camera.h"
#include "modelerapp.h"

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include "extra.h"
#include <cstdio>

// Accessing the values of sliders is a very lengthy function call.
// We use a macro VAL() to shorten it.
#define VAL(x) ( static_cast< float >( ModelerApplication::Instance()->GetControlValue( x ) ) )

ModelerView::ModelerView(int x, int y, int w, int h,
			 const char *label):Fl_Gl_Window(x, y, w, h, label)
{
    m_camera = new Camera();	

    m_camera->SetDimensions( w, h );
    m_camera->SetDistance( 5);
    //m_camera->SetCenter( Vector3f( 0.5, 0.5, 0.5 ) );
    m_camera->SetCenter( Vector3f( 0, 0, 0 ) );

	m_drawAxes = true;
	m_drawSkeleton = true;

}

// If you want to load files, etc, do that here.
void ModelerView::loadModel(int argc, char* argv[])
{
	glutInit( &argc, argv );

	// Load the model based on the command-line arguments
	string prefix = argv[ 1 ];
	//string skeletonFile = prefix + ".skel";
	string meshFile = prefix + ".obj";
	string textFile = prefix + ".bmp";
	//string attachmentsFile = prefix + ".attach";

	//TODO: we only want to load meshes, so get rid of the skeleton...
	//model.load(skeletonFile.c_str(), meshFile.c_str(), attachmentsFile.c_str());
	//m_mesh.load_mesh(meshFile.c_str());
	m_mesh.load_mesh(prefix.c_str());
	// Load the texture
	//m_mesh.load_text(textFile.c_str());
	m_mesh.load_text("data/Steve.bmp");

	// Pass the camera to mesh
	m_mesh.setCamera(m_camera);
}

ModelerView::~ModelerView()
{
    delete m_camera;
}

int ModelerView::handle( int event )
{
    unsigned eventCoordX = Fl::event_x();
    unsigned eventCoordY = Fl::event_y();
    unsigned eventButton = Fl::event_button();
    unsigned eventState  = Fl::event_state();	

    switch( event )
    {
	case FL_PUSH:
		{
			switch (eventButton)
			{
				case FL_LEFT_MOUSE:
					m_camera->MouseClick( Camera::LEFT, eventCoordX, eventCoordY );
					break;

				case FL_MIDDLE_MOUSE:
					m_camera->MouseClick( Camera::MIDDLE, eventCoordX, eventCoordY );
					break;

				case FL_RIGHT_MOUSE:
					m_camera->MouseClick( Camera::RIGHT, eventCoordX, eventCoordY );
					break;
			}
		}
		break;

	case FL_DRAG:
		{
			m_camera->MouseDrag(eventCoordX, eventCoordY);
		}
		break;

    case FL_RELEASE:
		{
            m_camera->MouseRelease(eventCoordX, eventCoordY);
		}
		break;	

    case FL_KEYUP:
    	{
			unsigned key = Fl::event_key();

			if( key == 'a' )
			{
				m_drawAxes = !m_drawAxes;
				cout << "drawAxes is now: " << m_drawAxes << endl;
			}
			else if( key == 's' )
			{
				m_drawSkeleton = !m_drawSkeleton;
				cout << "drawSkeleton is now: " << m_drawSkeleton << endl;
			}
    	}
		break;

    default:
		return 0;
    }

    redraw();

    return 1;
}

void ModelerView::update()
{
	// update the mesh from sliders
	updateBlendShapes();

	// Kai commented all of this out...
	// update the skeleton from sliders
	//updateJoints();


	// Update the bone to world transforms for SSD.
	//model.updateCurrentJointToWorldTransforms();

	// update the mesh given the new skeleton
	//model.updateMesh();

}

// TODO: finish this. 
// update the mesh from the sliders
void ModelerView::updateBlendShapes()
{
	vector<float> weights;
	weights.push_back(0.0); // headshape starts at 0 weight and only increases when total of other weights < 1
	float total_w = 0.0f;
	for(unsigned int faceNum = 1; faceNum < 11; faceNum++){
		float w = VAL(faceNum);
		weights.push_back(w);
		total_w += w;
	}
	if (total_w < 1.0f){
		// make sure that all of the weights sum to 1
		// fill in any remaining weight into the head position
		weights[0] += (1.0f - total_w);
	}
	else if (total_w > 1.0f){
		for(unsigned int faceNum = 0; faceNum < 11; faceNum++){
			weights[faceNum] = weights[faceNum] / total_w;
		}
	}

	for (unsigned int vNum = 0; vNum < m_mesh.currentVertices.size(); vNum++){		
		Vector3f newVert = Vector3f(0.0);
		//Vector3f& newNorm = Vector3f(0.0);
		for(unsigned int faceNum = 0; faceNum < 11; faceNum++){
			newVert += m_mesh.m_blendShapes[faceNum].b_vertices[vNum] * weights[faceNum];
			//newNorm += m_mesh.m_blendShapes[faceNum].b_normals[vNum] * weights[faceNum];
		}
		m_mesh.currentVertices[vNum] = newVert;
		//m_mesh.vertexNormals[vNum] = newNorm;
	}
	
}

void ModelerView::updateJoints()
{
	for(unsigned int jointNo = 0; jointNo < 18; jointNo++)
	{
		float rx = VAL( jointNo * 3 );
		float ry = VAL( jointNo * 3 + 1 );
		float rz = VAL( jointNo * 3 + 2 );

		model.setJointTransform(jointNo, rx, ry, rz);
	}
}

// Call the draw function of the parent.  This sets up the
// viewport, projection, and camera position, as well as some
// default lighting parameters.
void ModelerView::draw()
{
    // Window is !valid() upon resize
    // FLTK convention has you initializing rendering here.
    if( !valid() )
    {
        // Setup opengl
        glShadeModel( GL_SMOOTH );
        glEnable( GL_DEPTH_TEST );
        glEnable( GL_LIGHTING );
        glEnable( GL_LIGHT0 );
        glEnable( GL_NORMALIZE );
        
        m_camera->SetDimensions(w(),h());
        m_camera->SetViewport(0,0,w(),h());
        m_camera->ApplyViewport();
        
        // Camera set perspective matrix
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        m_camera->SetPerspective( 50.0f );
        glLoadMatrixf( m_camera->projectionMatrix() );
    }
        
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Note that the lighting is applied *before* applying the camera
    // transform.  This is so the light appeared fixed on the camera.    
    GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};
    GLfloat Lt0pos[] = {3.0,3.0,5.0,1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

    // These are just some default material colors
    GLfloat diffColor[] = {0.4f, 0.4f, 0.4f, 1.f};
    GLfloat specColor[] = {0.6f, 0.6f, 0.6f, 1.f};
    GLfloat shininess[] = {50.0f};
    
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColor );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specColor );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, shininess );

    // Load the camera view matrix
    // The lights are always in the same place reletive to the projection, 
    // because the viewMatrix is not loaded until after the lights are set. 
    // Note, the view matrix really moves the model matrix (the wolrd moves)
    // It does not change gllookat.
	glLoadMatrixf( m_camera->viewMatrix() );

    if(m_drawAxes)
    {
    	drawAxes();
    }

    //model.draw( m_camera->viewMatrix(), m_drawSkeleton );
	m_mesh.draw();
}

void ModelerView::drawAxes()
{
	glDisable( GL_LIGHTING );
	glBegin( GL_LINES );

	glColor3f( 1, 0, 0 );
	glVertex3f( 0, 0, 0 );
	glVertex3f( 1, 0, 0 );

	glColor3f( 0, 1, 0 );
	glVertex3f( 0, 0, 0 );
	glVertex3f( 0, 1, 0 );

	glColor3f( 0, 0, 1 );
	glVertex3f( 0, 0, 0 );
	glVertex3f( 0, 0, 1 );

	glEnd();
	glEnable( GL_LIGHTING );
}
