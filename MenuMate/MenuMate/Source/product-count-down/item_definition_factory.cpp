#include <cassert>

#include <i_size_definition.hh>
#include <item_definition.hh>
#include <item_definition_factory.hh>

namespace item_management {

item_definition_factory::item_definition_factory()
{
}

item_definition_factory::~item_definition_factory()
{
}

i_item_definition *
item_definition_factory::create(const i_item_definition &initializer)
const
{
        return new item_definition(initializer);
}

item_definition *
item_definition_factory::create(const item_definition &initializer)
const
{
        return new item_definition(initializer);
}

i_item_definition *
item_definition_factory::create(
    const string_type &name,
    std::vector<i_size_definition *> &size_definitions,
    item_key_type key)
const
{
        return new item_definition(this, name, size_definitions, key);
}

} /* item_management */

