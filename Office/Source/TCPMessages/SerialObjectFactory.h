//---------------------------------------------------------------------------
#ifndef SerialObjectFactoryH
#define SerialObjectFactoryH
//---------------------------------------------------------------------------
#include <memory>
#include <map>
#include <set>
#include "SerialData.h"
//---------------------------------------------------------------------------
// Don't derive from or create one of these!
//---------------------------------------------------------------------------
template <class TSerialObjBase, typename TObjectID=int>
class TSerialObjFactoryBase
{
public:
	// This object is pure-virtual but derived objects are only created once for
	// each ObjectID, perferably automatically via TSerialObjBase
	TSerialObjFactoryBase(TObjectID ObjectID)
	{
		FactoryMap()[ObjectID] = this;
	}
	// The main function for object creation!
	//
	// Read the raw data into a TSerialData object and use it like this:
	//
	//	TSerialObjBase_ *Obj = TSerialObjFactoryBase<TObjectID>::CreateInstance(Data);
	//	if (Obj) Obj->Execute();
	static TSerialObjBase *CreateInstance(TSerialData &Data)	// Check for NULL returns!
	{
		TObjectID ObjectID;
		Data >> ObjectID;

		TFactoryMap::iterator iMap = FactoryMap().find(ObjectID);
		if (iMap != FactoryMap().end())
		{
			return iMap->second->MakeObject(Data);
		}
		else
		{
			return NULL;
		}
	}
protected:
	virtual TSerialObjBase *MakeObject(TSerialData &Data) = 0;

	typedef std::map<TObjectID, TSerialObjFactoryBase *> TFactoryMap;
	// This function returns a reference to the static list of Factories, indexed by
	// the ObjectID. A function is used to avoid static initialization issues with a
	// pure static member. The map is garenteed to exists even if accessed in the constructor
	// of a static instance of this object.
	//
	// The map is used to look up a suitable factory (ie a derived version of this class)
	// given an ObjectID. MakeObject() would then be called to return a new filled-in object.
	static TFactoryMap& FactoryMap()
	{
		// May be destroyed after destruction of container object.
		// This is not an issue as it doesen't reference it in its ~.
		static TFactoryMap fFactoryMap;
		return fFactoryMap;
	}
};
//---------------------------------------------------------------------------
// This class is responsible for creating the object factory for the type you
// specify.
//
//	e.g.
//
// static const TSerialObjFactory<TMyObjectBase, TMyObject> RegisterFactory(ObjectID);
//
// where TMyObject is derrived from TMyObjectBase
//
// Create a TMyObject object using:
// TSerialData Data(Stream);
//	TMyObjectBase *MyObject = TSerialObjFactoryBase<IDType>::CreateInstance(Data);
//	Where data contains the ID of TMyObject
//---------------------------------------------------------------------------
template <class TSerialObjBase, class TSerialObject, typename TObjectID=int>
class TSerialObjFactory : TSerialObjFactoryBase<TSerialObjBase, TObjectID>
{
public:
	TSerialObjFactory(TObjectID ObjectID) : TSerialObjFactoryBase<TSerialObjBase, TObjectID>(ObjectID) {}
protected:
	virtual TSerialObjBase *MakeObject(TSerialData &Data)
	{
		TSerialObjBase *ObjectBase = new TSerialObject;
		try
		{
			if (ObjectBase->SerializeIn(Data))
			{
				return ObjectBase;
			}
			else
			{
				delete ObjectBase;
				return NULL;
			}
		}
		catch (...)
		{
			delete ObjectBase;
			return NULL;
		}
	}
};
//---------------------------------------------------------------------------
#endif
