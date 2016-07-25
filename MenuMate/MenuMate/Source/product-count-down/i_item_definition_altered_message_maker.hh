#ifndef __i_item_definition_altered_message_maker__
#define __i_item_definition_altered_message_maker__

#include <i_message_maker.hh>

namespace item_management {
namespace network_messaging {

class i_item_definition_altered_message;
class i_item_definition_altered_message_factory;

class i_item_definition_altered_message_maker
    : public i_message_maker<i_item_definition_altered_message_factory> {
public:
        i_item_definition_altered_message_maker();
        i_item_definition_altered_message_maker(
            i_signalling_item_manager *item_manager,
            const i_item_definition_altered_message_factory *message_factory);
        virtual ~i_item_definition_altered_message_maker();

        virtual i_item_definition_altered_message_maker &operator=(
            const i_item_definition_altered_message_maker &rhs) = 0;
};

} /* network_messaging */
} /* item_management */

#endif /* ! __i_item_definition_altered_message_maker__ */

