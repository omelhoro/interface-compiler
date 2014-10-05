#include "Model/Class.hpp"

namespace Everbase { namespace InterfaceCompiler { namespace Model {

const char* Class::TYPE_NAME = "Class";

Class::Class()
	: _behavior(Behavior::INTERFACE)
{
}

Class::~Class()
{
}

Class::Behavior Class::behavior()
{
	return _behavior;
}

void Class::setBehavior(Class::Behavior behavior)
{
    _behavior = behavior;
}

TypeBaseRef Class::super()
{
    return _super;
}

void Class::setSuper(TypeBaseRef super)
{
	_super = super;
}

void Class::addOperation(Class::OperationRef operation)
{
    _operations.push_back(operation);
}

std::vector<Class::OperationRef> Class::operations()
{
	return _operations;
}

void Class::addEvent(Class::EventRef event)
{
    _events.push_back(event);
}

std::vector<Class::EventRef> Class::events()
{
    return _events;
}

void Class::addConstant(Class::ConstantRef constant)
{
	_constants.push_back(constant);
}

std::vector<Class::ConstantRef> Class::constants()
{
	return _constants;
}

} } } // namespace Everbase::InterfaceCompiler::Model
