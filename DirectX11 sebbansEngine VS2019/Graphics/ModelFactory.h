#pragma once
#include "IModel.h"
#include <wrl/client.h>

class ModelFactory
{
public:
	std::unique_ptr<IModel> CreateQuadModel(Microsoft::WRL::ComPtr<ID3D11Device>& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dctx);
	std::unique_ptr<IModel> CreateBox(Microsoft::WRL::ComPtr<ID3D11Device>& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dctx);
};