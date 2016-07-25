//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ReportDisplay.h"
#include <mshtml.h>
#include <oleacc.h>
#include "SHDocVw_OCX.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SHDocVw_OCX"
#pragma link "touchbtn"
#pragma link "touchcontrols"
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmReportDisplay::TfrmReportDisplay(TComponent* Owner)
	: TZForm(Owner)
{
	Source = NULL;
	OleVariant url = "about:blank";
   webDisplay->Navigate2( url );
}

//---------------------------------------------------------------------------

void __fastcall TfrmReportDisplay::FormShow(TObject *Sender)
{
	FormResize(this);
   Top = 0;
   Left = 0;
   LoadHtml();
}

//---------------------------------------------------------------------------

void __fastcall TfrmReportDisplay::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;
                if((double)Screen->Width / Screen->Height < 1.4)
                {
			ScaleBy(Screen->Width, Temp);
                }
	}
   this->Width = Screen->Width;
   this->Height = Screen->Height;
}
//---------------------------------------------------------------------------


void __fastcall TfrmReportDisplay::TouchBtn1MouseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------


void TfrmReportDisplay::LoadHtml()
{
   if(Source != NULL)
   {
      std::auto_ptr<TMemoryStream> StreamRpt(new TMemoryStream);
      Source->SaveToStream(StreamRpt.get());
      IPersistStreamInit *psi;
      StreamRpt->Seek(0,0);
      TStreamAdapter *sa = new TStreamAdapter(StreamRpt.get(),soReference);
      _di_IDispatch doc = webDisplay->Document;
      if ( doc == NULL )
      {
         OleVariant url = "about:blank";
         webDisplay->Navigate2( url );
      }

      while( webDisplay->ReadyState != Shdocvw::READYSTATE_COMPLETE )
      {
         Application->ProcessMessages();
      }
      doc = webDisplay->Document;


      TComInterface<IHTMLDocument2> HTMLDocument;
      TComInterface<IHTMLWindow2> parentWindow;

      if( SUCCEEDED(webDisplay->Document->QueryInterface(IID_IHTMLDocument2,(LPVOID*)&HTMLDocument)) )
      {
         IHTMLElement* pBodyElem = 0;
         HRESULT hr = HTMLDocument->get_body(&pBodyElem);
         if (SUCCEEDED(hr))
         {
            IHTMLBodyElement* pBody = 0;
            hr = pBodyElem->QueryInterface(IID_IHTMLBodyElement, (void**)&pBody);
            if (SUCCEEDED(hr))
            {
               // hide 3D border
               IHTMLStyle* pStyle;
               hr = pBodyElem->get_style(&pStyle);
               if (SUCCEEDED(hr))
               {
                  pStyle->put_borderStyle(BSTR("none"));
                  pStyle->Release();
               }
               // hide scrollbars
               pBody->put_scroll(BSTR("no"));
            }
            pBodyElem->Release();
         }
         pBodyElem->Release();
      }

      if ( doc->QueryInterface(IID_IPersistStreamInit, (void**)&psi ) == S_OK )
      {
         if ( psi )
            psi->Load(*sa);
      }
   }
}

void TfrmReportDisplay::Navigate(TStringList *Html)
{
	Source = Html;
   if(this->Visible)
   {
		LoadHtml();
   }
}

void __fastcall TfrmReportDisplay::tbtnPageUpMouseClick(TObject *Sender)
{
	TComInterface<IHTMLDocument2> HTMLDocument;
	TComInterface<IHTMLWindow2> parentWindow;

	if( SUCCEEDED(webDisplay->Document->QueryInterface(IID_IHTMLDocument2,(LPVOID*)&HTMLDocument)) )
	{
		if( SUCCEEDED(HTMLDocument->get_parentWindow(&parentWindow)))
		{
			parentWindow->scrollBy( 0, -400 );
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmReportDisplay::tbtnPageDownMouseClick(TObject *Sender)
{
	TComInterface<IHTMLDocument2> HTMLDocument;
	TComInterface<IHTMLWindow2> parentWindow;

	if( SUCCEEDED(webDisplay->Document->QueryInterface(IID_IHTMLDocument2,(LPVOID*)&HTMLDocument)) )
	{
		if( SUCCEEDED(HTMLDocument->get_parentWindow(&parentWindow)))
		{
			parentWindow->scrollBy( 0, 400 );
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmReportDisplay::tbtnLineUpMouseClick(TObject *Sender)
{
	TComInterface<IHTMLDocument2> HTMLDocument;
	TComInterface<IHTMLWindow2> parentWindow;

	if( SUCCEEDED(webDisplay->Document->QueryInterface(IID_IHTMLDocument2,(LPVOID*)&HTMLDocument)) )
	{
		if( SUCCEEDED(HTMLDocument->get_parentWindow(&parentWindow)))
		{
			parentWindow->scrollBy( 0, -100 );
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmReportDisplay::tbtnLineDownMouseClick(TObject *Sender)
{
	TComInterface<IHTMLDocument2> HTMLDocument;
	TComInterface<IHTMLWindow2> parentWindow;

	if( SUCCEEDED(webDisplay->Document->QueryInterface(IID_IHTMLDocument2,(LPVOID*)&HTMLDocument)) )
	{
		if( SUCCEEDED(HTMLDocument->get_parentWindow(&parentWindow)))
		{
			parentWindow->scrollBy( 0, 100 );
		}
	}
}
//---------------------------------------------------------------------------

void TfrmReportDisplay::NavigateToURL(UnicodeString Url)
{
	webDisplay->Navigate(Url.w_str());
}

