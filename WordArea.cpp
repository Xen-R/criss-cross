#include "WordArea.h"
#include<iostream>
#include <ctime>



WordArea::WordArea()
{
}

void WordArea::create_area(std::vector<Word> &unused_words, WordArea &best_area, WordArea &cur_area, const WordClassificator &word_list)
{
	if (unused_words.size() == 0)
	{
		if ((best_area.words.size()==0)||(float(cur_area.crosses) / cur_area.surface() > (float(best_area.crosses) )/ best_area.surface() && cur_area.surface()<best_area.surface()))
		{
			best_area = cur_area;
		}
		return;
	}
	for (int m = 0; m < words.size(); m++)
	{
		std::clock_t c_start = std::clock();
		int flag = 0, crossing = 0;
		for (int i = 0; i < words[m].size(); i++)
		{
			int c = words[m][i] - 'a';
			WordArea::AdditionMode added = can_be_added(words[m], i, cur_area);
			int begin = 0, end = 0;
			if (added == any)
			{
				begin = 1;
				end = word_list.word_list[c].size();
			}
			if (added == first)
			{
				begin = 1;
				end = 2;
			}
			if (added == last)
			{
				end = 1;
			}
			for (int j = begin; j < end; j++)
			{
				for (int k = 0; k < word_list.word_list[c][j].size(); k++)
				{
					Word start(word_list.word_list[c][j][k]);
					if (!alredy_used(start))
					{
						start.set_orientation(-words[m].orient());
						start.set_coordinate(words[m].start()+i);
						start.set_start(words[m].coord()-j+1);
						int cr = count_crosses(start);
						if (cr)
						{
							crosses += cr;
							add(start);
							int pos;
							for (pos = 0; pos < unused_words.size(); pos++)
							{
								if (!(unused_words[pos].string() != start.string()))
								{
									break;
								}
							}
							unused_words.erase(unused_words.begin() + pos);
							if (pass(cur_area, best_area, unused_words))
							{
								create_area(unused_words, best_area, cur_area, word_list);
								std::clock_t c_end = std::clock();
								if ((c_end - c_start) / CLOCKS_PER_SEC > 30)
								{
									return;
								}
							}
							cur_area.erase(start);
							unused_words.insert(unused_words.begin() + pos, start);
							crosses -= cr;
						}
					}
				}
			}
		}
	}
	return;
}

void WordArea::push_front(int orientation, int n)
{
	if (orientation == horiz)
	{
		std::string tmp;
		for (int i = 0; i < area[0].size(); i++)
		{
			tmp = tmp + " ";
		}
		area.resize(area.size() + n);
		for (int i = area.size() - n -1; i >= 0; i--)
		{
			area[i + n].resize(area[0].size());
			area[i + n] = area[i];
		}
		for (int i = 0; i < n; i++)
		{
			area[i] = tmp;
		}
	}
	else
	{
		std::string tab;
		for (int i = 0; i < n; i++)
		{
			tab = tab + " ";
		}
		for (int i = 0; i < area.size(); i++)
		{
			area[i] = tab + area[i];
		}
	}
}

void WordArea::push_back(int orientation, int n)
{
	if (orientation == horiz)
	{
		std::string tmp;
		for (int i = 0; i < area[0].size(); i++)
		{
			tmp = tmp + " ";
		}
		for (int i = 0; i < n; i++)
		{
			area.push_back(tmp);
		}
		
	}
	else
	{
		std::string tab;
		for (int i = 0; i < n; i++)
		{
			tab = tab + " ";
		}
		for (int i = 0; i < area.size(); i++)
		{
			area[i] = area[i] + tab;
		}
	}
}

void WordArea::pop_front(int orientation, int n)
{
	if (orientation == horiz)
	{
		for (int i = 0; i < area.size()-n; i++)
		{
			area[i] = area[i + n];
		}
		for (int i = 0; i < n; i++)
		{
			area.pop_back();
		}
	}
	else
	{
		for (int k = 0; k < area.size(); k++)
		{
			for (int i = 0; i < area[k].size() - n; i++)
			{
				area[k][i] = area[k][i + n];
			}
			for (int i = 0; i < n; i++)
			{
				area[k].pop_back();
			}
		}
	}
}

void WordArea::pop_back(int orientation, int n)
{
	if (orientation == horiz)
	{
		for (int i = 0; i < n; i++)
		{
			area.pop_back();
		}
	}
	else
	{
		for (int i = 0; i < area.size(); i++)
		{
			for (int j = 0; j < n; j++)
			{
				area[i].pop_back();
			}
		}
	}
}

WordArea::AdditionMode WordArea::can_be_added(const Word & base, int n, const WordArea & cur_area)
{
	if (base.cross(n) == 1)
	{
		return none;
	}
	if (n != 0 && base.cross(n - 1) == 0 && n!= base.size()-1 && base.cross(n + 1) == 0)
	{
		return any;
	}
	if (n == 0 && n != base.size() - 1 && base.cross(n + 1) == 0)
	{
		return any;
	}
	if (n != 0 && base.cross(n - 1) == 0 && n == base.size() - 1)
	{
		return any;
	}
	if (n != 0 && base.cross(n - 1) == 1 &&((n == base.size() - 1)||( n != base.size() - 1 && base.cross(n + 1) == 0)))
	{
		if ((base.coord() == size(-base.orient()) - 1) || (base.coord() != size(-base.orient()) - 1 && cur_area.get_cell(base.coord() + 1, base.orient(), n - 1) == ' '))
		{
			return first;
		}
		else
		{
			if ((base.coord() == 0) || (base.coord() != 0 && cur_area.get_cell(base.coord() - 1, base.orient(), n - 1) == ' '))
			{
				return last;
			}
			else
			{
				return none;
			}
		}
	}
	if (n != base.size() - 1 && base.cross(n + 1) == 1 && ((n == 0) || (n != 0 && base.cross(n - 1) == 0)))
	{
		if ((base.coord() == size(-base.orient()) - 1) || (base.coord() != size(-base.orient()) - 1 && cur_area.get_cell(base.coord() + 1, base.orient(), n + 1) == ' '))
		{
			return first;
		}
		else
		{
			if ((base.coord() == 0) || (base.coord() != 0 && cur_area.get_cell(base.coord() - 1, base.orient(), n + 1) == ' '))
			{
				return last;
			}
			else
			{
				return none;
			}
		}
		
		
	}
	if (n != base.size() - 1 && base.cross(n + 1) == 1 && n != 0 && base.cross(n - 1) == 1)
	{
		if ((base.coord() == size(-base.orient()) - 1) || (base.coord() != size(-base.orient()) - 1 && cur_area.get_cell(base.coord() + 1, base.orient(), n + 1) == ' ' && cur_area.get_cell(base.coord() + 1, base.orient(), n - 1) == ' '))
		{
			return first;
		}
		else
		{
			if ((base.coord() == 0) || (base.coord() != 0 && cur_area.get_cell(base.coord() - 1, base.orient(), n + 1) == ' ' && cur_area.get_cell(base.coord() - 1, base.orient(), n - 1) == ' '))
			{
				return last;
			}
			else
			{
				return none;
			}
		}


	}
	return none;
}

bool WordArea::alredy_used(const Word & w)const
{
	for (int i = 0; i < words.size(); i++)
	{
		if (words[i].string() == w.string())
		{
			return true;
		}
	}
	return false;
}

int WordArea::count_crosses(Word & w)const
{
	int begin = w.start(), end = w.start()+w.size(), d = 0, crossing=0;
	if (w.start() < 0)
	{
		d = -w.start();
		begin = 0;
	}
	if (w.start() + w.size() > size(w.orient()))
	{
		end = size(w.orient());
	}
	int j = 0;
	for (int i = begin; i < end; i++)
	{
		if (get_cell(w.coord(), w.orient(), i) != ' ' && get_cell(w.coord(), w.orient(), i) != w[j + d]) 
		{
			crossing = 0;
			break;
		}
		if (get_cell(w.coord(), w.orient(), i) == w[j + d])
		{
			crossing++;
			w.set_cross(j+d);
		}
		else
		{
			if (w.coord() != 0 && get_cell(w.coord() - 1, w.orient(), i) != ' ')
			{
				crossing = 0;
				break;
			}
			if (w.coord() != size(-w.orient()) - 1 && get_cell(w.coord() + 1, w.orient(), i) != ' ')
			{
				crossing = 0;
				break;
			}
		}
		j++;
	}
	if (begin > 0)
	{
		if (get_cell(w.coord(), w.orient(), begin-1) != ' ')
		{
			crossing = 0;
		}
	}
	if (end < size(w.orient()))
	{
		if (get_cell(w.coord(), w.orient(), end) != ' ')
		{
			crossing = 0;
		}
	}
	if (crossing == 0)
	{
		for (int i = 0; i < w.size(); i++)
		{
			w.uncross(i);
		}
	}
	return crossing;
}

void WordArea::add(Word &w)
{
	words.push_back(w);
	if (area.size() == 0)
	{
		area.push_back(w.string());
	}
	else 
	{
		int w_start = w.start();


		if (w_start < 0)
		{
			for (int i = 0; i < words.size(); i++)
			{
				if (words[i].orient() == w.orient())
				{
					words[i].set_start(words[i].start() - w_start);
				}
				else
				{
					words[i].set_coordinate(words[i].coord() - w_start);
				}
			}
			push_front(-w.orient(), -w_start);
			w.set_start(0);
			w_start = 0;
		}
		if (w_start + w.size() - 1 > size(w.orient()) - 1)
		{
			push_back(-w.orient(), w_start + w.size() - size(w.orient()));
		}
		for (int i = w_start; i < w_start + w.size(); i++)
		{
			if (get_cell(w.coord(), w.orient(), i)  == ' ')
			{
				set_cell(w.coord(), w.orient(), i,  w[i - w_start]);
			}
			else
			{
				for (int j = 0; j < words.size(); j++)
				{
					if (words[j].orient() == -w.orient() && words[j].coord() == i && w.coord() >= words[j].start() && w.coord() <= words[j].start() + words[j].size() - 1)
					{
						words[j].set_cross(w.coord() - words[j].start());
						w.set_cross(i - w_start);
						break;
					}
				}
			}
		}
	}
}

void WordArea::erase(Word & w)
{
	words.pop_back();

	for (int i = w.start(); i < w.start() + w.size(); i++)
	{
		if (!((w.coord() != 0 && get_cell(w.coord() - 1, w.orient(), i) != ' ') || (w.coord() != size(-w.orient()) - 1 && get_cell(w.coord() + 1, w.orient(), i) != ' ')))
		{
			set_cell(w.coord(), w.orient(), i, ' ');
		}
		else
		{
			for (int j = 0; j < words.size(); j++)
			{
				if (words[j].orient() == -w.orient() && words[j].coord() == i && w.coord() >= words[j].start() && w.coord() <= words[j].start() + words[j].size() - 1)
				{
					words[j].uncross(w.coord() - words[j].start());
				}
			}
		}
	}
	if (w.start() + w.size() - 1 == size(w.orient()) - 1)
	{
		int flag = 0, counter = 0;
		while (1)
		{
			for (int i = 0; i < words.size(); i++)
			{
				if (w.orient() == words[i].orient())
				{
					if (words[i].start() + words[i].size() - 1 == size(w.orient()) - 1 - counter)
					{
						flag = 1;
						break;
					}
				}
				else
				{
					if (words[i].coord() == size(w.orient()) - 1 - counter)
					{
						flag = 1;
						break;
					}
				}

			}
			if (flag)
			{
				break;
			}
			counter++;
		}
		if (counter != 0)
		{
			pop_back(-w.orient(), counter);
		}
	}
	if (w.start() == 0)
	{
		int counter = 0, flag = 0;
		while (1)
		{
			for (int i = 0; i < words.size(); i++)
			{
				if (words[i].orient() == w.orient())
				{
					if (words[i].start() == counter)
					{
						flag = 1;
						break;
					}
				}
				else
				{
					if (words[i].coord() == counter )
					{
						flag = 1;
						break;
					}
				}

			}

			if (flag)
			{
				if (counter != 0)
				{
					pop_front(-w.orient(), counter);
					for (int i = 0; i < words.size(); i++)
					{
						if (words[i].orient() == w.orient())
						{
							words[i].set_start(words[i].start() - counter);
						}
						else
						{
							words[i].set_coordinate(words[i].coord() - counter);
						}
					}
				}
				break;
			}
			counter++;
		}
	}
	w.set_coordinate(0);
	w.set_orientation(0);
	w.set_start(0);
	for (int i = 0; i < w.size(); i++)
	{
		w.uncross(i);
	}
}

int WordArea::surface()const
{
	if (area.size() != 0) {
		return area.size()*area[0].size();
	}
	else
	{
		return 0;
	}
}

float WordArea::average_l(const std::vector<Word>& words)const
{
	float sum = 0;
	for (int i = 0; i < words.size(); i++)
	{
		sum = sum + words[i].size();
	}
	sum = sum / words.size();
	return sum;
}

int WordArea::size(int orientation)const
{
	if (orientation == -1)
	{
		return area[0].size();
	}
	else
	{
		return area.size();
	}
	
}

bool WordArea::pass(WordArea& cur, WordArea& best, std::vector<Word>& unused_words)const
{
	if (best.words.size() != 0 &&((average_l(cur.words)>=average_l(unused_words))||((modifier*float(cur.crosses)) / cur.surface() <= (float(best.crosses) / best.surface())) || (cur.surface() >= best.surface())))
	{
		return false;
	}
	return true;
}

char WordArea::get_cell(int coord, int orient, int pos)const
{
	if (orient == -1)
	{
		return area[coord][pos];
	}
	else
	{
		return area[pos][coord];
	}
	return 0;
}

void WordArea::set_cell(int coord, int orient, int pos, char symbol)
{
	if (orient == -1)
	{
		area[coord][pos] = symbol;
	}
	else
	{
		area[pos][coord] = symbol;
	}
}

void WordArea::show(std::ostream &out)const
{
	if (words.size() == 0)
	{
		out << "Unsuccesfull result!"<< std::endl;
		throw std::invalid_argument("This words cannot be tranformed into criss-cross\n");
		return;
	}
	for (int i = 0; i < area.size(); i++)
	{
		out << area[i] << std::endl;
	}
}

WordArea & WordArea::operator=(const WordArea & c_area)
{
	words = c_area.words;
	crosses = c_area.crosses;
	area = c_area.area;
	//modifier = c_area.modifier;
	return *this;
}

WordArea::~WordArea()
{
}
