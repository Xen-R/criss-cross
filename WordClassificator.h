#pragma once
#include "Word.h"
#include<exception>
class WordClassificator
{
private:
	
public:
	std::vector<std::vector<std::vector<Word>>> word_list;
	WordClassificator();;
	~WordClassificator();
	void create(std::vector<Word> &words);
	bool word_existence(const Word &word)const;
};

