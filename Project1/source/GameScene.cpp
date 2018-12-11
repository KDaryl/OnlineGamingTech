#include "GameScene.h"

GameScene::GameScene() :
	m_serverConnection("127.0.0.1", 100) //Create connection to server at ip and port
{
	//The four starting positions that the player can be in
	m_startPositions.push_back(Vector2f(0, 0));
	m_startPositions.push_back(Vector2f(1205, 0));
	m_startPositions.push_back(Vector2f(0, 645));
	m_startPositions.push_back(Vector2f(1205, 645));

	m_otherPlayers.push_back(Player());

	m_isHost = false;
}

GameScene::~GameScene()
{
}

void GameScene::init(int pos, int col, bool isHostBool)
{
	m_isHost = isHostBool;
	m_player.initPlayer(m_startPositions.at(pos), col);
}

void GameScene::update(double dt)
{
	//Get game update from server
	if (m_serverConnection.gotGameUpdate)
	{
		//Set the other players position
		m_otherPlayers.at(0).setPosition(m_serverConnection.gameUpdateData.at(0), m_serverConnection.gameUpdateData.at(1));
		m_serverConnection.gotGameUpdate = false; //Set to false
	}

	//Get the previous position of the player
	m_prevPos = m_player.getPosition();

	//Update the player
	m_player.update(dt);

	//Update other players
	for (auto& player : m_otherPlayers)
	{
		player.update(dt);
	}
	
	//If im the host, decide if collisions have occured
	if (m_isHost)
	{
		for (auto& player : m_otherPlayers)
		{
			if (player.getCircleCollider().intersects(m_player.getCircleCollider()))
			{
				std::cout << "Players colliding, end game" << std::endl;
			}
		}
	}

	//Send update if my position has changed at all
	if (m_prevPos != m_player.getPosition())
	{
		std::vector<float> values = {m_player.getPosition().x, m_player.getPosition().y };
		//Send update
		m_serverConnection.sendData(values, P_GameUpdate);
	}
}

void GameScene::draw(SDL_Renderer * renderer)
{
	//Draw the background
	m_bgSprite.draw(renderer);

	//Draw the player
	m_player.draw(renderer);

	//Draw other players
	for (auto& player : m_otherPlayers)
	{
		player.draw(renderer);
	}
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

void GameScene::setOtherPlayerPosition(int pos, int col)
{
	m_otherPlayers.at(0).initPlayer(m_startPositions.at(pos), col);
}
