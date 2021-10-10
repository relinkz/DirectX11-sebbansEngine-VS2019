#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "Vertex.h"
#include <vector>

class IResourceBuffer
{
public:
	virtual bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::vector<Vertex> vData) = 0;
	virtual UINT GetNrOfVerticies() const = 0;
	virtual ID3D11Buffer** GetBufferAddress() = 0;
};