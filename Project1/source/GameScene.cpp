#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::update()
{
	//Update the player
	m_player.update();
}

void GameScene::draw(SDL_Renderer * renderer)
{
	//Draw the player
	m_player.draw(renderer);
}

void GameScene::setTexture(ResourceHandler & resources)
{
	m_player.setTexture(resources);
}
