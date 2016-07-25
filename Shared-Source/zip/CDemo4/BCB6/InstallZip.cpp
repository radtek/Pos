#include <vcl\vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
USEFORM("Install.cpp", InstForm);
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

