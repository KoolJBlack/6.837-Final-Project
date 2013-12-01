#include "Mesh.h"
#define SMOOTH (true)

using namespace std;

// These are utility functions for splitting strings
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
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
		objString = f + "/HeadShape.obj";
		ifstream shapes(objString.c_str());
		if (shapes){
			cerr << endl << "*** Mesh::load reading in blend shapes " << objString.c_str() << " ***" << endl;
			useShapes = true;
		}
		else{
			cerr << objString << " not found. exiting." << endl;
			exit(0);
		}	
	}
	
    if (useShapes){
		// hardcoding this part to fit the blend shapes we are using. 

		// start with head shape. We will use this as a default if other shapes are suddenly missing
		BlendShape::load_shape(objString.c_str(), HeadShape);
		
		// load the headshape as the default
		bindVertices = HeadShape.b_vertices;
		faces = HeadShape.b_faces;
		vertexNormals = HeadShape.b_normals;
		currentVertices = bindVertices;

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
		
		BlendShape::load_shape(LFrown.c_str(), LFrownShape); 
		BlendShape::load_shape(LSmile.c_str(), LSmileShape);
		BlendShape::load_shape(LEyebrowUp.c_str(), LEyebrowUpShape);
		BlendShape::load_shape(LEyebrowDown.c_str(), LEyebrowDownShape);
		BlendShape::load_shape(LEyeClose.c_str(), LEyeCloseShape);
		BlendShape::load_shape(RFrown.c_str(), RFrownShape); 
		BlendShape::load_shape(RSmile.c_str(), RSmileShape);
		BlendShape::load_shape(REyebrowUp.c_str(), REyebrowUpShape);
		BlendShape::load_shape(REyebrowDown.c_str(), REyebrowDownShape);
		BlendShape::load_shape(REyeClose.c_str(), REyeCloseShape);
		

		return; 
	}
	

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
            //vector<Vector3f> face;
            string abc, def,ghi;
            ss >> abc >> def >> ghi;
            vector<string> f1 = split(abc, '/');
            vector<string> f2 = split(def, '/');
            vector<string> f3 = split(ghi, '/');
            unsigned a = atoi(f1[0].c_str());
            unsigned d = atoi(f2[0].c_str());
            unsigned g = atoi(f3[0].c_str());
            faces.push_back(Tuple3u(a, d, g));
        } else if (s == "vn") {// normals
			vertexNormals.push_back(Vector3f(0,0,0));
			Vector3f& curr = vertexNormals.back();
			ss >> curr[0] >> curr[1] >> curr[2];
		} else if (s == "vt") {
			Vector2f texcoord;
			ss>>texcoord[0];
			ss>>texcoord[1];
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
		// TODO: fill this in. 
	}

    // Load the texture if possible

}

/*
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
            vector<Vector3f> face;
            string abc, def,ghi;
            ss >> abc >> def >> ghi;
            vector<string> f1 = split(abc, '/');
            vector<string> f2 = split(def, '/');
            vector<string> f3 = split(ghi, '/');
            unsigned a = atoi(f1[0].c_str());
            unsigned d = atoi(f2[0].c_str());
            unsigned g = atoi(f3[0].c_str());
            faces.push_back(Tuple3u(a, d, g));
        } else if (s == "vn") {// normals
			vertexNormals.push_back(Vector3f(0,0,0));
			Vector3f& curr = vertexNormals.back();
			ss >> curr[0] >> curr[1] >> curr[2];
		} else if (s == "vt") {
			Vector2f texcoord;
			ss>>texcoord[0];
			ss>>texcoord[1];
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

    // Load the texture if possible

}
*/
/*
// helper function to load all of the blend shapes initially 
void Mesh::load_shape(const char *filename, vector<Vector3f>& vertices, vector<Tuple3u>& faces, vector<Vector3f>& normals) {

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
            vertices.push_back(v);
            // Set the default color
            vertexColors.push_back(Vector3f(1.0,1.0,1.0));
        } else if (s == "f") {
            string abc, def,ghi;
            ss >> abc >> def >> ghi;
            vector<string> f1 = split(abc, '/');
            vector<string> f2 = split(def, '/');
            vector<string> f3 = split(ghi, '/');
            unsigned a = atoi(f1[0].c_str());
            unsigned d = atoi(f2[0].c_str());
            unsigned g = atoi(f3[0].c_str());
            faces.push_back(Tuple3u(a, d, g));
        } else if (s == "vn") {// normals
			Vector3f& curr = Vector3f(0,0,0);
			ss >> curr[0] >> curr[1] >> curr[2];
			normals.push_back(curr);
		} 
		
		// TODO: deal with texture coordinates later. might need to be done, or just get rid of this
		else if (s == "vt") {
			Vector2f texcoord;
			ss>>texcoord[0];
			ss>>texcoord[1];
			textureCoords.push_back(texcoord);
		} 
		
		else {
            // ignore remaining tokens
        }
    } 
    // end while cin

	// make a copy of the bind vertices as the current vertices
	currentVertices = bindVertices;

	if (vertexNormals.size() == 0){ //compute the normals because we don't have that.

	}
}
*/

void Mesh::load_text(const char* filename ) {
    t.load(filename);
    cerr << "texture width " << t.getWidth() << " height " << t.getHeight() << endl;
    m_texture_init = false;
}

void Mesh::init_text() {
    // Create one OpenGL texture
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Nice trilinear filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t.getWidth(), 
                 t.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, 
                 t.getData());

    m_texture_init = true;
    cerr << "texture initialized " << endl;
}


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

void Mesh::draw() {
    // Init the texture if it hasn't been done yet
    if (t.valid() && !m_texture_init) {
        init_text();
    } 
	// Since these meshes don't have normals
	// be sure to generate a normal per triangle.
	// Notice that since we have per-triangle normals
	// rather than the analytical normals from
	// assignment 1, the appearance is "faceted".

    // Enable texturing
    if (t.valid()) {
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

	// Iterate through all of the faces. Draw complete mesh.
    for(unsigned int index=0; index < faces.size(); index++) {
        Tuple3u face = faces[index];
        // Read verticies
        Vector3f v1 = currentVertices[face[0] - 1];
        Vector3f v2 = currentVertices[face[1] - 1];
        Vector3f v3 = currentVertices[face[2] - 1];
        // Read colors
        Vector3f c1 = vertexColors[face[0] - 1];
        Vector3f c2 = vertexColors[face[1] - 1];
        Vector3f c3 = vertexColors[face[2] - 1];
        // Calculate normal
        Vector3f n = Vector3f::cross(v2 - v1, v3 - v1).normalized();

        // Draw with or without texturing
        if (m_texture_init) {
            //glDisable (GL_LIGHTING);
            glEnable(GL_COLOR_MATERIAL);
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
        } else {
            //glDisable (GL_LIGHTING);
            glEnable(GL_COLOR_MATERIAL);
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
    if (t.valid()) {
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

