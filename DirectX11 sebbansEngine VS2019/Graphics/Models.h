#pragma once
#include "IModel.h"
#include <string>

class Model : public IModel
{
protected:
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT3 m_rot;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMMATRIX m_tMtx;
	std::vector<Vertex> m_vertecies;

	std::vector<std::string> m_diffuseMap;
	std::vector<std::wstring> m_normalMaps;
	std::vector<std::wstring> m_occlusionMaps;
	std::vector<std::wstring> m_specularMaps;

	DirectX::XMFLOAT4 m_Ka;
	DirectX::XMFLOAT4 m_Kd;
	DirectX::XMFLOAT4 m_Ks;
	DirectX::XMFLOAT4 m_Ns;

	std::unique_ptr<IResourceVertexBuffer> m_vertexes;
	std::unique_ptr<IResourceConstantBuffer> m_vsCbuff;
	std::unique_ptr<IResourceConstantBuffer> m_psCbuff;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_diffuseTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_normalTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_occlusionTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_specularTexture;

	std::unique_ptr<IVertexShader> m_vShader;
	std::unique_ptr<IPixelShader> m_pShader;

	void InitializeConstantBuffers(Microsoft::WRL::ComPtr<ID3D11Device>& device);

	void SetPosition(const DirectX::XMFLOAT3&) override;
	void SetRotation(const DirectX::XMFLOAT3&) override;
	void SetScale(const DirectX::XMFLOAT3&) override;
	void ResetTransformation();
	void AddOffsetToLocalVerticies(const DirectX::XMFLOAT3& offset);

	DirectX::XMMATRIX GetWorldMatrix() const override;
	DirectX::XMMATRIX GetRotationMatrix() const override;
	void UpdateConstantBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dctx, CB_VS_vertexShader newData) const;
	void UpdateConstantBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dctx, CB_PS_pixelMaterialShader newData) const;


	std::unique_ptr<IResourceVertexBuffer> GetResourceVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Device>&) override;

	virtual void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dctx) const override;
	void ReadObjFile(const std::string& file);
};

class QuadModel : public Model
{
public:
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dCtx) override;
};

class Box : public Model
{
public:
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dCtx) override;
};