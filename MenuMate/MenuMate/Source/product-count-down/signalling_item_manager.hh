#ifndef __hh_signalling_item_manager__
#define __hh_signalling_item_manager__

#include <map>

#include <i_signalling_item_manager.hh>
#include <item_manager.hh>

namespace utilities {
namespace locking {

class i_spinlock;

} /* locking */
} /* utilities */

namespace item_management {

class i_signalling_item_definition;
class i_signalling_item_definition_factory;
class i_size_definition_factory;

class signalling_item_manager
    : public i_signalling_item_manager, public item_manager {
public:
        signalling_item_manager(
            const i_item_definition_factory *id_factory,
            const i_signalling_item_definition_factory *sid_factory,
            const i_size_definition_factory *sd_factory,
            const i_item_altered_signal_handler *ia_handler,
            const i_size_altered_signal_handler *sa_handler,
            const i_size_warning_reached_signal_handler *srw_handler);
        ~signalling_item_manager();

        using item_manager::claim_units;
        using item_manager::release_units;

        void add_item_definition(i_item_definition *definition);
        void remove_item_definition(item_key_type key);

        i_item_definition &get_item_definition(
            item_key_type key);
        const i_item_definition &get_item_definition(
            item_key_type key) const;

        i_size_definition &get_size_definition(
            item_key_type item_key, size_key_type size_key);
        const i_size_definition &get_size_definition(
            item_key_type item_key, size_key_type size_key) const;

        void quietly_set_item_enabled(item_key_type key, bool enabled);
        void quietly_update_size_definition(
            item_key_type item_key,
            size_key_type size_key,
            double available_quantity,
            double default_quantity,
            bool enabled,
            double price,
            double special_price,
            double warning_quantity);

private:
        signalling_item_manager(
            const signalling_item_manager &initializer);
        signalling_item_manager &operator=(
            const signalling_item_manager &rhs);

        typedef std::map<item_key_type,
                         i_signalling_item_definition *> sid_map;

        sid_map item_definitions_;

        const i_signalling_item_definition_factory *sid_factory_;

        const i_item_altered_signal_handler *ia_handler_;
        const i_size_altered_signal_handler *sa_handler_;
        const i_size_warning_reached_signal_handler *srw_handler_;

        utilities::locking::i_spinlock *manager_lock_;
};

} /* item_management */

#endif /* ! __hh_signalling_item_manager__ */

