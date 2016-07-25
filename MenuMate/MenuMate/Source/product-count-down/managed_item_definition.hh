#ifndef __hh_managed_item_definition__
#define __hh_managed_item_definition__

#include <memory>
#include <i_managed_item_definition.hh>

namespace item_management {

class managed_item_definition : public virtual i_managed_item_definition {
public:
        managed_item_definition();
        managed_item_definition(
            const managed_item_definition &initializer);
        managed_item_definition(
            const i_managed_item_definition &initializer);
        managed_item_definition(
            const i_item_definition_factory *item_definition_factory,
            i_item_definition *item_definiton_to_manage);
        virtual ~managed_item_definition();

        /* Operators */
        managed_item_definition &operator=(
            const i_item_definition &rhs);
        managed_item_definition &operator=(
            const i_managed_item_definition &rhs);
        managed_item_definition &operator=(
            const managed_item_definition &rhs);

        /* Getters */
        void get_available_sizes(
            std::map<size_key_type,
                     const i_size_definition *> &destination) const;

        double get_available_quantity(size_key_type key) const;
        const string_type &get_decorated_name() const;
        double get_default_quantity(size_key_type key) const;
        bool get_enabled() const;

        i_item_definition &get_item_definition() const;

        const i_item_definition_factory &
            get_item_definition_factory() const;

        item_key_type get_key() const;

        const string_type &get_name() const;

        double get_price(size_key_type key) const;

        const string_type &get_size_decorated_name(
            size_key_type key) const;

        const i_size_definition &get_size_definition(
            size_key_type key) const;

        bool get_size_enabled(size_key_type key) const;

        const string_type &get_size_name(size_key_type key) const;

        double get_special_price(size_key_type key) const;

        double get_warning_quantity(size_key_type key) const;

        size_key_type translate_size_name_to_id(
            const string_type &name) const;

        /* Getter / Setters */
        bool claim_units(size_key_type key, double n_units = 1.0);

        /* Setters */
        void release_units(size_key_type key, double n_units = 1.0);

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
        
        bool update_size_definition(
            const i_size_definition &definition);

private:
        std::auto_ptr<i_item_definition> item_definition_;
        const i_item_definition_factory *item_definition_factory_;

        void clone_definition(const i_item_definition &definition);
};

} /* item_management */

#endif /* ! __hh_managed_item_definition__ */

