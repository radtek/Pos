//---------------------------------------------------------------------------
#ifndef PoleDisplaysH
#define PoleDisplaysH
//---------------------------------------------------------------------------
#include <CPort.hpp>


class TPoleDisplayType
{
	public:
	virtual void UpdatePoleDisplay(AnsiString TopLine,AnsiString BottomLine,TComPort *PoleDisplayPort) = 0;
};

class TPoleDisplayTypeGeneric : public TPoleDisplayType
{
	public:
	void UpdatePoleDisplay(AnsiString TopLine,AnsiString BottomLine,TComPort *PoleDisplayPort);
};

class TPoleDisplayTypeBirch : public TPoleDisplayType
{
	public:
	void UpdatePoleDisplay(AnsiString TopLine,AnsiString BottomLine,TComPort *PoleDisplayPort);
};

class TPoleDisplayTypeDSP810 : public TPoleDisplayType
{
	public:
	void UpdatePoleDisplay(AnsiString TopLine,AnsiString BottomLine,TComPort *PoleDisplayPort);
};

class TPoleDisplayTypeDSP800F : public TPoleDisplayType
{
	public:
	void UpdatePoleDisplay(AnsiString TopLine,AnsiString BottomLine,TComPort *PoleDisplayPort);
};


#endif
