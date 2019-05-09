#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include "rendering_engine.h"
#include "tag_traits.h"


class GMLParser
{
	//*********************************************************
	 static  const  char tag_seperator { ',' };
	 static  const  char tag_value_seperator { ':' };
	 static  const  char tag_open { '[' };
	 static const char tag_close { ']' };
   //*********************************************************

protected:
	 // For testing code. In production code please remove the access specifier
   //*********************************************************
	static bool isClosed(const std::string & s1, const std::string & s2);
	static bool isClosed(std::string & str)
	{
		return str == "/";
	}
	static const std::string& trim(std::string & tag_text, char trimchar = ' ');
	static std::vector<std::string> splitIntoTokens(const std::string& tag_text, const char seperator=' ');
	static std::pair<std::string, std::string>splitIntoToken(const std::string & text, const char seperator = ' ');
	//*********************************************************

public:
	static bool extractTextFromFile(const std::string & filename, std::stringstream & outstream);
	static void parseOnFly(const std::string & str, std::ostream & out=std::cout);

private:
	//TODO: Add Helper functions to reduce dependencies

};

 const std::string& GMLParser::trim(std::string & tag_text, char trimchar)
{
	 auto it = std::remove(tag_text.begin( ), tag_text.end( ),trimchar);
	 tag_text.erase(it, tag_text.end( ));
	 return tag_text;
}

 std::vector<std::string> GMLParser::splitIntoTokens(const std::string & tag_text, const char seperator)
 {
	 
	 std::vector<std::string> tag_collection;
	 std::string tag;
	 if ( tag_text.empty())
	 {
		 return tag_collection;

	 }
	 for ( auto character : tag_text )
	 {
		 if ( character == seperator )
		 {
			 tag_collection.push_back(tag); tag.clear( ); continue;
		 }
		 tag += character;

	 }
	 if ( !tag.empty( ) )
	 {
		 tag_collection.push_back(tag);
	 }
	 return tag_collection;
 }

 std::pair<std::string, std::string> GMLParser::splitIntoToken(const std::string & text, const char seperator)
 {
	 auto txtbeg = text.begin( );
	 auto txtend = text.end( );
	 auto seperator_pos = std::find(txtbeg, txtend, seperator);

	 if ( seperator_pos==txtend )
	 {
		 return std::make_pair(std::string(), std::string());
	 }
	 else if ( seperator_pos == txtbeg)
	 {
		 return std::make_pair(std::string(), std::string(txtbeg + 1, txtend));
	 }
	 else if ( seperator_pos == txtend-1 )
	 {
		 return std::make_pair(std::string(txtbeg, txtend - 1),std::string());

	 }
	 else
	 {
		 return std::make_pair(std::string(txtbeg, seperator_pos), std::string(seperator_pos + 1, txtend));
	 }
 }

bool GMLParser::extractTextFromFile(const std::string & filename, std::stringstream & outstream)
{
	 std::ifstream file(filename);
	 if ( !file )
	 {
		 return false;
	 }
	 outstream << file.rdbuf( );
	 file.close( );
	 return true;
}

void GMLParser::parseOnFly( const std::string & str, std::ostream & out)
{

	auto strbeg = str.begin( );
	auto strend = str.end( );

	while ( strbeg != strend )
	{
		auto OTAG_openbrace = std::find(strbeg, strend, tag_open);

		//Error/SpecialCase
		if ( OTAG_openbrace == strend )
		{
			//..
			Rendering_Engine::render(strbeg, strend,out);
			return;
		}

		//Print the data not between two different tags
		Rendering_Engine::render(strbeg, OTAG_openbrace, out);

		auto OTAG_closebrace = std::find(OTAG_openbrace, strend, tag_close);

		//Error/SpecialCase
		if ( OTAG_closebrace == strend )
		{
			
			out << "SYNTAX ERROR FOUND";
			return;
		}

		std::string OTAG_attribtext(OTAG_openbrace + 1, OTAG_closebrace);
		
		//Requires major refactoring DO ASAP
		int search_pos=0;
		int inner_text_offset = 0;

		auto raw_str =(OTAG_closebrace+1);
		if (*raw_str == '"')
		{
			
			search_pos = (raw_str=std::find(OTAG_closebrace + 2, strend, '"'))!=strend ? ((raw_str)-(OTAG_closebrace+1)+1):0 ;
			inner_text_offset = search_pos != 0 ? 1 : 0;
		}

		//--------------------------------------------------------------------------------------------------------------------------

		auto CTAG_openbrace = std::find(OTAG_closebrace + 1+search_pos, strend, tag_open);



		//Error/SpecialCase
		if ( CTAG_openbrace == strend )
		{
			out << "SYNTAX ERROR FOUND";
			return;
		} //Error/SpecialCase


		auto CTAG_closebrace = std::find(CTAG_openbrace, strend, tag_close);

		//Error/SpecialCase
		if ( CTAG_closebrace == strend )
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
		if ( isClosed(CTAG_attribtext) || isClosed(OTAG_attribtext, CTAG_attribtext) )
		{

			
			std::string inner_text(OTAG_closebrace + 1+inner_text_offset, CTAG_openbrace-inner_text_offset);
			auto tags = splitIntoTokens(OTAG_attribtext,tag_seperator);
			if ( tags.empty( ) )
			{
				out << "SYNTAX ERROR FOUND"; return;
			}

			std::vector <tag> tag_value_pairs;
			for ( auto & tag : tags )
			{
				auto tag_value_pair = splitIntoToken(tag, tag_value_seperator);
				
				//TODO: Requires refactoring for improving extensiblity (tag_traits will become generic in future)
				if ( tag_value_pair.first.empty( )/*&& tag_value_pair.second.empty()*/ || (!tag_traits::isTag(tag_value_pair.first))  )
				{
						
						out << "SYNTAX ERROR FOUND"; return;
				}

							
				
	
				tag_value_pairs.push_back({ tag_value_pair.first,tag_value_pair.second,tag_traits::getTagProperties(tag_value_pair.first) });
				

			}

			Rendering_Engine::get_DispatchedDataFromParser(inner_text, tag_value_pairs,out);
			strbeg = CTAG_closebrace + 1;
		}
		else
		{
			out << "SYNTAX ERROR FOUND";
			return;
		}


	}

}


 bool GMLParser::isClosed(const std::string & s1, const std::string & s2)
 {
	 
	 std::string ending_tag = s2;
	 if ( *ending_tag.begin() != '/' )
	 {
		 return false;
	 }
	 ending_tag.erase(ending_tag.begin());
	 
	 auto split_tag_value = splitIntoToken(s1, tag_value_seperator);

	 return std::equal(split_tag_value.first.begin( ), split_tag_value.first.end( ), ending_tag.begin( ), ending_tag.end( ));
 }
