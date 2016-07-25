#include "webmate_order_receiver_impl.hh"

#define IMPL webmate_order_receiver_t::impl_t

IMPL::impl_t(
  i_order_received_action_t *received_action,
  configuration_t configuration)
  : configuration_(configuration),
    tcp_server_(new TIdTCPServer(NULL)),
    received_action_(received_action),
    receiver_is_enabled_(false)
{
    initialize_tcp_server();
}

/* Why isn't this being called? Embarcadero? Answer meeee! */
IMPL::~impl_t()
{
    tcp_server_->Active = false;
}

void
IMPL::disable_receiver()
{
    tcp_server_->Active = false;
}

void
IMPL::enable_receiver()
{
    tcp_server_->Active = true;
}

bool
IMPL::is_receiver_enabled()
const
{
    return tcp_server_->Active;
}

void
IMPL::initialize_tcp_server()
{
    tcp_server_->DefaultPort = configuration_.port_to_listen_on();
    tcp_server_->ReuseSocket = false;

    tcp_server_->ListenQueue = configuration_.max_waiting_connections();
    tcp_server_->MaxConnections = configuration_.max_active_connections();

    tcp_server_->OnExecute = talk_with_client;

    tcp_server_->Active = false;
}

void __fastcall
IMPL::talk_with_client(
  TIdContext *connection_context)
{
    TIdTCPConnection &connection = *connection_context->Connection;
    TIdIOHandler &io_handler = *connection.IOHandler;
    std::auto_ptr<TMemoryStream> input_buffer(new TMemoryStream());
    std::auto_ptr<TMemoryStream> output_buffer(new TMemoryStream());

    while (1) {
        if (io_handler.InputBufferIsEmpty())
            io_handler.CheckForDataOnSource();
        if (io_handler.InputBufferIsEmpty())
            break;
        io_handler.InputBufferToStream(input_buffer.get());
    }

    /* We can't be sure that the handler won't do something stupid and kill
     * this thread...
     */
    try {
        (*received_action_)(*input_buffer, *output_buffer);
    } catch (...) {
        throw;
    }

    if (connection.Connected())
        io_handler.Write(output_buffer.get());

    connection.Disconnect(true);
}

