#pragma once


#include <sstream>
#include <boost/utility/string_view.hpp>

#include "program_options.h"

namespace logging
{
	inline std::wstring format_header(boost::wstring_view const &header)
	{
		static const std::wstring wrapper_part = L"---------------------";
		return wrapper_part + header.to_string() + wrapper_part;
	}

	inline std::wstring format_tree_element(boost::wstring_view const &element)
	{
		return L"|-" + element.to_string();
	}

	inline std::wstring format_file_content_and_cfg_descr(configuration::FileConfig const *cfg, boost::wstring_view content)
	{
		std::wstringstream stream;
		stream << logging::format_header(L"Конфигурация файла") << L'\n'
			<< L"Файл:\n" << cfg->path_to_file << L'\n'
			<< L"Содержимое:\n" << content
			<< std::endl;

		return stream.str();
	}

	namespace t_sorter
	{
		inline std::wstring get_manual();
		inline std::wstring wagons_to_string(boost::wstring_view const &wagons_type, size_t count)
		{
			return std::to_wstring(count) + L" x " + wagons_type.to_string();
		}
	}
}