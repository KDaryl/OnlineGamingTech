#include "GameScene.h"

GameScene::GameScene()
{
	m_startPositions.push_back(Vector2f());
}

GameScene::~GameScene()
{
}

void GameScene::init(bool isHostBool)
{
	m_isHost = isHostBool;
}

void GameScene::update(double dt)
{
	//Update the player
	m_player.update(dt);
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

	m_player.handleInput(input);

	return newScene;
}

void GameScene::setTexture(ResourceHandler & resources)
{
	m_player.setTexture(resources);
	m_bgSprite.setTexture(resources.getTexture("Game BG"));
}