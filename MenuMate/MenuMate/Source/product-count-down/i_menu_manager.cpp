#include <i_menu_manager.hh>

namespace item_management {

#ifdef __BCPLUSPLUS__
#       pragma warn -par
#elif __GNUC__ /* __BCPLUSPLUS__ */
#       pragma GCC diagnostic push
#       pragma GCC diagnostic ignored "-Wunused-parameter"
#endif /* __GNUC__ */

i_menu_manager::i_menu_manager(i_item_manager *item_manager)
{
}

#ifdef __BCPLUSPLUS__
#       pragma warn .par
#elif __GNUC__ /* __BCPLUSPLUS__ */
#       pragma GCC diagnostic pop
#endif /* __GNUC__ */

i_menu_manager::~i_menu_manager()
{
}

} /* item_management */

