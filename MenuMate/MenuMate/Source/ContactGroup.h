//---------------------------------------------------------------------------

#ifndef ContactGroupH
#define ContactGroupH
//---------------------------------------------------------------------------
#include <Classes.hpp>

//#define HOTEL_NUMBER 5834

class THotelNumber
{
	private:
	THotelNumber(){}
	static const int Hotel_Number;

	public:
	static const int GetHotelNumber( void ){ return Hotel_Number; }
};

const int THotelNumber::Hotel_Number = 5843;

struct ContactGroup{
	int Type;
	int Key;
	UnicodeString Name;
};



#endif
