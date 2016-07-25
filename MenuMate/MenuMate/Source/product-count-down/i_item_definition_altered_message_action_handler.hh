#ifndef __hh_i_item_definition_altered_message_action_handler__
#define __hh_i_item_definition_altered_message_action_handler__

#include <i_message_action_handler.hh>

namespace item_management {

class i_item_definition_altered_message_action_handler
    : public i_message_action_handler<i_item_definition_altered_message &>
public:
        virtual ~i_item_definition_altered_message_action_handler() { };

protected:
        i_item_definition_altered_message_action_handler() { };
}

} /* item_management */

#endif /* ! __hh_i_item_definition_altered_message_action_handler__ */

