#ifndef __hh_i_item_definition_altered_message_factory__
#define __hh_i_item_definition_altered_message_factory__

#include <key_types.hh>

namespace item_management {

class i_signalling_item_manager;

namespace network_messaging {

class i_item_definition_altered_message;

class i_item_definition_altered_message_factory {
public:
        i_item_definition_altered_message_factory();
        virtual ~i_item_definition_altered_message_factory();

        virtual i_item_definition_altered_message *create(
            i_signalling_item_manager *item_manager) const = 0;

        virtual i_item_definition_altered_message *create(
            const i_item_definition_altered_message &initializer)
                const = 0;

        virtual i_item_definition_altered_message *create(
            i_signalling_item_manager *item_manager,
            bool enabled,
            item_key_type key) const = 0;
};

} /* network_messaging */
} /* item_management */

#endif /* ! __hh_i_item_definition_altered_message_factory__ */

