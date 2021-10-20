#pragma once
#include<string>
#include<vector>



class Word
{
protected:
	std::string word;
	std::vector<bool> crosses;
	int orientation;
	int coordinate;
	int start_coord;
public:
	Word();
	Word(std::string &w);
	void set_cross(int n);
	void change_letter(const char c, int n);
	void uncross(int n);
	void set_coordinate(int n);
	void set_orientation(int n);
	void set_start(int n);
	int size()const;
	int orient()const;
	int coord()const;
	int start()const;
	bool cross(int n)const;
	std::string string()const;
	char operator[] (int i);
	Word& operator= (const Word& w);
	bool operator!= (const Word& w)const;
	~Word();
};

