#include <signalling_item_definition.hh>
#include <signalling_item_definition_factory.hh>
#include <i_item_definition.hh>

namespace item_management {

signalling_item_definition_factory::signalling_item_definition_factory(
    const i_item_definition_factory &item_definition_factory)
    : item_definition_factory_(item_definition_factory)
{
}

signalling_item_definition_factory::~signalling_item_definition_factory()
{
}

i_item_definition *
signalling_item_definition_factory::create(
    const i_item_definition &initializer)
const
{
        return create(item_definition_factory_.create(initializer),
                      0x0,
                      0x0,
                      0x0);
}

i_managed_item_definition *
signalling_item_definition_factory::create(
    const i_managed_item_definition &initializer)
const
{
        return new signalling_item_definition(initializer);
}

i_item_definition *
signalling_item_definition_factory::create(
    const string_type &name,
    std::vector<i_size_definition *> &size_definitions,
    item_key_type key)
const
{
        return create(
                   item_definition_factory_.create(name,
                                                   size_definitions,
                                                   key),
                   0x0,
                   0x0,
                   0x0);
}

i_signalling_item_definition *
signalling_item_definition_factory::create(
    i_item_definition *definition_to_wrap)
const
{
        return new signalling_item_definition(this,
                                              definition_to_wrap);
}

i_signalling_item_definition *
signalling_item_definition_factory::create(
    i_item_definition *definition_to_wrap,
    const i_item_altered_signal_handler *item_altered_handler,
    const i_size_altered_signal_handler *size_altered_handler,
    const i_size_warning_reached_signal_handler *size_warning_reached_handler)
const
{
        return new signalling_item_definition(this,
                                              definition_to_wrap,
                                              item_altered_handler,
                                              size_altered_handler,
                                              size_warning_reached_handler);
}

} /* item_management */

