#include "Mesh.h"
#define SMOOTH (true)

#include "helpers.h"

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
		// TODO: fill this in. 
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

}

/*
void Mesh::load_text(const char* filename ) {
    t.load(filename);
    cerr << "texture width " << t.getWidth() << " height " << t.getHeight() << endl;
    m_texture_init = false;
    m_projected_init = false;
}
*/

void Mesh::init_projections_with_textures(const char* filename ){
    // Projeciton parameters
    Vector3f center(0.0,0.0,5.0);
    Vector3f target(0.0,0.0,0.0);
    Vector3f up = Vector3f::UP;
    float fov = 15.0;
    float aspect = 1.0;
    p = Projection(center, target, up, fov, aspect, this);
    p.updateTextureMatrix(m_camera->viewMatrix());
    p.initTextureCoords();
    // Load the texture
    p.loadTexture(filename);
    t = p.getTexture();

    m_texture_init = false;
    m_projected_init = false;
}



void Mesh::init_text() {
    // Create one OpenGL textures
    glGenTextures(1, &texture1ID);

    // "Bind" the newly created texture : all futuhre texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, texture1ID);

    // Nice trilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // Linear Filtering
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // Linear Filtering    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering
    
    // Creat the GL texture data
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Set the GL texture
    GLubyte* image = t->getGLTexture();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t->getWidth(), 
                 t->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, 
                 image); 
    delete [] image;


    m_texture_init = true;
    cerr << "texture initialized " << endl;
}

void Mesh::init_projective_text() {

	
	// Kai trying out frame buffers

	// adding glewInit ??? how does this work?
	glewExperimental=GL_TRUE;
	GLenum err=glewInit();
	if (err!= GLEW_OK) {
		cerr << "Glew init fail :( " << endl;
	}
	//glewInit();

	
    m_projected_init = true;
    cerr << "projected initialized " << endl;

}

// TODO: pass in a list of projections 
void Mesh::create_frame_buffer(int viewNum)
{
	

	/*
	// the texture we're going to render to
	// using what Kojo had before. 

    // Create one OpenGL textures
    glGenTextures(1, &texture2ID);

    // "Bind" the newly created texture : all futuhre texture functions will modify this texture
	glActiveTexture(GL_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, texture2ID);
	
	project_texture();

    // Nice trilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Linear Filtering

    // Set the GL texture
    GLubyte* image = t->getGLTexture();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t->getWidth(), 
                 t->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, 
                 image); 
    delete [] image;

    //Set up texture coordinate generation.
    Matrix4f m =  Matrix4f::identity();
    Matrix4f mm = Matrix4f(0.5f, 0.0f, 0.0f, 0.5f,
                            0.0f, 0.5f, 0.0f, 0.5f,
                            0.0f, 0.0f, 0.5f, 0.5f,
                            0.0f, 0.0f, 0.0f, 1.0f);    

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_S, GL_EYE_PLANE, m.getRow(0));

    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_T, GL_EYE_PLANE,  m.getRow(1));

    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_R, GL_EYE_PLANE, m.getRow(2));

    glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_Q, GL_EYE_PLANE, m.getRow(3));
	
	// end Kojo's create texture

	*/
	/*
	// render texture that's color only so that we can see if we can get weights on. as color.
	GLuint texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 600, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	*/


	// Initial frame buffer stuff. Before loop happens.
	GLuint framebuf0;
	glGenFramebuffers(1, &framebuf0);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuf0);

	// create a texture object
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 600, 600, 0,
				 GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);


	
	// depth buffer
	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 600, 600); 
	glBindRenderbuffer(GL_RENDERBUFFER, 0);


	//attach the texture to FBO color attachment point
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture2ID, 0);
	//attach the renderbuffer to depth attachment point
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);


	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, 
	//	GL_RENDERBUFFER, depthrenderbuffer);

	// configure frame buffer
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture2ID, 0);
		

	// set the list of draw buffers
	//GLenum Drawbuffers[2] = {GL_COLOR_ATTACHMENT0};
	//glDrawBuffers(1, Drawbuffers);

	

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		//return false;
		cout << "should return false" << endl;
	}

	//switch back to window-system-provided frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//set rendering destination to FBO	
	glBindFramebuffer(GL_FRAMEBUFFER, framebuf0);

	//clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_mesh(); // but with all of the colors == 0.0;

	// unbind FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	
	//loop stuff
	for (int j = 0; j < viewNum; j++){

		// stuff for blending? not sure how to use without shaders. 

		/*
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE); // first arg should be either src_alpha or gl_one
		glShadeModel(GL_SMOOTH); // need to look into this
		glClearColor(0.0, 0.0, 0.0, 0.0);

		// display
		glClear(GL_COLOR_BUFFER_BIT);
		draw_mesh(); // with the given uv projection for texture
		glFlush();

		*/


		// begin texture combiners - http://www.opengl.org/wiki/Texture_Combiners


		// create two framebuffers first so that we can get textures to multiply
		GLuint fbuf0;
		glGenFramebuffers(1, &fbuf0);
		glBindFramebuffer(GL_FRAMEBUFFER, fbuf0);
		GLuint tex0; // make this the image texture from the view 

		glGenTextures(1, &tex0);
		glBindTexture(GL_TEXTURE0, tex0);
		glTexParameterf(GL_TEXTURE0, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE0, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE0, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE0, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE0, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
		glTexImage2D(GL_TEXTURE0, 0, GL_RGBA8, 600, 600, 0,
				 GL_RGBA, GL_UNSIGNED_BYTE, 0);

		

			// depth buffer
		GLuint drbuf0;
		glGenRenderbuffers(1, &drbuf0);
		glBindRenderbuffer(GL_RENDERBUFFER, drbuf0);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 600, 600); 
		glBindRenderbuffer(GL_RENDERBUFFER, 0);


		//attach the texture to FBO color attachment point
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex0, 0);
		//attach the renderbuffer to depth attachment point
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, drbuf0);


		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			//return false;
			cout << "should return false" << endl;
		}

		//switch back to window-system-provided frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//set rendering destination to FBO	
		glBindFramebuffer(GL_FRAMEBUFFER, fbuf0);

		//clear buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// TODO: draw mesh with the uv projection and bind that to texture0...

		// unbind FBO
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		

		// end render to texture for view image



		GLuint fbuf1;
		glGenFramebuffers(1, &fbuf1);
		glBindFramebuffer(GL_FRAMEBUFFER, fbuf1); 
		GLuint tex1; // this the color/weight  texture. 

		glGenTextures(1, &tex1);
		glBindTexture(GL_TEXTURE1, tex1);
		glTexParameterf(GL_TEXTURE1, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE1, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE1, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE1, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE1, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
		glTexImage2D(GL_TEXTURE1, 0, GL_RGBA8, 600, 600, 0,
				 GL_RGBA, GL_UNSIGNED_BYTE, 0);


		// depth buffer
		GLuint drbuf1;
		glGenRenderbuffers(1, &drbuf1);
		glBindRenderbuffer(GL_RENDERBUFFER, drbuf1);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 600, 600); 
		glBindRenderbuffer(GL_RENDERBUFFER, 0);


		//attach the texture to FBO color attachment point
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, tex1, 0);
		//attach the renderbuffer to depth attachment point
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, drbuf1);


		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			//return false;
			cout << "should return false" << endl;
		}

		//switch back to window-system-provided frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//set rendering destination to FBO	
		glBindFramebuffer(GL_FRAMEBUFFER, fbuf1);

		//clear buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// TODO: draw the mesh with the weights and bind that to the texture1...
		
		// unbind FBO
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		

		// end render to texture for color/weight

		// begin the process of combining the textures 
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex0);
		//Simply sample the texture
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		//------------------------
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex1);
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


		// TODO: add this to framebuf0, our cumulative buffer
		// can possibly use add textures...
		// textureID is the cumulative texture in framebuf0

		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D, textureID); // TODO: need to pull texture from cumulative frame buffer...
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		//------------------------
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex0); // TODO: need to find the result of multiplying is. 
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

		// clean up by discarding the frame buffers

	}

	

	//glBindFramebuffer(GL_FRAMEBUFFER, framebuf0);
	//glViewport(0,0,600,600);


	//GL_FUNC_ADD(GL_ONE, GL_ONE);

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

void Mesh::project_texture() {
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glTranslatef(0.5, 0.5, 0.0);  // Scale and bias the [-1,1] NDC values 
    glScalef(0.5, 0.5, 1.0);  // to the [0,1] range of the texture map
    gluPerspective(15, 1, 5, 7);  // projector "projection" and view matrices
    gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    //glMultMatrixf( m_camera->viewMatrix() );
    //glMultMatrixf(m_camera->GetRotation().inverse());
    glMultMatrixf(Matrix4f::translation(-m_camera->GetCenter()).inverse());
    //glMultMatrixf(m_camera->GetRotation().inverse());
    glMatrixMode(GL_MODELVIEW);
}


void Mesh::draw() {
    // Init the texture if it hasn't been done yet
    // This is a hack because texture loading can only be done after the window is created
    if (t->valid() && !m_texture_init ) {
        init_text();
    } 
    if (t->valid() && !m_projected_init ) {
        //init_projective_text();
    }


    if (m_projected_init) {	
		create_frame_buffer();
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glEnable(GL_TEXTURE_GEN_R);
        glEnable(GL_TEXTURE_GEN_Q);
    }

    // Draw the mesh and texture that we project onto
    draw_mesh();

    if (m_projected_init) {
      glDisable(GL_TEXTURE_2D);
      glDisable(GL_TEXTURE_GEN_S);
      glDisable(GL_TEXTURE_GEN_T);
      glDisable(GL_TEXTURE_GEN_R);
      glDisable(GL_TEXTURE_GEN_Q);
    }


}

void Mesh::draw_mesh() {
    // Enable texturing
    
    if (m_texture_init) {
        glEnable(GL_TEXTURE_2D);
        //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, texture1ID);
    }
    

    // Iterate through all of the faces. Draw complete mesh.
    for(unsigned int index=0; index < faces.size(); index++) {
        vector<Tuple3u> face = faces[index];
        // Read verticies
        Vector3f v1 = currentVertices[face[0][0] - 1];
        Vector3f v2 = currentVertices[face[1][0] - 1];
        Vector3f v3 = currentVertices[face[2][0] - 1];
        // Read colors
        Vector3f c1 = vertexColors[face[0][0] - 1];
        Vector3f c2 = vertexColors[face[1][0] - 1];
        Vector3f c3 = vertexColors[face[2][0] - 1];
        // Calculate normal
		// TODO: let's see if we can use the normals given to us -- issue: it gives weird lighting
        Vector3f n = Vector3f::cross(v2 - v1, v3 - v1).normalized();
		//Vector3f n1 = vertexNormals[face[0] - 1];
		//Vector3f n2 = vertexNormals[face[1] - 1];
		//Vector3f n3 = vertexNormals[face[2] - 1];

        // Draw with or without texturing
        if (m_texture_init) {
            // Read texture coordinates
            //Vector2f t1 = textureCoords[face[0][1] - 1]; 
            //Vector2f t2 = textureCoords[face[1][1] - 1]; 
            //Vector2f t3 = textureCoords[face[2][1] - 1];
            if (p.textCoordsValid()){

            }
            Vector2f t1 = p.getTextureCoord(face[0][0] - 1); 
            Vector2f t2 = p.getTextureCoord(face[1][0] - 1); 
            Vector2f t3 = p.getTextureCoord(face[2][0] - 1); 
            //cerr << "texcoord1 " <<t1[0] << " " << t1[1] << endl;
            //cerr << "texcoord2 " <<t2[0] << " " << t2[1] << endl;
            //cerr << "texcoord3 " <<t3[0] << " " << t3[1] << endl;

            //glDisable (GL_LIGHTING);
            glBegin(GL_TRIANGLES);
            // Vertex 1
            glNormal3fv(n);
            glTexCoord2fv(t1);
            //glTexCoord2f(0.0,0.0);
            glVertex3fv(v1);
            // Vertex 2
            glNormal3fv(n);
            glTexCoord2fv(t2);
            //glTexCoord2f(0.0,1.0);
            glVertex3fv(v2);
            // Vertext 3
            glNormal3fv(n);
            glTexCoord2fv(t3);
            //glTexCoord2f(1.0,0.0);
            glVertex3fv(v3);
            glEnd();
            //glEnable (GL_LIGHTING);
        } else {
            //glDisable (GL_LIGHTING);
            //glEnable(GL_COLOR_MATERIAL);
            glBegin(GL_TRIANGLES);
            glColor3fv(c1); // this is where the weights go for each vertex. we need to do this per texture per vertex.
			//glColor4fv(Vector4f(c1, 0.2));
            glNormal3fv(n);
            glVertex3fv(v1);
            glColor3fv(c2);
            glNormal3fv(n);
            glVertex3fv(v2);
            glColor3fv(c3);
            glNormal3fv(n);
            glVertex3fv(v3);
            glEnd();
            //glEnable (GL_LIGHTING);
        }
    }

    
    // Disable Texturing
    if (m_texture_init) {
        glDisable(GL_TEXTURE_2D);
    }
    
}

void Mesh::loadAttachments( const char* filename, int numJoints )
{
	// 2.2. Implement this method to load the per-vertex attachment weights
	// this method should update m_mesh.attachments
	// 2.1.1. load() should populate bindVertices, currentVertices, and faces

	// Add your code here.
	ifstream in(filename);
    if (!in) {
        cerr<< filename << " not found\a" << endl;
        exit(0);
    }

    cerr << endl << "*** Mesh::loadAttachments reading in file " << filename << " ***" << endl;

    // load the OBJ file here
    string line;
    while (std::getline(in, line)) {
        // Parse line values through string stream.
        stringstream ss(line);
        float f;
        vector<float> weights;
        while (ss >> f) {
        	weights.push_back(f);
        }

        //cerr << endl << "weights size " << weights.size() << endl;
        attachments.push_back(weights);
    } 
}

