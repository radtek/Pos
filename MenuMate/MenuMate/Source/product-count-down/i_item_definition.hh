#ifndef __hh_i_item_definition__
#define __hh_i_item_definition__

#include <map>

#include <item_management_exceptions.hh>

#include <key_types.hh>
#include <string_type.hh>

namespace item_management {

class i_item_definition_factory;
class i_size_definition;
class i_size_definition_factory;

class i_item_definition {
public:
        i_item_definition();
        virtual ~i_item_definition();

        /* Operators */
        virtual i_item_definition &operator=(
            const i_item_definition &) = 0;

        /* Getters */
        virtual void get_available_sizes(
            std::map<size_key_type,
                     const i_size_definition *> &destination) const = 0;

        virtual double get_available_quantity(size_key_type key) const;

        virtual const string_type &get_decorated_name() const = 0;

        virtual double get_default_quantity(size_key_type key) const;

        virtual bool get_enabled() const = 0;

        virtual const i_item_definition_factory &
            get_item_definition_factory() const = 0;

        virtual item_key_type get_key() const = 0;
        
        virtual const string_type &get_name() const = 0;

        virtual double get_price(size_key_type key) const;

        virtual const string_type &get_size_decorated_name(
            size_key_type key) const;

        virtual const i_size_definition &
            get_size_definition(size_key_type key) const = 0;

        virtual bool get_size_enabled(size_key_type key) const;

        virtual const string_type &get_size_name(
            size_key_type key) const;

        virtual double get_special_price(size_key_type key) const;

        virtual double get_warning_quantity(size_key_type key) const;

        virtual size_key_type translate_size_name_to_id(
            const string_type &name) const = 0;

        /* Getter/Setters */
        virtual bool claim_units(size_key_type key,
                                 double n_units = 1.0) = 0;

        /* Setters */
        virtual void release_units(size_key_type key,
                                   double n_units = 1.0) = 0;

        virtual bool adjust_available_quantity(size_key_type key,
                                               double offset) = 0;
        virtual bool set_available_quantity(
            size_key_type key, double available_quantity) = 0;

        virtual bool set_default_quantity(size_key_type key,
                                          double default_quantity) = 0;

        virtual bool set_enabled(bool enabled) = 0;

        virtual bool set_name(const string_type &name) = 0;

        virtual bool set_price(size_key_type key, double price) = 0;

        virtual bool set_size_enabled(
            size_key_type key, bool enabled) = 0;

        virtual bool set_special_price(size_key_type key,
                                       double special_price) = 0;

        virtual bool set_warning_quantity(size_key_type key,
                                          double warning_quantity) = 0;

        virtual bool update_size_definition(
            const i_size_definition &definition) = 0;

private:
        i_item_definition(const i_item_definition &initializer);
}; /* i_item_definition */

} /* item_management */

#endif /* ! __hh_i_item_definition__ */

