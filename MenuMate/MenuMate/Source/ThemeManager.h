//---------------------------------------------------------------------------

#ifndef ThemeManagerH
#define ThemeManagerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <map>

#define LOADFROMNAME                      _T("Theme.xml")
#define SAVETONAME                        _T("SourceTheme.xml")
#define XMLTITLE                          _T("MenuMateTheme")
#define XMLINDEX                          _T("Index")
#define XMLTHEMES                         _T("Themes")
#define BASECLASSNAME                     _T("Base_Name")
#define UNKNOWNBASECLASS                  _T("Base_Unknown")
#define CONTROLIDENT                      _T("Control_Ident")
#define CONTROLTHEME                      _T("Control_Theme")
#define BUTTONTHEME                       _T("Button_Theme")

#define ATTRIB_COLOR                      _T("Color")
#define ATTRIB_BUTTONCOLOR                _T("ButtonColor")
#define ATTRIB_LATCHEDCOLOR               _T("LatchedColor")
#define ATTRIB_LATCHEDFONTCOLOR           _T("LatchedFontColor")
#define ATTRIB_DEFAULTBUTTONCOLOR         _T("DefaultButtonColor")
#define ATTRIB_DEFAULTBUTTONLATCHEDCOLOR  _T("DefaultButtonLatchedColor")
#define ATTRIB_ARROWBUTTONCOLOR           _T("ArrowButtonColor")
#define ATTRIB_GRIDCOLOR                  _T("GridColor")
#define ATTRIB_FONTCOLOR                  _T("Font_Color")
#define ATTRIB_FONTNAME                   _T("Font_Name")
#define ATTRIB_FONTSIZE                   _T("Font_Size")

#define BUTTONTYPE_UNSELECTED             _T("Unselected")
#define BUTTONTYPE_SELECTED               _T("Selected")
#define BUTTONTYPE_EMPTY                  _T("Empty")
#define BUTTONTYPE_FULL                   _T("Full")
#define BUTTONTYPE_LOCKED                 _T("Locked")
#define BUTTONTYPE_SEC_SELECTED           _T("Sec_Selected")
#define BUTTONTYPE_SEC_UNSELECTED         _T("Sec_Unselected")
#define ATTRIB_BUTTONCOLOR_CANCELLED      _T("Cancelled")

//const TColor CL_BUTTONCOLOUR_UNSELECTED						= clMaroon;

//Dary Grey
const TColor CL_BUTTONCOLOUR_STD						= RGB(92,93,96);
//Light Grey $00979492
const TColor CL_BUTTONCOLOUR_SCROLLBTNS					= RGB(169,171,174);
//Arrow Black
const TColor CL_BUTTONCOLOUR_ARROWS						= clBlack;
// Very Light Grey
//const TColor CL_BUTTONCOLOUR_UNSELECTED				= RGB(187,189,191);
const TColor CL_BUTTONCOLOUR_UNSELECTED					= CL_BUTTONCOLOUR_SCROLLBTNS;


const TColor CL_BUTTONCOLOUR_UNSELECTED_FONT_COLOR 		= clWhite;

const TColor CL_BUTTONCOLOUR_EMPTY_SEAT					= CL_BUTTONCOLOUR_UNSELECTED;
const TColor CL_BUTTONCOLOUR_EMPTY_SEAT_FONT_COLOR 		= clBlack;

// Button Orange.
const TColor CL_BUTTONCOLOUR_FULL_SEAT					= RGB(245,152,0);
// Button Orange.
const TColor CL_BUTTONCOLOUR_CANCEL						= CL_BUTTONCOLOUR_FULL_SEAT;

// Back Ground
const TColor CL_GRIDCOLOUR								= RGB(218,218,218);
const TColor CL_NUMPADCOLOUR							= CL_BUTTONCOLOUR_FULL_SEAT;

const TColor CL_BUTTONCOLOUR_FULL_SEAT_FONT_COLOR 		= clWhite;

const TColor CL_BUTTONCOLOUR_LOCKED_SEAT				= clRed;
const TColor CL_BUTTONCOLOUR_LOCKED_SEAT_FONT_COLOR		= clWhite;

const TColor CL_BUTTONCOLOUR_SELECTED_SEAT				= clNavy;
const TColor CL_BUTTONCOLOUR_SELECTED_SEAT_FONT_COLOR 	= clWhite;

const TColor CL_BUTTONCOLOUR_CANCELLED 					= clRed;

const TColor CL_BUTTONCOLOUR_SELECTED_SECONDARY_FUNC	= clMoneyGreen;
const TColor CL_BUTTONCOLOUR_UNSELECTED_SECONDARY_FUNC	= clInfoBk;

const TColor CL_BUTTONCOLOUR_SELECTED_SECONDARY_FUNC_FONT_COLOR   = clBlack;
const TColor CL_BUTTONCOLOUR_UNSELECTED_SECONDARY_FUNC_FONT_COLOR = clBlack;
/*
class TManagerTheme
{
	private:
	  void GenTheme(TComponent *Component,TiXmlHandle &doc);
	  UnicodeString GetControlIdent(TComponent *Control);
	  UnicodeString GetControlBaseClass(TComponent *Control);
	  bool IsThemeMatch(TiXmlElement* pThemeProperties,TComponent *Component);
	  TiXmlElement* ComponentToElement(TComponent *Component);
	  bool IntCompare(int Source ,TiXmlElement* pElement, UnicodeString AttribName );
	  bool ColorCompare(int Source ,TiXmlElement* pElement, UnicodeString AttribName );
	  bool IntAssign(TiXmlElement *pElement,UnicodeString Attrib,int &Value);
//	  void StrAssign(TiXmlElement *pElement,UnicodeString &Value,UnicodeString Attrib);
//	  void StrAssign(TiXmlElement *pElement,UnicodeString &Value,UnicodeString Attrib);
//	  void StrAssign(TiXmlElement *pElement,UnicodeString &Value,UnicodeString Attrib);
	  void StrAssign(TiXmlElement *pElement,UnicodeString &Value,UnicodeString Attrib);
	  bool HexAssign(TiXmlElement *pElement,UnicodeString Attrib,int &Value);
	  bool HexToInt(TiXmlElement *pElement,UnicodeString Attrib,int &Value);
	  UnicodeString AsHex(int Vaule);
	  void ElementToComponent(TComponent *Component,TiXmlElement * Element);
	  void AssignTheme(TiXmlHandle &doc,TComponent *Component);
	public :
		void ProcessControl(TComponent *Control);
	  void GenerateTheme(TComponent *Component);
	  void LoadButtonTheme(std::map< UnicodeString, std::map<UnicodeString,TColor> > &ButtonColors);
};     */

#endif



