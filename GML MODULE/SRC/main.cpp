
#include "main.h"
#include <conio.h>
#include "Brain_Fuck.h"
#include "Table.h"
#include "language_def.h"
#include "console_browser.h"




bool gotoxy(const WORD x, const WORD y) {
	COORD xy;
	xy.X = x;
	xy.Y = y;
	return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
}


class BrowserInterface
{
	const static int width{ 83 };
    const static int height{ 28 };
public:
	
	static void drawStartScreen()
	{
		// Not for production purposes. Literally bad code

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			//border left:[width:3 height:53] , right:[162, 53] , top [162,3] , bottom [162,53] ** Padding may be applied
		for (int i = 3; i <= 53; i++) { gotoxy(static_cast<WORD>(3), static_cast<WORD>(i)); std::cout << '|'; }
		for (int i = 3; i <= 53; i++) { gotoxy(static_cast < WORD>(162), static_cast<WORD>(i)); std::cout << '|'; }
		for (int i = 4; i <= 161; i++) { gotoxy(static_cast<WORD>(i), static_cast < WORD>(2)); std::cout << '-'; }
		for (int i = 4; i <= 161; i++) { gotoxy(static_cast<WORD>(i), static_cast < WORD>(54)); std::cout << '-'; }



		std::string displaytext = "Welcome To Comsoft Console Browser";
		gotoxy(static_cast<WORD>(width-(displaytext.length()/2)), static_cast<WORD>(height-10));
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN|FOREGROUND_INTENSITY);
		std::cout <<displaytext;
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN |FOREGROUND_BLUE|FOREGROUND_RED| FOREGROUND_INTENSITY);



		std::string format_line (80,'-');
		gotoxy(static_cast<WORD>(width - (format_line.length() / 2)), static_cast<WORD>(height - 8));
		std::cout << format_line;

		
		std::string session[3] = { "Start a new GML Session","Debug GML Session","Start Session as a File Browser" };
		gotoxy(static_cast<WORD>(width - (format_line.length() / 2)), static_cast<WORD>(height - 5));
		std::cout << "> "<<session[0];
		gotoxy(width - (format_line.length() / 2), height - 4);
		std::cout << "> "<<session[1];
		gotoxy(width - (format_line.length() / 2), height - 3);
		std::cout << "> "<<session[2];
		gotoxy(static_cast<WORD>(width - (format_line.length() / 2)), static_cast<WORD>(height - 1));
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED|FOREGROUND_BLUE);
		std::cout << "> Select option and press enter key to get started : ";


		int option = 0;
		char ch;
		while (ch = _getch())
		{
			if (ch == 'W'||ch=='w')
			{
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE);
				gotoxy(width - (format_line.length() / 2), height - 1);
				std::cout << "> Select option and press enter key to get started : " << std::string(32, ' ');
				gotoxy(width - (format_line.length() / 2), height - 1);
				std::cout << "> Select option and press enter key to get started : ";
				SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				std::cout << session[option];
				option = (option + 1) % 3;
				
			}
			else if (ch == 'Q' || ch == 'q')
			{
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
				switch (option)
				{

				case 1:
				newSession(); break;
				case 2:
				debugSession(); break;
				case 3:
				filebrowserSession(); break;

				}
				
			}
		}
		
	

	}
	static void newSession()
	{
	
		system("cls");
		//TODO: Dangerous Please Correct it ASAP
		std::cout << "Enter a filename to get started !";
		std::string filename;
		getline(std::cin, filename);
		std::cin.ignore();
		std::stringstream stream;
		GMLParser::extractTextFromFile(filename, stream);
		GMLParser::parseOnFly(stream.str());
		script_term();
		
		


	}
	static void script_term()
	{
		auto clear_row = [=](int height)
		{
			gotoxy(0, height);
			int wid = width * 2 + 3;
			while (wid--) { std::cout << ' '; }
		};
		clear_row((height * 2 - 3));
		auto draw_script_bar = []()
		{
			auto script_bar_height = (height * 2) - 4;
			gotoxy(0, script_bar_height);
			int w = width * 2 + 3;
			while (w--) { std::cout << (char)0xcd; }

		};
		draw_script_bar();
		std::string command;
		std::cout << "Enter a command : ";
		std::cin >> command;
		if (command == "exit")
		{
			exit(0);
		}
		// Add additional pieces of code

		
	}
	static void debugSession()
	{
		system("cls");
		std::cout << "Currently Under Maintainence Sorry !!";
	}
	static void filebrowserSession()
	{

		system("cls");
		std::cout << "Currently Under Maintainence Sorry !!";

	}
	
};

int main( )
{
	Rendering_Engine::initialize_screen( );
	const tag_traits::tag_properties prop("position",  tag_traits::tag_properties::format, false, false, position ,true );
	const tag_traits::tag_properties prop_1("color", tag_traits::tag_properties::format, false, false, color, true);
	const tag_traits::tag_properties prop_2("language", tag_traits::tag_properties::format, false, false, language, true);
	const tag_traits::tag_properties prop_3("Table", tag_traits::tag_properties::format, false, false, table, true);

	//Register the tag
	tag_traits::createBridge(prop);
	tag_traits::createBridge(prop_1);
	tag_traits::createBridge(prop_2);
	tag_traits::createBridge(prop_3);



	//BrowserInterface::drawStartScreen();
	//ConsoleBrowser::drawScreenBorder();
	/*Widget* wakanda = new Border({1,1},50,162);
    wakanda->draw();*/

	CForm form1(50,162);
	std::unique_ptr<Widget> w( new Border({ 2,2 }, 48, 160));
	std::unique_ptr<Widget> lab(new Label("Hello", { 3,3 }));
	

	form1.add_item("inner_border", std::move(w));
	form1.add_item("label_1", std::move(lab));
	form1.repaint();

	std::cin.get();
	return 0;
}
