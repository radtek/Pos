#ifndef __hh_item_definition__
#define __hh_item_definition__

#include <map>
#include <vector>

#include <i_item_definition.hh>

namespace utilities {
namespace locking {

class i_spinlock;

} /* locking */
} /* utilities */

namespace item_management {

class item_definition : public i_item_definition {
public:
        item_definition();
        item_definition(
            const i_item_definition &initializer);
        item_definition(
            const item_definition &initializer);
        item_definition(
            const i_item_definition_factory *item_definition_factory,
            const string_type &name,
            std::vector<i_size_definition *> &size_definitions,
            item_key_type key);
        item_definition(
            const i_item_definition_factory *item_definition_factory,
            item_key_type key);
        ~item_definition();

        item_definition &operator=(
            const i_item_definition &rhs);
        item_definition &operator=(
            const item_definition &rhs);

        /* Getters */
        void get_available_sizes(
            std::map<size_key_type,
                     const i_size_definition *> &destination) const;

        const string_type &get_decorated_name() const;

        const i_item_definition_factory &
            get_item_definition_factory() const;

        bool get_enabled() const;

        item_key_type get_key() const;

        const string_type &get_name() const;

        const i_size_definition &
            get_size_definition(size_key_type key) const;

        size_key_type translate_size_name_to_id(
            const string_type &name) const;

        /* Getter / Setters */
        bool update_size_definition(
            const i_size_definition &definition);

        bool claim_units(size_key_type key, double n_units = 1.0);

        /* Setters */
        void release_units(size_key_type key, double n_units = 1.0);

        bool adjust_available_quantity(size_key_type key,
                                       double offset);

        bool set_available_quantity(size_key_type key,
                                    double available_quantity);

        bool set_default_quantity(size_key_type key,
                                  double default_quantity);

        bool set_enabled(bool enabled);

        item_key_type set_key();

        bool set_name(const string_type &name);

        bool set_price(size_key_type key, double price);

        bool set_size_enabled(size_key_type key, bool enabled);

        bool set_special_price(size_key_type key, double special_price);

        bool set_warning_quantity(size_key_type key,
                                  double warning_quantity);

private:
        i_size_definition &get_size_definition_rw();
        i_size_definition &get_size_definition_rw(size_key_type key);

        void initialize_from(const i_item_definition &initializer);
        bool regenerate_decorated_name();
        bool regenerate_enabled_state();
        void release_held_sizes();

        const i_item_definition_factory *item_definition_factory_;

        item_key_type key_;

        bool enabled_;
        string_type decorated_name_;
        string_type name_;

        std::map<string_type, i_size_definition *> name_map_;
        std::map<size_key_type, i_size_definition *> sizes_;

        utilities::locking::i_spinlock *decorated_name_lock_;
        utilities::locking::i_spinlock *enable_lock_;
};

} /* item_management */

#endif /* ! __hh_item_definition__ */

