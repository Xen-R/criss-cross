#include "Word.h"
#include "WordArea.h"
#include<iostream>
#include <fstream> 
#include <algorithm>
#include <functional>
#include "WordClassificator.h"

struct sorting : public std::binary_function<Word, Word, bool>
{
	bool operator()(const Word& w1, const Word& w2) const
	{
		return w1.size() < w2.size();
	};
};


int
main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Invalid number of input files" << std::endl;
	}
	std::string str;
	std::vector<Word> unused_words;
	std::ifstream file(argv[1]);
	std::ofstream out(argv[2]);
	WordArea new_area, bb;
	if (!file.is_open())
	{
		std::cout << "Input file does not exist!" << std::endl;
		return 0;
	}
	while (std::getline(file, str)) { 
		if (str != "")
		{
			unused_words.push_back(str);
		}
	}
	std::sort(unused_words.begin(), unused_words.end(), sorting());
	WordClassificator w1;
	try
	{
		w1.create(unused_words);
	}
	catch (std::exception &e)
	{
		std::cout << "Exeption: " << e.what();
		return 0;
	}
	if (unused_words.size() == 0)
	{
		std::cout << "Input file is empty!" << std::endl;
		return 0;
	}
	unused_words[0].set_orientation(-1);
	new_area.add(unused_words[0]);	
	unused_words.erase(unused_words.begin());
	new_area.create_area(unused_words, bb, new_area, w1);
	try
	{
		bb.show(out);
	}
	catch (std::exception &e)
	{
		std::cout << "Exeption: " << e.what();
		return 0;
	}
	std::cout << "Succes!" << std::endl;
	return 0;
}