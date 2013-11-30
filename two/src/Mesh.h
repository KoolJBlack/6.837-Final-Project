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

typedef tuple< unsigned, 3 > Tuple3u;

struct Mesh {
	// list of vertices from the OBJ file
	// in the "bind pose"
	std::vector< Vector3f > bindVertices;

	// each face has 3 indices
	// referencing 3 vertices
	std::vector< Tuple3u > faces;

	// current vertex positions after animation
	std::vector< Vector3f > currentVertices;

	// current vertex normals
	std::vector< Vector3f > vertexNormals;


	// Need vertices for every blend shape pose

	// HeadShape = neutral face
	std::vector< Vector3f > HeadVertices;
	std::vector< Tuple3u > HeadFaces;
	std::vector< Vector3f > HeadNormals;

	// FrownShape
	std::vector< Vector3f > RFrownVertices;
	std::vector< Tuple3u > RFrownFaces;
	std::vector< Vector3f > RFrownNormals;

	std::vector< Vector3f > LFrownVertices;
	std::vector< Tuple3u > LFrownFaces;
	std::vector< Vector3f > LFrownNormals;

	// EyebrowUp
	std::vector< Vector3f > REyebrowUpVertices;
	std::vector< Tuple3u > REyebrowUpFaces;
	std::vector< Vector3f > REyebrowUpNormals;

	std::vector< Vector3f > LEyebrowUpVertices;
	std::vector< Tuple3u > LEyebrowUpFaces;
	std::vector< Vector3f > LEyebrowUpNormals;

	// EyebrowDown
	std::vector< Vector3f > REyebrowDownVertices;
	std::vector< Tuple3u > REyebrowDownFaces;
	std::vector< Vector3f > REyebrowDownNormals;

	std::vector< Vector3f > LEyebrowDownVertices;
	std::vector< Tuple3u > LEyebrowDownFaces;
	std::vector< Vector3f > LEyebrowDownNormals;

	// Smile
	std::vector< Vector3f > RSmileVertices;
	std::vector< Tuple3u > RSmileFaces;
	std::vector< Vector3f > RSmileNormals;

	std::vector< Vector3f > LSmileVertices;
	std::vector< Tuple3u > LSmileFaces;
	std::vector< Vector3f > LSmileNormals;

	// EyeClose
	std::vector< Vector3f > REyeCloseVertices;
	std::vector< Tuple3u > REyeCloseFaces;
	std::vector< Vector3f > REyeCloseNormals;

	std::vector< Vector3f > LEyeCloseVertices;
	std::vector< Tuple3u > LEyeCloseFaces;
	std::vector< Vector3f > LEyeCloseNormals;

	// current texture coordinates 
	std::vector< Vector2f > textureCoords;

	// current vertex colors 
	std::vector< Vector3f > vertexColors;

	// list of vertex to joint attachments
	// each element of attachments is a vector< float > containing
	// one attachment weight per joint
	std::vector< std::vector< float > > attachments;

	// 2.1.1. load() should populate bindVertices, currentVertices, and faces
	void load_mesh(const char *filename);

	// helper function to load multiple shapes
	void load_shape(const char *filename, vector<Vector3f>& vertices, vector<Tuple3u>& faces, vector<Vector3f>& normals);
	
	// Texture loading functions
	void load_text(const char *filename);
	void init_text();

	// compute the normals if they are not given in the object file
	void compute_norm();

	// 2.1.2. draw the current mesh.
	void draw();

	// 2.2. Implement this method to load the per-vertex attachment weights
	// this method should update m_mesh.attachments
	void loadAttachments( const char* filename, int numJoints );

	// Texture elements
	Texture t;
	bool m_texture_init;
	// GL texture info
  	GLuint textureID;
};

#endif
