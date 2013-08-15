#include "SteerCamera.h"

SteerCamera::SteerCamera()
{
	lastMousePos = NULL;

	position.Set(0, 0, 0);

	yaw = 0.0f;
	pitch = 0.0f;
	roll = 0.0f;
	
	//screenCentre.x = GetSystemMetrics(SM_CXSCREEN) / 2;
//	screenCentre.y = GetSystemMetrics(SM_CYSCREEN) / 2;

//	GetCursorPos(&lastPos);

//	lastMousePos = new POINT();
	lastMousePos ->x = 0;
	lastMousePos ->y = 0;
}

SteerCamera::~SteerCamera()
{
	delete lastMousePos;
}

void SteerCamera::Process(float ms)
{
	/*
	bool keys[256];

	keys['W'] = false;
	keys['A'] = false;
	keys['S'] = false;
	keys['D'] = false;
	keys['Q'] = false;
	keys['E'] = false;
	keys[VK_LBUTTON] = false;
	keys[VK_LCONTROL] = false;

	if (GetAsyncKeyState('W') & 0x8000) keys['W'] = true;
	if (GetAsyncKeyState('A') & 0x8000) keys['A'] = true;
	if (GetAsyncKeyState('S') & 0x8000) keys['S'] = true;
	if (GetAsyncKeyState('D') & 0x8000) keys['D'] = true;
	if (GetAsyncKeyState('Q') & 0x8000) keys['Q'] = true;
	if (GetAsyncKeyState('E') & 0x8000) keys['E'] = true;
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) keys[VK_LBUTTON] = true;
	if (GetAsyncKeyState(VK_LCONTROL) & 0x8000) keys[VK_LCONTROL] = true;

	if (keys[VK_LBUTTON])
	{
		POINT cur_pos;
		GetCursorPos(&cur_pos);

		float sensitivity = 1.0f / 2.0f;

		if (cur_pos.x != screenCentre.x)
			yaw += (float)(cur_pos.x - lastPos.x) * sensitivity;

		if (cur_pos.y != screenCentre.y)
			pitch += (float)(cur_pos.y - lastPos.y) * sensitivity;

		(*lastMousePos) = cur_pos;

		lastPos.x = cur_pos.x;
		lastPos.y = cur_pos.y;
	}
	else
	{
		POINT cur_pos;
		GetCursorPos(&cur_pos);
		lastPos.x = cur_pos.x;
		lastPos.y = cur_pos.y;

		(*lastMousePos) = cur_pos;
	}

	float ratio = 10.0f;
	keys[VK_LCONTROL] ? ratio = ms / 5.0f : ratio = ms / 50.0f;

	sm::Vec3 moveVec(0, 0, -1);
	moveVec.RotateX(-rad(pitch));
	moveVec.RotateY(rad(yaw));
	
	if (keys['W'] == TRUE)
	{
		moveVec = moveVec * ratio;
		position = position + moveVec;
	}

	if (keys['S'] == TRUE)
	{
		moveVec = moveVec * ratio;
		position = position - moveVec;
	}

	if (keys['A'] == TRUE)
	{
		moveVec.y = 0;
		moveVec.Normalize();
		moveVec.RotateY(rad(90.0f));
		moveVec = moveVec * ratio;
		position = position - moveVec;
	}

	if (keys['D'] == TRUE)
	{
		moveVec.y = 0;
		moveVec.Normalize();
		moveVec.RotateY(rad(90.0f));
		moveVec = moveVec * ratio;
		position = position + moveVec;
	}

	if (keys['Q'] == TRUE)
	{
		position.y += ratio;
	}

	if (keys['E'] == TRUE)
	{
		position.y -= ratio;
	}
	 
	*/
}

void SteerCamera::SetViewMatrix()
{
	glRotatef(roll, 0.0f, 0.0f, 1.0f);
	glRotatef(pitch, 1.0f, 0.0f, 0.0f);
	glRotatef(yaw, 0.0f, 1.0f, 0.0f);

	glTranslatef(-position.x, -position.y, -position.z);
}

sm::Matrix SteerCamera::GetViewMatrix()
{
	int prevMatrixMode = 0;
	glGetIntegerv(GL_MATRIX_MODE, &prevMatrixMode);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glLoadIdentity();
	SetViewMatrix();
	sm::Matrix view;
	glGetFloatv(GL_MODELVIEW_MATRIX, view);

	glPopMatrix();
	glMatrixMode(prevMatrixMode);

	return view;
}

sm::Vec3 SteerCamera::GetPosition()
{
	return position;
}
