#include "Formatter.hpp"

#include <boost/regex.hpp>
#include <iostream>

namespace Everbase { namespace InterfaceCompiler {

Formatter::Formatter(FormatterConfig config)
    : config(config)
{
}

void Formatter::execute ( const ConfigProvider& cprov, Model::RootRef input, std::ostream& output ) const
{
	output << definition(input);
}

FormatToken<Model::IdentifiableRef> Formatter::qname(Model::IdentifiableRef identifiable) const
{
    return FormatToken<Model::IdentifiableRef> { this, &Formatter::qname, std::tuple<Model::IdentifiableRef> { identifiable } };
}

FormatToken<Model::IdentifiableRef> Formatter::name(Model::IdentifiableRef identifiable) const
{
    return FormatToken<Model::IdentifiableRef> { this, &Formatter::name, std::tuple<Model::IdentifiableRef> { identifiable } };
}

FormatToken<std::string, std::string, FormatterConfig::NameConfigBase> Formatter::name(std::string longName, std::string shortName, FormatterConfig::NameConfigBase config) const
{
    return FormatToken<std::string, std::string, FormatterConfig::NameConfigBase> { this, &Formatter::name, std::tuple<std::string, std::string, FormatterConfig::NameConfigBase> { longName, shortName, config } };
}

FormatToken<Model::ParameterRef> Formatter::param(Model::ParameterRef parameter) const
{
    return FormatToken<Model::ParameterRef> { this, &Formatter::param, std::tuple<Model::ParameterRef> { parameter } };
}

FormatToken<Model::TypeRef> Formatter::type(Model::TypeBaseRef type) const
{
    auto resolvedType = std::dynamic_pointer_cast<Model::Type>(type);

    if (!resolvedType)
    {
        throw std::runtime_error("cannot format unresolved type");
    }

    return FormatToken<Model::TypeRef> { this, &Formatter::type, std::tuple<Model::TypeRef> { resolvedType } };
}

FormatToken<Model::PrimitiveRef> Formatter::type(Model::PrimitiveRef primitive) const
{
    return FormatToken<Model::PrimitiveRef> { this, &Formatter::type, std::tuple<Model::PrimitiveRef> { primitive } };
}

FormatToken<Model::DocumentationRef> Formatter::doc(Model::DocumentationRef documentation) const
{
    return FormatToken<Model::DocumentationRef> { this, &Formatter::doc, std::tuple<Model::DocumentationRef> { documentation } };
}

FormatToken<Model::RootRef> Formatter::definition(Model::RootRef root) const
{
	return FormatToken<Model::RootRef> { this, &Formatter::definition, std::tuple<Model::RootRef> { root } };
}

FormatToken<Model::NamespaceRef> Formatter::definition(Model::NamespaceRef namespace_) const
{
    return FormatToken<Model::NamespaceRef> { this, &Formatter::definition, std::tuple<Model::NamespaceRef> { namespace_ } };
}

FormatToken<Model::ElementRef> Formatter::definition(Model::ElementRef element) const
{
	return FormatToken<Model::ElementRef> { this, &Formatter::definition, std::tuple<Model::ElementRef> { element } };
}

FormatToken<Model::StructRef> Formatter::definition(Model::StructRef struct_) const
{
	return FormatToken<Model::StructRef> { this, &Formatter::definition, std::tuple<Model::StructRef> { struct_ } };
}

FormatToken<Model::ClassRef> Formatter::definition(Model::ClassRef class_) const
{
	return FormatToken<Model::ClassRef> { this, &Formatter::definition, std::tuple<Model::ClassRef> { class_ } };
}

FormatToken<Model::Class::ConstantRef> Formatter::definition(Model::Class::ConstantRef constant) const
{
    return FormatToken<Model::Class::ConstantRef> { this, &Formatter::definition, std::tuple<Model::Class::ConstantRef> { constant } };
}

FormatToken<Model::Class::EventRef> Formatter::definition(Model::Class::EventRef event) const
{
	return FormatToken<Model::Class::EventRef> { this, &Formatter::definition, std::tuple<Model::Class::EventRef> { event } };
}

FormatToken<Model::Class::OperationRef> Formatter::definition(Model::Class::OperationRef operation) const
{
	return FormatToken<Model::Class::OperationRef> { this, &Formatter::definition, std::tuple<Model::Class::OperationRef> { operation } };
}

FormatToken<Model::Class::OperationRef> Formatter::signature(Model::Class::OperationRef operation) const
{
	return FormatToken<Model::Class::OperationRef> { this, &Formatter::signature, std::tuple<Model::Class::OperationRef> { operation } };
}

FormatToken<Model::EnumRef> Formatter::definition(Model::EnumRef enum_) const
{
    return FormatToken<Model::EnumRef> { this, &Formatter::definition, std::tuple<Model::EnumRef> { enum_ } };
}

FormatToken<Model::Enum::ValueRef> Formatter::definition(Model::Enum::ValueRef value) const
{
    return FormatToken<Model::Enum::ValueRef> { this, &Formatter::definition, std::tuple<Model::Enum::ValueRef> { value } };
}

void Formatter::qname(std::ostream& stream, Model::IdentifiableRef identifiable) const
{
    if( auto parent = std::dynamic_pointer_cast<Model::Identifiable>(identifiable->parent()) )
    {
        stream << qname(parent) << config.namespaceDelimiter;
    }

    stream << name(identifiable);
}

void Formatter::name(std::ostream& stream, Model::IdentifiableRef identifiable) const
{
    if( std::dynamic_pointer_cast<Model::Namespace>(identifiable) )
    {
        stream << name(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Namespace>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Parameter>(identifiable) )
    {
        stream << name(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Parameter>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Enum>(identifiable) )
    {
        stream << name(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Enum>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Enum::Value>(identifiable) )
    {
        stream << name(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Enum::Value>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Struct>(identifiable) )
    {
        stream << name(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Struct>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Class>(identifiable) )
    {
        stream << name(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Class>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Class::Constant>(identifiable) )
    {
        stream << name(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Class::Constant>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Class::Event>(identifiable) )
    {
        stream << name(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Class::Event>());
    }
    else
    if( std::dynamic_pointer_cast<Model::Class::Operation>(identifiable) )
    {
        stream << name(identifiable->longName(), identifiable->shortName(), config.nameConfig<Model::Class::Operation>());
    }
    else
    {
        throw std::runtime_error("unknown identifiable type " + identifiable->typeName());
    }
}

void Formatter::name(std::ostream& stream, std::string longName, std::string shortName, FormatterConfig::NameConfigBase config) const
{
    boost::regex regex("[A-Z]?[a-z]+|[A-Z]?[0-9]+|[A-Z]");

    std::string name = config.useShort ? shortName : longName;
    
    std::string::const_iterator start = name.begin();
    std::string::const_iterator end = name.end();

    boost::smatch match;
    for(size_t count = 0; boost::regex_search(start, end, match, regex); ++count)
    {
        if (count > 0)
            { stream << config.delimiter; }

        std::string part = match[0];

        switch (config.style)
        {
            case FormatterConfig::NameStyle::LOWER_CAMELCASE:
                transform(part.begin(), part.end(), part.begin(), ::tolower);
                if (count > 0)
                {
                    transform(part.begin(), part.begin() + 1, part.begin(), ::toupper);
                }
                break;

            case FormatterConfig::NameStyle::UPPER_CAMELCASE:
                transform(part.begin(), part.end(), part.begin(), ::tolower);
                transform(part.begin(), part.begin() + 1, part.begin(), ::toupper);
                break;

            case FormatterConfig::NameStyle::UPPERCASE:
                transform(part.begin(), part.end(), part.begin(), ::toupper);
                break;

            case FormatterConfig::NameStyle::LOWERCASE:
                transform(part.begin(), part.end(), part.begin(), ::tolower);
                break;

            default:
                throw std::runtime_error("invalid name style");
        }

        stream << part;
        start = match[0].second;
    }
}

void Formatter::type(std::ostream& stream, Model::PrimitiveRef primitive) const
{
    switch(primitive->underlying())
    {
        case Model::Primitive::Underlying::BYTE:
            stream << config.primitiveConfig<Model::Primitive::Underlying::BYTE>().native;
            break;

        case Model::Primitive::Underlying::UINT16:
            stream << config.primitiveConfig<Model::Primitive::Underlying::UINT16>().native;
            break;

        case Model::Primitive::Underlying::UINT32:
            stream << config.primitiveConfig<Model::Primitive::Underlying::UINT32>().native;
            break;

        case Model::Primitive::Underlying::UINT64:
            stream << config.primitiveConfig<Model::Primitive::Underlying::UINT64>().native;
            break;

        case Model::Primitive::Underlying::BOOLEAN:
            stream << config.primitiveConfig<Model::Primitive::Underlying::BOOLEAN>().native;
            break;

        case Model::Primitive::Underlying::TIMESTAMP:
            stream << config.primitiveConfig<Model::Primitive::Underlying::TIMESTAMP>().native;
            break;

        case Model::Primitive::Underlying::STRING:
            stream << config.primitiveConfig<Model::Primitive::Underlying::STRING>().native;
            break;

        case Model::Primitive::Underlying::UUID:
            stream << config.primitiveConfig<Model::Primitive::Underlying::UUID>().native;
            break;

        case Model::Primitive::Underlying::BUFFER:
            stream << config.primitiveConfig<Model::Primitive::Underlying::BUFFER>().native;
            break;

        case Model::Primitive::Underlying::CONST_BUFFER:
            stream << config.primitiveConfig<Model::Primitive::Underlying::CONST_BUFFER>().native;
            break;

        case Model::Primitive::Underlying::VECTOR:
            stream << config.primitiveConfig<Model::Primitive::Underlying::VECTOR>().native;
            break;

        case Model::Primitive::Underlying::LIST:
            stream << config.primitiveConfig<Model::Primitive::Underlying::LIST>().native;
            break;

        case Model::Primitive::Underlying::SET:
            stream << config.primitiveConfig<Model::Primitive::Underlying::SET>().native;
            break;

        case Model::Primitive::Underlying::MAP:
            stream << config.primitiveConfig<Model::Primitive::Underlying::MAP>().native;
            break;

        default:
            throw std::runtime_error("invalid primitive");
    }
}

void Formatter::doc(std::ostream& stream, Model::DocumentationRef documentation) const
{
	using namespace std;
	using namespace Model;
	using namespace StreamFilter;

	stream << "/**" << endl;
	
    if (documentation->keyExists(Documentation::KEY_BRIEF))
    {
        filter(stream).push<indent>(" * ").push<wrap>() << "@" << Documentation::KEY_BRIEF << " " << documentation->description(Documentation::KEY_BRIEF) << endl;
    }

    if (documentation->keyExists(Documentation::KEY_MORE))
    {
        filter(stream).push<indent>(" * ").push<wrap>() << endl << documentation->description(Documentation::KEY_MORE) << endl;
    }

	stream << " */" << endl;
}

void Formatter::definition(std::ostream& stream, Model::RootRef root) const
{
    stream << definition(root->getNamespace());
}

void Formatter::definition(std::ostream& stream, Model::ElementRef element) const
{
    if ( element->doc() )
    {
        stream << doc(element->doc());
    }

    if ( auto namespace_ = std::dynamic_pointer_cast<Model::Namespace>(element) )
    {
        stream << definition(namespace_);
    }
    else
    if ( auto struct_ = std::dynamic_pointer_cast<Model::Struct>(element) )
    {
        stream << definition(struct_);
    }
    else
    if ( auto enum_ = std::dynamic_pointer_cast<Model::Enum>(element) )
    {
        stream << definition(enum_);
    }
    else
    if ( auto class_ = std::dynamic_pointer_cast<Model::Class>(element) )
    {
        stream << definition(class_);
    }
    else
    if ( std::dynamic_pointer_cast<Model::Primitive>(element) )
    {
        // primitives do not have definitions
    }
    else
    {
        throw std::runtime_error("unknown namespace element type " + element->typeName());
    }
}

} } // namespace: Everbase::InterfaceCompiler
