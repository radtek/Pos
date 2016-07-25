//---------------------------------------------------------------------------
#ifndef SerialDataH
#define SerialDataH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Graphics.hpp>
#include <Controls.hpp>
#include <memory>
//---------------------------------------------------------------------------
/*class TSerialData;
class TStreamingData
{
public:
	virtual void SerializeWrite(TSerialData &SerialData) const = 0;
	virtual void SerializeRead(const TSerialData &SerialData) = 0;
};*/
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

	void Clear();

	inline void *Data();
	inline __int64 Size();
	inline TMemoryStream *Stream();

	int GetPosition();
	void SetPosition(int Pos);

	TSerialData &Compress(int Level);
	void Decompress();

	TSerialData &Encrypt(AnsiString Key);
	bool Decrypt(AnsiString Key);

//	inline void operator + (TSerialData &Value);
	inline TSerialData& operator += (TSerialData &Value);
	inline TSerialData& operator = (TStream *Stream);

	inline TSerialData& Copy(TSerialData &Value, int Start, int Length=0);

	inline TSerialData& operator << (System::AnsiString &Value);
	inline TSerialData& operator << (System::AnsiString Value);
	inline void operator >> (System::AnsiString &Value) const;

	inline TSerialData& operator << (char *Value);
	inline TSerialData& operator << (const char *Value);
	inline void operator >> (char *Value) const;

	inline TSerialData& operator << (TDateTime &Value);
	inline TSerialData& operator << (TDateTime Value);
	inline void operator >> (TDateTime &Value) const;

	inline TSerialData& operator << (TStrings *Value);
	inline TSerialData& operator << (const TStrings *Value);
	inline void operator >> (TStrings *Value) const;

	inline TSerialData& operator << (TStringList *Value);
	inline TSerialData& operator << (const TStringList *Value);
	inline void operator >> (TStringList *Value) const;

	inline TSerialData& operator << (TIcon *Value);
	inline TSerialData& operator << (const TIcon *Value);
	inline void operator >> (TIcon *Value) const;

	inline TSerialData& operator << (Graphics::TBitmap *Value);
	inline TSerialData& operator << (const Graphics::TBitmap *Value);
	inline void operator >> (Graphics::TBitmap *Value) const;

	inline TSerialData& operator << (TStream *Value);
	inline void operator >> (TStream *Value) const;
	inline TSerialData& operator << (TFileStream *Value);
	inline void operator >> (TFileStream *Value) const;
	inline TSerialData& operator << (TMemoryStream *Value);
	inline void operator >> (TMemoryStream *Value) const;

//	inline TSerialData & operator << (const TStreamingData &Value);
//	inline void operator >> (TStreamingData &Value) const;

	inline template <typename T> TSerialData & operator << (T Value);
	inline template <typename T> void operator >> (T &Value) const;
private:
	std::auto_ptr<TMemoryStream> fData;

	void AddCRC();
	bool CheckCRC();
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

inline void TSerialData::Clear()
{
	fData->Clear();
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
inline TMemoryStream *TSerialData::Stream()
{
	fData->Position = 0;
	return fData.get();
}
//---------------------------------------------------------------------------
inline TSerialData& TSerialData::operator += (TSerialData &Value)
{
	fData->Position			= fData->Size;
	Value.fData->Position	= 0;
	fData->CopyFrom(Value.fData.get(), Value.fData->Size);
	return *this;
}
//---------------------------------------------------------------------------
inline TSerialData& TSerialData::operator = (TStream *Stream)
{
	fData->Clear();
	fData->CopyFrom(Stream, 0);
	fData->Position = 0;
	return *this;
}
//---------------------------------------------------------------------------
inline TSerialData & TSerialData::Copy(TSerialData &Value, int Start, int Length)
{
	fData->Clear();
	Value.fData->Position = Start;
	if (Length == 0)
	{
		Length = Value.fData->Size - Value.fData->Position;
	}
	fData->CopyFrom(Value.fData.get(), Length);
	fData->Position = 0;
	return *this;
}
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
inline TSerialData& TSerialData::operator << (char *Value)
{
	return *this<<AnsiString(Value);
}
//---------------------------------------------------------------------------
inline TSerialData & TSerialData::operator << (const char *Value)
{
	return *this<<AnsiString(Value);
}
//---------------------------------------------------------------------------
inline void TSerialData::operator >> (char *Value) const
{
	AnsiString Temp = Value;
	*this>>Temp;
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
inline TSerialData& TSerialData::operator << (const TStrings *Value)
{
	throw Exception("Unable to stream const TStrings");
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
inline TSerialData& TSerialData::operator << (const TStringList *Value)
{
	throw Exception("Unable to stream const TStringList");
}
//---------------------------------------------------------------------------
inline void TSerialData::operator >> (TStringList *Value) const
{
	*this>>static_cast<TStrings *>(Value);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
inline TSerialData& TSerialData::operator << (TIcon *Value)
{
	std::auto_ptr<Graphics::TBitmap> IconBitmap(new Graphics::TBitmap);
	std::auto_ptr<Graphics::TBitmap> MaskBitmap(new Graphics::TBitmap);

	IconBitmap->Height		= Value->Height;
	IconBitmap->Width			= Value->Width;
	IconBitmap->PixelFormat	= pf32bit;
	IconBitmap->Canvas->Draw(0, 0, Value);

	ICONINFO IconInfo;
	GetIconInfo(Value->Handle, &IconInfo);

	MaskBitmap->Monochrome	= true;
	MaskBitmap->PixelFormat	= pf1bit;
	MaskBitmap->Height		= Value->Height;
	MaskBitmap->Width			= Value->Width;
	MaskBitmap->Handle		= IconInfo.hbmMask;

	operator << (IconBitmap.get());
	operator << (MaskBitmap.get());
	return *this;
}
//---------------------------------------------------------------------------
inline TSerialData& TSerialData::operator << (const TIcon *Value)
{
	throw Exception("Unable to stream const TIcon");
}
//---------------------------------------------------------------------------
inline void TSerialData::operator >> (TIcon *Value) const
{
	std::auto_ptr<Graphics::TBitmap> IconBitmap(new Graphics::TBitmap);
	std::auto_ptr<Graphics::TBitmap> MaskBitmap(new Graphics::TBitmap);
	operator >> (IconBitmap.get());
	operator >> (MaskBitmap.get());

	std::auto_ptr<TImageList> ImageList(new TImageList(NULL));
	ImageList->Width	= IconBitmap->Width;
	ImageList->Height	= IconBitmap->Height;
	if (MaskBitmap->Width == IconBitmap->Width)
	{
		ImageList->Add(IconBitmap.get(), MaskBitmap.get());
	}
	else
	{
		ImageList->Add(IconBitmap.get(), NULL);
	}
	ImageList->GetIcon(0, Value);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
inline TSerialData& TSerialData::operator << (Graphics::TBitmap *Value)
{
	std::auto_ptr<TMemoryStream> BMPStream(new TMemoryStream);
	Value->SaveToStream(BMPStream.get());
	BMPStream->Position = 0;
	operator << (BMPStream.get());
	return *this;
}
//---------------------------------------------------------------------------
inline TSerialData& TSerialData::operator << (const Graphics::TBitmap *Value)
{
	throw Exception("Unable to stream const TBitmap");
}
//---------------------------------------------------------------------------
inline void TSerialData::operator >> (Graphics::TBitmap *Value) const
{
	std::auto_ptr<TMemoryStream> BMPStream(new TMemoryStream);
	operator >> (BMPStream.get());
	BMPStream->Position = 0;

	Value->LoadFromStream(BMPStream.get());
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
inline TSerialData& TSerialData::operator << (TStream *Value)
{
	Value->Position = 0;
	int Length = Value->Size;
	fData->Write(&Length, sizeof(Length));
	if (Length > 0)
	{
		fData->CopyFrom(Value, Length);
	}
	return *this;
}
//---------------------------------------------------------------------------
inline void TSerialData::operator >> (TStream *Value) const
{
	int Length;
	fData->Read(&Length, sizeof(Length));
	if (Length > 0)
	{
		Value->CopyFrom(fData.get(), Length);
	}
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
/*inline TSerialData& TSerialData::operator << (const TStreamingData &Value)
{
	Value.SerializeWrite(*this);
	return *this;
}
//---------------------------------------------------------------------------
inline void TSerialData::operator >> (TStreamingData &Value) const
{
	Value.SerializeRead(*this);
}*/
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
inline template <typename T>
TSerialData & TSerialData::operator << (T Value)
{
//#error Unknown Data Type
//	throw Exception("Unknown Format");
	fData->Write(&Value, sizeof(T));
	return *this;
}
//---------------------------------------------------------------------------
inline template <typename T>
void TSerialData::operator >> (T &Value) const
{
//#error Unknown Data Type
//	throw Exception("Unknown Format");
	fData->Read(&Value, sizeof(T));
}
//---------------------------------------------------------------------------
#endif
