


#pragma once
#include <cstdlib>
#include <crtdbg.h>
#include <Windows.h>
#include <iostream>

int compile(int)
{
	return 0;
}


bool check(int)
{
	return true;
}


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
