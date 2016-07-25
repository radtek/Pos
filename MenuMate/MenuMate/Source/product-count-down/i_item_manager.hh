#ifndef __hh_i_item_manager__
#define __hh_i_item_manager__

#include <map>
#include <vector>

#include <key_types.hh>
#include <string_type.hh>

namespace item_management {

class i_item_definition;
class i_item_definition_factory;
class i_size_definition;
class i_size_definition_factory;

class i_item_manager {
public:
        i_item_manager();
        virtual ~i_item_manager();

        virtual void add_item_definition(i_item_definition *definition) = 0;
        virtual void add_item_definitions(const std::vector<i_item_definition *> &definitions) = 0;

        virtual void remove_item_definition(item_key_type key) = 0;
        virtual void remove_item_definitions(const std::vector<item_key_type> &keys) = 0;

        virtual const i_item_definition_factory &get_item_definition_factory() const = 0;
        virtual const i_size_definition_factory &get_size_definition_factory() const = 0;

        virtual size_key_type translate_size_name_to_key(item_key_type item_key, const string_type &name) const = 0;

        virtual void get_item_available_sizes(item_key_type key, std::map<size_key_type, const i_size_definition *> &destination) const = 0;

        virtual i_item_definition &get_item_definition(item_key_type key) = 0;
        virtual const i_item_definition &get_item_definition(item_key_type key) const = 0;

        virtual i_size_definition &get_size_definition(item_key_type item_key, size_key_type size_key) = 0;
        virtual const i_size_definition &get_size_definition(item_key_type item_key, size_key_type size_key) const = 0;

        virtual const string_type &get_item_decorated_name(item_key_type key) const = 0;
        virtual const string_type &get_item_name(item_key_type key) const = 0;
        virtual const string_type &get_size_decorated_name(item_key_type item_key, size_key_type size_key) const = 0;
        virtual const string_type &get_size_name(item_key_type item_key, size_key_type size_key) const = 0;

        virtual bool get_item_enabled(item_key_type key) const = 0;
        virtual bool get_size_enabled(item_key_type item_key, size_key_type size_key) const = 0;

        virtual double get_available_quantity(item_key_type item_key, size_key_type size_key) const = 0;
        virtual double get_default_quantity(item_key_type item_key, size_key_type size_key) const = 0;
        virtual double get_price(item_key_type item_key, size_key_type size_key) const = 0;
        virtual double get_special_price(item_key_type item_key, size_key_type size_key) const = 0;
        virtual double get_warning_quantity(item_key_type item_key, size_key_type size_key) const = 0;

        virtual bool claim_units(item_key_type item_key, size_key_type size_key, double n_units = 1.0) = 0;
        virtual void release_units(item_key_type item_key, size_key_type size_key, double n_units = 1.0) = 0;

private:
        i_item_manager(const i_item_manager &initializer);
        i_item_manager &operator=(const i_item_manager &rhs);
};

} /* item_management */

#endif /* ! __hh_i_item_manager__ */

