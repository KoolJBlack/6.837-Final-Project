#include "MatrixStack.h"

MatrixStack::MatrixStack()
{
	// Initialize the matrix stack with the identity matrix.
}

void MatrixStack::clear()
{
	// Revert to just containing the identity matrix.
	m_matrices.clear();
}

Matrix4f MatrixStack::top()
{
	if(m_matrices.empty()) {
		return Matrix4f::identity();
	} else {
		return m_matrices.back();
	}
}

void MatrixStack::push( const Matrix4f& m )
{
	// Push m onto the stack.
	// Your stack should have OpenGL semantics:
	// the new top should be the old top multiplied by m
	if(m_matrices.empty()) {
		m_matrices.push_back(m);
	} else {
		m_matrices.push_back(m_matrices.back() * m);
	}
}

void MatrixStack::pop()
{
	// Remove the top element from the stack
	m_matrices.pop_back();
}
