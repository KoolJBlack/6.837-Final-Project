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
    

    Matrix4f scaling = Matrix4f::scaling(0.01,0.01,0.01);

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
            v = (scaling * Vector4f(v,1)).xyz();
			shape.b_vertices.push_back(v);
            // Set the default color
            //vertexColors.push_back(Vector3f(1.0,1.0,1.0));
        } 
		/*
		else if (s == "vn") {// normals
			Vector3f curr = Vector3f(0,0,0);
			ss >> curr[0] >> curr[1] >> curr[2];
			shape.b_normals.push_back(curr);
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
		cout << "ignoring normals" << endl;
	}

	return shape;
}

BlendShape::~BlendShape(){
}