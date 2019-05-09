#pragma once

#include "main.h"
#include "Brain_Fuck.h"
#include "Table.h"
#define TAG(a,b,c,d,e) std::pair<std::string,bool> ##a (const tag_traits::tag_properties& ##b, const std::string& ##c, const std::string& ##d, std::ostream &##e)


const std::string& trim(std::string& tag_text, char trimchar)
{
	auto it = std::remove(tag_text.begin(), tag_text.end(), trimchar);
	tag_text.erase(it, tag_text.end());
	return tag_text;
}

// Global registration and definition of function
TAG(position,prop,value,innertext,out)
{
	std::string text;
	if (value == "center")
	{
		text = "\t\t\t\t\t\t\t\t\t" + innertext;
	}
	//Requires some work
	else if (value == "right")
	{
		text = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t" + innertext;
	}
	return std::make_pair(text, false);

}
TAG(color, prop, value, innertext, out)
{


	if (value == "red")
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
	}
	else if (value == "blue")
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);
	}
	else if (value == "green")
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
	}
	if (value == "RED")
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
	}
	else if (value == "BLUE")
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}
	else if (value == "GREEN")
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}
	Rendering_Engine::render(innertext, out);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	return make_pair(std::string(""), true);
}
TAG(language, prop, value, innertext, out)
{
	std::stringstream strstream;
	if (value == "brainfuck")
	{
		BrainFuckInterpreter::evaluate(innertext, std::cin, strstream);
		return std::make_pair(strstream.str(), false);
	}
}
TAG(table, prop, value, innertext, out)
{
	std::string innerText = innertext;
	trim(innerText, '\n');
	std::stringstream strstream;
	if (value == "normal"|| value=="")
	{
		
		TableParser::parseOnFly(innerText, strstream);
		return std::make_pair(strstream.str(), false);

	}
}


