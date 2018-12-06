#pragma once
#include "Scene.h"
#include "Player.h"

class GameScene : public Scene
{

public:
	GameScene();
	~GameScene();
	void init(bool isHostBool);
	void update();
	void draw(SDL_Renderer* renderer);
	std::string handleInput(InputHandler& input, std::string currentScene);
	void setTexture(ResourceHandler& resources);
	void setAsHost(bool aBool);
private:
	Player m_player; //Local player
	std::vector<Player> m_otherPlayers; //The other players that will be in the game
	Sprite m_bgSprite;
	bool m_isHost;
};

