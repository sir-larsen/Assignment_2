/**
 * @file Animator.cpp
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Source code for the Animator class.
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "Animator.h"
#include <fstream>
#include <iostream>

/**
 * @brief Construct a new Animator:: Animator object
 * 
 * @param filepaths - link to the file containing the paths to the actual images
 */
Animator::Animator(std::string filepaths)
{
	std::ifstream spriteLocations(filepaths);

	spriteLocations >> spriteCount; spriteLocations.ignore();

	std::string path;
	for (int i = 0; i < spriteCount; i++)
	{
		getline(spriteLocations, path);
		spritePaths.push_back(path);
	}
	generateTextures();
	animationTime = 0;
	spritesPerDirection = spriteCount / 4;
	startIndex = endIndex = index = 0;
}

/**
 * @brief Destroy the Animator:: Animator object
 * 
 */
Animator::~Animator()
{
	for (int i = 0; i < textures.size(); i++)
	{
		free(textures[i]);
	}
}

/**
 * @brief Generates a new texture for every filepath saved in the spritePaths vector.
 * 
 */
void Animator::generateTextures()
{
	for (int i = 0; i < spriteCount; i++)
		textures.push_back(new Texture(spritePaths[i])); 
}

/**
 * @brief - Animates the MovableObject to which the animator is attached to.
 * 
 * @param direction 	- The direction the object is currently moving
 * @param dt 			- Delta time (time elapsed since last iteration of the loop)
 * @param mapID 		- What identifier this MovableObject has in the level
 * @param newDirection  - Wheter or not the object has changed its moving direction
 */
void Animator::animate(const int direction, const float dt, const int mapID, bool& newDirection)
{
	if (newDirection)
	{
		newDirection = false;
		startIndex	 = spritesPerDirection * direction;
		endIndex	 = startIndex + spritesPerDirection;
		index		 = startIndex;
	}

	animationTime += dt;
	if (animationTime >= 0.07 && index < endIndex)
	{
		textures[index]->Bind(mapID);
		index++;
		animationTime = 0;
	}
	if (index >= endIndex) index = startIndex;
}


