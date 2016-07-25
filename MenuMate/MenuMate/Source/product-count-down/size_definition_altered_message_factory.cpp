#include <size_definition_altered_message.hh>
#include <size_definition_altered_message_factory.hh>

namespace item_management {
namespace network_messaging {

size_definition_altered_message_factory
    ::size_definition_altered_message_factory()
{
}

size_definition_altered_message_factory
    ::~size_definition_altered_message_factory()
{
}

i_size_definition_altered_message *
size_definition_altered_message_factory::create(
    i_signalling_item_manager *item_manager)
const
{
        return new size_definition_altered_message(item_manager);
}

i_size_definition_altered_message *
size_definition_altered_message_factory::create(
    const i_size_definition_altered_message &initializer)
const
{
        return new size_definition_altered_message(initializer);
}

i_size_definition_altered_message *
size_definition_altered_message_factory::create(
    i_signalling_item_manager *item_manager,
    item_key_type item_key,
    size_key_type size_key,
    double available_quantity,
    double default_quantity,
    bool enabled,
    double price,
    double special_price,
    double warning_quantity)
const
{
        return new size_definition_altered_message(
                       item_manager,
                       item_key,
                       size_key,
                       available_quantity,
                       default_quantity,
                       enabled,
                       price,
                       special_price,
                       warning_quantity);
}

} /* network_messaging */
} /* item_management */

