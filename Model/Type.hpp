#pragma once

#include "Model/TypeBase.hpp"
#include "Model/NamespaceMember.hpp"

#include <vector>

namespace Everbase { namespace InterfaceCompiler { namespace Model {

class Type : public TypeBase
{
public:
    static constexpr auto TYPE_NAME = "Type";

private:
    NamespaceMemberRef mPrimary;
    std::vector<NamespaceMemberRef> mParams;

public:
    Type();
    virtual ~Type();
    virtual std::string objectTypeName() override { return TYPE_NAME; }

    NamespaceMemberRef primary();
    void setPrimary(NamespaceMemberRef primary);

    void addParam(NamespaceMemberRef param);
    const std::vector<NamespaceMemberRef>& params();
};

typedef std::shared_ptr<Type> TypeRef;

} } } // namespace Everbase::InterfaceCompiler::Model