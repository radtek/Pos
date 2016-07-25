#include <memory>

#include <managed_item_definition.hh>
#include <managed_item_definition_factory.hh>

namespace item_management {

managed_item_definition_factory::managed_item_definition_factory(
    const i_item_definition_factory &item_definition_factory)
    : item_definition_factory_(item_definition_factory)
{
}

managed_item_definition_factory::~managed_item_definition_factory()
{
}

i_item_definition *
managed_item_definition_factory::create(
    const i_item_definition &initializer)
const
{
        return create(
            item_definition_factory_.create(initializer));
}

i_managed_item_definition *
managed_item_definition_factory::create(
    const i_managed_item_definition &initializer)
const
{
        return new managed_item_definition(initializer);
}

i_managed_item_definition *
managed_item_definition_factory::create(
    i_item_definition *definition_to_manage)
const
{
        std::auto_ptr<i_item_definition> managed(
            definition_to_manage);
        std::auto_ptr<i_managed_item_definition> definition(
            new managed_item_definition(this, definition_to_manage));
        managed.release();

        return definition.release();
}

i_item_definition *
managed_item_definition_factory::create(
    const string_type &name,
    std::vector<i_size_definition *> &size_definitions,
    item_key_type key)
const
{
        return create(
            item_definition_factory_.create(name, size_definitions, key));
}

} /* item_management */

