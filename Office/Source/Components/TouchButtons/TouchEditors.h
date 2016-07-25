//---------------------------------------------------------------------------

#ifndef TouchEditorsH
#define TouchEditorsH
//---------------------------------------------------------------------------
//#include <DesignEditors.hpp>	// Pascal unit
#include <VCLEditors.hpp>	// Pascal unit
//---------------------------------------------------------------------------
class PACKAGE TTouchGridEditor : public Designeditors::TComponentEditor
{
public:
	inline __fastcall virtual TTouchGridEditor(Classes::TComponent* AComponent,
			Designintf::_di_IDesigner ADesigner) : Designeditors::TComponentEditor(AComponent, ADesigner) { }

	inline __fastcall virtual ~TTouchGridEditor() { }

	void __fastcall					Edit();
	virtual void __fastcall			ExecuteVerb(int Index);
	virtual AnsiString __fastcall	GetVerb(int Index);
	virtual int __fastcall			GetVerbCount();
};
//---------------------------------------------------------------------------
//class PACKAGE TTouchPagesEditor : public Designeditors::TComponentEditor
class PACKAGE TTouchPagesEditor : public Designeditors::TDefaultEditor
{
public:
	inline __fastcall virtual TTouchPagesEditor(Classes::TComponent* AComponent,
			Designintf::_di_IDesigner ADesigner) : Designeditors::TDefaultEditor(AComponent, ADesigner) { }

	inline __fastcall virtual ~TTouchPagesEditor() { }

	virtual void __fastcall			ExecuteVerb(int Index);
	virtual AnsiString __fastcall	GetVerb(int Index);
	virtual int __fastcall			GetVerbCount();
};
//---------------------------------------------------------------------------
#endif
