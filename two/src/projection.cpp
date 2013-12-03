#include "projection.h"
#include "Mesh.h"


using namespace std;

Projection::Projection() {

}

Projection::Projection(const Vector3f& center, const Vector3f& target, const Vector3f& up, float fov, float aspect, Mesh* m) {
	m_center = center;
	m_target = target;
    m_direction = (target - center).normalized();
    m_right = Vector3f::cross(m_direction, up).normalized();
    m_up = Vector3f::cross(m_right, m_direction).normalized();
    m_fov = fov;
    m_tanf = tan(fov/ 2.0f);
    m_aspect = aspect; // x over y

    m_base_mesh = m;

    // Init projection matrices
    m_view_mat = Matrix4f::lookAt(center, target, up);
    m_proj_mat = Matrix4f::perspectiveProjection(fov, aspect, 5, 7, false);
    m_bias = Matrix4f(0.5f, 0.0f, 0.0f, 0.5f,
                        0.0f, 0.5f, 0.0f, 0.5f,
                        0.0f, 0.0f, 0.5f, 0.5f,
                        0.0f, 0.0f, 0.0f, 1.0f);
    resetTextureMatrix();

	cerr << "PerspectiveCamera:: center=" << m_center << " fov=" << fov <<endl;
}

Projection::~Projection() {
	//delete [] m_t;
}

void Projection::updateTextureMatrix(Matrix4f model_mat ) {
    m_text_mat = m_bias * m_proj_mat * m_view_mat.inverse() * model_mat ;
}

void Projection::resetTextureMatrix() {
    m_text_mat = m_bias * m_proj_mat * m_view_mat.inverse();
}

void Projection::loadTexture(const char* filename ){
	m_t.load(filename);
}


void Projection::initTextureCoords() {
	// Get the mesh vertices
	std::vector< Vector3f > vertices = m_base_mesh->currentVertices;
	// Reset texture coordinates
	m_textureCoords = std::vector<Vector2f>();
	// For each vertex, generate its UV coordinaats
	for(unsigned int index=0; index < vertices.size(); index++) {
		Vector3f vertex = vertices[index];
		m_textureCoords.push_back(computeUV(vertex));
	}
}

Vector2f Projection::computeUV(Vector3f v){
	Vector4f strq = m_text_mat * Vector4f(v, 1.0);
	Vector2f st = Vector2f(strq[0]/strq[3], strq[1]/strq[3]);
	return st;
}


Vector2f Projection::getTextureCoord(int vertexIndex) {
	return m_textureCoords[vertexIndex];
}

Texture* Projection::getTexture(){
	return &m_t;
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

Vector3f Projection::dirToVertex(Vector3f vertex) {
	return (vertex - m_center).normalized();
}



