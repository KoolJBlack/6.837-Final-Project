#ifndef VIEWS_H
#define VIEWS_H

#include "projection.h"

// need to create a new view each time the camera moves
class Views {
public:
	Views(std::vector<Vector3f> vertices, std::vector<Projection> projections);
	~Views();

	std::vector<Projection> v_projections;

	std::vector<std::vector<float> > v_weights; //size = currentVertices.size()
	//each element is a list of weights with size = projections.size()

	void calculate_weights(Vector3f cam_center);
	
	std::vector<Vector3f> v_vertices;

private:
};


#endif