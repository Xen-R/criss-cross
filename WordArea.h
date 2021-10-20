#pragma once
#include"Word.h"
#include "WordClassificator.h"



class WordArea
{
private:
	std::vector<Word> words;
	std::vector<std::string> area;
	int crosses = 0;
	const double modifier = 1.3547;
	
public:
	enum OrientationMode :int
	{
		horiz = -1, vert = 1
	};
	enum AdditionMode
	{
		first, last, any, none
	};
	WordArea();
	void create_area(std::vector<Word> &unused_words, WordArea &best_area, WordArea &cur_area, const WordClassificator &word_list);
	void push_front(int orientation, int n);
	void push_back(int orientation, int n);
	void pop_front(int orientation, int n);
	void pop_back(int orientation, int n);
	AdditionMode can_be_added(const Word &base, int n, const WordArea &cur_area);
	//int can_be_added(const Word &base, int n, const WordArea &cur_area);
	bool alredy_used(const Word& w)const;
	int count_crosses(Word& w)const;
	void add(Word &w);
	void erase(Word &w);
	int surface()const;
	float average_l(const std::vector<Word> &words)const;
	int size(int orientation)const;
	bool pass(WordArea &cur, WordArea &best, std::vector<Word> &unused_words)const;
	char get_cell(int coord, int orient, int pos)const;
	void set_cell(int coord, int orient, int pos, char symbol);
	void show(std::ostream &out)const;
	WordArea& operator=(const WordArea& area);
	~WordArea();
};

