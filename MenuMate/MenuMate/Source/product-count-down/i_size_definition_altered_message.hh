#ifndef __hh_i_size_definition_altered_message__
#define __hh_i_size_definition_altered_message__

#include <SimpleTCP.h>

#include <key_types.hh>

namespace item_management {

class i_signalling_item_manager;

namespace network_messaging {

class i_size_definition_altered_message : public TNetMessage {
public:
        i_size_definition_altered_message();
        virtual ~i_size_definition_altered_message();

        virtual i_size_definition_altered_message &operator=(
            const i_size_definition_altered_message &rhs) = 0;

        virtual i_signalling_item_manager &get_item_manager() const = 0;

        virtual item_key_type get_item_key() const = 0;
        virtual size_key_type get_size_key() const = 0;

        virtual double get_available_quantity() const = 0;
        virtual double get_default_quantity() const = 0;
        virtual bool get_enabled() const = 0;
        virtual double get_price() const = 0;
        virtual double get_special_price() const = 0;
        virtual double get_warning_quantity() const = 0;
};

} /* network_messaging */
} /* item_management */

#endif /* ! __hh_i_size_definition_altered_message__ */


