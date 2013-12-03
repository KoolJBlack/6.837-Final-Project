#include "BlendShape.h"

#include "helpers.h"

using namespace std;

BlendShape::BlendShape()
{
	b_vertices = vector<Vector3f>();
	b_normals = vector<Vector3f>();
	//b_faces = vector<vector<Tuple3u>>();
}

BlendShape& BlendShape::load_shape(const char* filename, BlendShape& shape){
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
			shape.b_vertices.push_back(v);
            // Set the default color
            //vertexColors.push_back(Vector3f(1.0,1.0,1.0));
        } 
		/*else if (loadFace && s == "f") {
            string abc, def,ghi;
            ss >> abc >> def >> ghi;
			
            vector<string> f1 = split(abc, '/');
            vector<string> f2 = split(def, '/');
            vector<string> f3 = split(ghi, '/');
			
            unsigned a = atoi(f1[0].c_str());
            unsigned d = atoi(f2[0].c_str());
            unsigned g = atoi(f3[0].c_str());
            shape.b_faces.push_back(Tuple3u(a, d, g));
			
			
        } 
		*/
		else if (s == "vn") {// normals
			Vector3f curr = Vector3f(0,0,0);
			ss >> curr[0] >> curr[1] >> curr[2];
			shape.b_normals.push_back(curr);
		} 
		
		// TODO: deal with texture coordinates later. might need to be done, or just get rid of this
		/*else if (s == "vt") {
			Vector2f texcoord;
			ss>>texcoord[0];
			ss>>texcoord[1];
			textureCoords.push_back(texcoord);
		} */
		
		else {
            // ignore remaining tokens
        }
    } 
    // end while cin
	cerr << "b_vertices size " << shape.b_vertices.size() << endl;
    cerr << "v_normals size " << shape.b_normals.size() << endl;

	if (shape.b_normals.size() == 0){ //compute the normals because we don't have that.
		//TODO: fill this in.
		cout << "need to calculate normals" << endl;
	}

	return shape;
}

BlendShape::~BlendShape(){
}