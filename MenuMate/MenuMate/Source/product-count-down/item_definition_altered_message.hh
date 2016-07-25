#ifndef __item_definition_altered_message__
#define __item_definition_altered_message__

#include <Stream.h>
#include <i_item_definition_altered_message.hh>
#include <i_signalling_item_manager.hh>

namespace item_management {
namespace network_messaging {

class item_definition_altered_message
    : public i_item_definition_altered_message {
public:
        item_definition_altered_message();
        item_definition_altered_message(
            i_signalling_item_manager *item_manager);
        item_definition_altered_message(
            i_signalling_item_manager *item_manager,
            bool enabled,
            item_key_type item_key);
        item_definition_altered_message(
            const i_item_definition_altered_message &initializer);
        item_definition_altered_message(
            const item_definition_altered_message &initializer);
        ~item_definition_altered_message();

        item_definition_altered_message &operator=(
            const i_item_definition_altered_message &rhs);
        item_definition_altered_message &operator=(
            const item_definition_altered_message &rhs);

        i_signalling_item_manager &get_item_manager() const;

        bool get_enabled() const;

        item_key_type get_item_key() const;
        menu_key_type get_menu_key() const;

        void Action();
        void Decode(TMemoryStream *istream);
        void Encode(TMemoryStream *ostream);

private:
        bool enabled_;
        item_key_type item_key_;

        i_signalling_item_manager *manager_;
};

} /* network_messaging */
} /* item_management */

#endif /* ! __item_definition_altered_message__ */
