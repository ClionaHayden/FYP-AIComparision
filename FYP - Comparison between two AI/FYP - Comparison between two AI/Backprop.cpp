#include "BackProp.h"

Backprop::Backprop()
{
	loadBPData();
}

Backprop::~Backprop()
{
}

void Backprop::addTrainingData(Data t_new)
{
	m_trainingData.push_back(t_new);
}

void Backprop::loadBPData()
{
	vector<string> row;
	string line, word;
	float num = 0.0f;
	int index = 0;
	int col = 0;
	std::ifstream file;

	file.open("DATA/temp.csv");
	while (!file.eof())
	{
		row.clear();
		// read an entire row and 
		// store it in a string variable 'line' 
		getline(file, line);

		// used for breaking words 
		stringstream s(line);
		// read every column data of a row and 
		// store it in a string variable, 'word'
		index = 0;
		Data d;
		while (getline(s, word, ','))
		{
			num = stof(word);
			if (col == 0)
			{
				d.m_position.x = num;
				col++;
			}
			else if (col == 1)
			{
				d.m_position.y = num;
				col++;
			}
			else if (col == 2)
			{
				d.m_rotation = num;
				col++;
			}
			else if (col == 3)
			{
				d.m_speed = num;
				col++;
			}
			index = index + 1;
		}
		m_backpropData.push_back(d);
		col = 0;
	}
	m_backpropData.pop_back();
}
