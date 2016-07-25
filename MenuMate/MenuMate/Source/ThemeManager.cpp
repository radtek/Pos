//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ThemeManager.h"
#include "TouchBtn.h"
#include "TouchGrid.h"
#include "TouchNumpad.h"
#include <SysUtils.hpp>

//---------------------------------------------------------------------------

#pragma package(smart_init)
/*
void TManagerTheme::ProcessControl(TComponent *Control)
{
   try
   {
      if(FileExists(UnicodeString(ExtractFilePath(Application->ExeName) + ExtractFileName(Application->ExeName)+ "." + LOADFROMNAME)))
      {
		 TiXmlDocument doc( UnicodeString(ExtractFilePath(Application->ExeName) + ExtractFileName(Application->ExeName)+ "." + LOADFROMNAME).t_str());
         if (doc.LoadFile())
         {
            TiXmlHandle hDoc(&doc);
            AssignTheme(hDoc,Control);
            for (int i = 0; i < Control->ComponentCount; i++)
            {
               TComponent *CurrentComponent = Control->Components[i];
               AssignTheme(hDoc,CurrentComponent);
            }
         }
      }
   }
   catch(Exception &E)
   {
      ShowMessage(E.Message);
   }
}

void TManagerTheme::LoadButtonTheme(std::map< UnicodeString, std::map<UnicodeString,TColor> > &ButtonColors)
{
   try
   {
	  if(FileExists(UnicodeString(ExtractFilePath(Application->ExeName) + ExtractFileName(Application->ExeName)+ "." + LOADFROMNAME)))
	  {
		 TiXmlDocument doc( UnicodeString(ExtractFilePath(Application->ExeName) + ExtractFileName(Application->ExeName)+ "." + LOADFROMNAME).t_str());
         if (doc.LoadFile())
         {
            TiXmlHandle hDoc(&doc);
            TiXmlElement* pElement;
            TiXmlHandle pRoot(0);
            TiXmlHandle pButtonsThemes(0);
            pElement = doc.FirstChildElement();
            if (pElement)
            {
               pRoot = TiXmlHandle(pElement);
			   pButtonsThemes = pRoot.FirstChild(BUTTONTHEME).Element();

			   for(TiXmlNode* pButtonNode = pButtonsThemes.FirstChild().ToNode();
                   pButtonNode != NULL;
                   pButtonNode = pButtonNode->NextSibling())
               {
                  TiXmlElement* pButtonTheme = pButtonNode->ToElement();
                  UnicodeString ButtonName = pButtonTheme->Value();
                  for(TiXmlAttribute* Attrib = pButtonTheme->FirstAttribute();
                      Attrib != NULL;
                      Attrib = Attrib->Next())
                  {
                     int Value = 0;
                     UnicodeString PropertyName = Attrib->Name();
                     if(HexAssign(pButtonTheme,PropertyName,Value))
                     {
                        ButtonColors[ButtonName][PropertyName] = Value;
                     }
                  }
               }
            }
         }
      }
   }
   catch(Exception &E)
   {
      ShowMessage(E.Message);
   }
}

void TManagerTheme::AssignTheme(TiXmlHandle &doc,TComponent *Component)
{
   TiXmlElement* pElement;
   TiXmlHandle pRoot(0);
   TiXmlHandle pThemeIndex(0);
   TiXmlHandle pControlThemes(0);
   pElement = doc.FirstChildElement().Element();
   if (pElement)
   {
      pRoot = TiXmlHandle(pElement);
      pThemeIndex = pRoot.FirstChild(XMLINDEX).Element();
      pControlThemes = pRoot.FirstChild(XMLTHEMES).Element();

	  UnicodeString ControlIdent = GetControlIdent(Component);
      TiXmlElement* pControlIndexNode = pThemeIndex.FirstChild( ControlIdent.t_str() ).Element();
      if(pControlIndexNode)
      { // Index found for this item.
         ControlIdent = pControlIndexNode->Attribute(CONTROLTHEME);
         TiXmlElement* pControlBaseClassNode = pControlThemes.FirstChild( GetControlBaseClass(Component).t_str() ).Element();
         if(pControlBaseClassNode)
         { // Index found for this item.
            TiXmlElement* pControlThemeNode = pControlBaseClassNode->FirstChild( ControlIdent.t_str())->ToElement();
            if(pControlThemeNode)
            { // Index found for this item.
               ElementToComponent(Component,pControlThemeNode);
            }
         }
      }
   }
}

void TManagerTheme::GenerateTheme(TComponent *Component)
{
   try
   {
      TiXmlDocument doc;
      if(FileExists(UnicodeString(ExtractFilePath(Application->ExeName) + ExtractFileName(Application->ExeName)+ "." + SAVETONAME)))
      {
         doc.LoadFile(UnicodeString(ExtractFilePath(Application->ExeName) + ExtractFileName(Application->ExeName)+ "." + SAVETONAME).t_str());
      }
      else
      {
		 TiXmlDeclaration * decl = new TiXmlDeclaration( _T("1.0"), _T(""), _T("") );
         doc.LinkEndChild( decl );

         TiXmlElement * Root = new TiXmlElement(XMLTITLE);
         doc.LinkEndChild( Root );

         TiXmlElement * CtrlThemeButtons = new TiXmlElement(BUTTONTHEME);
         Root->LinkEndChild( CtrlThemeButtons );

         TiXmlElement * ButtonTheme = new TiXmlElement(BUTTONTYPE_UNSELECTED);
         ButtonTheme->SetAttribute(ATTRIB_BUTTONCOLOR,   AsHex(CL_BUTTONCOLOUR_UNSELECTED).t_str());
         ButtonTheme->SetAttribute(ATTRIB_FONTCOLOR,     AsHex(CL_BUTTONCOLOUR_UNSELECTED_FONT_COLOR).t_str());
         CtrlThemeButtons->LinkEndChild( ButtonTheme );

         ButtonTheme = new TiXmlElement(BUTTONTYPE_EMPTY);
         ButtonTheme->SetAttribute(ATTRIB_BUTTONCOLOR,   AsHex(CL_BUTTONCOLOUR_EMPTY_SEAT).t_str());
         ButtonTheme->SetAttribute(ATTRIB_FONTCOLOR,     AsHex(CL_BUTTONCOLOUR_EMPTY_SEAT_FONT_COLOR).t_str());
         CtrlThemeButtons->LinkEndChild( ButtonTheme );

         ButtonTheme = new TiXmlElement(BUTTONTYPE_FULL);
         ButtonTheme->SetAttribute(ATTRIB_BUTTONCOLOR,   AsHex(CL_BUTTONCOLOUR_FULL_SEAT).t_str());
         ButtonTheme->SetAttribute(ATTRIB_FONTCOLOR,     AsHex(CL_BUTTONCOLOUR_FULL_SEAT_FONT_COLOR).t_str());
         CtrlThemeButtons->LinkEndChild( ButtonTheme );

         ButtonTheme = new TiXmlElement(BUTTONTYPE_LOCKED);
         ButtonTheme->SetAttribute(ATTRIB_BUTTONCOLOR,   AsHex(CL_BUTTONCOLOUR_LOCKED_SEAT).t_str());
         ButtonTheme->SetAttribute(ATTRIB_FONTCOLOR,     AsHex(CL_BUTTONCOLOUR_LOCKED_SEAT_FONT_COLOR).t_str());
         CtrlThemeButtons->LinkEndChild( ButtonTheme );

         ButtonTheme = new TiXmlElement(BUTTONTYPE_SELECTED);
         ButtonTheme->SetAttribute(ATTRIB_BUTTONCOLOR,   AsHex(CL_BUTTONCOLOUR_SELECTED_SEAT).t_str());
         ButtonTheme->SetAttribute(ATTRIB_FONTCOLOR,     AsHex(CL_BUTTONCOLOUR_SELECTED_SEAT_FONT_COLOR).t_str());
         CtrlThemeButtons->LinkEndChild( ButtonTheme );

         ButtonTheme = new TiXmlElement(BUTTONTYPE_SEC_SELECTED);
         ButtonTheme->SetAttribute(ATTRIB_BUTTONCOLOR,   AsHex(CL_BUTTONCOLOUR_SELECTED_SECONDARY_FUNC).t_str());
         ButtonTheme->SetAttribute(ATTRIB_FONTCOLOR,     AsHex(CL_BUTTONCOLOUR_SELECTED_SECONDARY_FUNC_FONT_COLOR).t_str());
         CtrlThemeButtons->LinkEndChild( ButtonTheme );

         ButtonTheme = new TiXmlElement(BUTTONTYPE_SEC_UNSELECTED);
         ButtonTheme->SetAttribute(ATTRIB_BUTTONCOLOR,   AsHex(CL_BUTTONCOLOUR_UNSELECTED_SECONDARY_FUNC).t_str());
         ButtonTheme->SetAttribute(ATTRIB_FONTCOLOR,     AsHex(CL_BUTTONCOLOUR_UNSELECTED_SECONDARY_FUNC_FONT_COLOR).t_str());
         CtrlThemeButtons->LinkEndChild( ButtonTheme );

         TiXmlElement * CtrlThemeIndex = new TiXmlElement(XMLINDEX);
         Root->LinkEndChild( CtrlThemeIndex );

         TiXmlElement * CtrlThemes = new TiXmlElement(XMLTHEMES);
         Root->LinkEndChild( CtrlThemes );
      }

      TiXmlHandle hDoc(&doc);
      GenTheme(Component,hDoc);
      for (int i = 0; i < Component->ComponentCount; i++)
      {
         TComponent *CurrentComponent = Component->Components[i];
         GenTheme(CurrentComponent,hDoc);
      }
      doc.SaveFile(UnicodeString(ExtractFileName(Application->ExeName) + "." + SAVETONAME).t_str());
   }
   catch(Exception &E)
   {
   }
}

void TManagerTheme::GenTheme(TComponent *Component,TiXmlHandle &doc)
{
   UnicodeString Theme = "";
   // Get the Base class for this object.
   UnicodeString BaseClass = GetControlBaseClass(Component);
   if(BaseClass != UNKNOWNBASECLASS)
   {
      // Checks though Themes for this class type and retrives a Matching one.
      // If no Theme then will add one.
      // Adds the Control and its Theme to the Style Sheet Index.


//       XML Style.
//      <Control Themes Index>
//         <Control>
//            <Ident = xxxx.yyyy.zzzz>
//            <Theme = xxxx.yyyy.zzzz>
//         </Control>
//         <Control>
//            <Ident = xxxx.yyyy.zzzz>
//            <Theme = xxxx.yyyy.zzzz>
//         </Control>
//      </Control Themes Index>
//
//      <Control Themes> // Static pControlThemes
//         <TForm>       // Class Name
//            <Ident>
//               <Colour = clblue>
//               <etc >
//            </Ident>
//         </TForm>
//         <TBtn>       // Class Name
//            <Ident>
//               <Colour = clblue>
//               <DownColour = clGreen>
//               <etc >
//            </Ident>
//         </TBtn>
//
//      </Control Themes>

      TiXmlElement* pElement;
      TiXmlHandle pRoot(0);
      TiXmlHandle pThemeIndex(0);
      TiXmlHandle pControlThemes(0);
      pElement = doc.FirstChildElement().Element();
      if (pElement)
      {
         pRoot = TiXmlHandle(pElement);
         // Save off the Curernt Root Handle.
         pThemeIndex = pRoot.FirstChild(XMLINDEX).Element();
         // Save off the Curernt Button Set.
         pControlThemes = pRoot.FirstChild(XMLTHEMES).Element();

         // Find The Theme by property matching brute forced search of all the idents
         // for that Base Class.
         TiXmlHandle pBaseClassNodeNode = pControlThemes.FirstChild( BaseClass.t_str() );
         if(pBaseClassNodeNode.Element())
         {
            bool CreateNewTheme = true;
            TiXmlElement* pThemeProperties = pBaseClassNodeNode.FirstChild().Element();
            for(pThemeProperties; pThemeProperties; pThemeProperties = pThemeProperties->NextSiblingElement())
            {
               if(IsThemeMatch(pThemeProperties,Component))
               {
                  Theme = pThemeProperties->Value();
                  CreateNewTheme = false;
               }
            }

            if(CreateNewTheme)
            {
               // Create the Element and add it to the BaseClassNode.
               TiXmlElement* ComponentAsElement = ComponentToElement(Component);
               Theme = ComponentAsElement->Value();
               pBaseClassNodeNode.Element()->LinkEndChild( ComponentAsElement );
            }
            else
            {

            }
         }
         else
         {
            // If no matching theme for this base class then create a new one and add it.
            // Use the Controls Ident for its name.

            // Create The Base class node.
            TiXmlElement* pBaseClass = new TiXmlElement( BaseClass.t_str() );
            pBaseClass->SetAttribute(BASECLASSNAME, BaseClass.t_str());
            pControlThemes.ToElement()->LinkEndChild( pBaseClass );
            // Create the Element and add it to the BaseClassNode.
            TiXmlElement* ComponentAsElement = ComponentToElement(Component);
            Theme = ComponentAsElement->Value();
            pBaseClass->LinkEndChild( ComponentAsElement );
         }
      }

      // If found return the Ident for this base class that matches its properties.
      UnicodeString ControlIdent = GetControlIdent(Component);
      TiXmlElement* pControlIndexNode = pThemeIndex.FirstChild( ControlIdent.t_str() ).Element();
      if(!pControlIndexNode)
      { // No Index for this item, create it.
         pControlIndexNode = new TiXmlElement( ControlIdent.t_str() );
         // Add this controls Ident + Theme to the Control Themes Index.
         pThemeIndex.ToElement()->LinkEndChild( pControlIndexNode );
      }
      // Update the index.
      //pControlIndexNode->SetAttribute(CONTROLIDENT, ControlIdent.t_str());
      pControlIndexNode->SetAttribute(CONTROLTHEME, Theme.t_str());
   }
}

bool TManagerTheme::IntCompare(int Source ,TiXmlElement* pElement, UnicodeString AttribName )
{
   int Temp;
   int RetVal = pElement->QueryIntAttribute(AttribName.t_str(),&Temp);
   if(RetVal == TIXML_SUCCESS)
   {
      return Temp == Source;
   }
   else
   {
      return false;
   }
}

bool TManagerTheme::HexAssign(TiXmlElement *pElement,UnicodeString Attrib,int &Value)
{
   UnicodeString Data = pElement->GetText();
   if(pElement->Attribute(Attrib.t_str()) != NULL)
   {
      UnicodeString AnsiTemp = UnicodeString("0x") + UnicodeString(pElement->Attribute(Attrib.t_str()));
      Value = StrToIntDef(AnsiTemp,0);
  // Swapped the R & B because the Text repsentation of it is in RGB Order.
      BYTE R = GetRValue(Value);
      BYTE G = GetGValue(Value);
      BYTE B = GetBValue(Value);
      Value = RGB(B,G,R);
      return true;
   }
   return false;
}

// Used in Hex compare dosent Mangel the RGB values.
bool TManagerTheme::HexToInt(TiXmlElement *pElement,UnicodeString Attrib,int &Value)
{
   UnicodeString Data = pElement->GetText();
   if(pElement->Attribute(Attrib.t_str()) != NULL)
   {
      UnicodeString AnsiTemp = UnicodeString("0x") + UnicodeString(pElement->Attribute(Attrib.t_str()));
      Value = StrToIntDef(AnsiTemp,0);
      return true;
   }
   return false;
}

bool TManagerTheme::ColorCompare(int Source ,TiXmlElement* pElement, UnicodeString AttribName )
{
   int Temp;
   if(HexAssign(pElement,AttribName,Temp))
   {
      return Temp == RGB(GetRValue(Source),GetGValue(Source),GetBValue(Source));
   }
   else
   {
      return false;
   }
}

bool TManagerTheme::IsThemeMatch(TiXmlElement* pThemeProperties,TComponent *Component)
{
   bool Match = false;
   if(Component->InheritsFrom(__classid(TForm)))
   {
      TForm *Control = static_cast<TForm *>(Component);
      if(ColorCompare(Control->Color,pThemeProperties,ATTRIB_COLOR)) Match = true;
   }
   else if(Component->InheritsFrom(__classid(TTouchBtn)))
   {
      TTouchBtn *Control = static_cast<TTouchBtn *>(Component);
      if(ColorCompare(Control->Color,pThemeProperties,ATTRIB_COLOR) &&
         ColorCompare(Control->ButtonColor,pThemeProperties,ATTRIB_BUTTONCOLOR) &&
         ColorCompare(Control->LatchedColor,pThemeProperties,ATTRIB_LATCHEDCOLOR) &&
         ColorCompare(Control->Font->Color,pThemeProperties,ATTRIB_FONTCOLOR) &&
         IntCompare(Control->Font->Size,pThemeProperties,ATTRIB_FONTSIZE) &&
         Control->Font->Name == UnicodeString(pThemeProperties->Attribute(ATTRIB_FONTNAME))
        ) Match = true;
   }
   else if(Component->InheritsFrom(__classid(TTouchGrid)))
   {
      TTouchGrid *Control = static_cast<TTouchGrid *>(Component);

      if(ColorCompare(Control->Color,pThemeProperties,ATTRIB_COLOR) &&
         ColorCompare(Control->DefaultButtonColor,pThemeProperties,ATTRIB_DEFAULTBUTTONCOLOR) &&
         ColorCompare(Control->DefaultButtonLatchedColor,pThemeProperties,ATTRIB_DEFAULTBUTTONLATCHEDCOLOR) &&
         ColorCompare(Control->GridColor,pThemeProperties,ATTRIB_GRIDCOLOR) &&
         ColorCompare(Control->Font->Color,pThemeProperties,ATTRIB_FONTCOLOR) &&
         IntCompare(Control->Font->Size,pThemeProperties,ATTRIB_FONTSIZE) &&
         Control->Font->Name == UnicodeString(pThemeProperties->Attribute(ATTRIB_FONTNAME))
        ) Match = true;
   }
   else if(Component->InheritsFrom(__classid(TTouchNumpad)))
   {
      TTouchNumpad *Control = static_cast<TTouchNumpad *>(Component);

      if(ColorCompare(Control->Color,pThemeProperties,ATTRIB_COLOR) &&
         ColorCompare(Control->ButtonColor,pThemeProperties,ATTRIB_BUTTONCOLOR) &&
         ColorCompare(Control->Font->Color,pThemeProperties,ATTRIB_FONTCOLOR) &&
         IntCompare(Control->Font->Size,pThemeProperties,ATTRIB_FONTSIZE) &&
         Control->Font->Name == UnicodeString(pThemeProperties->Attribute(ATTRIB_FONTNAME))
        ) Match = true;

   }
   else if(Component->InheritsFrom(__classid(TNumpadDisplay)))
   {
      TNumpadDisplay *Control = static_cast<TNumpadDisplay *>(Component);

      if(ColorCompare(Control->Color,pThemeProperties,ATTRIB_COLOR) &&
         ColorCompare(Control->Font->Color,pThemeProperties,ATTRIB_FONTCOLOR) &&
         IntCompare(Control->Font->Size,pThemeProperties,ATTRIB_FONTSIZE) &&
         Control->Font->Name == UnicodeString(pThemeProperties->Attribute(ATTRIB_FONTNAME))
        ) Match = true;
   }
   else if(Component->InheritsFrom(__classid(TPanel)))
   {
      TPanel *Control = static_cast<TPanel *>(Component);
      if(ColorCompare(Control->Color,pThemeProperties,ATTRIB_COLOR) ) Match = true;
   }
   else if(Component->InheritsFrom(__classid(TButton)))
   {
      TButton *Control = static_cast<TButton *>(Component);
      if(ColorCompare(Control->Font->Color,pThemeProperties,ATTRIB_FONTCOLOR) &&
         IntCompare(Control->Font->Size,pThemeProperties,ATTRIB_FONTSIZE) &&
         Control->Font->Name == UnicodeString(pThemeProperties->Attribute(ATTRIB_FONTNAME))
      ) Match = true;
   }
   return Match;
}


TiXmlElement* TManagerTheme::ComponentToElement(TComponent *Component)
{
   UnicodeString ControlIdent = GetControlIdent(Component);
   TiXmlElement* ReturnValue = new TiXmlElement( ControlIdent.t_str() );
   if(Component->InheritsFrom(__classid(TForm)))
   {
      TForm *Control = static_cast<TForm *>(Component);
      ReturnValue->SetAttribute(ATTRIB_COLOR,         AsHex(Control->Color).t_str());
   }
   else if(Component->InheritsFrom(__classid(TTouchBtn)))
   {
      TTouchBtn *Control = static_cast<TTouchBtn *>(Component);
      ReturnValue->SetAttribute(ATTRIB_BUTTONCOLOR,   AsHex(Control->ButtonColor).t_str());
      ReturnValue->SetAttribute(ATTRIB_COLOR,         AsHex(Control->Color).t_str());
      ReturnValue->SetAttribute(ATTRIB_LATCHEDCOLOR,  AsHex(Control->LatchedColor).t_str());
      ReturnValue->SetAttribute(ATTRIB_FONTCOLOR,     AsHex(Control->Font->Color).t_str());
      ReturnValue->SetAttribute(ATTRIB_FONTNAME,      Control->Font->Name.t_str());
      ReturnValue->SetAttribute(ATTRIB_FONTSIZE,      Control->Font->Size);
   }
   else if(Component->InheritsFrom(__classid(TTouchGrid)))
   {
      TTouchGrid *Control = static_cast<TTouchGrid *>(Component);
      ReturnValue->SetAttribute(ATTRIB_DEFAULTBUTTONCOLOR,        AsHex(Control->DefaultButtonColor).t_str());
      ReturnValue->SetAttribute(ATTRIB_DEFAULTBUTTONLATCHEDCOLOR, AsHex(Control->DefaultButtonLatchedColor).t_str());
      ReturnValue->SetAttribute(ATTRIB_COLOR,                     AsHex(Control->Color).t_str());
      ReturnValue->SetAttribute(ATTRIB_GRIDCOLOR,                 AsHex(Control->GridColor).t_str());
      ReturnValue->SetAttribute(ATTRIB_FONTCOLOR,                 AsHex(Control->Font->Color).t_str());
      ReturnValue->SetAttribute(ATTRIB_ARROWBUTTONCOLOR,          AsHex(Control->ScrollBtnDown->Color).t_str());
      ReturnValue->SetAttribute(ATTRIB_FONTNAME,                  Control->Font->Name.t_str());
      ReturnValue->SetAttribute(ATTRIB_FONTSIZE,                  Control->Font->Size);
   }
   else if(Component->InheritsFrom(__classid(TTouchNumpad)))
   {
      TTouchNumpad *Control = static_cast<TTouchNumpad *>(Component);
      ReturnValue->SetAttribute(ATTRIB_COLOR,                     AsHex(Control->Color).t_str());
      ReturnValue->SetAttribute(ATTRIB_BUTTONCOLOR,               AsHex(Control->ButtonColor).t_str());
      ReturnValue->SetAttribute(ATTRIB_FONTCOLOR,                 AsHex(Control->Font->Color).t_str());
      ReturnValue->SetAttribute(ATTRIB_FONTNAME,                  Control->Font->Name.t_str());
      ReturnValue->SetAttribute(ATTRIB_FONTSIZE,                  Control->Font->Size);
   }
   else if(Component->InheritsFrom(__classid(TNumpadDisplay)))
   {
      TNumpadDisplay *Control = static_cast<TNumpadDisplay *>(Component);
      ReturnValue->SetAttribute(ATTRIB_COLOR,                     AsHex(Control->Color).t_str());
      ReturnValue->SetAttribute(ATTRIB_FONTCOLOR,                 AsHex(Control->Font->Color).t_str());
      ReturnValue->SetAttribute(ATTRIB_FONTNAME,                  Control->Font->Name.t_str());
      ReturnValue->SetAttribute(ATTRIB_FONTSIZE,                  Control->Font->Size);
   }
   else if(Component->InheritsFrom(__classid(TPanel)))
   {
      TPanel *Control = static_cast<TPanel *>(Component);
      ReturnValue->SetAttribute(ATTRIB_COLOR,         AsHex(Control->Color).t_str());
   }
   else if(Component->InheritsFrom(__classid(TButton)))
   {
      TButton *Control = static_cast<TButton *>(Component);
      ReturnValue->SetAttribute(ATTRIB_FONTCOLOR, AsHex(Control->Font->Color).t_str());
      ReturnValue->SetAttribute(ATTRIB_FONTNAME, Control->Font->Name.t_str());
      ReturnValue->SetAttribute(ATTRIB_FONTSIZE, Control->Font->Size);
   }
   return ReturnValue;
}

UnicodeString TManagerTheme::GetControlIdent(TComponent *Component)
{
   UnicodeString Ident = "";
   TComponent *CurrentControl = Component;
   std::vector<UnicodeString> Names;
   do
   {
      Names.push_back("_" + CurrentControl->Name);
      if(CurrentControl->HasParent())
      {
         CurrentControl = CurrentControl->GetParentComponent();
      }
      else
      {
         CurrentControl = NULL;
      }
   }
   while(CurrentControl != NULL);

   while(Names.size() != 0)
   {
      Ident += Names[Names.size()-1];
      Names.pop_back();
   }

   return Ident;
}

UnicodeString TManagerTheme::GetControlBaseClass(TComponent *Component)
{
   if(Component->InheritsFrom(__classid(TForm)))
   {
      return "TForm";
   }
   else if(Component->InheritsFrom(__classid(TTouchBtn)))
   {
      return "TTouchBtn";
   }
   else if(Component->InheritsFrom(__classid(TTouchGrid)))
   {
      return "TTouchGrid";
   }
   else if(Component->InheritsFrom(__classid(TTouchNumpad)))
   {
      return "TTouchNumpad";
   }
   else if(Component->InheritsFrom(__classid(TNumpadDisplay)))
   {
      return "TNumpadDisplay";
   }
   else if(Component->InheritsFrom(__classid(TPanel)))
   {
      return "TPanel";
   }
   else if(Component->InheritsFrom(__classid(TButton)))
   {
      return "TButton";
   }
   else
   {
      return UNKNOWNBASECLASS;
   }
}

bool TManagerTheme::IntAssign(TiXmlElement *pElement,UnicodeString Attrib,int &Value)
{
   int Temp;
   if(pElement->QueryIntAttribute(Attrib.t_str(),&Temp) == TIXML_SUCCESS)
   {
      Value = Temp;
      return true;
   }
   return false;
}

UnicodeString TManagerTheme::AsHex(int Value)
{
   BYTE R = GetRValue(Value);
   BYTE G = GetGValue(Value);
   BYTE B = GetBValue(Value);
  // Swapped the R & B so the Text repsentation of it is in RGB Order.
  return IntToHex(int(RGB(B,G,R)),8);
}

//void TManagerTheme::StrAssign(TiXmlElement *pElement,UnicodeString &Value,UnicodeString Attrib)
//{
//   Value = pElement->Attribute(Attrib.t_str());
//}
//
//void TManagerTheme::StrAssign(TiXmlElement *pElement,UnicodeString &Value,UnicodeString Attrib)
//{
//   Value = pElement->Attribute(Attrib.t_str());
//}
//
//void TManagerTheme::StrAssign(TiXmlElement *pElement,UnicodeString &Value,UnicodeString Attrib)
//{
//   Value = pElement->Attribute(Attrib.t_str());
//}

void TManagerTheme::StrAssign(TiXmlElement *pElement,UnicodeString &Value,UnicodeString Attrib)
{
   Value = pElement->Attribute(Attrib.t_str());
}

void TManagerTheme::ElementToComponent(TComponent *Component,TiXmlElement *pElement)
{
   int TempVar = 0;
   UnicodeString TempString = "";
   if(Component->InheritsFrom(__classid(TForm)))
   {
      TForm *Control = static_cast<TForm *>(Component);
      if(HexAssign(pElement,ATTRIB_COLOR,TempVar))    Control->Color = TempVar;
   }
   else if(Component->InheritsFrom(__classid(TTouchBtn)))
   {
      TTouchBtn *Control = static_cast<TTouchBtn *>(Component);
      if(HexAssign(pElement,ATTRIB_COLOR,TempVar))          Control->Color = TempVar;
      if(HexAssign(pElement,ATTRIB_BUTTONCOLOR,TempVar))    Control->ButtonColor = TempVar;
      if(HexAssign(pElement,ATTRIB_LATCHEDCOLOR,TempVar))   Control->LatchedColor = TempVar;
      if(HexAssign(pElement,ATTRIB_FONTCOLOR,TempVar))      Control->Font->Color = TempVar;
      if(IntAssign(pElement,ATTRIB_FONTSIZE,TempVar))       Control->Font->Size = TempVar;
	  StrAssign(pElement,TempString,ATTRIB_FONTNAME);       Control->Font->Name = TempString;
   }
   else if(Component->InheritsFrom(__classid(TTouchGrid)))
   {
	  TTouchGrid *Control = static_cast<TTouchGrid *>(Component);
	  if(HexAssign(pElement,ATTRIB_DEFAULTBUTTONCOLOR,TempVar))         Control->DefaultButtonColor = TempVar;
	  if(HexAssign(pElement,ATTRIB_DEFAULTBUTTONLATCHEDCOLOR,TempVar))  Control->DefaultButtonLatchedColor = TempVar;
	  if(HexAssign(pElement,ATTRIB_COLOR,TempVar))                      Control->Color = TempVar;
	  if(HexAssign(pElement,ATTRIB_GRIDCOLOR,TempVar))                  Control->GridColor = TempVar;
	  if(HexAssign(pElement,ATTRIB_FONTCOLOR,TempVar))                  Control->Font->Color = TempVar;
	  if(HexAssign(pElement,ATTRIB_ARROWBUTTONCOLOR,TempVar))           Control->ScrollBtnDown->Color = TempVar;
	  if(HexAssign(pElement,ATTRIB_ARROWBUTTONCOLOR,TempVar))           Control->ScrollBtnLeft->Color = TempVar;
	  if(HexAssign(pElement,ATTRIB_ARROWBUTTONCOLOR,TempVar))           Control->ScrollBtnRight->Color = TempVar;
	  if(HexAssign(pElement,ATTRIB_ARROWBUTTONCOLOR,TempVar))           Control->ScrollBtnUp->Color = TempVar;
	  if(IntAssign(pElement,ATTRIB_FONTSIZE,TempVar))                   Control->Font->Size = TempVar;
	  StrAssign(pElement,TempString,ATTRIB_FONTNAME);       			Control->Font->Name = TempString;
   }
   else if(Component->InheritsFrom(__classid(TTouchNumpad)))
   {
	  TTouchNumpad *Control = static_cast<TTouchNumpad *>(Component);
	  if(HexAssign(pElement,ATTRIB_COLOR,TempVar))                      Control->Color = TempVar;
	  if(HexAssign(pElement,ATTRIB_BUTTONCOLOR,TempVar))                Control->ButtonColor = TempVar;
	  if(HexAssign(pElement,ATTRIB_FONTCOLOR,TempVar))                  Control->Font->Color = TempVar;
	  if(IntAssign(pElement,ATTRIB_FONTSIZE,TempVar))                   Control->Font->Size = TempVar;
	  StrAssign(pElement,TempString,ATTRIB_FONTNAME);       			Control->Font->Name = TempString;
   }
   else if(Component->InheritsFrom(__classid(TNumpadDisplay)))
   {
      TNumpadDisplay *Control = static_cast<TNumpadDisplay *>(Component);
      if(HexAssign(pElement,ATTRIB_COLOR,TempVar))                      Control->Color = TempVar;
      if(HexAssign(pElement,ATTRIB_FONTCOLOR,TempVar))                  Control->Font->Color = TempVar;
      if(IntAssign(pElement,ATTRIB_FONTSIZE,TempVar))                   Control->Font->Size = TempVar;
	  StrAssign(pElement,TempString,ATTRIB_FONTNAME);       			Control->Font->Name = TempString;
   }
   else if(Component->InheritsFrom(__classid(TPanel)))
   {
      TPanel *Control = static_cast<TPanel *>(Component);
      if(HexAssign(pElement,ATTRIB_COLOR,TempVar))                   Control->Color = TempVar;
   }
   else if(Component->InheritsFrom(__classid(TButton)))
   {
      TTouchBtn *Control = static_cast<TTouchBtn *>(Component);
      if(HexAssign(pElement,ATTRIB_COLOR,TempVar))                   Control->Color = TempVar;
      if(HexAssign(pElement,ATTRIB_FONTCOLOR,TempVar))               Control->Font->Color = TempVar;
      if(IntAssign(pElement,ATTRIB_FONTSIZE,TempVar))                Control->Font->Size = TempVar;
	  StrAssign(pElement,TempString,ATTRIB_FONTNAME);       		 Control->Font->Name = TempString;
   }
}  */

