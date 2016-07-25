#ifndef __hh_signalling_item_definition__
#define __hh_signalling_item_definition__

#include <i_signalling_item_definition.hh>
#include <managed_item_definition.hh>

namespace item_management {

class signalling_item_definition
    : public i_signalling_item_definition,
      public managed_item_definition {
public:
        signalling_item_definition();
        signalling_item_definition(
            const i_managed_item_definition &initializer);
        signalling_item_definition(
            const i_signalling_item_definition &initializer);
        signalling_item_definition(
            const signalling_item_definition &initializer);
        signalling_item_definition(
            const i_item_definition_factory *item_definition_factory,
            i_item_definition *definition_to_wrap,
            const i_item_altered_signal_handler *item_altered_handler = 0x0,
            const i_size_altered_signal_handler *size_altered_handler = 0x0,
            const i_size_warning_reached_signal_handler *
                size_warning_reached_handler = 0x0);
        ~signalling_item_definition();

        /* Operators */
        signalling_item_definition &operator=(
            const i_item_definition &rhs);
        signalling_item_definition &operator=(
            const i_managed_item_definition &rhs);
        signalling_item_definition &operator=(
            const i_signalling_item_definition &rhs);
        signalling_item_definition &operator=(
            const signalling_item_definition &rhs);

        /* Getters */
        using managed_item_definition::get_available_quantity;
        using managed_item_definition::get_available_sizes;
        using managed_item_definition::get_decorated_name;
        using managed_item_definition::get_default_quantity;
        using managed_item_definition::get_item_definition;
        using managed_item_definition::get_item_definition_factory;
        using managed_item_definition::get_key;
        using managed_item_definition::get_name;
        using managed_item_definition::get_price;
        using managed_item_definition::get_size_decorated_name;
        using managed_item_definition::get_size_definition;
        using managed_item_definition::get_size_enabled;
        using managed_item_definition::get_size_name;
        using managed_item_definition::get_special_price;

        const i_item_altered_signal_handler &
            get_item_altered_handler() const;
        const i_size_altered_signal_handler &
            get_size_altered_handler() const;
        const i_size_warning_reached_signal_handler &
            get_size_warning_reached_handler() const;

        /* Setters */
        bool adjust_available_quantity(size_key_type key,
                                       double offset);

        bool set_available_quantity(
            size_key_type key, double available_quantity);

        bool set_default_quantity(
            size_key_type key, double default_quantity);

        bool set_enabled(bool enabled);

        bool set_name(const string_type &name);

        bool set_price(size_key_type key, double price);

        bool set_size_enabled(size_key_type key, bool enabled);

        bool set_special_price(size_key_type key, double special_price);

        bool set_warning_quantity(size_key_type key,
                                  double warning_quantity);

        bool claim_units(size_key_type key, double n_units = 1.0);
        void release_units(size_key_type key, double n_units = 1.0);

        bool quietly_set_enabled(bool enabled);
        bool quietly_update_size_definition(
            size_key_type key,
            double available_quantity,
            double default_quantity,
            bool enabled,
            double price,
            double special_price,
            double warning_quantity);
        
        bool update_size_definition(const i_size_definition &definition);

private:
        bool forward_item_alteration(bool was_altered);
        bool forward_size_alteration(size_key_type key, bool was_altered);

        const i_item_altered_signal_handler *item_altered_handler_;
        const i_size_altered_signal_handler *size_altered_handler_;
        const i_size_warning_reached_signal_handler *
            size_warning_reached_handler_;
};

} /* item_management */

#endif /* ! __hh_signalling_item_definition__ */

