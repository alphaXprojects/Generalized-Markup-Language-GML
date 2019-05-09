#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <Windows.h>
#include "tag_traits.h"

class Rendering_Engine
{
	
public:
	[[deprecated("Rely on this only when the second overload dosent work") ]]
	static bool initialize_screen(const RECT & dimension);
	static bool initialize_screen(short width, short height);
	static bool initialize_screen( )
	{
		return ShowWindow(GetConsoleWindow( ), SW_MAXIMIZE);

	}
	static RECT getDesktopResolution( )
	{
		RECT desktop_res;
		HWND hdesktop = GetDesktopWindow( );
		GetWindowRect(hdesktop, &desktop_res);
		return desktop_res;
	}
	
	
	static void get_DispatchedDataFromParser( const std::string & text,  std::vector<tag> & tags, std::ostream & out = std::cout);
	static void render(std::string::const_iterator &beg, std::string::const_iterator &end, std::ostream & out = std::cout);
	static void render(const std::string& str, std::ostream& out = std::cout)
	{
		out << str;
	}

};



bool Rendering_Engine::initialize_screen(const RECT & dimension)
{
	// Set the position to the top left corner of the screen
	HWND hwnd = GetConsoleWindow( );
	if ( hwnd != NULL )
	{
		bool m = MoveWindow(hwnd, 0, 0, 0, 0, TRUE);


	}

	//Set the window size according to the parameters
	//------------------------------------------------
	short width = static_cast<short>(dimension.right *0.125) ;    // width in characters
	short height = static_cast<short>( dimension.bottom * 0.125); // height in characters
	//-------------------------------------------------
	HANDLE buff = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD  buffdimension;
	buffdimension.X = width;
	buffdimension.Y = height;
	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = height-1;
	Rect.Right = width -1;

	bool x = SetConsoleWindowInfo(buff, TRUE, &Rect);
	bool state = SetConsoleScreenBufferSize(buff, buffdimension);
	if ( !state )
	{
		return false;
	}
	return true;
}

 bool Rendering_Engine::initialize_screen(short width, short height)
{
	 const RECT dimension { 0,0,width,height };
#pragma warning(disable:4996)
	 return initialize_screen(dimension);
}

 void Rendering_Engine::get_DispatchedDataFromParser( const  std::string & inner_text,  std::vector<tag> & tags, std::ostream & out)
 {
	 std::string mod_innertext=inner_text;
	 std::pair<std::string, int> result;
	 auto precedence_condition=[](const tag & tag_value_prop ) ->bool
	 {
		 return tag_value_prop.prop.type == tag_traits::tag_properties::tag_type::text;
	 };
	 //out << "\n"<<inner_text<<":"<<"\n";
	std::partition(tags.begin(), tags.end(), precedence_condition);

	//DIRTY CODE:
	auto get_cursor_pos = []()
	{
		CONSOLE_SCREEN_BUFFER_INFO cbsi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cbsi);
		return cbsi.dwCursorPosition;
	};
	auto set_cursor_pos = [](WORD x, WORD y)
	{
		COORD xy;
		xy.X = x;
		xy.Y = y;
		return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
	};


	 for (auto  x= tags.begin(); x!=tags.end();x++ )
	 {  
	     // Errors in code
		 // Proposed Code : This is the way the handler is going to be called  recursively
		
		 auto current_pos = get_cursor_pos();
		 result=  x->prop.handler(x->prop,x->value, mod_innertext,out);
		 if (result.first != "")
		 {
			 mod_innertext = result.first;
			 
		 }
		 else if (x ==(tags.end()-1))
		 {
			 continue;
		 }
		else if( tags.size()>1)
		{
			set_cursor_pos(current_pos.X,current_pos.Y);
		}
		  // Remove this code only when  tests are  cleared up
	//	out << x.name<<"-"<<x.value << std::endl;
	 }
	 if (result.second==false)
	 {
		 out << mod_innertext;
		
	 }
 }

 void Rendering_Engine::render(std::string::const_iterator & beg, std::string::const_iterator & end, std::ostream & out)
 {
	  for ( auto it = beg; it != end; it++ )
	  {
		  out << *it;
	  }

 }