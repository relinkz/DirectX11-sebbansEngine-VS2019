#pragma once
#include "IModel.h"

class ModelFactory
{
public:
	std::unique_ptr<IModel> CreateQuadModel(Microsoft::WRL::ComPtr<ID3D11Device>& device);
};