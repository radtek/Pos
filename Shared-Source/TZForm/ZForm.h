//---------------------------------------------------------------------------
#ifndef ZFormH
#define ZFormH
//---------------------------------------------------------------------------
#include <forms.hpp>
#include "MM_DBCore.h"
#include "TouchGrid.h"
#include "TouchNumpad.h"
#include "ThemeManager.h"
//---------------------------------------------------------------------------
// This class may fix the Z-order issue of modal windows appearing behind
// their owner.
//
// See usage example below.
//---------------------------------------------------------------------------
// Use this function to create a new form.
//	TForm2 *form = TZForm::Create<TForm2>(this);
//	form->ShowModal();
// delete form;
// or
// std::auto_ptr<TForm2>(form)(TForm2::Create<TForm2>(this));
//	form->ShowModal();
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// This function can be called prior to Application->Initialize();
// in WinMain. Untested and not necessarily required if using the above.
//---------------------------------------------------------------------------
// void DisableProcessWindowsGhosting()
// {
// 	typedef int (WINAPI *TDisableProcessWindowsGhostingImp)();
// 	TDisableProcessWindowsGhostingImp DisableProcessWindowsGhostingImp = NULL;
// 	DisableProcessWindowsGhostingImp = GetProcAddress(GetModuleHandle("user32.dll"), "DisableProcessWindowsGhosting");
// 	if (DisableProcessWindowsGhostingImp)
// 	{
// 		DisableProcessWindowsGhostingImp();
// 	}
//	}
//---------------------------------------------------------------------------
#include <Dialogs.hpp>
class TZForm : public TForm
{
private:
	static TForm *WinOwner;
	static void SetOwner(TForm *form);
	virtual void __fastcall TZForm::CreateParams(Controls::TCreateParams &params);
protected:
__fastcall TZForm(TComponent* Owner);
public:
   std::map< UnicodeString, std::map<UnicodeString,TColor> > ButtonColors;
	template <class FormType> static FormType *Create(TForm *LowerWindow);
	template <class FormType> static FormType *Create(TForm *LowerWindow,Database::TDBControl &inDBControl);
	template <class FormType> static FormType *Create(TForm *LowerWindow,Database::TDBTransaction &inDBTransaction);
	void SetGridColors(TTouchGrid *Grid);
	void SetNumpadColors(TTouchNumpad *Pad);
};
//---------------------------------------------------------------------------
// This is not thread safe! (Although you shouldn't be creating forms in
// other threads anyway)
//---------------------------------------------------------------------------
template <class FormType> FormType *TZForm::Create(TForm *Owner)
{
	WinOwner = Owner;
	return new FormType(Owner);
}
//---------------------------------------------------------------------------
template <class FormType> FormType *TZForm::Create(TForm *Owner,Database::TDBControl &inDBControl)
{
	WinOwner = Owner;
	return new FormType(Owner,inDBControl);
}
//---------------------------------------------------------------------------

template <class FormType> FormType *TZForm::Create(TForm *Owner,Database::TDBTransaction &inDBTransaction)
{
	WinOwner = Owner;
	return new FormType(Owner,inDBTransaction);
}
//---------------------------------------------------------------------------


#endif

