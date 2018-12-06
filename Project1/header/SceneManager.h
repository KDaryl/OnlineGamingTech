#pragma once
#include <map>
//Including all of our scene headers
#include "GameScene.h"
#include "MainMenu.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void update();
	void draw(SDL_Renderer* renderer);
	std::string handleInput(InputHandler& input);

	void setCurrent(std::string sceneName);
	void setTexture(ResourceHandler& resources);
	Scene* getScene(std::string sceneName) { return m_scenes[sceneName]; }
	GameScene& getGameSene() { return m_gameScene; }
private:
	typedef std::pair<std::string, Scene*> MTuple; //A tuple for handling the current scene 

	std::map<std::string, Scene*> m_scenes; //Our map of scenes
	
	MTuple m_current; //The current scene

	//Our scenes
	MainMenuScene m_menuScene;
	GameScene m_gameScene;
};

