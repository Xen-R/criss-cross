#include "WordClassificator.h"



WordClassificator::WordClassificator()
{
	word_list.resize(26);
}

WordClassificator::~WordClassificator()
{
}

void WordClassificator::create(std::vector<Word> &words)
{
	for (int i = 0; i < words.size(); i++)
	{
		for (int j = i + 1; j < words.size(); j++)
		{
			if (words[i].string() == words[j].string())
			{
				throw std::invalid_argument("There are two similar words in the word list\n");
			}
		}
	}
	int num;
	word_list.resize(26);
	for (int i = 0; i < words.size(); i++)
	{
		if (words[i].size() < 2)
		{
			throw std::invalid_argument("There are some words containing less than 2 letters\n");
		}
		for (int j = 0; j < words[i].size(); j++)
		{
			if (words[i][j] >= 'A' && words[i][j] <= 'Z')
			{
				words[i].change_letter(words[i][j] + 'a' - 'A', j);
			}
			num = words[i][j] - 'a';
			if ((num >= 26) || (num < 0))
			{
				throw std::invalid_argument("Invalid symbols in the word list\n");
			}
			if (j + 2 > word_list[num].size())
			{
				word_list[num].resize(j + 2);
			}
			word_list[num][j + 1].push_back(words[i]);
		}
	}
	int flag = 0, i_size = 0;
	for (int i = 0; i < 26; i++)
	{
		i_size = word_list[i].size();
		for (int j = 1; j < i_size; j++)
		{

			for (int k = 0; k < word_list[i][j].size(); k++)
			{
				if (word_list[i][j][k].size() == j)
				{
					word_list[i][0].push_back(word_list[i][j][k]);
				}
			}

		}
		flag = 0;
	}
}

bool WordClassificator::word_existence(const Word & word)const
{
	for (int i = 0; i < 26; i++)
	{
		for (int j = 0; j < word_list[i].size(); j++)
		{
			for (int k = 0; k < word_list[i][j].size(); k++)
			{
				if (word.string() == word_list[i][j][k].string())
				{
					return true;
				}
			}
		}
	}
	return false;
}
