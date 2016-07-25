#include <size_definition.hh>
#include <size_definition_factory.hh>

namespace item_management {

size_definition_factory::size_definition_factory()
{
}

size_definition_factory::~size_definition_factory()
{
}

i_size_definition *
size_definition_factory::create()
const
{
        return new size_definition(this);
}

i_size_definition *
size_definition_factory::create(size_key_type key)
const
{
        return new size_definition(this, key);
}

i_size_definition *
size_definition_factory::create(const i_size_definition &initializer)
const
{
        return new size_definition(initializer);
}

i_size_definition *
size_definition_factory::create(
    double available_quantity,
    double default_quantity,
    bool enabled,
    size_key_type key,
    const string_type &name,
    double price,
    double maxRetailPrice,
    double special_price,
    double warning_quantity)
const
{
        return i_size_definition_factory::create(available_quantity,
                                                 default_quantity,
                                                 enabled,
                                                 key,
                                                 name,
                                                 price,
                                                 maxRetailPrice,
                                                 special_price,
                                                 warning_quantity);
}

} /* item_management */

