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

void Mesh::load_text(const char* filename ) {

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

void Mesh::draw()
{
	// Since these meshes don't have normals
	// be sure to generate a normal per triangle.
	// Notice that since we have per-triangle normals
	// rather than the analytical normals from
	// assignment 1, the appearance is "faceted".

	// Iterate through all of the faces. Draw complete mesh.
    for(unsigned int index=0; index < faces.size(); index++) {
        Tuple3u face = faces[index];
        Vector3f v1 = currentVertices[face[0] - 1];
        Vector3f v2 = currentVertices[face[1] - 1];
        Vector3f v3 = currentVertices[face[2] - 1];
        Vector3f c1 = vertexColors[face[0] - 1];
        Vector3f c2 = vertexColors[face[1] - 1];
        Vector3f c3 = vertexColors[face[2] - 1];
        Vector3f n = Vector3f::cross(v2 - v1, v3 - v1).normalized();

        //glDisable (GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);
        glBegin(GL_TRIANGLES);
        glColor3fv(c1);
        glNormal3d(n[0], n[1], n[2]);
        glVertex3d(v1[0], v1[1], v1[2]);
        glColor3fv(c2);
        glNormal3d(n[0], n[1], n[2]);
        glVertex3d(v2[0], v2[1], v2[2]);
        glColor3fv(c3);
        glNormal3d(n[0], n[1], n[2]);
        glVertex3d(v3[0], v3[1], v3[2]);
        glEnd();
        //glEnable (GL_LIGHTING);

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

