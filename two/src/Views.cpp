#include "views.h"

using namespace std;

Views::Views(vector<Vector3f>* vertices, vector<Projection*>* projections){
	v_vertices = vertices;
	v_projections = projections;
}

Views::~Views(){
}

void Views::calculate_weights(Vector3f cam_center){
	vector<Vector3f>& vs = *v_vertices;
	vector<Projection*>& ps = *v_projections;
	// use headshape and not vertices because it's the neutral shape

	//v_weights = vector<vector<float> >();
	v_weights.clear();
	//v_weights = vector<vector<float>>();
	// look at each vertex
	for (unsigned int i = 0; i < v_vertices->size(); i++){
		vector<float> weights;
		// for each projection, find the dot product
		for (unsigned int j = 0; j < ps.size(); j++){
			Vector3f projDir = ps[j]->dirToProjection(vs[i]);
			Vector3f camDir = -(cam_center - vs[i]).normalized();
			float w = Vector3f::dot(projDir, camDir);
			weights.push_back(max(0.0f,w)); // not dealing with occlusion right now
		}

		//figure out the three closest projections according to their weights
		int maxind[3] = {0,0,0}; 
		for (unsigned int j = 1; j < ps.size(); j++){
			if (weights[j] >= weights[maxind[0]]){
				//reset the weight first
				weights[maxind[2]] = 0.0f;				
				// shift the indices
				maxind[2] = maxind[1];
				maxind[1] = maxind[0];
				maxind[0] = j;				
			}
			else if (weights[j] >= weights[maxind[1]]){	
				weights[maxind[2]] = 0.0f;			
				maxind[2] = maxind[1];
				maxind[1] = j;
			}
			else if (weights[j] >= weights[maxind[2]]) {
				weights[maxind[2]] = 0.0f;	
				maxind[2] = j;
			}
			else if (maxind[0] == maxind[1]){
				maxind[1] = j;
				maxind[2] = j;
			}
			else if (maxind[1] == maxind[2]){
				maxind[2] = j;
			}
			else 
			{
				// weight is too small, not one of the closest, set to 0
				weights[j] = 0.0f;
			}
		} // end for each projection
		weights[maxind[0]] *= 3.5;
		float total_weight = weights[maxind[0]] + weights[maxind[1]] + weights[maxind[2]];
		// normalize the weights. 
		weights[maxind[0]] = weights[maxind[0]] / total_weight;
		weights[maxind[1]] = weights[maxind[1]] / total_weight;
		weights[maxind[2]] = weights[maxind[2]] / total_weight;
		v_weights.push_back(weights);

	} // end for each vertex
}