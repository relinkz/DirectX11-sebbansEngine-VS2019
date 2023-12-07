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

	void SetPosition(const DirectX::XMFLOAT3&) override;
	void SetRotation(const DirectX::XMFLOAT3&) override;
	void SetScale(const DirectX::XMFLOAT3&) override;
	void ResetTransformation();
	void AddOffsetToLocalVerticies(const DirectX::XMFLOAT3& offset);

	DirectX::XMMATRIX GetWorldMatrix() const override;
	DirectX::XMMATRIX GetRotationMatrix() const override;
	std::unique_ptr<IResourceVertexBuffer> GetResourceVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Device>&) override;

	virtual std::vector<std::wstring> GetDiffuseMaps() const override;
	virtual std::vector<std::wstring> GetNormalMaps() const override;
	virtual std::vector<std::wstring> GetOcclusionMaps() const override;
	virtual std::vector<std::wstring> GetSpecularMaps() const override;

	virtual DirectX::XMFLOAT4 GetKa() const override;
	virtual DirectX::XMFLOAT4 GetKd() const override;
	virtual DirectX::XMFLOAT4 GetKs() const override;
	virtual DirectX::XMFLOAT4 GetNs() const override;

	void ReadObjFile(const std::string& file);
};

class QuadModel : public Model
{
public:
	void Initialize() override;
};

class Box : public Model
{
public:
	void Initialize() override;
};