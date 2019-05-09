#pragma	once
#include <map>
#include <string>
#include <functional>
// TODO : To make it generic make the class non-static in nature. But all tests must be compiled again with 99.65% coverage
class tag_traits
{
	
public:
	struct tag_properties
	{
		std::string tag_name;
		
		enum tag_type{text, format} type;
		bool present_at_start;
		bool present_at_end;
		bool is_inbuilt;

		
		//TODO : Possible refactoring required for improving extensiblity
		using TAG_HANDLER=std::function<std::pair<std::string,bool>(const tag_properties& prop, const std::string& value, const std::string& text,std::ostream & out)>;
		TAG_HANDLER handler;
		tag_properties():tag_name(""),type(format),present_at_start(false),present_at_end(false),handler(nullptr),is_inbuilt(false)
		{}
		tag_properties(const std::string & name, tag_type type, bool ps, bool pe, TAG_HANDLER handler, bool is_inbuilt) :tag_name(name), type(type), present_at_start(ps), present_at_end(pe), handler(handler), is_inbuilt(is_inbuilt)
		{}

	};
	static bool isTag(const std::string & name)
	{
		return taglist.count(name) > 0;
	}
	static bool createBridge(const tag_properties & tagprop)
	{

		if (isTag(tagprop.tag_name))
		{
			if (tagprop.is_inbuilt)
				return false;
		}
		taglist[tagprop.tag_name] = tagprop;
		return true;
	}
	static const auto& getTagProperties(std::string tag_name)
	{
		return taglist.at(tag_name);
	}
private:
	// Complete list of  tags along with their corrosponding property
	static std::map<std::string, tag_properties> taglist;
};
// Initializing the map with inbuilt tags : :) Sorry for beautifying the code
std::map<std::string, tag_traits::tag_properties> tag_traits::taglist 
{
	{ "blink",  {"blink",  tag_traits::tag_properties::format, false, false, 0 ,true} },
	{ "style",  {"style",  tag_traits::tag_properties::format, false, false, 0 ,true} },
	{ "link",   {"link",   tag_traits::tag_properties::format, false, false, 0 ,true } },
	{ "dummy2", {"dummy2", tag_traits::tag_properties::format, false, false, 0 ,true} },
	{ "dummy3", {"dummy3", tag_traits::tag_properties::format, false, false, 0 ,true} }
};



// Stores the tag data along with  information about that tag
struct tag
{
	std::string name;
	std::string value;
	tag_traits::tag_properties prop;

};