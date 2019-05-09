#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

struct Row
{
	std::vector<std::string> row_values;
	std::vector<std::pair<std::string, std::string>> tag_val_pairs;
};



class TableParser
{
	//*********************************************************
	static  const  char tag_seperator{ ',' };
	static  const  char tag_value_seperator{ ':' };
	static  const  char tag_open{ '[' };
	static const char tag_close{ ']' };
	//*********************************************************

protected:
	// For testing code. In production code please remove the access specifier
  //*********************************************************
	static bool isClosed(const std::string& s1, const std::string& s2);
	static bool isClosed(std::string& str)
	{
		return str == "/";
	}
	static const std::string& trim(std::string& tag_text, char trimchar = ' ');
	static std::vector<std::string> splitIntoTokens(const std::string& tag_text, const char seperator = ' ');
	static std::pair<std::string, std::string>splitIntoToken(const std::string& text, const char seperator = ' ');
	static int getPrintableTextSize(Row r1);
	static bool TagContainsValue(const std::string& tag) { return std::find(tag.begin(), tag.end(), ':') != tag.end(); }
	//*********************************************************

public:
	static bool extractTextFromFile(const std::string& filename, std::stringstream& outstream);
	static void parseOnFly( const std::string& str, std::ostream& out = std::cout);
	static void printTable(std::vector<Row> rows, std::ostream& out);

private:
	//TODO: Add Helper functions to reduce dependencies

};
const std::string& TableParser::trim(std::string& tag_text, char trimchar)
{
	auto it = std::remove(tag_text.begin(), tag_text.end(), trimchar);
	tag_text.erase(it, tag_text.end());
	return tag_text;
}
std::vector<std::string> TableParser::splitIntoTokens(const std::string& tag_text, const char seperator)
{

	std::vector<std::string> tag_collection;
	std::string tag;
	if (tag_text.empty())
	{
		return tag_collection;

	}
	for (auto character : tag_text)
	{
		if (character == seperator)
		{
			tag_collection.push_back(tag); tag.clear(); continue;
		}
		tag += character;

	}
	if (!tag.empty())
	{
		tag_collection.push_back(tag);
	}
	return tag_collection;
}
std::pair<std::string, std::string> TableParser::splitIntoToken(const std::string& text, const char seperator)
{
	auto txtbeg = text.begin();
	auto txtend = text.end();
	auto seperator_pos = std::find(txtbeg, txtend, seperator);

	if (seperator_pos == txtend)
	{
		return std::make_pair(std::string(), std::string());
	}
	else if (seperator_pos == txtbeg)
	{
		return std::make_pair(std::string(), std::string(txtbeg + 1, txtend));
	}
	else if (seperator_pos == txtend - 1)
	{
		return std::make_pair(std::string(txtbeg, txtend - 1), std::string());

	}
	else
	{
		return std::make_pair(std::string(txtbeg, seperator_pos), std::string(seperator_pos + 1, txtend));
	}
}
int TableParser::getPrintableTextSize(Row r)
{
	int length=0;
	for (auto& x : r.row_values)
		length += x.length() + 1;
	return length;
}
bool TableParser::extractTextFromFile(const std::string & filename, std::stringstream & outstream)
{
	std::ifstream file(filename);
	if (!file)
	{
		return false;
	}
	outstream << file.rdbuf();
	file.close();
	return true;
}
void TableParser::parseOnFly(const std::string& str, std::ostream& out)
{
	
	std::vector<Row> rows;
	auto strbeg = str.begin();
	auto strend = str.end();

	while (strbeg != strend)
	{
		auto OTAG_openbrace = std::find(strbeg, strend, tag_open);

		//Error/SpecialCase
		if (OTAG_openbrace == strend)
		{
			//..
			//Rendering_Engine::render(strbeg, strend, out);
			out<< "ERROR";
			return;
		}

		//Print the data not between two different tags
		//Rendering_Engine::render(strbeg, OTAG_openbrace, out);

		auto OTAG_closebrace = std::find(OTAG_openbrace, strend, tag_close);

		//Error/SpecialCase
		if (OTAG_closebrace == strend)
		{

			out << "SYNTAX ERROR FOUND";
			return;
		}

		std::string OTAG_attribtext(OTAG_openbrace + 1, OTAG_closebrace);
		  
		//--------------------------------------------------------------------------------------------------------------------------

		auto CTAG_openbrace = std::find(OTAG_closebrace + 1 , strend, tag_open);



		//Error/SpecialCase
		if (CTAG_openbrace == strend)
		{
			out << "SYNTAX ERROR FOUND";
			return;
		} //Error/SpecialCase


		auto CTAG_closebrace = std::find(CTAG_openbrace, strend, tag_close);

		//Error/SpecialCase
		if (CTAG_closebrace == strend)
		{
			out << "SYNTAX ERROR FOUND";
			return;
		}

		std::string CTAG_attribtext(CTAG_openbrace + 1, CTAG_closebrace);

		//***********************
		trim(OTAG_attribtext);
		trim(CTAG_attribtext);
		//***********************

		//Check whether the tags are in the proper format and dispatch them out to rendering engine
		if (isClosed(CTAG_attribtext) || isClosed(OTAG_attribtext, CTAG_attribtext))
		{

			bool contains_row_tag = false;
			std::string inner_text(OTAG_closebrace + 1 , CTAG_openbrace);
			std::vector<std::string> inner_text_vector=splitIntoTokens(inner_text,',');


			auto tags = splitIntoTokens(OTAG_attribtext, tag_seperator);
			if (tags.empty())
			{
				out << "SYNTAX ERROR FOUND"; return;
			}

			std::vector <std::pair<std::string,std::string>> tag_value_pairs;
			for (auto& tag : tags)
			{
				auto tag_state = TagContainsValue(tag);
				std::pair<std::string, std::string> tag_value_pair;
				if (tag_state)
					tag_value_pair = splitIntoToken(tag, tag_value_seperator);
				else
					tag_value_pair = std::make_pair(tag, std::string(""));

				std::string rtag = "R";
				if (std::equal(rtag.begin(), rtag.end(), tag_value_pair.first.begin()))
				{
					contains_row_tag=true;
				}

				//TODO: Requires refactoring for improving extensiblity (tag_traits will become generic in future)
				if (tag_value_pair.first.empty()/*&& tag_value_pair.second.empty()*/ )
				{

					out << "SYNTAX ERROR FOUND"; return;
				}




				tag_value_pairs.push_back(std::make_pair(tag_value_pair.first,tag_value_pair.second));


			}

			if (!contains_row_tag)
			{
				out << "Error"; return;

			}
			rows.push_back({ inner_text_vector, tag_value_pairs });
			strbeg = CTAG_closebrace + 1;
		}
		else
		{
			out << "SYNTAX ERROR FOUND";
			return;
		}


	}
	printTable(rows, out);

}


void TableParser::printTable(std::vector<Row> rows, std::ostream& out)
{
	auto length_of_table = 0;
	
	for (auto& row : rows)
		length_of_table = length_of_table < getPrintableTextSize(row) ? getPrintableTextSize(row) : length_of_table;
	
	auto no_of_cols = rows[0].row_values.size();
	std::vector<int>column_padding;

	for (auto i = 0; i < no_of_cols; i++)
	{
		int max = 0;

		for (auto& row : rows)
		{
			max = max < row.row_values[i].length() ? row.row_values[i].length() : max;
		}
		column_padding.push_back(max);
	}

	std::string resultant_table(length_of_table+4,'-');
	for (auto row : rows)
	{
		resultant_table += '\n';
		for (int i = 0; i < no_of_cols; i++)
		{
			if (i==0)
			{
				resultant_table += "| ";
			}
			else if (i == 1)
			{
				resultant_table += " |";
			}
			std::string padded_data(column_padding[i], ' ');
			int half_length = (row.row_values[i].length())/2;
			int padding = column_padding[i] / 2;
			int positional_index = padding - half_length;
			for (auto it = 0; it < row.row_values[i].length(); it++)
			{
				padded_data[positional_index + it] = row.row_values[i][it];

			}
				resultant_table += padded_data;
			if (i)
			{
					resultant_table += "| ";
			}

		}
		
	    resultant_table+="\n"+std::string(length_of_table + 4, '-');
	}
	out << resultant_table;
	


}



bool TableParser::isClosed(const std::string & s1, const std::string & s2)
{

	std::string ending_tag = s2;
	if (*ending_tag.begin() != '/')
	{
		return false;
	}
	ending_tag.erase(ending_tag.begin());

	auto split_tag_value = splitIntoToken(s1, tag_value_seperator);

	return std::equal(split_tag_value.first.begin(), split_tag_value.first.end(), ending_tag.begin(), ending_tag.end());
}


//int main()
//{
//	std::cout << "Enter the GML Code -";
//	std::string str;
//	std::getline(std::cin, str);
//	TableParser::parseOnFly(str, std::cout);
//	std::cin.get();
//}
