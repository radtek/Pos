#include <item_definition_altered_message.hh>
#include <item_definition_altered_message_factory.hh>

namespace item_management
{
namespace network_messaging
{

item_definition_altered_message_factory
    ::item_definition_altered_message_factory()
{
}

item_definition_altered_message_factory
    ::~item_definition_altered_message_factory()
{
}

i_item_definition_altered_message *
item_definition_altered_message_factory::create(
    i_signalling_item_manager *item_manager)
const
{
        return new item_definition_altered_message(item_manager);
}

i_item_definition_altered_message *
item_definition_altered_message_factory::
    create(const i_item_definition_altered_message &initializer)
const
{
        return new item_definition_altered_message(initializer);
}

i_item_definition_altered_message *
item_definition_altered_message_factory::create(
    i_signalling_item_manager *item_manager,
    bool enabled,
    item_key_type key)
const
{
        return new item_definition_altered_message(
                       item_manager, enabled, key);
}

} /* network_messaging */
} /* item_management */

