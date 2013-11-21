#ifndef JOINT_H
#define JOINT_H

#include <vector>
#include <vecmath.h>

struct Joint
{
	Matrix4f transform; // transform relative to its parent
	std::vector< Joint* > children; // list of children

	Joint* parent;  // Pointer to joint's parent
	unsigned index;  // Joints index within m_joints
	Vector3f pos;  // Original position passed to joint
	Vector3f color;  //  Color assinged to joint

	// This matrix transforms world space into joint space for the initial ("bind") configuration of the joints.
	Matrix4f bindWorldToJointTransform;

	// This matrix maps joint space into world space for the *current* configuration of the joints.
	Matrix4f currentJointToWorldTransform;
};

#endif
