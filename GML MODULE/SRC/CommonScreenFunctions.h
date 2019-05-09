#pragma once
#include <Windows.h>
#include <memory>
#include <vector>
#include <map>
#include <thread>
#include <vector>
#include <mutex>
class Caret
{
public:
	static COORD getxy()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
			return csbi.dwCursorPosition;
		else
			throw std::exception("GetConsoleScreenBufferInfo-Returned-False");
	}
	static void gotoxy(const WORD x, const WORD y) 
	{
		COORD xy;
		xy.X = x;
		xy.Y = y;
		if (!SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy))
			throw std::exception("Set_Console_Cursor_PositionReturned_False");
		
	}
};

//Widget: Tools on Form
class Widget
{
public:
	bool  isfocused{ false };
	virtual void draw()=0;

};


class Border :public Widget
{
	COORD start_coordinate;
	char top_border;
	char left_border;
	char right_border;
	char bottom_border;

	int length;
	int width;

public:
	Border(COORD start_coord, int length, int width, char border_character) 
	:length(length),width(width),top_border(border_character),left_border(border_character),bottom_border(border_character),right_border(border_character),start_coordinate(start_coord)
	{}

	Border(COORD start_coord, int length, int width, char left_bord_char='|', char right_bord_char='|', char top_bord_char='-' , char bottom_bord_char='-') \
	: length(length), width(width), top_border(top_bord_char), left_border(left_bord_char), bottom_border(bottom_bord_char), right_border(right_bord_char),start_coordinate(start_coord)
	{}


	void draw() override
	{
		
		Caret::gotoxy(start_coordinate.X, start_coordinate.Y);
		COORD current_coordinate = start_coordinate;
		auto print_without_moving_cursor = [](char character, COORD coord)
		{
			DWORD dummy;
			WriteConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), &character, 1, coord, &dummy);
		};
		
	    //Top border
		for (int i = current_coordinate.X+1; i <= current_coordinate.X + width; i++) 
		{
			Caret::gotoxy(i, current_coordinate.Y); print_without_moving_cursor(top_border, {static_cast<SHORT>(i),current_coordinate.Y});
		}

	   //Right border
		current_coordinate=Caret::getxy();
		for (int i = current_coordinate.Y + 1; i <= current_coordinate.Y + length; i++) 
		{
			Caret::gotoxy(current_coordinate.X, i); print_without_moving_cursor(right_border, { current_coordinate.X + static_cast < SHORT>(1),static_cast<SHORT>(i) });
		}

		// Left border
		current_coordinate = start_coordinate;
		for (int i = current_coordinate.Y+1 ; i <= current_coordinate.Y + length; i++)
		{
			Caret::gotoxy(current_coordinate.X, i); print_without_moving_cursor(right_border, { current_coordinate.X ,static_cast<SHORT>(i) });
		}

		//Bottom Border
		current_coordinate = { start_coordinate.X, start_coordinate.Y + static_cast<SHORT>(length+1) };
		for (int i = current_coordinate.X + 1; i <= current_coordinate.X + width; i++)
		{
			Caret::gotoxy(i, current_coordinate.Y); print_without_moving_cursor(top_border, { static_cast<SHORT>(i),current_coordinate.Y});
		}

	
	}

};
class Label :public Widget
{
	std::string text;
	COORD position;
public:
	Label() :text(""), position({ 0,0 }) {}
	Label(std::string txt, COORD pos = {1,1} ) :text(txt), position(pos) {}

	std::string& Text(std::string txt) 
	{return text = txt; }

	
	void draw() override
	{
		auto previous_position = Caret::getxy();
		Caret::gotoxy(position.X, position.Y);
		std::cout << text;
		Caret::gotoxy(previous_position.X,previous_position.Y);
	}
};
class ItemSelector
{


};

class CForm
{
	int height;
	int width;
	std::map<std::string,std::unique_ptr<Widget>>elements;
	std::vector<std::string> tab_order;
	int current_focus;

public:
	CForm() :height(0),width(0),current_focus(0)
	{

	}
	CForm(int height, int width, COORD coordinates = { 1,1 }) :height(height), width(width),current_focus(0)
	{
		Widget* border = new Border(coordinates,height,width);
		elements["base_border"] = std::unique_ptr<Widget>(border);

		// TODO : Additional Book Keeping in future


	}
	void add_item(std::string object_name, std::unique_ptr<Widget> w)
	{
		elements[object_name] = std::move(w);
		tab_order.push_back(object_name);
		
	}
	void remove_item(std::string object_name)
	{
		auto deletion_element = elements.find(object_name);
		elements.erase(deletion_element);

		/**/
		auto del_element = std::find(tab_order.begin(), tab_order.end(), object_name);
		tab_order.erase(del_element);
		
	}
	void paint()
	{
		
		for (auto& x : elements)
		{
			x.second->draw();
		}

	}
	void repaint()
	{
		system("cls");
		paint();
	}
	bool changeTabOrder(std::string object_name, int new_position)
	{
		auto current_position = std::find(tab_order.begin(), tab_order.end(), object_name);
		if(current_position == tab_order.end())
		{
			return false;
		}
		else
		{
			tab_order.erase(current_position);
		}

		tab_order.insert(tab_order.begin() + new_position, object_name);
		return true;

	}

	//If runtime edition of elements become possible then there will be a race around condition
	void keyeventController()
	{
		int current_tab_order = 0;
		while (_kbhit())
		{
		  
		}
	}
};