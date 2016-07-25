//---------------------------------------------------------------------------
#ifndef SerialObjectsH
#define SerialObjectsH
//---------------------------------------------------------------------------
#include "SerialData.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Include the line below in derived classes, with appropriate types.
//	TSerialObj is your derived class type. TIDType is an ID type used to
//	create the apropriate class. e.g. int
//
//	typedef AnsiString TIDType;
//	const TIDType ID = "OBJ1";
//	class TSerialObj : public TSerialObjBase
//	{
//	public:
//		int			MyData1;
//		AnsiString	MyData2;
//
//		void TMyTCPMessage::SerializeOut(TSerialData &Data)	{ Data << ObjectID << MyData1 << MyData2; }
//		void TMyTCPMessage::SerializeIn(TSerialData &Data)		{ Data >> MyData1; Data >> MyData2; }
//	};
//
// Then add to your CPP file:
//
//	const TSerialObjFactory<TSerialObjBase, TSerialObj, TIDType> RegisterFactory(ID);
//
// To create an instance, given a stream of data, just use
//	TSerialObjBase *Obj = TSerialObjFactoryBase<TSerialObjBase>::CreateInstance(Data);
//
//	Then you can call:
//
//	if (Obj) Obj->Execute();
//
//---------------------------------------------------------------------------
class TSerialObjBase
{
public:
	virtual void SerializeOut(TSerialData &Data) = 0;	// Always write out the ObjectID first.
	virtual bool SerializeIn(TSerialData &Data) = 0;	// No need to read in ID. Already read. Return false in invalid.

	virtual void Execute() = 0;

private:
};
//---------------------------------------------------------------------------
#endif
