#pragma once

class IKeyboardCallback
{
public:
	virtual void OnLeftMouseDown() {}
	virtual void OnKeyDown(int keyCode) {}
};

