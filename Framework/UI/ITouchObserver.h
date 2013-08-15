#pragma once

class Control;

class ITouchObserver
{
public:
	virtual void TouchPressed(Control *control, int x, int y) {};
	virtual void TouchBegin(Control *control, int x, int y) {};
	virtual void TouchEnd(Control *control, int x, int y) {};
};
