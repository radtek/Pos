#ifndef __hh_size_definition__
#define __hh_size_definition__

#include <i_size_definition.hh>

namespace utilities {
namespace locking {

class i_spinlock;

} /* locking */
} /* utilities */

namespace item_management {

class size_definition : public i_size_definition {
public:
        size_definition();
        size_definition(const i_size_definition &initializer);
        size_definition(
            const size_definition &initializer);
        size_definition(
            const i_size_definition_factory *size_definition_factory);
        size_definition(
            const i_size_definition_factory *size_definition_factory,
            size_key_type key);
        ~size_definition();

        /* Operators */
        size_definition &operator=(const i_size_definition &rhs);
        size_definition &operator=(const size_definition &rhs);

        /* Getters */
        double get_available_quantity() const;
        const string_type &get_decorated_name() const;
        double get_default_quantity() const;
        bool get_disable_when_count_reaches_zero() const;
        bool get_enabled() const;
        size_key_type get_key() const;
        const string_type &get_name() const;
        double get_price() const;
        double get_maxRetailPrice() const;

        const i_size_definition_factory &
            get_size_definition_factory() const;

        bool get_reset_to_default_value_on_zed() const;
        double get_special_price() const;
        double get_warning_quantity() const;

        /* Setters */
        bool set_all(double available_quantity,
                     double default_quantity,
                     bool disable_when_count_reaches_zero,
                     bool enabled,
                     size_key_type key,
                     const string_type &name,
                     double price,
                     double maxRetailPrice,
                     bool reset_to_default_value_on_zed,
                     double special_price,
                     double warning_quantity);
        bool set_all(const i_size_definition &size_definition);
        bool set_available_quantity(double available_quantity);
        bool set_default_quantity(double available_quantity);
        bool set_disable_when_count_reaches_zero(
        bool disable_when_count_reaches_zero);
        bool set_enabled(bool enabled);
        bool set_key(size_key_type key);
        bool set_name(const string_type &name);
        bool set_price(double price);
        bool set_maxRetailPrice(double maxRetailPrice);
        bool set_reset_to_default_value_on_zed(
        bool reset_to_default_value_on_zed);
        bool set_special_price(double price);
        bool set_warning_quantity(double available_quantity);

private:
        bool regenerate_decorated_name();

        double available_quantity_;
        double default_quantity_;
        bool disable_when_count_reaches_zero_;
        double price_;
        double maxRetailPrice_;
        bool reset_to_default_value_on_zed_;
        double special_price_;
        double warning_quantity_;

        string_type base_name_;
        string_type decorated_name_;

        size_key_type key_;

        bool enabled_;

        const i_size_definition_factory *size_definition_factory_;
        
        utilities::locking::i_spinlock *mutator_lock_;
};

} /* item_management */

#endif /* ! __hh_size_definition__ */
