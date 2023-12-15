#include "ModelFactory.h"
#include "Models.h"

using namespace std;
using namespace Microsoft::WRL;

unique_ptr<IModel> ModelFactory::CreateQuadModel(Microsoft::WRL::ComPtr<ID3D11Device>& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dctx)
{
	auto object = std::make_unique<QuadModel>();
	object->Initialize(device, dctx);

	return move(object);
}

std::unique_ptr<IModel> ModelFactory::CreateBox(Microsoft::WRL::ComPtr<ID3D11Device>& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dctx)
{
	auto object = std::make_unique<Box>();
	object->Initialize(device, dctx);
	
	return move(object);
}
