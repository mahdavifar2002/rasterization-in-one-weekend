#ifndef CUBE_H
#define CUBE_H

#include <glm/glm.hpp>
using namespace glm;

vec3 vertices[] =
{
	{  1.0f, -1.0f, -1.0f },
	{  1.0f, -1.0f,  1.0f },
	{ -1.0f, -1.0f,  1.0f },
	{ -1.0f, -1.0f, -1.0f },
	{  1.0f,  1.0f, -1.0f },
	{  1.0f,  1.0f,  1.0f },
	{ -1.0f,  1.0f,  1.0f },
	{ -1.0f,  1.0f, -1.0f }
};

int indices[] = {
	// 1,3,0, 7,5,4, 4,1,0, 5,2,1, 2,7,3, 0,7,4, 1,2,3, 7,6,5, 4,5,1, 5,6,2, 2,6,7, 0,3,7
       1,3,0, 7,5,4, 4,1,0, 5,2,1, 2,7,3, 0,7,4, 1,2,3, 7,6,5, 4,5,1, 5,6,2, 2,6,7, 0,3,7
};

vec3 colors[] {
	{ 0.5f, 0.5f, 1.0f },
	{ 0.5f, 1.0f, 0.5f },
	{ 1.0f, 0.5f, 0.5f },
	{ 1.0f, 1.0f, 0.5f },
	{ 1.0f, 0.5f, 1.0f },
	{ 0.5f, 1.0f, 1.0f }
};

#endif