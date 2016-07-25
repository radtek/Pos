#ifndef __hh_i_message_maker__
#define __hh_i_message_maker__

#include <SimpleTCP.h>

namespace item_management {

class i_signalling_item_manager;

namespace network_messaging {

template<typename message_factory_type>
class i_message_maker : public TNetMessageMaker {
public:
    virtual ~i_message_maker() { };

    virtual const message_factory_type &get_factory() const = 0;

    virtual i_signalling_item_manager &get_item_manager() const = 0;

protected:
    i_message_maker(int message_type) : TNetMessageMaker(message_type) { };

#ifdef __BCPLUSPLUS__
#       pragma warn -par
#elif __GNUC__ /* __BCPLUSPLUS__ */
#       pragma GCC diagnostic push
#       pragma GCC diagnostic ignored "-Wunused-parameter"
#endif /* __GNUC__ */

    i_message_maker(i_signalling_item_manager *item_manager, const message_factory_type *message_factory, int message_type)
        : TNetMessageMaker(message_type) { };

#ifdef __BCPLUSPLUS__
#       pragma warn .par
#elif __GNUC__ /* __BCPLUSPLUS__ */
#       pragma GCC diagnostic pop
#endif /* __GNUC__ */
};

} /* network_messaging */
} /* item_management */ 

#endif /* ! __hh_i_message_maker__ */


