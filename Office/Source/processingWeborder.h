#ifndef processingWeborderH
#define processingWeborderH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ScktComp.hpp>
#include <Sockets.hpp>
#include <ExtCtrls.hpp>

#include <memory>

#include "web_order.h"

using std::auto_ptr;

class TfrmProcessingWeborder : public TForm
{
__published:
    TLabel       *lblStatus;
    TProgressBar *pbTxProgress;
    TTcpClient   *socket;
    TTimer *tmrTrigger;
    void __fastcall socketError(TObject *Sender, int SocketError);
    void __fastcall tmrTriggerTimer(TObject *Sender);
private:
    enum processing_state {
        completed = 0,
        failed,
        connecting,
        sending,
        waiting_for_response,
        checking_response
    };

    bool submit_order();

    TModalResult result;
    bool shutdown;
    unsigned int state;

    auto_ptr<web_order> weborder;

    static const unsigned int process_marker[];
    static const string state_string[];
public:
    __fastcall TfrmProcessingWeborder(TComponent* Owner,
                                      AnsiString host, AnsiString port,
                                      auto_ptr<web_order> weborder);
};

extern PACKAGE TfrmProcessingWeborder *frmProcessingWeborder;

#endif
