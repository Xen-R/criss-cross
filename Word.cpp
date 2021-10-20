#include "Word.h"



Word::Word()
{
	orientation = 0;
	coordinate = 0;
	start_coord = 0;
}

Word::Word(std::string &w)
{
	word = w;
	orientation = 0;
	coordinate = 0;
	start_coord = 0;
	crosses.resize(w.size());
	for (int i = 0; i < w.size(); i++)
	{
		crosses[i] = false ;
	}
	
}

void Word::set_cross(int n)
{
	crosses[n] = true;
}

void Word::change_letter(const char c, int n)
{
	word[n] = c;
}

void Word::uncross(int n)
{
	crosses[n] = false;
}

void Word::set_coordinate(int n)
{
	coordinate = n;
}

void Word::set_orientation(int n)
{
	orientation = n;
}

void Word::set_start(int n)
{
	start_coord = n;
}

int Word::size()const
{
	return word.size();
}

int Word::orient()const
{
	return orientation;
}

int Word::coord()const
{
	return coordinate;
}

int Word::start()const
{
	return start_coord;
}

bool Word::cross(const int n)const
{
	return crosses[n];
}

std::string Word::string()const
{
	return word;
}

char Word::operator[](int i)
{
	return word[i];
}

Word & Word::operator=(const Word& w)
{
	word = w.word;
	crosses = w.crosses;
	orientation = w.orientation;
	coordinate = w.coordinate;
	start_coord = w.start_coord;
	return *this;
}

bool Word::operator!=(const Word & w)const
{
	if (this->word == w.word && this->coordinate == w.coordinate && this->orientation == w.orientation && this->crosses == w.crosses && this->start_coord == w.start_coord)
	{
		return false;
	}

	return true;
}

Word::~Word()
{
}
