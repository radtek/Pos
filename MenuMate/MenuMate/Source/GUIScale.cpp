//---------------------------------------------------------------------------


#pragma hdrstop

#include "GUIScale.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void GUIScale::ParentHeight(TWinControl *Control, float scale)
{
	TWinControl *Parent = Control->Parent;
   Control->Height = Parent->Height * scale;
}

void GUIScale::ParentWidth(TWinControl *Control, float scale)
{
	TWinControl *Parent = Control->Parent;
   Control->Width = Parent->Width * scale;
}

void GUIScale::ChildrenHeight(TWinControl *Parent, float scale)
{
	for(int i = 0; i < Parent->ControlCount; i++)
   {
   	TWinControl *Child = dynamic_cast<TWinControl*>(Parent->Controls[i]);
      if(Child)
      {
      	Child->Height = Parent->Height * scale;
      }
   }
}

void GUIScale::ChildrenWidth(TWinControl *Parent, float scale)
{
	for(int i = 0; i < Parent->ControlCount; i++)
   {
   	TWinControl *Child = dynamic_cast<TWinControl*>(Parent->Controls[i]);
      if(Child)
      {
	      Child->Width = Parent->Width * scale;
      }
   }
}

void GUIScale::ChildrenTop(TWinControl *Parent, float scale)
{
	for(int i = 0; i < Parent->ControlCount;i++)
   {
        TWinControl *Child = dynamic_cast<TWinControl*>(Parent->Controls[i]);
        if(Child)
        {
        	Child->Top = Child->Top * scale;
        }
   }
}

void GUIScale::ChildrenPercentageHeight(TWinControl *Parent, float scale)
{
	for(int i = 0; i < Parent->ControlCount;i++)
        {
        TWinControl *Child = dynamic_cast<TWinControl*>(Parent->Controls[i]);
        if(Child)
        {
        	Child->Height = Child->Height * scale;
        }
   }
}
