#include "MainMenu.h"

MainMenuScene::MainMenuScene()
{
	m_buttons["Play Button"] = Button();
	m_buttons["Options Button"] = Button();
	m_buttons["Exit Button"] = Button();
}

MainMenuScene::~MainMenuScene()
{
}

void MainMenuScene::update()
{
	//Update our buttons
	for (auto& pair : m_buttons)
	{
		pair.second.update();
	}
}

void MainMenuScene::draw(SDL_Renderer * renderer)
{
	//Draw our buttons
	for (auto& pair : m_buttons)
	{
		pair.second.draw(renderer);
	}
}

std::string MainMenuScene::handleInput(InputHandler & input, std::string currentScene)
{
	std::string newScene = currentScene;

	if (input.isButtonPressed("LEFT_MOUSE"))
	{
		if (m_buttons["Play Button"].clicked(input))
			newScene = "Game Scene";
		if (m_buttons["Exit Button"].clicked(input))
			newScene = "Exit Game";
		if (m_buttons["Options Button"].clicked(input))
			newScene = "Options Scene";
	}

	return newScene;
}

void MainMenuScene::setTexture(ResourceHandler & resources)
{
	m_buttons["Play Button"].setTexture(resources, "Join Game Button");
	m_buttons["Options Button"].setTexture(resources, "Options Button");
	m_buttons["Exit Button"].setTexture(resources, "Exit Game Button");
}