#include "Model/Event.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Event::TYPE_NAME = "Event";

Event::Event()
{
}

Event::~Event()
{
}

void Event::addValue(ParameterRef value)
{
    _values[value->longName()] = value;
}

std::vector<ParameterRef> Event::values()
{
    return mapToVector<ParameterRef>(_values);
}

void Event::setTypeId(boost::uuids::uuid typeId)
{
	_typeId = typeId;
}

boost::uuids::uuid Event::typeId()
{
	return _typeId;
}

} } } // namespace Everbase::InterfaceCompiler::Model
