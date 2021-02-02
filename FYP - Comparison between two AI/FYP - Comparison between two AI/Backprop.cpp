#include "BackProp.h"

Backprop::Backprop()
{
}

Backprop::~Backprop()
{
}

void Backprop::addTrainingData(Data t_new)
{
	m_trainingData.push_back(t_new);
}
