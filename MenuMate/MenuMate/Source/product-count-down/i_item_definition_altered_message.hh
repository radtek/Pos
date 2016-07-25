#ifndef __hh_i_item_definition_altered_message__
#define __hh_i_item_definition_altered_message__

#include <i_message.hh>
#include <key_types.hh>

namespace item_management {

namespace network_messaging {

class i_item_definition_altered_message : public i_message {
public:
        i_item_definition_altered_message();
        virtual ~i_item_definition_altered_message();

        virtual i_item_definition_altered_message &operator=(
            const i_item_definition_altered_message &rhs) = 0;
        
        virtual bool get_enabled() const = 0;
        virtual item_key_type get_item_key() const = 0;
};

} /* network_messaging */
} /* item_management */

#endif /* ! __hh_i_item_definition_altered_message__ */

