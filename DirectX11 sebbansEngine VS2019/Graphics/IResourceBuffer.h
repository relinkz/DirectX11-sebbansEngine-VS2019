#pragma once
#include "BufferTypes.h"

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

class IResourceVertexBuffer
{
public:
	virtual bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::vector<Vertex> vData) = 0;
	virtual UINT GetNrOfVerticies() const = 0;
	virtual ID3D11Buffer** GetBufferAddress() = 0;
	virtual const UINT GetStride() const = 0;
};

class IResourceIndexBuffer
{
public:
	virtual bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::vector<DWORD> iData) = 0;
	virtual UINT GetNrOfIndencies() const = 0;
	virtual ID3D11Buffer** GetBufferAddress() = 0;
	virtual ID3D11Buffer* GetBuffer() = 0;
};

class IResourceConstantBuffer
{
public:
	virtual bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device) = 0;
	virtual ID3D11Buffer** GetBufferAddress() = 0;
	virtual ID3D11Buffer* GetBuffer() = 0;
};