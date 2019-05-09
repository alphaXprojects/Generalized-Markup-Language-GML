#pragma once
#include <Windows.h>
#include <Mmsystem.h>
#include <mciapi.h>
#include <string>

class Audio
{
public:
	void playAudio(std::string format, std::string filename)
	{
		
		//open
		mciSendString(std::string("open \"" + filename + "." + format + "\"" + " type mpegvideo alias " + format).c_str(), NULL, 0, NULL);
		//play
		mciSendString(std::string("play " + format).c_str(), NULL, 0, NULL);
	}
};