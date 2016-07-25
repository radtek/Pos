#include <i_item_definition_altered_message.hh>

namespace item_management {
namespace network_messaging {

i_item_definition_altered_message::i_item_definition_altered_message()
    : i_message(udp_item_definition_altered)
{
}

i_item_definition_altered_message::~i_item_definition_altered_message()
{
}

} /* network_messaging */
} /* item_management */

