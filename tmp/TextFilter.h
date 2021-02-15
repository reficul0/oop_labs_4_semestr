#pragma once

#include <iomanip>

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/regex.hpp>
#include <boost/regex.hpp>

#include "program_options.h"
#include "Demonstratator.h"
#include "tools/interprocess_safe_buffered_file_stream.h"

// Вариант 10
// Написать программу, которая считывает английский текст из файла и выводит
// на экран слова текста, начинающиеся с гласных букв и оканчивающиеся гласными буквами.// \b(?i)(([aeiou]\b)|[aeiou]([a-z]*)[aeiou])\b// вольный перевод: без учёта регистраЭ // \b граница значения// (?i) case insensetive// [aeiou] гласная// [a-z]* любая буква
template<>
struct Demonstratator<struct FileFiltherTag>
{
	static void demonstrate(configuration::Configuration const *cfg_base)
	{
		decltype(auto) lab_cfg = dynamic_cast<configuration::FileFilterConfig const*>(cfg_base);
		BOOST_ASSERT_MSG(!lab_cfg, "В функцию демонстрации должна передаваться только верная конфигурация.");

		tools::interprocess::buffered::SafeToReadStlFileStream istream(lab_cfg->path_to_file, lab_cfg->locale);

		try
		{
			istream.initialize();
		}
		catch (std::exception const &e)
		{
			std::cerr << "При открытии файла " << std::quoted(lab_cfg->path_to_file.string()) <<  " и выкачке его содержимого в память произошла ошибка: " << e.what() << std::endl;
			return;
		}
		decltype(auto) file_data = istream.data();
		decltype(auto) current = istream.current();
		auto end = istream.end();

		while (true)
		{
			std::wstring filter;
			std::cout << "Введите regex выражение: " << std::endl;
			std::wcin >> filter;

			boost::wregex expr;
			try
			{
				expr = boost::wregex{ filter };
			}
			catch (const std::exception &e)
			{
				std::cerr << "Ошибка в выражении: " << e.what() << std::endl;
				continue;
			}

			boost::wcregex_iterator matches_current(file_data.c_str(), file_data.c_str() + file_data.size(), expr, boost::match_default | boost::match_partial);
			boost::wcregex_iterator matches_end;
			size_t match_id(0);

			if (matches_current == matches_end)
			{
				std::cout << "Совпадений нет." << std::endl;
				continue;
			}

			std::cout << "Совпадения: " << std::endl;
			while (matches_current != matches_end)
			{
				decltype(auto) match = (*matches_current)[0];
				std::wcout
					<< std::to_wstring(match_id) << L". "
					<< (match.matched ? L"" : L"частичное совпадение")
					<< std::quoted(match.str())
					<< std::endl;

				++match_id;
				++matches_current;
			}
		}
	}
};