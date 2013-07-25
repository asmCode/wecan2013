#pragma once

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include <Math\Vec3.h>
#include <Math\Vec2.h>

#include "Vertex.h"

class Utils
{
public:
	Utils(void);
	~Utils(void);

	//oblicza styczna w vierzcholku v0
	static sm::Vec3 CalcTangent(Vertex *v0, Vertex *v1, Vertex *v2);

	//oblicza styczne dla wszystkich wierzcholkow
	static void CalcTangents(Vertex *vertices, int triangleCount, sm::Vec3 *tangents);

	static void SetOrtho()
	{
		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(viewport[0], viewport[2], viewport[1], viewport[3]);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	static void SetPerspective()
	{
		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0f, (float)abs(viewport[2]) / (float)abs(viewport[3]), 1.0f, 1000.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	static void PushProjModelMetrices()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();

		glMatrixMode(GL_MODELVIEW);
	}

	static void PopProjModelMetrices()
	{
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
	}

	static void DrawSprite(int texId, int x, int y, int width, int height)
	{
		PushProjModelMetrices();
		glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);

		SetOrtho();

		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		//glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glDepthMask(false);
		glBindTexture(GL_TEXTURE_2D, texId);

		glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex2i(x, y + height);
		glTexCoord2f(1, 1); glVertex2i(x + width, y + height);
		glTexCoord2f(1, 0); glVertex2i(x + width, y);
		glTexCoord2f(0, 0); glVertex2i(x, y);
		glEnd();

		glPopAttrib();
		PopProjModelMetrices();
	}

	static void GetCursorRay(int wx, int wy, sm::Vec3 &start, sm::Vec3 &target)
	{
		double projectionMatrix[16];
		glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);

		double modelviewMatrix[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, modelviewMatrix);

		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		wy = viewport[3] - wy; //flip

		sm::Vec3 nearPoint;
		sm::Vec3 farPoint;

		double x, y, z;

		gluUnProject(wx, wy, 0.0f, modelviewMatrix, projectionMatrix, viewport, &x, &y, &z);
		nearPoint.Set((float)x, (float)y, (float)z);

		gluUnProject(wx, wy, 1.0f, modelviewMatrix, projectionMatrix, viewport, &x, &y, &z);
		farPoint.Set((float)x, (float)y, (float)z);

		start = nearPoint;
		target = (farPoint - nearPoint).GetNormalized();
	}

	static bool ProjectPointToScreen(sm::Vec3 point, sm::Vec3 &screenPoint)
	{
		double projectionMatrix[16];
		glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);

		double modelviewMatrix[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, modelviewMatrix);

		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		double sx, sy, sz;

		int results = gluProject(point.x, point.y, point.z, modelviewMatrix, projectionMatrix, viewport, &sx, &sy, &sz);

		if (results == GL_FALSE)
			return false;

		sy = viewport[3] - sy; //flip

		screenPoint.x = (float)sx;
		screenPoint.y = (float)sy;
		screenPoint.z = (float)sz;

		return true;
	}

	static bool ProjectScreenToWorld(sm::Vec3 screenPoint, sm::Vec3 &worldPoint)
	{
		double projectionMatrix[16];
		glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);

		double modelviewMatrix[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, modelviewMatrix);

		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		double wx, wy, wz;

		int results = gluUnProject(screenPoint.x, viewport[3] - screenPoint.y, screenPoint.z,
			modelviewMatrix, projectionMatrix, viewport, &wx, &wy, &wz);

		if (results == GL_FALSE)
			return false;

		worldPoint.Set((float)wx, (float)wy, (float)wz);

		return true;
	}
};
