#ifndef PROJECTION_HPP
#define PROJECTION_HPP
#include "texture.h"
#include "extra.h"
#include <vector>
#include <cmath>
#include <iostream>

class Mesh;

class Projection {
public:
  Projection();
  Projection(const Vector3f& center, const Vector3f& target, const Vector3f& up, float fov, float aspect, Mesh* m);
  ~Projection();

  // Setup projection matrices
  void updateTextureMatrix(Matrix4f model_mat );
  void resetTextureMatrix();

  // Texture methods
  void loadTexture(const char* filename );
  void initTexture();
  void bindTexture();

  // Texture coordinate methods
  void initTextureCoords();
  bool textCoordsValid();
  Vector3f computeUV(Vector3f v);

  // UV text coordinate accessor
  Vector2f getTextureCoord(int vertexIndex);
  Texture * getTexture();

  // Blend weight getters/setters
  void setVertexBlendWeights(std::vector<float> weights);
  void setVertexBlendWeight(int vertexIndex, float weight);
  std::vector<float> getVertexBlendWeights();
  float getVertexBlendWeight(int vertexIndex);

  // Returns the vertex to center of projection vector (normalized)
  Vector3f dirToProjection(Vector3f vertex);
  Vector3f dirToVertex(Vector3f vertex);

  void drawProjectionCamera();

  bool isVertexOccluded(int vertexIndex);

  void initWeightedTexture(int size);
  GLubyte * getWeightedTexture();


private:
  // Mesh and texture members
  Texture m_t;
  GLuint m_textureID;

  Mesh* m_base_mesh;

  // Texture coordinates
  std::vector< Vector2f > m_textureCoords;
  bool m_texture_coords_init;
  // Blend weights
  std::vector< float > m_vertexBlendWeights;

  // Camera projection members
  Vector3f m_center; 
  Vector3f m_direction;
  Vector3f m_up;
  Vector3f m_right;
  Vector3f m_target;

  float m_fov;
  float m_tanf;
  float m_aspect;
  Matrix4f m_view_mat;
  Matrix4f m_proj_mat;
  Matrix4f m_bias;
  Matrix4f m_text_mat;

  GLubyte * m_weightedTexture;

};

#endif
