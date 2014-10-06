#include "Components/Cpp/HeaderFormatter.hpp"

#include <boost/algorithm/string/replace.hpp>

namespace Everbase { namespace InterfaceCompiler { namespace Components { namespace Cpp {

using std::endl;
using std::flush;

using IndexList::indices;

using namespace Model;
using namespace StreamFilter;

void HeaderFormatter::_definition(std::ostream& stream, Model::StructRef struct_) const
{
    if ( struct_->doc() )
    {
        stream << doc(struct_->doc());
    }
    
    stream << "struct " << name(struct_) << endl << "{" << endl;

    for (auto field : struct_->fields())
    {
        filter(stream).push<indent>(config.indentData) << param(field) << ";" << endl;
    }

    stream << "};" << endl;
}

void HeaderFormatter::_definition(std::ostream& stream, Model::ClassRef class_) const
{
    if ( class_->doc() )
    {
        stream << doc(class_->doc());
    }
    
    stream << "class " << name(class_) << endl << "{" << endl;

    for ( auto constant : class_->constants() )
    {
        filter(stream).push<indent>(config.indentData) << definition(constant) << endl;
    }

    for( auto operation : class_->operations() )
    {
        filter(stream).push<indent>(config.indentData) << definition(operation) << endl;
    }

    for ( auto event : class_->events() )
    {
        filter(stream).push<indent>(config.indentData) << definition(event) << endl;
    }

    stream << "};" << endl;
}

void HeaderFormatter::_definition(std::ostream& stream, Model::Class::ConstantRef constant) const
{
    if ( constant->doc() )
    {
        stream << doc(constant->doc());
    }

    stream << "static constexpr " << type(constant->type()) << " " << name(constant) << " = ";

    if( auto primitive = std::dynamic_pointer_cast<Primitive>(std::dynamic_pointer_cast<Type>(constant->type())->primary()) )
    {
        switch( primitive->underlying() )
        {
            case Primitive::Underlying::BYTE:
                stream << "0x" << std::hex << static_cast<std::uint64_t>(boost::any_cast<std::uint8_t>(constant->value()));
                break;

            case Primitive::Underlying::UINT16:
                stream << "0x" << std::hex << static_cast<std::uint64_t>(boost::any_cast<std::uint16_t>(constant->value()));
                break;

            case Primitive::Underlying::UINT32:
                stream << "0x" << std::hex << static_cast<std::uint64_t>(boost::any_cast<std::uint32_t>(constant->value()));
                break;

            case Primitive::Underlying::UINT64:
                stream << "0x" << std::hex << boost::any_cast<std::uint64_t>(constant->value());
                break;

            case Primitive::Underlying::BOOLEAN:
                stream << (boost::any_cast<bool>(constant->value()) ? "true" : "false");
                break;

            case Primitive::Underlying::TIMESTAMP:
                throw std::runtime_error("not supported");

            case Primitive::Underlying::STRING:
                stream << "\"" << boost::replace_all_copy(boost::any_cast<std::string>(constant->value()), "\"", "\\\"") << "\"";
                break;

            case Primitive::Underlying::UUID:
                {
                    auto uuid = boost::any_cast<boost::uuids::uuid>(constant->value());

                    stream << "{ { ";

                    for( auto i : indices(std::vector<std::uint8_t>(uuid.data, uuid.data + 16)) )
                    {
                        stream << "0x" << std::hex << static_cast<std::uint64_t>(i.value()) << (!i.last() ? ", " : "");
                    }

                    stream << " } }";
                }
                break;

            default:
                throw std::runtime_error("not supported");
        }
    }

    stream << ";" << endl;
}

void HeaderFormatter::_definition(std::ostream& stream, Model::Class::EventRef event) const
{
    // TODO
    
    if ( event->doc() )
    {
        stream << doc(event->doc());
    }
    
    for (auto value : event->values())
    {
        stream << type(value->type()) << " " << name(event) << "();" << endl;
    }
}

void HeaderFormatter::_definition(std::ostream& stream, Model::Class::OperationRef operation) const
{
    if (operation->doc())
    {
        stream << doc(operation->doc());
    }

    stream << signature(operation) << ";" << endl;
}

void HeaderFormatter::_definition(std::ostream& stream, Model::EnumRef enum_) const
{
    if ( enum_->doc() )
    {
        stream << doc(enum_->doc());
    }
    
    stream << "enum class " << name(enum_) << endl << "{" << endl;

    for (auto value : indices(enum_->values()))
    {
        filter(stream).push<indent>(config.indentData) << definition(value.value()) << (!value.last() ? "," : "") << endl;
    }

    stream << "};" << endl;
}

void HeaderFormatter::_definition(std::ostream& stream, Model::Enum::ValueRef value) const
{
    if ( value->doc() )
    {
        stream << doc(value->doc());
    }
    
    stream << name(value) << " = 0x" << std::hex << static_cast<std::uint64_t>(value->value());
}

} } } } // namespace: Everbase::InterfaceCompiler::Components::Cpp