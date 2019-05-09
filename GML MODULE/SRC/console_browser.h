#pragma once
#include <Windows.h>
#include "CommonScreenFunctions.h"	
#include <iostream>

class ConsoleBrowser
{
	static int screen_width;
	static int screen_height;
	static HANDLE hconsole;
public:
	static void drawUI()
	{


	}

private:
	
	
};
int ConsoleBrowser::screen_width { 0 };
int ConsoleBrowser::screen_height{ 0 };
HANDLE ConsoleBrowser::hconsole{ NULL};
