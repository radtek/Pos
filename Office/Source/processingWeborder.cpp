#include <vcl.h>
#pragma hdrstop

#include "processingWeborder.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmProcessingWeborder *frmProcessingWeborder;

class weborder_exception {
protected:
    typedef bool (weborder_exception::*prompt_func)();

    string error_message;
    bool   unrecoverable;
public:
    weborder_exception() : unrecoverable(false) { }
    virtual ~weborder_exception() { }

    virtual void handle() { };
    virtual bool prompt() = 0;
private:
    weborder_exception &operator=(const weborder_exception &);

};

class weborder_response_exception : public weborder_exception {
public:
    weborder_response_exception(string error_message)
    {
        this->error_message = error_message;
    }
    ~weborder_response_exception() { }

    bool prompt();
};

class weborder_socket_exception : public weborder_exception {
public:
    weborder_socket_exception(unsigned int socket_error_code)
      : socket_error_code(socket_error_code) { }
    ~weborder_socket_exception() { }

    void handle();
    bool prompt();
private:
    unsigned int socket_error_code;
};

const string TfrmProcessingWeborder::state_string[] = {
    "Order sent successfully.",
    "The order couldn't be sent successfully.",
    "Connecting to web-order service...",
    "Sending web-order...",
    "Waiting for service response...",
    "Checking server response..."
};

const unsigned int TfrmProcessingWeborder::process_marker[] = {
    100,
    0,
    0,
    25,
    50,
    75
};

bool
weborder_response_exception::prompt()
{
    error_message = error_message + "\r\rDo you want to retry?";

    return Application->MessageBox(error_message.c_str(), "Error",
                                   MB_ICONERROR | MB_YESNO) == IDYES;
}

void
weborder_socket_exception::handle()
{
    switch (socket_error_code) {
    case WSAENETDOWN:
    case WSAENETUNREACH:
        error_message =
          "Windows reports that the network is down; "
            "Try rebooting your computer, re-enabling "
            "your network interface or calling your "
            "administrator.";
        unrecoverable = true;
        break;
    case WSAENETRESET:
    case WSAECONNRESET:
    case WSAECONNREFUSED:
    case WSAETIMEDOUT:
        error_message =
          "We couldn't contact the web-order service.\r\rIt could be that the "
            "service took too long to respond to our request or that the "
            "service isn't running on the target server.\r\rWould you like "
            "to try again?";
        break;
    default:
        error_message =
          "We've encountered a networking error that "
            "we don't know how to recover from. Please "
            "attempt your order later.";
        unrecoverable = true;
        break;
    }
}

bool
weborder_socket_exception::prompt()
{
    if (unrecoverable) {
        Application->MessageBox(error_message.c_str(), "Error",
                                MB_ICONERROR | MB_OK);
        return false;
    }
    return Application->MessageBox(error_message.c_str(), "Error",
                                   MB_ICONERROR | MB_YESNO) == IDYES;
}

__fastcall TfrmProcessingWeborder::TfrmProcessingWeborder(
  TComponent* Owner,
  AnsiString host,
  AnsiString port,
  auto_ptr<web_order> weborder)
  : TForm(Owner), shutdown(false)
{
    socket->RemotePort = port;
    socket->RemoteHost = host;

    this->weborder = weborder; 
}

#define set_and_paint_label(_caption) do { \
    lblStatus->Caption = (_caption); \
    SendMessage(Handle, WM_PAINT, 0, 0); \
} while (0)

#define set_state(_new_state) do { \
    pbTxProgress->Position = \
      process_marker[state = (_new_state)]; \
    set_and_paint_label(state_string[state].c_str()); \
} while (0)

bool
TfrmProcessingWeborder::submit_order()
{
    union un {
        const char *cch;
        char       *ch;
        void       *raw;
    };

    string err_str;
    string req_str;
    processing_state state = connecting;
    union un inbuf;
    union un outbuf;

    inbuf.raw = calloc(1, 512);

    while (state >= connecting) {
        try {
            req_str = weborder->serialize();

            outbuf.cch = req_str.c_str();

            set_state(connecting);
            socket->Connect();

            set_state(sending);
            socket->SendBuf(outbuf.raw, req_str.size());

            set_state(waiting_for_response);
            if (!socket->WaitForData(32000))
                socketError(this, WSAETIMEDOUT);
            socket->ReceiveBuf(inbuf.raw, 512);

            set_state(checking_response);
            if (!weborder->check_order_response_valid(inbuf.cch, err_str))
                throw weborder_response_exception(err_str);

            set_state(completed);
        } catch (weborder_exception &e) {
            socket->Close();
            set_state(failed);
            e.handle();
            state = e.prompt() ? connecting : failed;
        }
    }

    free(inbuf.raw);

    return !state;
}

#undef set_state

void __fastcall TfrmProcessingWeborder::socketError(
  TObject *Sender, int SocketError)
{
    throw weborder_socket_exception(SocketError);
}

void __fastcall TfrmProcessingWeborder::tmrTriggerTimer(TObject *Sender)
{
    tmrTrigger->Enabled = false;
    ModalResult = 1 + submit_order();
}


