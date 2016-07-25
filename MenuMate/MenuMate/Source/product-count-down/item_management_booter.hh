#ifndef __hh_item_management_booter__
#define __hh_item_management_booter__

#include <memory>
#include <i_spinlock.hh>

namespace utilities {
namespace locking {

class i_spinlock;
class spinlock_reference;

} /* locking */
} /* utilities */

namespace item_management {

namespace network_messaging {

class i_item_definition_altered_message_factory;
class i_item_definition_altered_message_maker;
class i_size_definition_altered_message_factory;
class i_size_definition_altered_message_maker;

} /* network_messaging */

using network_messaging::i_item_definition_altered_message_factory;
using network_messaging::i_item_definition_altered_message_maker;
using network_messaging::i_size_definition_altered_message_factory;
using network_messaging::i_size_definition_altered_message_maker;

class i_item_definition_factory;
class i_item_manager;
class i_menu_manager;
class i_signalling_item_definition_factory;
class i_signalling_item_manager;
class i_size_definition_factory;

class kickstarter {
public:
        static i_item_manager &get_item_manager();
        static i_menu_manager &get_menu_manager();

        static const i_item_definition_factory &
            get_item_definition_factory();
        static const i_size_definition_factory &
            get_size_definition_factory();

        static void kickstart();

private:
        kickstarter();
        ~kickstarter();

        kickstarter(const kickstarter &initializer);
        kickstarter &operator=(const kickstarter &rhs);

        void cleanup();

        void destroy_factories();
        void destroy_makers();
        void destroy_managers();

        static void initialize();

        void initialize_proper();
        void initialize_factories();
        void initialize_makers();
        void initialize_managers();

        i_signalling_item_manager *item_manager_;
        i_menu_manager *menu_manager_;

        const i_item_definition_factory *item_definition_factory_;
        const i_signalling_item_definition_factory *
            signalling_item_definition_factory_;
        const i_size_definition_factory *size_definition_factory_;

        const i_item_definition_altered_message_factory *
            item_definition_altered_message_factory_;
        const i_item_definition_altered_message_maker *
            item_definition_altered_message_maker_;
        const i_size_definition_altered_message_factory *
            size_definition_altered_message_factory_;
        const i_size_definition_altered_message_maker *
            size_definition_altered_message_maker_;

        static kickstarter *instance_;

        static utilities::locking::i_spinlock *instance_initialization_lock_;
};

} /* item_management */

#endif /* __hh_item_management_booter__ */

