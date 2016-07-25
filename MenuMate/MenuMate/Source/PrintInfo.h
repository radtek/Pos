//---------------------------------------------------------------------------

#ifndef PrintInfoH
#define PrintInfoH

//---------------------------------------------------------------------------
enum TFontSize {fsNormalSize, fsDoubleSize};
enum TFontColour {fcBlack, fcRed};
enum TFontType {ftFontA, ftFontB};

class TPrintInfo
{
	public :
		bool Underlined;
		bool Bold;
		TFontSize 	Width;
		TFontSize 	Height;
		TFontType 	Font;
		TFontColour Colour;
	TPrintInfo();
	void Reset();
};

#endif

