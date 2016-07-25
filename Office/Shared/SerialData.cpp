//---------------------------------------------------------------------------
#pragma hdrstop

#include "SerialData.h"
#include "Zlib.h"
#include "Rijndael.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace Zlib;
//---------------------------------------------------------------------------
TSerialData &TSerialData::Compress(int Level)
{
	std::auto_ptr<TZlib>Zlib(new TZlib);
	std::auto_ptr<TMemoryStream>Output(new TMemoryStream);

	fData->Position = 0;

	Zlib->Compress(fData.get(), Output.get(), 9);

	fData = Output;
	fData->Position = 0;

	return *this;
}
//---------------------------------------------------------------------------
void TSerialData::Decompress()
{
	std::auto_ptr<TZlib>Zlib(new TZlib);
	std::auto_ptr<TMemoryStream>Output(new TMemoryStream);

	fData->Position = 0;

	Zlib->Decompress(fData.get(), Output.get());

	fData = Output;
	fData->Position = 0;
}
//---------------------------------------------------------------------------
TSerialData &TSerialData::Encrypt(AnsiString Key)
{
	if (Key.Length() > 16)
	{
		Key = Key.SubString(1, 16);	// No more than 16 chars.
	}
	if (Key.Length() < 16)
	{
		Key = Key + AnsiString::StringOfChar(' ', 16 - Key.Length());
	}
	const int PadBoundary = 16;

	CRijndael oRijndael;
	oRijndael.MakeKey(Key.c_str(), CRijndael::sm_chain0, 16, 16);

	int Size = fData->Size;

	int PadSize = PadBoundary - (Size%PadBoundary) - 4;
	if (PadSize < 0) PadSize += PadBoundary;
	if (PadSize > 0)
	{
		int Padding[PadBoundary] = {0};
		fData->Position = fData->Size;
		fData->Write(Padding, PadSize);
	}
	AddCRC();

	int BufferSize	= fData->Size;
	char *Buffer	= new char[BufferSize];

	try
	{
		oRijndael.Encrypt(static_cast<char *>(fData->Memory), Buffer, BufferSize, CRijndael::ECB);

		fData->Clear();
		fData->Write(Buffer, BufferSize);
		fData->Position = 0;
	}
	__finally
	{
		delete [] Buffer;
	}
	return *this;
}
//---------------------------------------------------------------------------
bool TSerialData::Decrypt(AnsiString Key)
{
	if (Key.Length() > 16)
	{
		Key = Key.SubString(1, 16);	// No more than 16 chars.
	}
	if (Key.Length() < 16)
	{
		Key = Key + AnsiString::StringOfChar(' ', 16 - Key.Length());
	}
	CRijndael oRijndael;
	oRijndael.MakeKey(Key.c_str(), CRijndael::sm_chain0, 16, 16);

	int BufferSize	= fData->Size;
	char *Buffer	= new char[BufferSize];
	try
	{
		oRijndael.Decrypt(static_cast<char *>(fData->Memory), Buffer, BufferSize, CRijndael::ECB);

		fData->Clear();
		fData->Write(Buffer, BufferSize);
		fData->Position = 0;
	}
	__finally
	{
		delete [] Buffer;
	}
	return CheckCRC();
}
//---------------------------------------------------------------------------
void TSerialData::AddCRC()
{
	DWORD Crc32 = CRC((char *)fData->Memory, fData->Size);

	fData->Position = fData->Size;
	fData->Write(&Crc32, sizeof(Crc32));
}
//---------------------------------------------------------------------------
bool TSerialData::CheckCRC()
{
	int CRCPos = fData->Size-sizeof(DWORD);

	DWORD Crc32 = CRC((char *)fData->Memory, CRCPos);

	return *(DWORD *)(static_cast<BYTE *>(fData->Memory) + CRCPos) == Crc32;
}
//---------------------------------------------------------------------------
int TSerialData::GetPosition()
{
	return fData->Position;
}
//---------------------------------------------------------------------------
void TSerialData::SetPosition(int Level)
{
	fData->Position = Level;
}
//---------------------------------------------------------------------------

