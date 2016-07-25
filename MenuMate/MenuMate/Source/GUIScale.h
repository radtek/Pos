//---------------------------------------------------------------------------

#ifndef GUIScaleH
#define GUIScaleH
#include <Controls.hpp>

//---------------------------------------------------------------------------
class GUIScale
{

	public :
	static void ParentHeight(TWinControl *Control, float scale);
   static void ParentWidth(TWinControl *Control, float scale);
	static void ChildrenHeight(TWinControl *Parent, float scale);
	static void ChildrenWidth(TWinControl *Parent, float scale);
 	static void ChildrenTop(TWinControl *Parent, float scale);
        static void ChildrenPercentageHeight(TWinControl *Parent, float scale);
};
#endif
