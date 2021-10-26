


#pragma once
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glew.h>
#include <cstdlib>
#include <crtdbg.h>
#include <Windows.h>
#include <iostream>


thread_local Assimp::Importer s_Importer;
thread_local const aiScene* s_Scene;

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	int t = argc;
	while (t--)
	{
		std::cout << argv[t] << std::endl;
		return t;
	}
}
