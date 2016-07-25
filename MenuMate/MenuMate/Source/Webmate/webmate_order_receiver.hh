#ifndef hh_webmate_order_receiver
#define hh_webmate_order_receiver

#include <Classes.hpp>

#include <memory>

#include "i_disallow_assignment.hh"
#include "i_disallow_copy_construction.hh"

class webmate_order_receiver_t :
  public i_disallow_assignment_t,
  public i_disallow_copy_construction_t {
public:
    /* Specifies the configuration for the receiver instance. */
    class configuration_t {
    public:
        static const int default_max_active_connections;
        static const int default_max_waiting_connections;
        static const unsigned short default_port_to_listen_on;

        configuration_t(
          int max_active_connections = default_max_active_connections,
          int max_waiting_connections = default_max_waiting_connections,
          unsigned short port_to_listen_on = default_port_to_listen_on);

        int max_active_connections() const;
        int max_waiting_connections() const;
        unsigned short port_to_listen_on() const;

    private:
        int max_active_connections_;
        int max_waiting_connections_;
        unsigned short port_to_listen_on_;
    }; /* configuration_t */

    /* The receiver itself only buffers all incoming data. What's done with
     * that data and what should be sent back to the peer is up to the
     * receiver's "received_action."'
     *
     * Subclasses can access the received data by using the input_buffer
     * memory stream. Data written to the output_buffer memory stream will be
     * sent back to the peer by the receiver.
     *
     * Exceptions that escape the operator() method will be ignored by the
     * receiver.
     */
    class i_order_received_action_t
      : public i_disallow_assignment_t,
        public i_disallow_copy_construction_t {
    public:
        virtual ~i_order_received_action_t() { };

        virtual void operator()(TMemoryStream &input_buffer,
                                TMemoryStream &output_buffer) = 0;

    protected:
        i_order_received_action_t() { };
    }; /* i_order_received_action_t */

    webmate_order_receiver_t(
      i_order_received_action_t *received_action,
      configuration_t configuration = configuration_t());
    ~webmate_order_receiver_t();

    void disable_receiver();
    void enable_receiver();

    bool is_receiver_enabled() const;

private:
    class impl_t;

    std::auto_ptr<impl_t> impl_;
}; /* webmate_order_receiver_t */

#endif /* ! hh_webmate_order_receiver */
