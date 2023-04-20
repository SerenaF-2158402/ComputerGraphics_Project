#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include "maze.h"

class mazeGenerator {

public:
	mazeGenerator();

	void createWallCubes(const std::vector<std::vector<int>>& mazeArray, int mazeSize, float cubeSize);
	void drawFloor(const std::vector<std::vector<int>>& maze, float floorSize);

private:

};

#endif // !MAZEGENERATOR_H
