#ifndef __hh_i_message__
#define __hh_i_message__

#include <SimpleTCP.h>

namespace item_management {

class i_signalling_item_manager;

namespace network_messaging {

class i_message : public TNetMessage {
public:
        virtual ~i_message() { };
        
        virtual i_signalling_item_manager &get_item_manager() const = 0;

protected:
#ifdef __BCPLUSPLUS__
#       pragma warn -par
#elif __GNUC__ /* __BCPLUSPLUS__ */
#       pragma GCC diagnostic push
#       pragma GCC diagnostic ignored "-Wunused-parameter"
#endif /* __GNUC__ */

        i_message(udpPackets message_type) : TNetMessage(message_type) { };
        i_message(
            i_signalling_item_manager &item_manager,
            int message_type);

#ifdef __BCPLUSPLUS__
#       pragma warn .par
#elif __GNUC__ /* __BCPLUSPLUS__ */
#       pragma GCC diagnostic pop
#endif /* __GNUC__ */
};

} /* network_messaging */
} /* item_management */

#endif /* ! __hh_i_message__ */

