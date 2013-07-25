#include "Animation.h"
#include "Model.h"
#include "Mesh.h"

Animation::Animation(void)
{
	id = 0;
	mesh = NULL;
	worldTMInv = sm::Matrix::IdentityMatrix();

	pos = NULL;
	rot = NULL;
	scale = NULL;

	localPos.Set(0, 0, 0);
	localRot.RotateToQuat(0, 0, 0, 0);
	localScale.Set(1, 1, 1);

	hasOwnRotate = false;
	ownAngle = 0.0f;
	angleScale = 0.0f;
}

Animation::~Animation(void)
{
	if (pos != NULL)
		delete pos;

	if (rot != NULL)
		delete rot;

	if (scale != NULL)
		delete scale;

	for (unsigned i = 0; i < subAnims.size(); i++)
		delete subAnims[i];
}

void Animation::AssignModel(Model *model)
{
	for (unsigned i = 0; i < subAnims.size(); i++)
		subAnims[i] ->AssignModel(model);

	if (id == 0)
		return;

	std::vector<Mesh*> &meshes = model ->GetMeshes();
	for (unsigned i = 0; i < meshes.size(); i++)
	{
		if (meshes[i] ->id == id)
		{
			mesh = meshes[i];
			return;
		}
	}
}

void Animation::AssignTransformable(ITransformable *transformable)
{
	for (unsigned i = 0; i < subAnims.size(); i++)
		subAnims[i] ->AssignTransformable(transformable);

	if (id == 0)
		return;

	if (transformable->GetId() == id)
		mesh = transformable;
}

void Animation::Update(float time, const sm::Matrix &transform, float seconds)
{
	if (mesh != NULL && mesh->IsCam())
	{
		sm::Matrix tr = transform;

		sm::Vec3 posVal;
		sm::Quat rotVal;

		float angle;
		sm::Vec3 axis;

		if (pos != NULL)
			pos ->GetValue(time, posVal);
		else
			posVal = localPos;

		if (rot != NULL)
			rot ->GetValue(time, rotVal);
		else
			rotVal = localRot;

		rotVal.Normalize();
		rotVal.QuatToRotate(angle, axis);

		tr *= sm::Matrix::TranslateMatrix(posVal);
		tr *= sm::Matrix::RotateAxisMatrix(angle, axis);

		if (mesh != NULL)
		{
			mesh ->Transform() = sm::Matrix::RotateAxisMatrix(1.5707963f, sm::Vec3(1, 0, 0));
			mesh ->Transform() *= tr.GetInversed();
			//mesh ->Transform() *= worldTMInv.GetInversed();
		}
	}
	else
	{
		sm::Matrix tr = transform;

		sm::Vec3 posVal;
		sm::Quat rotVal;
		sm::Vec3 scaleVal;

		float angle;
		sm::Vec3 axis;

		if (pos != NULL)
			pos ->GetValue(time, posVal);
		else
			posVal = localPos;

		if (rot != NULL)
			rot ->GetValue(time, rotVal);
		else
			rotVal = localRot;

		if (scale != NULL)
			scale ->GetValue(time, scaleVal);
		else
			scaleVal = localScale;

		rotVal.Normalize();
		rotVal.QuatToRotate(angle, axis);

		tr *= sm::Matrix::TranslateMatrix(posVal);
		tr *= sm::Matrix::ScaleMatrix(scaleVal);

		sm::Matrix loda = tr;
		
		loda *= sm::Matrix::RotateAxisMatrix(angle, axis);

		if (hasOwnRotate)
		{
			if (time > 82.5f)
			{
				tr *= sm::Matrix::RotateAxisMatrix(ownAngle, axis);
				//tr *= sm::Matrix::RotateAxisMatrix(angle, axis);
				ownAngle += seconds * 8.0f * angleScale;
			}
		}
		else
		{
			tr *= sm::Matrix::RotateAxisMatrix(angle, axis);
		}

		if (mesh != NULL)
		{
			mesh ->AnimTransform() = tr;
			mesh ->Transform() = tr;
			mesh ->Transform() *= worldTMInv;
		}

		for (unsigned i = 0; i < subAnims.size(); i++)
			subAnims[i] ->Update(time, loda, seconds);
	}
}

Animation *Animation::GetAnimationById(int id)
{
	if (this->id == id)
		return this;

	Animation *anim = NULL;

	for (unsigned i = 0; i < subAnims.size(); i++)
	{
		anim = subAnims[i]->GetAnimationById(id);
		if (anim != NULL)
			return anim;
	}

	return NULL;
}

float Animation::GetAnimLength(int id)
{
	Animation *anim = GetAnimationById(id);
	assert(anim != NULL);

	float posTime = 0.0f;
	float rotTime = 0.0f;
	float scaleTime = 0.0f;

	sm::Vec3 vdummy;
	sm::Quat qdummy;
	bool bdummy;

	if (anim->pos != NULL && anim->pos->GetKeysCount() > 0)
		anim->pos->GetKeyframe(anim->pos->GetKeysCount() - 1, posTime, vdummy, bdummy);
	if (anim->rot != NULL && anim->rot->GetKeysCount() > 0)
		anim->rot->GetKeyframe(anim->rot->GetKeysCount() - 1, rotTime, qdummy, bdummy);
	if (anim->scale != NULL && anim->scale->GetKeysCount() > 0)
		anim->scale->GetKeyframe(anim->scale->GetKeysCount() - 1, scaleTime, vdummy, bdummy);

	return max(max(posTime, rotTime), scaleTime);
}