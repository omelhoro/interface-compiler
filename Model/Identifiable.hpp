#pragma once

#include "Model/Object.hpp"
#include "Model/Documentation.hpp"

#include <string>
#include <memory>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Identifiable : public Object
{
public:
    Identifiable();
    virtual ~Identifiable();

public:
    std::string longName();
    void setLongName(std::string longName);

    std::string shortName();
    void setShortName(std::string shortName);

    DocumentationRef doc();
    void setDoc(DocumentationRef doc);

private:
    std::string _longName;
    std::string _shortName;
    DocumentationRef _doc;
};

typedef std::shared_ptr<Identifiable> IdentifiableRef;

} } } // namespace Everbase::InterfaceCompiler::Model
