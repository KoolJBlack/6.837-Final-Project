#include "Mesh.h"
#define SMOOTH (true)

#include "helpers.h"

using namespace std;


void Mesh::setCamera(Camera * c) {
    m_camera = c;
}

void Mesh::load_mesh( const char* filename )
{
	// 2.1.1. load() should populate bindVertices, currentVertices, and faces

	// Add your code here.
	ifstream in(filename);
    if (!in) {
        cerr<< filename << " not found\a" << endl;
        exit(0);
    }

    cerr << endl << "*** Mesh::load reading in file " << filename << " ***" << endl;

    // load the OBJ file here
    string line;
    while (std::getline(in, line)) {
        // Parse line values through string stream.
        stringstream ss(line);
        string s;
        ss >> s; 

        if (s == "v") {
            Vector3f v;
            ss >> v[0] >> v[1] >> v[2];
            bindVertices.push_back(v);
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
    cerr << "bindVertices size " << bindVertices.size() << endl;
    cerr << "textCoords size " << textureCoords.size() << endl;
    cerr << "faces size " << faces.size() << endl;
    cerr << "normals size " << vertexNormals.size() << endl;

	// make a copy of the bind vertices as the current vertices
	currentVertices = bindVertices;

	if (vertexNormals.size() == 0){ //compute the normals because we don't have that.

	}

}

void Mesh::load_text(const char* filename ) {
    t.load(filename);
    cerr << "texture width " << t.getWidth() << " height " << t.getHeight() << endl;
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
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // Linear Filtering    
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering
    
    // Creat the GL texture data
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Set the GL texture
    GLubyte* image = t.getGLTexture();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t.getWidth(), 
                 t.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, 
                 image); 
    delete [] image;


    m_texture_init = true;
    cerr << "texture initialized " << endl;
}

void Mesh::init_projective_text() {
    // Create one OpenGL textures
    glGenTextures(1, &texture2ID);

    // "Bind" the newly created texture : all futuhre texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, texture2ID);

    // Nice trilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // Linear Filtering

    // Set the GL texture
    GLubyte* image = t.getGLTexture();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t.getWidth(), 
                 t.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, 
                 image); 
    delete [] image;

    //Set up texture coordinate generation.
    Matrix4f mm =  Matrix4f::identity();
    Matrix4f m = Matrix4f(0.5f, 0.0f, 0.0f, 0.5f,
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

    m_projected_init = true;
    cerr << "projected initialized " << endl;
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
    //glTranslatef(0.5, 0.5, 0.0);  // Scale and bias the [-1,1] NDC values 
    //glScalef(0.5, 0.5, 1.0);  // to the [0,1] range of the texture map
    //gluPerspective(15, 1, 5, 7);  // projector "projection" and view matrices
    gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    //m_camera->SetPerspective( 50.0f );
    //glLoadMatrixf( m_camera->projectionMatrix() );
    glMatrixMode(GL_MODELVIEW);
}


void Mesh::draw() {
    // Init the texture if it hasn't been done yet
    // This is a hack because texture loading can only be done after the window is created
    if (t.valid() && !m_texture_init ) {
        //init_text();
    } 
    if (t.valid() && !m_projected_init ) {
        init_projective_text();
    } 

    if (m_projected_init) {
        glBindTexture(GL_TEXTURE_2D, texture2ID);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glEnable(GL_TEXTURE_GEN_R);
        glEnable(GL_TEXTURE_GEN_Q);
        project_texture();
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
        Vector3f n = Vector3f::cross(v2 - v1, v3 - v1).normalized();

        // Draw with or without texturing
        if (m_texture_init) {
            // Read texture coordinates
            Vector2f t1 = textureCoords[face[0][1] - 1]; 
            Vector2f t2 = textureCoords[face[1][1] - 1]; 
            Vector2f t3 = textureCoords[face[2][1] - 1]; 
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
            glColor3fv(c1);
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

