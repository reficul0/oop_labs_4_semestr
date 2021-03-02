#pragma once

#include <iomanip>

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/regex.hpp>
#include <boost/regex.hpp>
#include <boost/cast.hpp>

#include "program_options.h"
#include "Demonstratator.h"
#include "tools/interprocess_safe_buffered_file_stream.h"

// ������� 10
// �������� ���������, ������� ��������� ���������� ����� �� ����� � �������
// �� ����� ����� ������, ������������ � ������� ���� � �������������� �������� �������.// \b(?i)(([aeiou]\b)|[aeiou]([a-z]*)[aeiou])\b// \b ������� ��������// (?i) case insensetive// [aeiou] �������// [a-z]* ����� ����� ������� ������ ���
template<>
struct Demonstratator<struct FileFiltherTag>
{
	static void demonstrate(configuration::Configuration const *cfg_base)
	{
		decltype(auto) file_cfg = boost::polymorphic_cast<configuration::FileConfig const*>(cfg_base);
		tools::interprocess::buffered::SafeToReadStlFileStream istream(file_cfg->path_to_file, file_cfg->locale);

		try
		{
			istream.initialize();
		}
		catch (std::exception const &e)
		{
			std::cerr << "��� �������� ����� " << std::quoted(file_cfg->path_to_file.string()) <<  " � ������� ��� ����������� � ������ ��������� ������: " << e.what() << std::endl;
			return;
		}
		decltype(auto) file_data = istream.data();
		decltype(auto) current = istream.current();
		auto end = istream.end();

		std::wcout << logging::wformat_file_content_and_cfg_descr(file_cfg, file_data) << std::endl;

		while (true)
		{
			std::wstring filter;
			std::cout << "������� regex ���������: " << std::endl;
			std::wcin >> filter;

			boost::wregex expr;
			try
			{
				expr = boost::wregex{ filter };
			}
			catch (const std::exception &e)
			{
				std::cerr << "������ � ���������: " << e.what() << std::endl;
				continue;
			}

			boost::wcregex_iterator matches_current(file_data.c_str(), file_data.c_str() + file_data.size(), expr, boost::match_default | boost::match_partial);
			boost::wcregex_iterator matches_end;
			size_t match_id(1);

			if (matches_current == matches_end)
			{
				std::cout << "���������� ���." << std::endl;
				continue;
			}

			std::cout << "����������: " << std::endl;
			for (;matches_current != matches_end; ++matches_current)
			{
				decltype(auto) match = (*matches_current)[0];

				if (!match.matched)
					continue;

				std::wcout
					<< std::to_wstring(match_id) << L". "
					<< std::quoted(match.str())
					<< std::endl;

				++match_id;
			}
		}
	}
};