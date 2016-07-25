//---------------------------------------------------------------------------

#ifndef ProcessingControllerH
#define ProcessingControllerH
//---------------------------------------------------------------------------
#include <system.hpp>
#include <memory>
#include "Processing.h"

class TMMProcessingState
{
   public :
   TMMProcessingState(
      TForm *inOwner,
      AnsiString inMessage,
      AnsiString inTitle,      
      bool inCanCancel = false,
      bool inShowProgress = false,
      int inMaxTimeOut = 0,
      int inTop = 0,
      int inLeft = 0);

   TMMProcessingState();

   TForm *Owner;
   AnsiString Message;
   AnsiString Title;
   bool CanCancel;
   bool ShowProgress;
   int MaxTimeOut;
   int Top;
   int Left;
   friend bool operator==(const TMMProcessingState &f1, const TMMProcessingState &f2);
};

bool operator == (const TMMProcessingState &f1,const TMMProcessingState &f2)
{
	if (  (f1.Title == f2.Title) &&
         (f1.Message == f2.Message) &&
         (f1.CanCancel == f2.CanCancel) &&
         (f1.ShowProgress == f2.ShowProgress) &&
         (f1.MaxTimeOut == f2.MaxTimeOut) &&
			(f1.Top == f2.Top) &&
         (f1.Left == f2.Left)
      )
      {
         return true;
      }
   return false;
};

class TProcessingController
{
   std::auto_ptr<TfrmProcessing> frmProcessing;
   std::vector<TMMProcessingState> States;
   public :
   TProcessingController();
   void Push(TMMProcessingState &State);
   void PushOnce(TMMProcessingState &State);   
	void Pop();
	void PopAll();
   void Repaint();
   bool Cancelled();
   void ResetCancelled();
   void SetPosition(int Pos);
   TForm *GetTopOwner();
};
#endif
