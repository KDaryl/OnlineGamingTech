#include "ResourceHandler.h"

ResourceHandler::ResourceHandler(std::string filePath) :
	m_filePath(filePath)
{
}

ResourceHandler::~ResourceHandler()
{
}

//Load all of our textures we need for the game
void ResourceHandler::loadTextures(SDL_Renderer& renderer)
{
	m_map["Player"] = loadFromPath("Player.png", renderer);
	m_map["Game BG"] = loadFromPath("Game BG.png", renderer);
	m_map["Join Game Button"] = loadFromPath("Join Game Button.png", renderer);
	m_map["Options Button"] = loadFromPath("Options Button.png", renderer);
	m_map["Exit Game Button"] = loadFromPath("Exit Button.png", renderer);
	m_map["You Indicator"] = loadFromPath("You Indicator.png", renderer);
}

SDL_Texture* ResourceHandler::loadFromPath(std::string fileName, SDL_Renderer& renderer)
{
	std::string path = m_filePath + fileName;
	SDL_Texture* newTexture = NULL; //This will be the loaded texture

	//Load image
	SDL_Surface* loadedS = IMG_Load(path.c_str());
	if (loadedS == NULL)
		std::cout << "Error loading image at path " << path << std::endl;
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(&renderer, loadedS);

		if(newTexture == NULL)
			std::cout << "Unable to create texture" << std::endl;

		//Get rid of old surface
		SDL_FreeSurface(loadedS);
	}

	return newTexture;
}

void ResourceHandler::destroyTextures()
{
	//Loop through our map
	for (auto& texture : m_map)
	{
		//Destroy the texture
		SDL_DestroyTexture(texture.second);
	}
}

SDL_Texture * ResourceHandler::getTexture(std::string name)
{
	return m_map.at(name);
}
