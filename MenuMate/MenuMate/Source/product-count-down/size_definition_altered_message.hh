#ifndef __hh_size_definition_altered_message__
#define __hh_size_definition_altered_message__

#include <i_size_definition_altered_message.hh>

namespace item_management {
namespace network_messaging {

class size_definition_altered_message
    : public i_size_definition_altered_message {
public:
        size_definition_altered_message();
        size_definition_altered_message(
            const i_size_definition_altered_message &initializer);
        size_definition_altered_message(
            const size_definition_altered_message &initializer);
        size_definition_altered_message(
            i_signalling_item_manager *item_manager);
        size_definition_altered_message(
            i_signalling_item_manager *item_manager,
            item_key_type item_key,
            size_key_type size_key,
            double available_quantity,
            double default_quantity,
            bool enabled,
            double price,
            double special_price,
            double warning_quantity);
        ~size_definition_altered_message();

        size_definition_altered_message &operator=(
            const i_size_definition_altered_message &rhs);
        size_definition_altered_message &operator=(
            const size_definition_altered_message &rhs);

        i_signalling_item_manager &get_item_manager() const;

        double get_available_quantity() const;
        double get_default_quantity() const;
        bool get_enabled() const;
        item_key_type get_item_key() const;
        menu_key_type get_menu_key() const;
        double get_price() const;
        size_key_type get_size_key() const;
        double get_special_price() const;
        double get_warning_quantity() const;

        void Action();
        void Decode(TMemoryStream *istream);
        void Encode(TMemoryStream *ostream);

private:
        double available_quantity_;
        double default_quantity_;
        bool enabled_;
        item_key_type item_key_;
        menu_key_type menu_key_;
        double price_;
        size_key_type size_key_;
        double special_price_;
        double warning_quantity_;

        i_signalling_item_manager *manager_;
};

} /* network_messaging */
} /* item_management */

#endif /* ! __hh_size_definition_altered_message__ */

