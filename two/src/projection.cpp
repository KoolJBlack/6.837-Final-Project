#include "projection.h"  
using namespace std;



Projection::Projection(const Vector3f& center, const Vector3f& target, const Vector3f& up, float fov, float aspect, Mesh* m,Texture* t) {
	m_center = center;
    m_direction = (target - center).normalized();
    m_right = Vector3f::cross(m_direction, up).normalized();
    m_up = Vector3f::cross(m_right, m_direction).normalized();
    m_fov = fov;
    m_tanf = tan(fov/ 2.0f);
    m_aspect = aspect; // x over y

    m_base_mesh = m;
    m_t = t;

	cerr << "PerspectiveCamera:: center=" << m_center << " fov=" << fov <<endl;
}

Projection::~Projection() {
	//delete [] m_t;
}

void Projection::initTextureCoords() {

}


Vector2f Projection::getTextureCoord(int vertexIndex) {
	return m_textureCoords[vertexIndex];
}

Texture* Projection::getTexture(){
	return m_t;
}

void Projection::setVertexBlendWeights(std::vector<float> weights) {
	m_vertexBlendWeights = weights;
}

std::vector<float> Projection::getVertexBlendWeights() {
	return m_vertexBlendWeights;
}

float Projection::getVertexBlendWeight(int vertexIndex) {
	return m_vertexBlendWeights[vertexIndex];
}

Vector3f Projection::dirToProjection(Vector3f vertex) {
	return (m_center - vertex).normalized();
}



