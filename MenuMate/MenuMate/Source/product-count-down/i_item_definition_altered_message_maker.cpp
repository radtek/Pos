#include <i_item_definition_altered_message_maker.hh>

namespace item_management {
namespace network_messaging {

i_item_definition_altered_message_maker
    ::i_item_definition_altered_message_maker()
        : i_message_maker<i_item_definition_altered_message_factory>(
              udp_item_definition_altered)
{
}

#ifdef __BCPLUSPLUS__
#       pragma warn -par
#elif __GNUC__ /* __BCPLUSPLUS__ */
#       pragma GCC diagnostic push
#       pragma GCC diagnostic ignored "-Wunused-parameter"
#endif /* __GNUC__ */

i_item_definition_altered_message_maker
    ::i_item_definition_altered_message_maker(
        i_signalling_item_manager *item_manager,
        const i_item_definition_altered_message_factory *message_factory)
            : i_message_maker<i_item_definition_altered_message_factory>(
                  udp_item_definition_altered)
{
}

#ifdef __BCPLUSPLUS__
#       pragma warn .par
#elif __GNUC__ /* __BCPLUSPLUS__ */
#       pragma GCC diagnostic pop
#endif /* __GNUC__ */

i_item_definition_altered_message_maker
    ::~i_item_definition_altered_message_maker()
{
}

} /* network_messaging */
} /* item_management */

