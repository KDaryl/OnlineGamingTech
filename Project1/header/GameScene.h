#pragma once
#include "Scene.h"
#include "Player.h"

struct Vector2f;

class GameScene : public Scene
{

public:
	GameScene();
	~GameScene();
	void init(int pos, int col, bool isHostBool);
	void update(double dt);
	void draw(SDL_Renderer* renderer);
	std::string handleInput(InputHandler& input, std::string currentScene);
	void setTexture(ResourceHandler& resources);

private:
	Player m_player; //Local player
	std::vector<Player> m_otherPlayers; //The other players that will be in the game
	Sprite m_bgSprite;
	bool m_isHost;

	std::vector<Vector2f> m_startPositions;
};

