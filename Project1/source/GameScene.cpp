#include "GameScene.h"

GameScene::GameScene()
{
	//The four starting positions that the player can be in
	m_startPositions.push_back(Vector2f(0, 0));
	m_startPositions.push_back(Vector2f(1205, 0));
	m_startPositions.push_back(Vector2f(0, 645));
	m_startPositions.push_back(Vector2f(1205, 645));

	m_otherPlayers.push_back(Player());

	m_isHost = true;
}

GameScene::~GameScene()
{
}

void GameScene::init(int pos, int col, bool isHostBool)
{
	m_isHost = isHostBool;
	m_player.initPlayer(m_startPositions.at(0), 0);

	m_otherPlayers.at(0).initPlayer(m_startPositions.at(1), 1);
}

void GameScene::update(double dt)
{
	//Update the player
	m_player.update(dt);

	//Update other players
	m_otherPlayers.at(0).update(dt);

	//If im the host, decide if collisions have occured
	if (m_isHost)
	{
		if (m_otherPlayers.at(0).getCircleCollider().intersects(m_player.getCircleCollider()))
		{
			std::cout << "Players colliding" << std::endl;
		}
		else
		{
			std::cout << "No collision" << std::endl;
		}
	}

}

void GameScene::draw(SDL_Renderer * renderer)
{
	//Draw the background
	m_bgSprite.draw(renderer);

	//Draw the player
	m_player.draw(renderer);

	m_otherPlayers.at(0).draw(renderer);
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
	m_otherPlayers.at(0).setTexture(resources);
	m_bgSprite.setTexture(resources.getTexture("Game BG"));
}