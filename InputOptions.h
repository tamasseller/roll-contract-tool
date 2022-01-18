#ifndef RPC_TOOL_INPUTOPTIONS_H_
#define RPC_TOOL_INPUTOPTIONS_H_

#include "PathArguments.h"

#include <iostream>
#include <fstream>

class InputOptions
{
	std::ifstream inputFile;

public:
	std::istream *input = &std::cin;

	template<class Host>
	void add(Host* h)
	{
		h->addOptions({"-i", "--input"}, "Set input file [default: standard input]", [this](const FilePath &p)
		{
			if(!(this->inputFile = std::ifstream(p, std::ios::binary)))
			{
				throw std::runtime_error("Input file '" + std::filesystem::absolute(p).string() + "' could not be opened");
			}
			else
			{
				this->input = &inputFile;
			}
		});
	}
};

#endif /* RPC_TOOL_INPUTOPTIONS_H_ */
