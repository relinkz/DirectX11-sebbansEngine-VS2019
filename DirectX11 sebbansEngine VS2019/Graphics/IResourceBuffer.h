#pragma once
#include <d3d11.h>
#include <wrl/client.h>

class IResourceBuffer
{
public:
	virtual bool Initialize(Microsoft::WRL::ComPtr < ID3D11Device>& device) = 0;
	virtual UINT GetNrOfVerticies() const = 0;
	virtual ID3D11Buffer** GetBufferAddress() = 0;
};