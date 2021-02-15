#include "pch.h"


#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <iterator>
#include <sstream>
#include <string>

class BasicStringView
{
public:
	using string_type = std::string;
protected:
	string_type *_data;
public:
	BasicStringView(std::string *data)
		: _data(data)
	{}
	virtual ~BasicStringView() = default;

	virtual string_type GetOct() = 0;
	virtual string_type GetDec() = 0;
	virtual string_type GetHex() = 0;
};

class StringView
	: public BasicStringView
{
public:
	StringView(std::string *data)
		: BasicStringView(data)
	{}

	string_type GetOct() override
	{
		std::stringstream ss;
		ss << std::oct;
		WriteData(ss);
		
		return ss.str();
	}
	string_type GetDec() override
	{
		std::stringstream ss;
		ss << std::dec;
		WriteData(ss);

		return ss.str();
	}
	string_type GetHex() override
	{
		std::stringstream ss;
		ss << std::hex;
		WriteData(ss);
		
		return ss.str();
	}
private:
	void WriteData(std::stringstream &ss)
	{
		for (auto character : *_data)
			ss << (uint16_t)character;
	}
};

class BinaryStringView
	: public StringView
{
protected:
	string_type _data_as_simple_char;
public:
	BinaryStringView(std::string *data)
		: StringView(&_data_as_simple_char)
		, _data_as_simple_char(_ToSimleString(data))
	{
	}
private:
	std::string _ToSimleString(std::string *data)
	{
		static constexpr  uint8_t bits_count_in_byte = 8;

		assert( !(data->size() % bits_count_in_byte) );
		
		std::string result;
		result.reserve(data->size() / bits_count_in_byte);

		uint8_t character_value = 0;
		uint8_t offset = 0;
		for(auto bit_value : *data)
		{
			bool is_true = bit_value == '1';
			character_value |= is_true << offset++;
			
			if(offset == bits_count_in_byte)
			{
				result.push_back(*reinterpret_cast<char*>(&character_value));
				offset = 0;
				character_value = 0;
			}
		}
		return std::move(result);
	}
};

int main()
{
	std::string source = "1 2 3";
	StringView string_view(&source);

	std::cout << "Source string :" << source << std::endl;
	std::cout << "Dec string:" << string_view.GetDec() << std::endl;
	std::cout << "Hex string:" << string_view.GetHex() << std::endl;
	std::cout << "Oct string:" << string_view.GetOct() << std::endl;

	std::string bin_source = "11110000";
	BinaryStringView bin_string_view(&bin_source);

	std::cout << "Source bin string :" << bin_source << std::endl;
	std::cout << "Dec string from bin string:" << bin_string_view.GetDec() << std::endl;
	std::cout << "Hex string from bin string:" << bin_string_view.GetHex() << std::endl;
	std::cout << "Oct string from bin string:" << bin_string_view.GetOct() << std::endl;
	
	return 0;
}