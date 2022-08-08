/**
 * @file Animator.h
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Header file for the Animator class
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once
#include "Texture.h"
#include <vector>

/**
 * @class Animator
 * @brief Class handling everything regarding the animation of the MovableObjects
 */
class Animator
{
private:
	std::vector <std::string> spritePaths;

	int spriteCount;
	double animationTime;

	int startIndex, endIndex, index;
	int spritesPerDirection;
public:
	std::vector <Texture*> textures;
	Animator(std::string filepaths);
	~Animator();
	void generateTextures();
	void animate(const int direction, const float dt, const int mapID, bool& newDirection);
};
