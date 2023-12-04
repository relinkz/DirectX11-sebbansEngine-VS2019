#pragma once
#include "IModel.h"

class ModelFactory
{
public:
	std::unique_ptr<IModel> CreateQuadModel();
	std::unique_ptr<IModel> CreateBox();
};