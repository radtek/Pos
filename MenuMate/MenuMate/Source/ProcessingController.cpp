//---------------------------------------------------------------------------


#pragma hdrstop

#include "ProcessingController.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TMMProcessingState::TMMProcessingState(TForm *inOwner,AnsiString inMessage,AnsiString inTitle,bool inCanCancel,bool inShowProgress,int inMaxTimeOut,int inTop, int inLeft)
{
   Owner = inOwner;
   Message = inMessage;
   Title = inTitle;
   CanCancel = inCanCancel;
   ShowProgress = inShowProgress;
   MaxTimeOut = inMaxTimeOut;
   Left = inLeft;
   Top = inTop;
}

TMMProcessingState::TMMProcessingState()
{
   Owner = NULL;
   Message = "";
   Title = "";
   CanCancel = true;
   ShowProgress = false;
   MaxTimeOut = 0;
   Left = 0;
   Top = 0;
}

TProcessingController::TProcessingController() : frmProcessing(NULL)
{
}

void TProcessingController::Push(TMMProcessingState &State)
{
   States.push_back(State);
   if(frmProcessing.get() != NULL && frmProcessing.get() == State.Owner)
   {
      State.Owner = (TForm *)frmProcessing->Owner;
   }
   frmProcessing.reset(TfrmProcessing::Create<TfrmProcessing>(State.Owner));
   frmProcessing->CanCancel = State.CanCancel;
   frmProcessing->Title = State.Title;
   frmProcessing->Message = State.Message;
   frmProcessing->ShowProgress = State.ShowProgress;
   frmProcessing->Top = State.Top;
   frmProcessing->Left = State.Left;
   frmProcessing->Max = State.MaxTimeOut;
   frmProcessing->Show();
}

void TProcessingController::PushOnce(TMMProcessingState &State)
{
   if(std::find(States.begin(),States.end(),State) == States.end())
   {
      Push(State);
   }
}

void TProcessingController::PopAll()
{
	if(!States.empty())
	{
		while (!States.empty())
		{
			States.pop_back();
		}
		if(States.empty())
		{
			if(frmProcessing.get() != NULL)
			{
				frmProcessing->Close();
				frmProcessing.reset(NULL);
			}
		}
	}
}

void TProcessingController::Pop()
{
	if(!States.empty())
	{
		States.pop_back();
		if(States.empty())
		{
			if(frmProcessing.get() != NULL)
			{
				frmProcessing->Close();
				frmProcessing.reset(NULL);
			}
		}
		else
		{
			TMMProcessingState State = States.back();
         frmProcessing.reset(TfrmProcessing::Create<TfrmProcessing>(State.Owner));
         frmProcessing->CanCancel = State.CanCancel;
         frmProcessing->Title = State.Title;
         frmProcessing->Message = State.Message;
         frmProcessing->ShowProgress = State.ShowProgress;
         frmProcessing->Top = State.Top;
         frmProcessing->Left = State.Left;
         frmProcessing->Max = State.MaxTimeOut;
         frmProcessing->Show();
      }
   }
}

void TProcessingController::Repaint()
{
   if(frmProcessing.get() != NULL)
	{
      frmProcessing->Repaint();
   }
}

bool TProcessingController::Cancelled()
{
   bool RetVal = false;
   if(frmProcessing.get() != NULL)
   {
      RetVal = frmProcessing->Cancelled;
   }
   return RetVal;
}

void TProcessingController::ResetCancelled()
{
   if(frmProcessing.get() != NULL)
   {
      frmProcessing->Cancelled = false;
   }
}

void TProcessingController::SetPosition(int Pos)
{
   if(frmProcessing.get() != NULL)
   {
      frmProcessing->Position = Pos;
   }
}


TForm *TProcessingController::GetTopOwner()
{
   if(States.empty())
   {
      Screen->ActiveForm;
   }
   else
   {
      return States.back().Owner;
   }
}

