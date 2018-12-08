#include "GameScene.h"

GameScene::GameScene()
{
	//The four starting positions that the player can be in
	m_startPositions.push_back(Vector2f(0, 0));
	m_startPositions.push_back(Vector2f(1205, 0));
	m_startPositions.push_back(Vector2f(0, 645));
	m_startPositions.push_back(Vector2f(1205, 645));
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