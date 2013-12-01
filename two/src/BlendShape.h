#ifndef BLENDSHAPE_H
#define BLENDSHAPE_H

#include <vector>
#include <vecmath.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "extra.h"
#include "tuple.h"

typedef tuple< unsigned, 3 > Tuple3u;


class BlendShape {

public:
	BlendShape();
	static void load_shape(const char* filename, BlendShape& shape);

	~BlendShape();

	std::vector<Vector3f> b_vertices;
	std::vector<Vector3f> b_normals;
	std::vector<Tuple3u> b_faces;


};

#endif