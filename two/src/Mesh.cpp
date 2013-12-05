#include "Mesh.h"
#define SMOOTH (true)

#include "helpers.h"
#include <cassert>

using namespace std;

void Mesh::setCamera(Camera * c) {
    m_camera = c;
}

Camera * Mesh::getCamera() {
    return m_camera;
}

void Mesh::load_mesh( const char* filename )
{
	// check if we are loading a single .obj file or all blendshapes
	string f = string(filename);
	string objString = f + ".obj";
	bool useShapes = false;
	// Add your code here.
	ifstream in(objString.c_str());
    if (in) {
        cerr << endl << "*** Mesh::load reading in file " << objString.c_str() << " ***" << endl;
    }
	else {
		objString = f + "/Head.obj";
		ifstream in(objString.c_str());
		if (in){
			cerr << endl << "*** Mesh::load reading in blend shapes " << objString.c_str() << " ***" << endl;
			useShapes = true;
		}
		else{
			cerr << objString << " not found. exiting." << endl;
			exit(0);
		}	
	}
	
	ifstream in1(objString.c_str());
    // load the BJ fOile here
    string line;
    while (std::getline(in1, line)) {
        // Parse line values through string stream.
        stringstream ss(line);
        string s;
        ss >> s; 

        if (s == "v") {
            Vector3f v;
            ss >> v[0] >> v[1] >> v[2];
            currentVertices.push_back(v);
            // Set the default color
            vertexColors.push_back(Vector3f(1.0,1.0,1.0));
        } else if (s == "f") {
            vector<Tuple3u> face;
            string abc, def,ghi;
            ss >> abc >> def >> ghi;
            // Parse face 1
            vector<string> f1 = split(abc, '/');
            unsigned a, b, c;
            a = atoi(f1[0].c_str());
            b = atoi(f1[1].c_str());
            if (f1.size() >2) {
                c = atoi(f1[2].c_str());
            }
            face.push_back(Tuple3u(a, b, c));
            // Parse face 2
            vector<string> f2 = split(def, '/');
            unsigned d, e, f;
            d = atoi(f2[0].c_str());
            e = atoi(f2[1].c_str());
            if (f2.size() > 2) {
                f = atoi(f2[2].c_str());
            }
            face.push_back(Tuple3u(d, e, f));
            // Parse face 3
            vector<string> f3 = split(ghi, '/');
            unsigned g, h, i;
            g = atoi(f3[0].c_str());
            h = atoi(f3[1].c_str());
            if (f3.size() > 2) {
                i = atoi(f3[2].c_str());
            }
            face.push_back(Tuple3u(g, h, i));
            // Add faces to fector
            faces.push_back(face);
        } else if (s == "vn") {// normals
			vertexNormals.push_back(Vector3f(0,0,0));
			Vector3f& curr = vertexNormals.back();
			ss >> curr[0] >> curr[1] >> curr[2];
		} else if (s == "vt") {
			Vector2f texcoord;
			ss>>texcoord[0];
			ss>>texcoord[1];
            //texcoord[1] = 1.0-texcoord[1];
			textureCoords.push_back(texcoord);
		} else {
            // ignore remaining tokens
        }
    } 
    // end while cin
    cerr << "currentVertices size " << currentVertices.size() << endl;
    cerr << "textCoords size " << textureCoords.size() << endl;
    cerr << "faces size " << faces.size() << endl;
    cerr << "normals size " << vertexNormals.size() << endl;


	if (vertexNormals.size() == 0){ //compute the normals because we don't have that.
		//compute_norm();
		cout << "need to compute the normals" << endl;
	}

    // Load the texture if possible
	
	// load the headshape as the default
	HeadShape.b_vertices = currentVertices;
	//HeadShape.b_faces = faces;
	HeadShape.b_normals = vertexNormals;

    if (useShapes){
		// hardcoding this part to fit the blend shapes we are using. 

		string LFrown = f + "/LFrown.obj";
		string LSmile = f + "/LSmile.obj";
		string LEyebrowUp = f + "/LEyebrowUp.obj";
		string LEyebrowDown = f + "/LEyebrowDown.obj";
		string LEyeClose = f + "/LEyeClose.obj";
		string RFrown = f + "/RFrown.obj";
		string RSmile = f + "/RSmile.obj";
		string REyebrowUp = f + "/REyebrowUp.obj";
		string REyebrowDown = f + "/REyebrowDown.obj";
		string REyeClose = f + "/REyeClose.obj";
		
		m_blendShapes[1] = BlendShape::load_shape(LSmile.c_str(), LSmileShape);		
		m_blendShapes[2] = BlendShape::load_shape(RSmile.c_str(), RSmileShape);
		m_blendShapes[3] = BlendShape::load_shape(LFrown.c_str(), LFrownShape); 
		m_blendShapes[4] = BlendShape::load_shape(RFrown.c_str(), RFrownShape); 
		m_blendShapes[5] = BlendShape::load_shape(LEyeClose.c_str(), LEyeCloseShape);
		m_blendShapes[6] = BlendShape::load_shape(REyeClose.c_str(), REyeCloseShape);		
		m_blendShapes[7] = BlendShape::load_shape(LEyebrowUp.c_str(), LEyebrowUpShape);
		m_blendShapes[8] = BlendShape::load_shape(REyebrowUp.c_str(), REyebrowUpShape);
		m_blendShapes[9] = BlendShape::load_shape(LEyebrowDown.c_str(), LEyebrowDownShape);
		m_blendShapes[10] = BlendShape::load_shape(REyebrowDown.c_str(), REyebrowDownShape);
		
		m_blendShapes[0] = HeadShape;
	}
	else{
		for(int i = 0; i < 11; i++){
			m_blendShapes[i] = HeadShape;
		}
	}

	// once loaded, we can create a View object from the neutral mesh object
	// TODO: not sure if the projections are loaded yet at this point. 
	//m_viewObj = new Views(currentVertices, projections);
    //init_frame_buffer();
	//projections.push_back(p); // this is temporary for testing
	m_viewObj = new Views(&currentVertices, &projections);
}


void Mesh::init_projections_with_textures(string prefix ){
    // Define camera centers
    Vector3f centers[] = {Vector3f(-5.0,0.0,0.0),
		                  Vector3f(-3.5,0.0,3.5),
                          //Vector3f(0.5,0.5,1.5),
                          Vector3f(0.0,0.0,5.0),
						  Vector3f(3.5,0.0,3.5),
                          Vector3f(5.0,0.0,0.0)};


    // Projeciton parameters
    //Vector3f center(0.0,0.0,5.0);
    Vector3f target(0.0,0.0,0.0);
    Vector3f up = Vector3f::UP;
    float fov = 15.0;
    float aspect = 1.0;

    // For each camera center, create projection with texture
    for (int i = 0; i < 5; ++i) {
        Projection * p = new Projection(centers[i], target, up, fov, aspect, this);
        p->updateTextureMatrix(m_camera->viewMatrix());
        p->initTextureCoords();
        // Load texture using prefix and index
        string index = static_cast<ostringstream*>( &(ostringstream() << i) )->str();
        string textFile = prefix + index + ".bmp";
        p->loadTexture(prefix.c_str());
        // Add to projeciton matrix
        projections.push_back(p);
        // Init weighted textures
        p->initWeightedTexture(4*m_camera->getWidth()*m_camera->getHeight());
    }

    m_texture_init = false;
    m_frame_init = false;
}



void Mesh::init_text() {
    // Initialize each projections texture
    for (int i = 0; i < projections.size(); ++i) {
        projections[i]->initTexture();
    }

    // Init frame buffer textures
    m_width = m_camera->getWidth();
    m_height = m_camera->getHeight();
    m_depth = 4;
    m_size = m_depth*m_camera->getWidth()*m_camera->getHeight();
    // Init the frame buffer image. Do not delete this image manually. 
    // Just call reset_final_image() when you want to clear it.
    final_image = new GLubyte[m_depth*m_camera->getWidth()*m_camera->getHeight()];
    texture_image = new GLubyte[m_depth*m_camera->getWidth()*m_camera->getHeight()]; // image for return data
    weights_image = new GLubyte[m_depth*m_camera->getWidth()*m_camera->getHeight()]; // image for return data

    // Create one OpenGL textures
    glGenTextures(1, &tex0ID);
    glGenTextures(1, &tex1ID);
    glGenTextures(1, &finalImageID);

    m_texture_init = true;
    cerr << "texture initialized " << endl;
}

void Mesh::zero_texture(GLubyte * image) {
	for (int i = 0; i < m_size; ++i) {
		image[i] = 0;
	}
}


void Mesh::bind_projeciton_text(Projection &p) {
    p.bindTexture();
}


void Mesh::init_frame_buffer() {
	
	glewExperimental=GL_TRUE;
	GLenum err=glewInit();
	if (err!= GLEW_OK) {
		cerr << "Glew init fail :( " << endl;
	}

    m_frame_init = true;
    cerr << "frame buffer initialized " << endl;
}

// TODO: pass in a list of projections 
void Mesh::multipass_render() {
    // Get width and height
	int width = m_camera->getWidth();
	int height = m_camera->getHeight();
	int size = width * height * m_depth;

    //zero_texture(final_image);	

	// Render all of my textures and weights
	//cout << "projections size = " << projections.size() << endl;
    for (int projectionIndex = 0; projectionIndex < projections.size(); projectionIndex++){
        //Projection *p = projections[projectionIndex];

        // Clear the buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render the textured image and pass in projection 
        draw_mesh(true, projectionIndex); // but with all of the colors == 0.0;

        // Read pixel data
        // Get extra values from the camra class 
		
        glReadPixels(0,
            0,
            width,
            height,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            texture_image);

        // Clear the buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render the mesh weights image mask and pass in projection 
        draw_mesh(false, projectionIndex); // but with all of the colors == 0.0;

        // Read pixel data
        // Get extra values from the camra class
        glReadPixels(0,
            0,
            width,
            height,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            weights_image);


		// begin texture combiners - http://www.opengl.org/wiki/Texture_Combiners
        // Blend texture with weight mask;
        //mult_textures(texture_image, weights_image, size);
        //set_alpha(texture_image, weights_image);
        set_projection_weighted_texture(projectionIndex);

        // Add to final output
        //add_textures(final_image, texture_image, size);
        //add_textures(final_image, texture_image, size);


        // Clear the frame buffer
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
	//delete [] result;
    //delete [] texture_image;
    //delete [] weights_image;
		
}

void Mesh::set_projection_weighted_texture(int projectionIndex) {
    Projection *p = projections[projectionIndex];
    GLubyte * weightedTexture = p->getWeightedTexture();
    int size = m_width * m_height;
    for (int i = 0; i < size; ++i) {
        int ii = i * m_depth;
        weightedTexture[ii + 0] = texture_image[ii + 0];
        weightedTexture[ii + 1] = texture_image[ii + 1];
        weightedTexture[ii + 2] = texture_image[ii + 2];
        weightedTexture[ii + 3] = weights_image[ii + 0];
        //weightedTexture[ii + 3] = (GLubyte) 255;
    }
}


void Mesh::set_alpha(GLubyte* im1, GLubyte* im2){
    int size = m_width * m_height;
    for (int i = 0; i < size; ++i) {
        im1[i* m_depth + 3] = im2[i * m_depth];
    }
}


void Mesh::mult_textures(GLubyte* im1, GLubyte* im2, int size){
    float left, right;
    GLubyte final;
	for (int i = 0; i < size ; ++i) {
        left = (float)im1[i];
        right = ((float)im2[i])/255.0;
		final = (GLubyte) left * right;
        if (final_image[i] + final > final_image[i]){
            final_image[i] += final;
        }
	}

	/*
	glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, tex0ID);
	glBindTexture(GL_TEXTURE_2D, *im_text);
	//Simply sample the texture
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//------------------------
	glActiveTexture(GL_TEXTURE1);
    glClientActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, tex1ID);
	glBindTexture(GL_TEXTURE_2D, *w_text);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	//Sample RGB, multiply by previous texunit result
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);   //Modulate RGB with RGB
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PREVIOUS);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
	//Sample ALPHA, multiply by previous texunit result -- maybe not necessary for this project
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);  //Modulate ALPHA with ALPHA
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_PREVIOUS);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
	*/
}

void Mesh::add_textures(GLubyte* stored_text, GLubyte* new_text, int size){
    
	for (int i = 0; i < size; ++i) {
        if (stored_text[i] + new_text[i] < stored_text[i]){
            stored_text[i] = 255;
        } else {
		    stored_text[i] += new_text[i];
        }
	}


	/*
	glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, *stored_text); // TODO: need to pull texture from cumulative frame buffer...
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//------------------------
	glActiveTexture(GL_TEXTURE1);
    glClientActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, *new_text); // TODO: need to find the result of multiplying is. 
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_ADD);    //Add RGB with RGB
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PREVIOUS);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
	//------------------------
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_ADD);    //Add ALPHA with ALPHA
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_PREVIOUS);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
	*/
}


/*
void Mesh::compute_norm()
{
	
	vector<Vector3f> v = currentVertices;
	vector<Tuple3u> t = faces;
	vector<Vector3f> n;

	if (SMOOTH){
		n.resize(v.size());
		for(unsigned int ii=0; ii<t.size(); ii++) {
			Vector3f a = v[t[ii][1]] - v[t[ii][0]];
			Vector3f b = v[t[ii][2]] - v[t[ii][0]];
			b=Vector3f::cross(a,b);
			for(int jj=0; jj<3; jj++) {
				n[t[ii][jj]]+=b;
			}
		}
		for(unsigned int ii=0; ii<v.size(); ii++) {
			n[ii] = n[ii]/ n[ii].abs();
		}
	}else{
		n.resize(t.size());
		for(unsigned int ii=0; ii<t.size(); ii++) {
			Vector3f a = v[t[ii][1]] - v[t[ii][0]];
			Vector3f b = v[t[ii][2]] - v[t[ii][0]];
			b=Vector3f::cross(a,b);
		  n[ii]=b.normalized();
		}

	}
	vertexNormals = n;
}
*/



void Mesh::update() {
    // Update blend weights
    updateProjectionBlendWeights();
}

void Mesh::updateProjectionBlendWeights(){
	// Only process is the camera has changed
	if (!m_camera->isUpdated()) {
		return;
	}
	m_camera->resetUpdated();

    // TODO: reset the weights using the View given by the camera
    //m_viewObj->calculate_weights(m_camera->GetCenter());

    /*
    Vector3f d = m_camera->getViewingDir();

	// Iterate through all of the faces. For each projection on each face, 
	// Compute the weights based on camera viewing dir and the incoming
	// 
    for(unsigned int index=0; index < faces.size(); index++) {
        vector<Tuple3u> face = faces[index];
        // Read verticies
        Vector3f v1 = currentVertices[face[0][0] - 1];
        Vector3f v2 = currentVertices[face[1][0] - 1];
        Vector3f v3 = currentVertices[face[2][0] - 1];
	}
    */
}


void Mesh::draw() {
    // Init the texture if it hasn't been done yet
    // This is a hack because texture loading can only be done after the window is created
    if ( !m_texture_init ) {
        init_text();
    } 
    if ( !m_frame_init ) {
        init_frame_buffer();
    } 
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.0,0.0,0.0,1.0);

    // For now, always draw projeciton 0 with texture
    //glDisable(GL_BLEND);
    //draw_mesh(true, 0);

    //glEnable(GL_BLEND);
    // Add
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    // Multiply
    //glBlendFunc(GL_DST_COLOR, GL_ZERO );
    // Subtract
    //glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);


    // Here we draw two meshes. If you have blending add enabled, 
    // The color components of the meshes well add together
    //draw_mesh(false, 0);
    //draw_mesh(false, 0);
    //draw_mesh(false, 2);
    //draw_mesh(false, 2);

    // Multipass rendering
    multipass_render();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    for (int i = 0; i < 5; ++i) {
        // Draw final image
        glDisable(GL_DEPTH_TEST);
        draw_image(projections[i]->getWeightedTexture());
        glEnable(GL_DEPTH_TEST);
    }
    glDisable(GL_BLEND);
    
}

void Mesh::draw_image(GLubyte* image) {
    int width = m_camera->getWidth();
    int height = m_camera->getHeight();

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, finalImageID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // Linear Filtering    

     // Set the GL texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, 
                 height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
                 image); 

    draw_screen_quad();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();


}

void Mesh::draw_screen_quad() {
    int width = m_camera->getWidth();
    int height = m_camera->getHeight();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();


    glDisable(GL_LIGHTING);
    // Draw a textured quad
    glColor4f(0.0,0.0,0.0,1.0);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(0, 1); glVertex3f(0, height, 0);
    glTexCoord2f(1, 1); glVertex3f(width, height, 0);
    glTexCoord2f(1, 0); glVertex3f(width, 0, 0);
    glEnd();
    glEnable (GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}



void Mesh::draw_mesh(bool useTexture, int projectionIndex) {
    // Set the corresponding projection
    Projection *p = projections[projectionIndex];

    //p->drawProjectionCamera();

    // Enable texturing
    if (m_texture_init && useTexture) {
        // Enable texturing
        glEnable(GL_TEXTURE_2D);
        //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        //glBindTexture(GL_TEXTURE_2D, texture1ID);
        // Bind the projeciton texture
        p->bindTexture();
    }
    

    // Iterate through all of the faces. Draw complete mesh.
    for(unsigned int index=0; index < faces.size(); index++) {
        vector<Tuple3u> face = faces[index];
        // Read verticies
        Vector3f v1 = currentVertices[face[0][0] - 1];
        Vector3f v2 = currentVertices[face[1][0] - 1];
        Vector3f v3 = currentVertices[face[2][0] - 1];
        // Calculate normal
		// TODO: let's see if we can use the normals given to us -- issue: it gives weird lighting
        Vector3f n = Vector3f::cross(v2 - v1, v3 - v1).normalized();
		//Vector3f n1 = vertexNormals[face[0] - 1];
		//Vector3f n2 = vertexNormals[face[1] - 1];
		//Vector3f n3 = vertexNormals[face[2] - 1];

        // Draw with or without texturing
        //if (m_texture_init) {
		if (useTexture) {
            // Read texture coordinates
            if (!p->textCoordsValid()){
                assert(false);
            }
            Vector2f t1 = p->getTextureCoord(face[0][0] - 1); 
            Vector2f t2 = p->getTextureCoord(face[1][0] - 1); 
            Vector2f t3 = p->getTextureCoord(face[2][0] - 1); 
            //cerr << "texcoord1 " <<t1[0] << " " << t1[1] << endl;
            //cerr << "texcoord2 " <<t2[0] << " " << t2[1] << endl;
            //cerr << "texcoord3 " <<t3[0] << " " << t3[1] << endl;

            //glDisable (GL_LIGHTING);
            glBegin(GL_TRIANGLES);
            // Vertex 1
            glNormal3fv(n);
            glTexCoord2fv(t1);
            glVertex3fv(v1);
            // Vertex 2
            glNormal3fv(n);
            glTexCoord2fv(t2);
            glVertex3fv(v2);
            // Vertext 3
            glNormal3fv(n);
            glTexCoord2fv(t3);
            glVertex3fv(v3);
            glEnd();
            //glEnable (GL_LIGHTING);
        } else {
            /*
            Vector3f dir = m_camera->getViewingDir();
            if(Vector3f::dot(dir, n) > 0) {
                continue;
            }
            */

            // Change this
            float brightness = 0.5;
            // Don't change this (alpha blending is not what we want);
            float alpha = 1.0;
			Vector4f c1 = Vector4f(brightness, brightness, brightness, alpha);
			Vector4f c2 = Vector4f(brightness, brightness, brightness, alpha);
			Vector4f c3 = Vector4f(brightness, brightness, brightness, alpha);
            glDisable (GL_LIGHTING);
            glBegin(GL_TRIANGLES);
            glColor4fv(c1); // this is where the weights go for each vertex. we need to do this per texture per vertex.
            glNormal3fv(n);
            glVertex3fv(v1);
            glColor4fv(c2);
            glNormal3fv(n);
            glVertex3fv(v2);
            glColor4fv(c3);
            glNormal3fv(n);
            glVertex3fv(v3);
            glEnd();
            glEnable (GL_LIGHTING);
        }
    }

    
    // Disable Texturing
    if (m_texture_init && useTexture) {
        glDisable(GL_TEXTURE_2D);
    }
    
}

