#ifndef MESH_H
#define MESH_H

#include <vector>
#include <vecmath.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include "extra.h"
#include "tuple.h"
#include "Texture.h"
#include "camera.h"

typedef tuple< unsigned, 3 > Tuple3u;

struct Mesh {
	// list of vertices from the OBJ file
	// in the "bind pose"
	std::vector< Vector3f > bindVertices;

	// each face has 3 indices
	// referencing 3 vertices
	std::vector<std::vector<Tuple3u>> faces;

	// current vertex positions after animation
	std::vector< Vector3f > currentVertices;

	// current vertex normals
	std::vector< Vector3f > vertexNormals;

	// current texture coordinates 
	std::vector< Vector2f > textureCoords;

	// current vertex colors 
	std::vector< Vector3f > vertexColors;

	// list of vertex to joint attachments
	// each element of attachments is a vector< float > containing
	// one attachment weight per joint
	std::vector< std::vector< float > > attachments;

	void setCamera(Camera * c);

	// 2.1.1. load() should populate bindVertices, currentVertices, and faces
	void load_mesh(const char *filename);
	
	// Texture loading functions
	void load_text(const char *filename);

	// Texture initialization
	void init_text();
	void init_projective_text();

	// compute the normals if they are not given in the object file
	void compute_norm();

	// Draw the current mesh and texture projections
	void draw();
	void draw_mesh();
	void project_texture();

	// 2.2. Implement this method to load the per-vertex attachment weights
	// this method should update m_mesh.attachments
	void loadAttachments( const char* filename, int numJoints );

	// Texture elements
	Texture t;
	bool m_texture_init;
	bool m_projected_init;

	// GL texture info
  	GLuint texture1ID;
  	GLuint texture2ID;

  	// Camera info
  	Camera *m_camera;

};

#endif
