#pragma once
#include <stack>
#include <string>
#include <vector>
#include <iterator>

class BrainFuckInterpreter
{
	static std::stack<std::string::iterator> state;
	static std::vector<char> memory_cells;
	static std::string::iterator instr_ptr;
	static std::vector<char>::iterator data_ptr;
	static void nextCell()
	{
		data_ptr++;
	}
	static void prevCell()
	{
		data_ptr--;
	}
	static void getinp(std::istream & in)
	{
		*data_ptr = in.get();
	}
	static void putOut(std::ostream &out)
	{
		out << (*data_ptr);
	}
	static void incrementCellVal()
	{
		(*data_ptr)++;
		
	}
	static void decrementCellVal()
	{
		(*data_ptr)--;
	}
	
	static void loop_beg()
	{
		state.push(instr_ptr);

	}
	static std::string::iterator loop_end()
	{
		if (state.empty())
		{
			return instr_ptr;
		}
		else if (*data_ptr != 0)
		{
		
			// Suspense GUESS THE REASON :)
			return std::prev(state.top()+2);
			
			
		}
		else
		{
			state.pop();
			return instr_ptr+1;
		}


	}

public:

	static  void evaluate(std::string code, std::istream & in, std::ostream& out)
	{
		data_ptr = memory_cells.begin();
		instr_ptr = code.begin();
		while(instr_ptr!=code.end())
		{
			switch (*instr_ptr)
			{
			case '>':
				nextCell(); 
				break;
			case '<':
				prevCell(); 
				break;
			case ',':
				getinp(in); 
				break;
			case '.':
				putOut(out); 
				break;
			case '+':
				incrementCellVal();
			break;
			case '-':
				decrementCellVal(); 
				break;
			case '[':
				loop_beg();
				break;
			case ']':
				instr_ptr=loop_end();
				continue;

			}
			instr_ptr++;
		}

	}

};

 std::stack<std::string::iterator> BrainFuckInterpreter::state;
 std::vector<char> BrainFuckInterpreter::memory_cells(30000); 
 std::string::iterator BrainFuckInterpreter::instr_ptr;
 std::vector<char>::iterator BrainFuckInterpreter::data_ptr;


//int main()
//{
//	std::cout << "Please Enter the code";
//	std::string str;
//	std::string filename;
//	std::getline(std::cin, filename);
//	std::ifstream file(filename);
//	std::stringstream s;
//	s << file.rdbuf();
//	str = s.str();
//	BrainFuckInterpreter::evaluate(str);
//	std::cin.get();
//}
