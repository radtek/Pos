#ifndef __hh_item_manager__
#define __hh_item_manager__

#include <vector>

#include <i_item_manager.hh>

namespace item_management {

class i_item_definition;
class i_size_definition;

class item_manager : public virtual i_item_manager {
public:
        item_manager(
            const i_item_definition_factory *item_definition_factory,
            const i_size_definition_factory *size_definition_factory);
        virtual ~item_manager();

        void add_item_definitions(
            const std::vector<i_item_definition *> &definitions);
        void remove_item_definitions(
            const std::vector<item_key_type> &keys);

        const i_item_definition_factory &
            get_item_definition_factory() const;
        const i_size_definition_factory &
            get_size_definition_factory() const;

        size_key_type translate_size_name_to_key(
            item_key_type item_key, const string_type &name) const;

        void get_item_available_sizes(
            item_key_type key,
            std::map<size_key_type,
                     const i_size_definition *> &destination) const;

        const string_type &get_item_decorated_name(
            item_key_type key) const;
        const string_type &get_item_name(item_key_type key) const;
        const string_type &get_size_decorated_name(
            item_key_type item_key, size_key_type size_key) const;
        const string_type &get_size_name(
            item_key_type item_key, size_key_type size_key) const;

        bool get_item_enabled(item_key_type key) const;
        bool get_size_enabled(
            item_key_type item_key, size_key_type size_key) const;

        double get_available_quantity(
            item_key_type item_key, size_key_type size_key) const;
        double get_default_quantity(
            item_key_type item_key, size_key_type size_key) const;
        double get_price(
            item_key_type item_key, size_key_type size_key) const;
        double get_special_price(
            item_key_type item_key, size_key_type size_key) const;
        double get_warning_quantity(
            item_key_type item_key, size_key_type size_key) const;

        bool claim_units(item_key_type item_key,
                         size_key_type size_key,
                         double n_units = 1.0);
        void release_units(item_key_type item_key,
                           size_key_type size_key,
                           double n_units = 1.0);

private:
        item_manager(const item_manager &initializer);
        item_manager &operator=(item_manager &rhs);

        const i_item_definition_factory *item_definition_factory_;
        const i_size_definition_factory *size_definition_factory_;
};

} /* item_management */

#endif /* ! __hh_item_manager__ */


