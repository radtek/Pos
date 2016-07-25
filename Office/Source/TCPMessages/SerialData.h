//---------------------------------------------------------------------------
#ifndef SerialDataH
#define SerialDataH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <memory>
//---------------------------------------------------------------------------
// This data class is used for all serial object classes.
// Be careful using non-built-in datatypes ie where its size is non constant
//
// Specializeation functions are added for AnsiString and TStrings * (and
// TStringList *) but watch __property and others.
//---------------------------------------------------------------------------
class TSerialData
{
public:
	TSerialData();
	TSerialData(TStream *Stream);

	inline void *Data();
	inline __int64 Size();

	inline void operator + (TSerialData &Value);

	inline TSerialData &  operator << (System::AnsiString &Value);
	inline TSerialData &  operator << (System::AnsiString Value);
	inline void operator >> (System::AnsiString &Value) const;

	inline TSerialData &  operator << (TDateTime &Value);
	inline TSerialData &  operator << (TDateTime Value);
	inline void operator >> (TDateTime &Value) const;

	inline TSerialData &  operator << (TStrings *Value);
	inline void operator >> (TStrings *Value) const;

	inline TSerialData& operator << (TStringList *Value);
	inline void operator >> (TStringList *Value) const;

	inline TSerialData &  operator << (TStream *Value);
	inline void operator >> (TStream *Value) const;
	inline TSerialData &  operator << (TFileStream *Value);
	inline void operator >> (TFileStream *Value) const;
	inline TSerialData &  operator << (TMemoryStream *Value);
	inline void operator >> (TMemoryStream *Value) const;

	inline template <typename T> TSerialData & operator << (T Value);
	inline template <typename T> void operator >> (T &Value) const;
private:
	std::auto_ptr<TMemoryStream> fData;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TSerialData::TSerialData() : fData(new TMemoryStream)
{
}
//---------------------------------------------------------------------------
TSerialData::TSerialData(TStream *Stream) : fData(new TMemoryStream)
{
	fData->CopyFrom(Stream, 0);
	fData->Position = 0;
}
//---------------------------------------------------------------------------
inline void *TSerialData::Data()
{
	fData->Position = 0;
	return fData->Memory;
}
//---------------------------------------------------------------------------
inline __int64 TSerialData::Size()
{
	return fData->Size;
}
//---------------------------------------------------------------------------
/*inline void TSerialData::operator + (TSerialData &Value)
{
	fData->Position			= fData->Size;
	Value.fData->Position	= 0;
	fData->CopyFrom(Value.fData.get(), Value.fData->Size);
} */
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
inline TSerialData& TSerialData::operator << (System::AnsiString &Value)
{
	int Length = Value.Length();
	fData->Write(&Length, sizeof(Length));
	fData->Write(Value.c_str(), Length);
	return *this;
}
//---------------------------------------------------------------------------
inline TSerialData& TSerialData::operator << (System::AnsiString Value)
{
	return *this<<Value;
}
//---------------------------------------------------------------------------
inline void TSerialData::operator >> (System::AnsiString &Value) const
{
	int Length;
	fData->Read(&Length, sizeof(Length));
	Value.SetLength(Length);
	fData->Read(Value.c_str(), Length);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
inline TSerialData& TSerialData::operator << (TDateTime &Value)
{
	fData->Write(&Value, sizeof(Value));
	return *this;
}
//---------------------------------------------------------------------------
inline TSerialData& TSerialData::operator << (TDateTime Value)
{
	return *this<<Value;
}
//---------------------------------------------------------------------------
inline void TSerialData::operator >> (TDateTime &Value) const
{
	fData->Read(&Value, sizeof(Value));
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
inline TSerialData& TSerialData::operator << (TStrings *Value)
{
	int LineCount = Value->Count;
	fData->Write(&LineCount, sizeof(LineCount));
	for (int i=0; i<LineCount; i++)
	{
		// Put in temporary string as '__property AnsiString' isn't an AnsiString!
		AnsiString Line = Value->Strings[i];
		*this<<Line;
	}
	return *this;
}
//---------------------------------------------------------------------------
inline void TSerialData::operator >> (TStrings *Value) const
{
	Value->Clear();
	int LineCount;
	fData->Read(&LineCount, sizeof(LineCount));
	for (int i=0; i<LineCount; i++)
	{
		AnsiString Line;
		*this>>Line;
		Value->Add(Line);
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
inline TSerialData& TSerialData::operator << (TStringList *Value)
{
	return *this<<static_cast<TStrings *>(Value);
}
//---------------------------------------------------------------------------
inline void TSerialData::operator >> (TStringList *Value) const
{
	*this>>static_cast<TStrings *>(Value);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
inline TSerialData& TSerialData::operator << (TStream *Value)
{
	Value->Position = 0;
	int Length = Value->Size;
	fData->Write(&Length, sizeof(Length));
	fData->CopyFrom(Value, Length);
	return *this;
}
//---------------------------------------------------------------------------
inline void TSerialData::operator >> (TStream *Value) const
{
	int Length;
	fData->Read(&Length, sizeof(Length));
	Value->CopyFrom(fData.get(), Length);
}
//---------------------------------------------------------------------------
inline TSerialData& TSerialData::operator << (TFileStream *Value)
{
	return *this<<static_cast<TStream *>(Value);
}
//---------------------------------------------------------------------------
inline void TSerialData::operator >> (TFileStream *Value) const
{
	*this>>static_cast<TStream *>(Value);
}
//---------------------------------------------------------------------------
inline TSerialData& TSerialData::operator << (TMemoryStream *Value)
{
	return *this<<static_cast<TStream *>(Value);
}
//---------------------------------------------------------------------------
inline void TSerialData::operator >> (TMemoryStream *Value) const
{
	*this>>static_cast<TStream *>(Value);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
inline template <typename T>
TSerialData & TSerialData::operator << (T Value)
{
	fData->Write(&Value, sizeof(T));
	return *this;
}
//---------------------------------------------------------------------------
inline template <typename T>
void TSerialData::operator >> (T &Value) const
{
	fData->Read(&Value, sizeof(T));
}
//---------------------------------------------------------------------------
#endif
