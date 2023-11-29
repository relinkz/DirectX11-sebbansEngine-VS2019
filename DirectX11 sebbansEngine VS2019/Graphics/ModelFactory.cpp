#include "ModelFactory.h"
#include "QuadModel.h"

using namespace std;
using namespace Microsoft::WRL;

unique_ptr<IModel> ModelFactory::CreateQuadModel()
{
	auto object = std::make_unique<QuadModel>();
	object->Initialize();

	return move(object);
}