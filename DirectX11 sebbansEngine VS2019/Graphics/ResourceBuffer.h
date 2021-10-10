#pragma once
#include "IResourceBuffer.h"

class SimpleTriangleVertexBuffer : public IResourceBuffer
{
public:
	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::vector<Vertex> vData) override;
	virtual UINT GetNrOfVerticies() const override;
	virtual ID3D11Buffer** GetBufferAddress() override;
private:
	// A buffer interface accesses a buffer resource, which is unstructured memory. Buffers typically store vertex or index data.
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	UINT m_nrOfVerticies = 0;
};