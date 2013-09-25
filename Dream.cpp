#include "Dream.h"
#include <Graphics/Texture.h>
#include <Graphics/TextureLoader.h>
#include <Graphics/Shader.h>
#include <Graphics/SpriteBatch.h>
#include "DemoController.h"

float Dream::start = 1 * 60 + 45.0f + (4686.0f / 4800.0f);
float Dream::end = Dream::start + 6.0f;

Dream::Dream(void)
{
	a.AddKeyframe(start, 0.0f, false);
	a.AddKeyframe(start + 1.5f, 1.0f, false);
	a.AddKeyframe(start + 4.5f, 1.0f, false);
	a.AddKeyframe(start + 6.0f, 0.0f, false);
}

Dream::~Dream(void)
{
}

void Dream::Initialize()
{
	t = new Texture*[90];
	currentT = 0;
	tPerSec = 15;

	for (uint32_t i = 0; i < 90; i++)
	{
		char s[65];
		sprintf(s, "dream%d", i + 1);
		t[i] = demo->m_content->Get<Texture>(s);
		assert(t[i] != NULL);
	}

	scrWidth = demo ->glWnd ->GetWindowWidth();
	scrHeight = demo ->glWnd ->GetWindowHeight();

	m_spriteBatch = new SpriteBatch(
		demo->m_content->Get<Shader>("Sprite"),
		sm::Matrix::Ortho2DMatrix(0, scrWidth, 0, scrHeight));
}

void Dream::Release()
{

}

void Dream::Update(float time, float ms)
{
	if (time < start)
		return;

	currentT += tPerSec * ms;

	if (currentT >= 89)
		currentT = 89;
}

void Dream::Draw(float time)
{
	if (time < start || time > end)
		return;

	float opacity = 0.0f;
	a.GetValue(time, opacity);

	m_spriteBatch->Begin();
	m_spriteBatch->Draw(t[(int)currentT], sm::Vec4(1, 1, 1, opacity), 0, 0, scrWidth, scrHeight);
	m_spriteBatch->End();

	OpenglWindow *gl = demo ->glWnd;
}