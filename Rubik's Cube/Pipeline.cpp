#include "Pipeline.h"

void Pipeline::Scale(float s)
{
	Scale(s, s, s);
}

void Pipeline::Scale(float ScaleX, float ScaleY, float ScaleZ)
{
	m_scale[0] = ScaleX;
	m_scale[1] = ScaleY;
	m_scale[2] = ScaleZ;
}

void Pipeline::Scale(vec3 scale) {
	m_scale = scale;
}

void Pipeline::WorldPos(float x, float y, float z)
{
	m_worldPos[0] = x;
	m_worldPos[1] = y;
	m_worldPos[2] = z;
}

void Pipeline::WorldPos(vec3 XYZ)
{
	m_worldPos = XYZ;
}

void Pipeline::Rotate(float RotateX, float RotateY, float RotateZ)
{
	m_rotateInfo[0] = RotateX;
	m_rotateInfo[1] = RotateY;
	m_rotateInfo[2] = RotateZ;
}

void Pipeline::Rotate(vec3 RotateXYZ)
{
	m_rotateInfo = RotateXYZ;
}

void Pipeline::setCamera(vec3 cameraPos, vec3 cameraTarget, vec3 cameraUp)
{
	m_camera.Pos = cameraPos;
	m_camera.Target = cameraTarget;
	m_camera.Up = cameraUp;
}

void Pipeline::setCamera(const Camera& camera)
{
	setCamera(camera.GetPos(), camera.GetTarget(), camera.GetUp());
}

void Pipeline::setPersProj(const PersProjInfo& p)
{
	m_persProj = p;
}

const mat4* Pipeline::getWVPTrans()
{
	mat4 ScaleTrans, RotateTrans, TranslationTrans, CameraTranslationTrans, CameraRotateTrans, PersProjection;

	ScaleTrans = initScaleTrans(m_scale);
	RotateTrans = initRotateTrans(m_rotateInfo);
	TranslationTrans = initTranslationTrans(m_worldPos);
	CameraTranslationTrans = initTranslationTrans(-m_camera.Pos);
	CameraRotateTrans = initCameraTransform(m_camera.Target, m_camera.Up);
	PersProjection = initPersProjection(m_persProj);

	m_transformation = PersProjection * CameraRotateTrans * CameraTranslationTrans * TranslationTrans * RotateTrans * ScaleTrans;
	return &m_transformation;
}