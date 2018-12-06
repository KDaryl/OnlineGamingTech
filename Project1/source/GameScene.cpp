#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::init(bool isHostBool)
{
	m_isHost = isHostBool;
}

void GameScene::update()
{
	//Update the player
	m_player.update();
}

void GameScene::draw(SDL_Renderer * renderer)
{
	//Draw the background
	m_bgSprite.draw(renderer);

	//Draw the player
	m_player.draw(renderer);
}

std::string GameScene::handleInput(InputHandler & input, std::string currentScene)
{
	std::string newScene = currentScene;


	return newScene;
}

void GameScene::setTexture(ResourceHandler & resources)
{
	m_player.setTexture(resources);
	m_bgSprite.setTexture(resources.getTexture("Game BG"));
}