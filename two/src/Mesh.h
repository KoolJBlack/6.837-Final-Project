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
#include "texture.h"
#include "camera.h"
#include "BlendShape.h"
#include "projection.h"
#include "views.h"
#include <cstdio>


typedef tuple< unsigned, 3 > Tuple3u;

struct Mesh {
	// list of vertices from the OBJ file
	// in the "bind pose"
	//std::vector< Vector3f > bindVertices;

	// each face has 3 indices
	// referencing 3 vertices
	std::vector<std::vector<Tuple3u> > faces;

	// current vertex positions after animation
	std::vector< Vector3f > currentVertices;

	// current vertex normals
	std::vector< Vector3f > vertexNormals;


	// Need vertices for every blend shape pose
	
	// HeadShape = neutral face
	BlendShape HeadShape;
	
	// Smile
	BlendShape LSmileShape;
	BlendShape RSmileShape;

	// FrownShape
	BlendShape LFrownShape;
	BlendShape RFrownShape;
	
	// EyeClose
	BlendShape LEyeCloseShape;
	BlendShape REyeCloseShape;

	// EyebrowUp
	BlendShape LEyebrowUpShape;
	BlendShape REyebrowUpShape;

	// EyebrowDown
	BlendShape LEyebrowDownShape;
	BlendShape REyebrowDownShape;

	//vector<BlendShape> m_blendShapes;
	BlendShape m_blendShapes[11];
	

	// current texture coordinates 
	std::vector< Vector2f > textureCoords;

	// current vertex colors 
	std::vector< Vector3f > vertexColors;

	// list of vertex to joint attachments
	// each element of attachments is a vector< float > containing
	// one attachment weight per joint
	std::vector< std::vector< float > > attachments;

	void setCamera(Camera * c);
	Camera * getCamera();

	// 2.1.1. load() should populate bindVertices, currentVertices, and faces
	void load_mesh(const char* filename);

	// helper function to load multiple shapes
	void load_shape(const char *filename, std::vector<Vector3f>& vertices, std::vector<Tuple3u>& faces, std::vector<Vector3f>& normals);
	
	// Texture loading functions
	//void load_text(const char *filename);
	// Projection loading functions
	void init_projections_with_textures(std::string prefix );
	void bind_projeciton_text(Projection &p);


	// Texture initialization
	void init_text();
	void zero_texture(GLubyte * image);
	void init_frame_buffer();
	void multipass_render();
	void getWText(int p);
	void getIText(int p);
	void mult_textures(GLubyte* in0, GLubyte* in1, GLubyte* out, int size);
	void add_textures(GLubyte* stored_text, GLubyte* new_text, int size);

	// compute the normals if they are not given in the object file
	void compute_norm();

	// Update data
	void update();
	void updateProjectionBlendWeights();

	// Draw the current mesh and texture projections
	void draw();
	void draw_final(GLubyte* image, int proejectionIndex);
	void draw_mesh(bool useTexture, int projectionIndex);
	void draw_image(GLubyte * image); //not using this
	void draw_screen_quad();

	// Projection elements
	std::vector<Projection*> projections;

	// Texture elements
	bool m_texture_init;
	bool m_frame_init;

	// GL texture info
	GLuint tex0ID;
	GLuint tex1ID;
	GLuint finalImageID;

	GLubyte * final_image;
	GLubyte * texture_image;
	GLubyte * weights_image;

  	// Camera info
  	Camera *m_camera;

	// Views object
	Views *m_viewObj;

};

#endif
