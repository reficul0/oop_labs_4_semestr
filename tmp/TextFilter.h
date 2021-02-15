#pragma once

#include <iomanip>

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/regex.hpp>
#include <boost/regex.hpp>

#include "program_options.h"
#include "Demonstratator.h"
#include "tools/interprocess_safe_buffered_file_stream.h"

// ������� 10
// �������� ���������, ������� ��������� ���������� ����� �� ����� � �������
// �� ����� ����� ������, ������������ � ������� ���� � �������������� �������� �������.// \b(?i)(([aeiou]\b)|[aeiou]([a-z]*)[aeiou])\b// ������� �������: ��� ����� ��������� // \b ������� ��������// (?i) case insensetive// [aeiou] �������// [a-z]* ����� �����
template<>
struct Demonstratator<struct FileFiltherTag>
{
	static void demonstrate(configuration::Configuration const *cfg_base)
	{
		decltype(auto) lab_cfg = dynamic_cast<configuration::FileFilterConfig const*>(cfg_base);
		BOOST_ASSERT_MSG(!lab_cfg, "� ������� ������������ ������ ������������ ������ ������ ������������.");

		tools::interprocess::buffered::SafeToReadStlFileStream istream(lab_cfg->path_to_file, lab_cfg->locale);

		try
		{
			istream.initialize();
		}
		catch (std::exception const &e)
		{
			std::cerr << "��� �������� ����� " << std::quoted(lab_cfg->path_to_file.string()) <<  " � ������� ��� ����������� � ������ ��������� ������: " << e.what() << std::endl;
			return;
		}
		decltype(auto) file_data = istream.data();
		decltype(auto) current = istream.current();
		auto end = istream.end();

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
			size_t match_id(0);

			if (matches_current == matches_end)
			{
				std::cout << "���������� ���." << std::endl;
				continue;
			}

			std::cout << "����������: " << std::endl;
			while (matches_current != matches_end)
			{
				decltype(auto) match = (*matches_current)[0];
				std::wcout
					<< std::to_wstring(match_id) << L". "
					<< (match.matched ? L"" : L"��������� ����������")
					<< std::quoted(match.str())
					<< std::endl;

				++match_id;
				++matches_current;
			}
		}
	}
};