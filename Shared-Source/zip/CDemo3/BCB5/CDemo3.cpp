#include <vcl\vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
USERES("Cdemo3.res");
USERC("DemoManifest.rc");
//---------------------------------------------------------------------------
WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int ) {
	try {
		Application->Initialize();
		Application->Run();
	} catch ( Exception &exception ) {
		Application->ShowException( &exception );
	}
	return 0;
}
//---------------------------------------------------------------------------

