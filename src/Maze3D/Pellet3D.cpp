/**
 * @file Pellet3D.cpp
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Source code for the Pellet3D class
 * @version 0.1
 * @date 2020-11-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "Pellet3D.h"

/**
 * @brief Construct a new Pellet3D::Pellet3D object
 * 
 * @param pellet 
 * @param maze 
 */
Pellet3D::Pellet3D(Model* pellet, Maze3D* maze)
    : allEaten(false)
{
	this->pellet = pellet;
    pelletCount = maze->getPelletCount();
    generateModelMatrices(maze);
    addModelMatrices();
}

/**
 * @brief generates the matrices to be used when instance drawing the pellets
 * 
 * @param maze - The maze in which the pellets are drawn
 */
void Pellet3D::generateModelMatrices(Maze3D* maze)
{
    for (int y = 0; y < maze->map2d.size(); y++)
        for (int x = 0; x < maze->map2d[y].size(); x++)
            if (maze->map2d[y][x] != 1) {
                glm::mat4 translation = glm::translate(glm::mat4(1), glm::vec3(x + .5f, 0, y + .5f));
                glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(1 * 25.f), glm::vec3(0, 1, 0));
                glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(0.1f));

                glm::mat4 transformation = translation * rotation * scale;
                modelMatrices.push_back(transformation);
            }
}

/**
 * @brief Adds the model matrices to the pellet VBO and adds it to the VAO
 * 
 */
void Pellet3D::addModelMatrices()
{
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, pelletCount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    for (unsigned int i = 0; i < pellet->meshes.size(); i++)
    {
        VAO = pellet->meshes[i].VAO;
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }
}

/**
 * @brief Removes a pellet from the game. 
 * 
 * @param camera    - The player controlled camera
 * @param maze      - The maze in which the game occours. 
 */
void Pellet3D::eatPellet(Camera* camera, Maze3D* maze)
{
    float posX = floor(camera->Position.x);
    float posY = floor(camera->Position.z);
    //std::cout << posY << ' ' << posX << std::endl;
    int height = maze->getHeight(); int width = maze->getWidth();
    int pelletIndex = 0; bool pelletEaten = false;
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            if (maze->map2d[y][x] != 1 && maze->map2d[y][x] != 9) {
                pelletIndex++;
                if (posX == x && posY == y)
                    if (!pelletEaten)
                    {
                        pelletCount--;
                        if (pelletCount > 0)
                        {
                            modelMatrices.erase(modelMatrices.begin() + --pelletIndex);
                            glBindVertexArray(VAO);
                            glBindBuffer(GL_ARRAY_BUFFER, VBO);
                            glBufferData(GL_ARRAY_BUFFER, pelletCount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
                            pelletEaten = true;
                            maze->map2d[posY][posX] = 9;
                        } else
                            allEaten = true;
                    }
            }

}

/**
 * @brief Draws the pellets
 * 
 * @param shader        - The pellets shader
 * @param projection    - The players projection matrix
 * @param view          - The players view matrix
 */
void Pellet3D::Draw(Shader* shader,glm::mat4 projection, glm::mat4 view)
{
    if (pelletCount > 0)
    {
        shader->use();
        shader->setMat4("projection", projection);
        shader->setMat4("view", view);
        shader->setInt("texture_diffuse1", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, pellet->textures_loaded[0].id);
        for (unsigned int i = 0; i < pellet->meshes.size(); i++)
        {
            glBindVertexArray(pellet->meshes[i].VAO);
            glDrawElementsInstanced(GL_TRIANGLES, pellet->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, pelletCount);
            glBindVertexArray(0);
        }
    }
}
