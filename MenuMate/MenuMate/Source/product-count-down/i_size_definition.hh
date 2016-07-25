#ifndef __hh_i_size_definition__
#define __hh_i_size_definition__

#include <key_types.hh>
#include <string_type.hh>

namespace item_management {

class i_size_definition_factory;

class i_size_definition {
public:
        virtual ~i_size_definition();

        /* Operators */
        virtual i_size_definition &operator=(
            const i_size_definition &rhs) = 0;

        virtual bool operator==(const i_size_definition &rhs) const;
        virtual bool operator!=(const i_size_definition &rhs) const;

        /* Aux. Getters */
        virtual void extract_properties(
            double &available_quantity,
            double &default_quantity,
            bool &disable_when_counts_reaches_zero,
            bool &enabled,
            size_key_type &key,
            string_type &name,
            double &price,
            double &maxRetailPrice,
            bool &reset_to_default_value_on_zed,
            double &special_price,
            double &warning_quantity) const;

        /* Getters */
        virtual double get_available_quantity() const = 0;
        virtual const string_type &get_decorated_name() const = 0;
        virtual double get_default_quantity() const = 0;
        virtual bool get_disable_when_count_reaches_zero() const = 0;
        virtual bool get_enabled() const = 0;
        virtual size_key_type get_key() const = 0;
        virtual const string_type &get_name() const = 0;
        virtual double get_price() const = 0;
        virtual double get_maxRetailPrice() const = 0;

        virtual bool get_reset_to_default_value_on_zed() const = 0;

        virtual const i_size_definition_factory &
            get_size_definition_factory() const = 0;

        virtual double get_special_price() const = 0;
        virtual double get_warning_quantity() const = 0;

        /* Getter/Setters */
        bool claim_units(double n_units = 1.0);

        /* Setters */
        void release_units(double n_units = 1.0);

        virtual bool adjust_available_quantity(double offset);

        virtual bool set_all(double available_quantity,
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
        virtual bool set_all(const i_size_definition &size_definition);

        virtual bool set_available_quantity(
            double available_quantity) = 0;
        virtual bool set_default_quantity(double default_quantity) = 0;
        virtual bool set_disable_when_count_reaches_zero(
            bool disable_when_count_reaches_zero) = 0;
        virtual bool set_enabled(bool enabled) = 0;
        virtual bool set_key(size_key_type key) = 0;
        virtual bool set_name(const string_type &name) = 0;
        virtual bool set_price(double price) = 0;
        virtual bool set_maxRetailPrice(double maxRetailPrice) = 0;
        virtual bool set_reset_to_default_value_on_zed(
            bool reset_to_default_value_on_zed) = 0;
        virtual bool set_special_price(double special_price) = 0;
        virtual bool set_warning_quantity(double warning_quantity) = 0;

protected:
        i_size_definition();
};

} /* item_management */

#endif /* ! __hh_i_size_definition__ */

