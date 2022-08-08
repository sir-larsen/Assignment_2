# Assignment 2: 3D Pacman

## Run Instructions (Visual Studio)

1. Open the folder
2. Allow CMake to build everything
3. Set ``main.cpp`` as the startup item
4. Press run

In case of the program ***not*** compiling/running, please try resaving the ``CMakeLists.txt`` file


# Functionality of the program

* The program reads the provided map layout and renders it on the screen. The walls have textures.
* There is a PacMan being controlled by the player using ***WASD***. 
* The player is ***not*** able to walk through the walls.
* There are edible pellets included, they disappear when they are eaten
* The ghosts are moving, using the Astar pathfinding algorithm, more on this in the next section.
* Collision between the ghosts and pacman terminates the game. 
* There are two models loaded from file, both the ***ghosts*** and the ***pellets*** are from .obj files
* A lighting model consisting of a pointlight in the middle of the map and a spotlight (flashlight) positioned at the cameras location
* The game has been implemented in a ***first person*** view. Therefore there is also a minimap which represents the game seen from above. 

# Optional features / Extra functionality

* When doing the assignment, the main goal was to create an overall best possible delivery.

## Architecture / Framework

* The code is structured in three different folders:
 1. The ``core`` folder
    * Contains core code for handling boilerplate OpenGL code, aswell as functionality such as the ***minimap***
    * The main focus of this code is being reusable in many different areas, also after the assignment has been completed. 
 2. The ``Maze2D`` folder
    * Contains the code for rendering the 2D maze, which is what is being outputted to the minimap. 
 3. The ``Maze3D`` folder
    * Contains the code for rendering the 3D maze, which is what the "world" consists of. 

## Gameplay / Usability

* There are multiple factors that play a crucial role in the quality of the gameplay of which the game provides.
  * One of those things is the actual challenge the game has to offer. To truly provide a challenge the ghosts needs to know where to move in order to catch the player. Therefore we really wanted to implement a proper ***pathfinding*** algorithm, we landed on the ***Astar*** pathfinding algorithm, as it is a particularly interesting algorithm, which almost always guarantees that the shortest path between two nodes will be calculated. 
  * The algorithm proved itself extremely efficient in catching the player, leading us to turn down the ghost's movement speed in order to make it possible to actually win the game. This was because one of the other factors that make the game fun is the possibility of actually winning. 
  * The camera can be "detached" by pressing ***C***. Enabling freecam will disable the ghosts pathfinding. 

## Aesthetics / Quality of Graphics

* Some key focus areas regarding the aesthetics are:
  * The minimap. We wanted the minimap to be a 2D representation of the game, which it is. The data from the 3D world gets sent to the minimap, which handles these posisions and animates the relevant sprites accordingly. I think it visually turned out really great.
  * We also did not want there to be too many lightsources, as it would have removed the "spooky" feeling the game has. We therefore ruled out directional lights as they did not make much sense in this type of game in my opinion. 
  * We ended up going for the flashlight approach with a weak point light in the middle of the map. 
  * All textures was made for this assignment specifically, except for the 2d ghosts and 2d pacman from assignment 1. 
  * We feel like the overall "mood" of the game reflects the original intention in this regard, a "spooky" feeling. 

# Further questions


* A picture illustrating how the program is suppose to look like in case of drivers causing issues:
![The 3D maze with the minimap](/res/readme/3DPacman.png "The 3D maze with the minimap")