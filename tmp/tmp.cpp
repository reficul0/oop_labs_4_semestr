#include "pch.h"

#define _SILENCE_FPOS_SEEKPOS_DEPRECATION_WARNING

#include <list>
#include <boost/pool/pool_alloc.hpp>

#include "program_options.h"

// LAB_3
#include "Stack.h"

// LAB_4
#include "Student.h"

// LAB_5
#include "TextFilter.h"

// LAB_6
#include "TShapedSorter.h"

std::unordered_map<LabId, void(*)(configuration::Configuration const*)> labs_demonstrators{
	{ LabId::Stack, &Demonstratator<container::Stack<bool>>::demonstrate },
	{ LabId::Student, &Demonstratator<Student>::demonstrate },
	{ LabId::FileFilter, &Demonstratator<FileFiltherTag>::demonstrate },
	{ LabId::TShapedSorter, &Demonstratator<TShapedSorter>::demonstrate }
};

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Russian");

	std::unique_ptr<configuration::Configuration> config;
	try
	{
		if (auto optional_cfg = configuration::get_configuration(argc, argv))
			config = std::move(optional_cfg);
		else
			return EXIT_SUCCESS;

	}
	catch (std::exception const &e)
	{
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	
	auto demonstrator_by_lab_number = labs_demonstrators.find(config->lab_number);
	assert(demonstrator_by_lab_number != labs_demonstrators.end());
	
	demonstrator_by_lab_number->second(config.get());
	
	return EXIT_SUCCESS;
}