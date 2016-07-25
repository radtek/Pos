#include <i_size_definition_altered_message.hh>

namespace item_management {
namespace network_messaging {

i_size_definition_altered_message::i_size_definition_altered_message()
    : TNetMessage(udp_size_definition_altered)
{
}

i_size_definition_altered_message::~i_size_definition_altered_message()
{
}

} /* network_messaging */
} /* item_management */


