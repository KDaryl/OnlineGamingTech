#include "SceneManager.h"

SceneManager::SceneManager()
{
	//Add all of our scenes to our map
	m_scenes["Main Menu"] = &m_menuScene;
	m_scenes["Game Scene"] = &m_gameScene;

	//Set the current Scene
	m_current = MTuple("Main Menu", &m_menuScene);
}

SceneManager::~SceneManager()
{
}

void SceneManager::update()
{
	//If the current ptr is not null then update the current scene
	if (nullptr != m_current.second)
	{
		m_current.second->update();
	}
}

void SceneManager::draw(SDL_Renderer* renderer)
{
	//If the current ptr is not null then draw the current scene
	if (nullptr != m_current.second)
	{
		m_current.second->draw(renderer);
	}
}

void SceneManager::handleInput(InputHandler & input)
{
	//If nullptr is nopt equal to current then handle input
	if (nullptr != m_current.second)
	{
		m_current.second->handleInput(input, m_current.first);
	}
}

void SceneManager::setCurrent(std::string sceneName)
{
	//If the key is in the map
	bool hasKey = false;
	for (auto& scene : m_scenes)
	{
		//If the scene name is equal to the passed over string then set the current scene
		if (scene.first == sceneName)
		{
			m_current = MTuple(sceneName, scene.second); //Set the current scene
			break;
		}
	}
}

void SceneManager::setTexture(ResourceHandler & resources)
{
	//Loop through each scene and pass the ResourceHandler to each scene
	for (auto& scene : m_scenes)
	{
		scene.second->setTexture(resources);
	}
}
