#include <IdTCPServer.hpp>

#include "webmate_order_receiver.hh"

#define CONFIGURATION webmate_order_receiver_t::configuration_t

const int CONFIGURATION::default_max_active_connections = 64;
const int CONFIGURATION::default_max_waiting_connections =
  IdListenQueueDefault;
const unsigned short CONFIGURATION::default_port_to_listen_on = 65007;

CONFIGURATION::configuration_t(
  int max_active_connections,
  int max_waiting_connections,
  unsigned short port_to_listen_on)
  : max_active_connections_(max_active_connections),
    max_waiting_connections_(max_waiting_connections),
    port_to_listen_on_(port_to_listen_on)
{
}

int
CONFIGURATION::max_active_connections()
const
{
    return max_active_connections_;
}

int
CONFIGURATION::max_waiting_connections()
const
{
    return  max_waiting_connections_;
}

unsigned short
CONFIGURATION::port_to_listen_on()
const
{
    return port_to_listen_on_;
}

#undef CONFIGURATION

