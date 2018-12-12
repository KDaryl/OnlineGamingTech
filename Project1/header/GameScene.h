#pragma once
#include "Scene.h"
#include "Player.h"
#include "Network.h"

struct Vector2f;

class GameScene : public Scene
{

public:
	GameScene();
	~GameScene();
	void init(std::vector<int> values, bool isHostBool);
	void update(double dt);
	void draw(SDL_Renderer* renderer);
	std::string handleInput(InputHandler& input, std::string currentScene);
	void setTexture(ResourceHandler& resources);

	//Game setting methods
	void setOtherPlayerPosition(int pos, int col);

	Client& server() { return m_serverConnection; };
private:

	Client m_serverConnection; //Sever connection
	Player m_player; //Local player
	std::vector<Player> m_otherPlayers; //The other players that will be in the game
	Sprite m_bgSprite;
	bool m_isHost, m_gameOver;

	std::vector<Vector2f> m_startPositions;
};

