// Main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Engine.h"
#include "TextureManager.h"
#include <vld.h>

int main(int argc, char* argv[])
{
	Engine engine;

	if (engine.Initialize())
		engine.Run();

	engine.Shutdown();

	return 0;
}

