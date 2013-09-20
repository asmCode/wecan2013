#include "LoadingScreen.h"
#include <Graphics/Texture.h>
#include <Graphics/TextureLoader.h>
#include <Graphics/Shader.h>
#include "DemoController.h"

LoadingScreen::LoadingScreen(void)
{
}

LoadingScreen::~LoadingScreen(void)
{
	delete loadingTex;
}

void LoadingScreen::Initialize()
{

}

void LoadingScreen::Initialize(const std::string &basePath)
{
	loadingTex = TextureLoader::Load((basePath + "loader.jpg"));
	assert(loadingTex != NULL);

	scrWidth = demo ->glWnd ->GetWindowWidth();
	scrHeight = demo ->glWnd ->GetWindowHeight();

	m_spriteBatch = new SpriteBatch(
		Shader::LoadFromFile((basePath + "Effects/Sprite.vpr").c_str(), (basePath + "Effects/Sprite.fpr").c_str()),
		sm::Matrix::Ortho2DMatrix(0, scrWidth, 0, scrHeight));
}

void LoadingScreen::Release()
{

}

void LoadingScreen::Update(float time, float ms)
{
}

void LoadingScreen::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	m_spriteBatch->Begin();
	glDisable(GL_BLEND);
	m_spriteBatch->Draw(loadingTex, 0, 0, scrWidth, scrHeight);
	m_spriteBatch->End();

	OpenglWindow *gl = demo ->glWnd;

	int left = gl ->Log2PX(323.0f / 1280.0f);
	int bottom = gl ->Log2PY(1.0f - (664.0f / 768.0f));
	int top = gl ->Log2PY(1.0f - (665.0f / 768.0f));
	int right = gl ->Log2PX((min(1, (float)progressStep / (float)steps)) * (636.0f / 1280.0f)) + left;

	//glDisable(GL_LIGHTING);
	//glDisable(GL_TEXTURE_2D);
	//glDisable(GL_BLEND);
	////glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glColor4ub(0x16, 0xa0, 0xfc, 0xff);
	//glBegin(GL_QUADS);
	//glVertex2i(left, bottom);
	//glVertex2i(left, top);
	//glVertex2i(right, top);
	//glVertex2i(right, bottom);
	//glEnd();
	//glColor4f(1, 1, 1, 1);

	//glDisable(GL_BLEND);

	demo ->glWnd ->SwapBuffers();
}

void LoadingScreen::SetStepsCount(int steps)
{
	this ->steps = steps;
	this ->progressStep = 0;
}

void LoadingScreen::ProgressStep()
{
	progressStep++;
}