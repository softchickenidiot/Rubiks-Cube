#pragma once

#include "Transformations.h"
#include "PerspectiveInfo.h"
#include "Camera.h"
#include "glm.hpp"

using namespace glm;

class Pipeline
{
	vec3 m_scale;
	vec3 m_worldPos;
	vec3 m_rotateInfo;
	PersProjInfo m_persProj;
	mat4 m_transformation;

public:
	Pipeline() {
		m_scale = vec3(1.0f, 1.0f, 1.0f);
		m_worldPos = vec3(0.0f, 0.0f, 0.0f);
		m_rotateInfo = vec3(0.0f, 0.0f, 0.0f);
	}

	struct {
		vec3 Pos;
		vec3 Target;
		vec3 Up;
	}m_camera;

	void Scale(float s);
	void Scale(float ScaleX, float ScaleY, float ScaleZ);
	void Scale(vec3 scale);
	void WorldPos(float x, float y, float z);
	void WorldPos(vec3 XYZ);
	void Rotate(float RotateX, float RotateY, float RotateZ);
	void Rotate(vec3 rotateXYZ);
	void setCamera(vec3 cameraPos, vec3 cameraTarget, vec3 cameraUp);
	void setCamera(const Camera& camera);
	void setPersProj(const PersProjInfo& p);
	const mat4* getWVPTrans();
};
