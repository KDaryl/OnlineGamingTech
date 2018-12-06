#include "MainMenu.h"

MainMenuScene::MainMenuScene()
{
	m_buttons["Play Button"] = new Button(370, 122.5);
	m_buttons["Options Button"] = new Button(370, 322.5);
	m_buttons["Exit Button"] = new Button(370, 522.5);
}

MainMenuScene::~MainMenuScene()
{
}

void MainMenuScene::init(bool isHostBool)
{
}

void MainMenuScene::update()
{
	//Update our buttons
	for (auto& pair : m_buttons)
	{
		//Pass in the ticks sinc ethe timer was restarted
		pair.second->update(m_timer.getTicks());
	}

	//Restart the timer here
	m_timer.start();
}

void MainMenuScene::draw(SDL_Renderer * renderer)
{
	//Draw our buttons
	for (auto& pair : m_buttons)
	{
		pair.second->draw(renderer);
	}
}

std::string MainMenuScene::handleInput(InputHandler & input, std::string currentScene)
{
	std::string newScene = currentScene;

	if (input.isButtonPressed("LEFT_MOUSE"))
	{
		if (m_buttons["Play Button"]->clicked(input))
			newScene = "Join Game";
		if (m_buttons["Exit Button"]->clicked(input))
			newScene = "Exit Game";
		if (m_buttons["Options Button"]->clicked(input))
			newScene = "Options Scene";
	}
	else
	{
		//Check for highlight on each of our buttons
		for (auto& btn : m_buttons)
		{
			btn.second->checkHighlight(input);
		}
	}

	return newScene;
}

void MainMenuScene::setTexture(ResourceHandler & resources)
{
	m_buttons["Play Button"]->setTexture(resources, "Join Game Button");
	m_buttons["Options Button"]->setTexture(resources, "Options Button");
	m_buttons["Exit Button"]->setTexture(resources, "Exit Game Button");
}