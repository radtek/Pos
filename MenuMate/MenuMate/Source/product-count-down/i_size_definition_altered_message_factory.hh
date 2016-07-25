#ifndef __hh_i_size_definition_altered_message_factory__
#define __hh_i_size_definition_altered_message_factory__

#include <key_types.hh>

namespace item_management {

class i_signalling_item_manager;

namespace network_messaging {

class i_size_definition_altered_message;

class i_size_definition_altered_message_factory {
public:
        i_size_definition_altered_message_factory();
        virtual ~i_size_definition_altered_message_factory();

        virtual i_size_definition_altered_message *create(
            i_signalling_item_manager *item_manager) const = 0;

        virtual i_size_definition_altered_message *create(
            const i_size_definition_altered_message &initializer)
                const = 0;

        virtual i_size_definition_altered_message *create(
            i_signalling_item_manager *item_manager,
            item_key_type item_key,
            size_key_type size_key,
            double available_quantity,
            double default_quantity,
            bool enabled,
            double price,
            double special_price,
            double warning_quantity) const = 0;
};

} /* network_messaging */
} /* item_management */

#endif /* ! __hh_i_size_definition_altered_message_factory__ */

