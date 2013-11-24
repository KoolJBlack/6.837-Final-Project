#include "SkeletalModel.h"

#include <FL/Fl.H>
#include <iostream>
#include <math.h>
#define PI (3.14159265359)

#define RAND_FLOAT (((float) rand() / (RAND_MAX)))

using namespace std;


void SkeletalModel::load(const char *skeletonFile, const char *meshFile, const char *attachmentsFile)
{
	loadSkeleton(skeletonFile);

	m_mesh.load(meshFile);
	m_mesh.loadAttachments(attachmentsFile, m_joints.size());

	computeBindWorldToJointTransforms();
	updateCurrentJointToWorldTransforms();
}

void SkeletalModel::draw(Matrix4f cameraMatrix, bool skeletonVisible)
{
	// draw() gets called whenever a redraw is required
	// (after an update() occurs, when the camera moves, the window is resized, etc)

	m_matrixStack.clear();
	m_matrixStack.push(cameraMatrix);
	/*
	if( skeletonVisible )
	{
		drawJoints();

		drawSkeleton();
	}
	else
	{
		// Clear out any weird matrix we may have been using for drawing the bones and revert to the camera matrix.
		glLoadMatrixf(m_matrixStack.top());

		// Tell the mesh to draw itself.
		m_mesh.draw();
	}*/

	// Clear out any weird matrix we may have been using for drawing the bones and revert to the camera matrix.
	glLoadMatrixf(m_matrixStack.top());

	// Tell the mesh to draw itself.
	m_mesh.draw();
}

void SkeletalModel::loadSkeleton( const char* filename ){
	// Load the skeleton from file here.        
	ifstream in(filename);
    if (!in) {
        cerr<< filename << " not found\a" << endl;
        exit(0);
    }

    cerr << endl << "*** loadSkeleton reading in file " << filename << " ***" << endl;

    float x, y, z;
    int parentIndex;
    while (in >> x >> y >> z >> parentIndex) {
    	Joint * joint = new Joint;
    	joint->transform = Matrix4f::translation(x,y,z);
    	joint->pos = Vector3f(x,y,z);
    	joint->index = m_joints.size();
    	joint->color = Vector3f(RAND_FLOAT, RAND_FLOAT, RAND_FLOAT);
    	m_joints.push_back(joint);
    	if (parentIndex == -1) {
    		m_rootJoint = joint;
    	} else {
    		m_joints[parentIndex]->children.push_back(joint);
    		joint->parent = m_joints[parentIndex];
    	}
    }
    in.close();

    cerr << endl << "*** loadSkeleton a total of " << m_joints.size() << " joints read ***" << endl;
}


void SkeletalModel::jointDFS(Joint * joint) {
	m_matrixStack.push(joint->transform);

	glLoadMatrix(m_matrixStack.top());

    glPushAttrib( GL_ALL_ATTRIB_BITS );
    Vector3f c = joint->color;
    GLfloat diffColor[] = {c[0], c[1], c[2]};
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColor );


	if (joint-> index == 0) {
		glutSolidSphere( 0.025f, 12, 12 );
	} else {
		glutSolidSphere( 0.025f, 12, 12 );
    }
    glPopAttrib();


	vector< Joint* > children =  joint->children;

    for(vector<Joint*>::iterator it = children.begin(); it != children.end(); ++it) {
    	jointDFS(*it);
    }

    m_matrixStack.pop();
}


void SkeletalModel::drawJoints( )
{
	// Draw a sphere at each joint. You will need to add a recursive helper function to traverse the joint hierarchy.
	//
	// We recommend using glutSolidSphere( 0.025f, 12, 12 )
	// to draw a sphere of reasonable size.
	//
	// You are *not* permitted to use the OpenGL matrix stack commands
	// (glPushMatrix, glPopMatrix, glMultMatrix).
	// You should use your MatrixStack class
	// and use glLoadMatrix() before your drawing call.
	jointDFS(m_rootJoint);
}


// http://stackoverflow.com/questions/6992541/opengl-rotation-in-given-direction
Matrix4f rotateToVec(Vector3f z) {
	z = z.normalized();
	Vector3f rnd(0,0,1);
	Vector3f y = Vector3f::cross(z, rnd).normalized();
	Vector3f x = Vector3f::cross(y, z).normalized();
	Matrix4f rot = Matrix4f();
	rot.setCol(0, Vector4f(x, 0));
	rot.setCol(1, Vector4f(y, 0));
	rot.setCol(2, Vector4f(z, 0));
	rot.setCol(3, Vector4f(0, 0, 0, 1));
	return rot;
}


void SkeletalModel::skeletonDFS(Joint * joint) {
	m_matrixStack.push(joint->transform);
	glLoadMatrix(m_matrixStack.top());	

	vector< Joint* > children =  joint->children;
    for(vector<Joint*>::iterator it = children.begin(); it != children.end(); ++it) {
        Joint* childJoint = *it;

		Vector3f pos = childJoint->pos;
		float dist = pos.abs();
		Matrix4f m =  Matrix4f::scaling(0.05, 0.05, dist) * Matrix4f::translation(0, 0, 0.5);
		m = rotateToVec(pos) * m;
		m_matrixStack.push(m);
		
	    glLoadMatrix(m_matrixStack.top());

		glutSolidCube(1.0);

	    m_matrixStack.pop();

    	skeletonDFS(childJoint);
    }

    m_matrixStack.pop();
}

void SkeletalModel::drawSkeleton( )
{
	// Draw boxes between the joints. You will need to add a recursive helper function to traverse the joint hierarchy.
	skeletonDFS(m_rootJoint);
}


void SkeletalModel::setJointTransform(int jointIndex, float rX, float rY, float rZ)
{
	// Set the rotation part of the joint's transformation matrix based on the passed in Euler angles.
	//Matrix4f m = Matrix4f::rotateZ(rZ) * Matrix4f::rotateY(rY) * Matrix4f::rotateX(rX);
	Matrix4f m = Matrix4f::rotateX(rX) * Matrix4f::rotateY(rY) * Matrix4f::rotateZ(rZ);
	m.setCol(3, m_joints[jointIndex]->transform.getCol(3));
	//Vector3f pos = m_joints[jointIndex]->pos;
	//m = Matrix4f::translation(pos) * m;
	m_joints[jointIndex]->transform = m;
}


void SkeletalModel::jointBindDFS(Joint * joint, Matrix4f m) {
	m = m * joint->transform;
	joint->bindWorldToJointTransform = m.inverse();

    vector< Joint* > children =  joint->children;
	for(vector<Joint*>::iterator it = children.begin(); it != children.end(); ++it) {
        Joint* childJoint = *it;
        jointBindDFS(childJoint, m);
    }
}


void SkeletalModel::computeBindWorldToJointTransforms()
{
	// 2.3.1. Implement this method to compute a per-joint transform from
	// world-space to joint space in the BIND POSE.
	//
	// Note that this needs to be computed only once since there is only
	// a single bind pose.
	//
	// This method should update each joint's bindWorldToJointTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.
	jointBindDFS(m_rootJoint, Matrix4f::identity());
}


void SkeletalModel::jointUpdateDFS(Joint * joint, Matrix4f m) {
	m = m * joint->transform;
	joint->currentJointToWorldTransform = m;

    vector< Joint* > children =  joint->children;
	for(vector<Joint*>::iterator it = children.begin(); it != children.end(); ++it) {
        Joint* childJoint = *it;
        jointUpdateDFS(childJoint, m);
    }
}

void SkeletalModel::updateCurrentJointToWorldTransforms()
{
	// 2.3.2. Implement this method to compute a per-joint transform from
	// joint space to world space in the CURRENT POSE.
	//
	// The current pose is defined by the rotations you've applied to the
	// joints and hence needs to be *updated* every time the joint angles change.
	//
	// This method should update each joint's currentJointToWorldTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.
	cerr << endl << "*** updateCurrentJointToWorldTransforms ***" << endl;

	jointUpdateDFS(m_rootJoint, Matrix4f::identity());
}

void SkeletalModel::updateMesh()
{
	// 2.3.2. This is the core of SSD.
	// Implement this method to update the vertices of the mesh
	// given the current state of the skeleton.
	// You will need both the bind pose world --> joint transforms.
	// and the current joint --> world transforms.
    cerr << endl << "*** updateMesh ***" << endl;

    for(unsigned int i=0; i < m_mesh.bindVertices.size(); ++i) {
    	Vector3f bindVertex = m_mesh.bindVertices[i];
    	Vector3f currentVertex;
    	Vector3f color(0,0,0);
    	vector<float> weights = m_mesh.attachments[i];

    	for(unsigned int j=0; j < weights.size(); ++j) {
    		currentVertex += (weights[j]) * (m_joints[j + 1]->currentJointToWorldTransform * m_joints[j + 1]->bindWorldToJointTransform * Vector4f(bindVertex, 1)).xyz();
    		color += weights[j] * m_joints[j + 1]->color;
    	}
    	m_mesh.currentVertices[i] = currentVertex;
    	m_mesh.vertexColors[i] = color.normalized();
	}
}

