//---------------------------------------------------------------------------

#ifndef MMCustomerDisplayAdapterH
#define MMCustomerDisplayAdapterH

//---------------------------------------------------------------------------

<<<<<<< HEAD
#include <Classes.hpp>

//---------------------------------------------------------------------------

class TiXmlDocument;
=======
#include <vcl.h>
#include "tinyxml.h"

//---------------------------------------------------------------------------

 class TiXmlDocument;
>>>>>>> ee47c11899dd623a2c139da1df7b3fba1ae176f2

//:::::::::::::::::::::::::::
// TMMCustomerDisplayAdapter
//:::::::::::::::::::::::::::
class TMMCustomerDisplayAdapter
{
    public:
      static TiXmlDocument* CreateOrderXML( TList* inPOSOrder );
<<<<<<< HEAD
} ;
=======
};
>>>>>>> ee47c11899dd623a2c139da1df7b3fba1ae176f2

//---------------------------------------------------------------------------

#endif
