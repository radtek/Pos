//---------------------------------------------------------------------------

#ifndef GraphicPrinterSettingsH
#define GraphicPrinterSettingsH
//---------------------------------------------------------------------------
#include <system.hpp>

class TGraphicPrinterSettings
{
   public :
   TGraphicPrinterSettings() : GraphicPrinter(false),
                               GraphicFontName("Terminal"),
                               GraphicFontSize(12),
                               GraphicHeaderX(0),
                               GraphicHeaderY(0),
                               GraphicHeaderWidth(0),
                               GraphicHeaderHeight(0),
                               GraphicHeaderFile(""),
                               GraphicReceiptX(0),
                               GraphicReceiptY(0),
                               GraphicReceiptMaxLines(30),
                               GraphicBackGroundX(0),
                               GraphicBackGroundY(0),
                               GraphicBackGroundWidth(0),
                               GraphicBackGroundHeight(0),
                               GraphicBackGroundFile(""),
                               GraphicPageCountX(-1),
                               GraphicPageCountY(-1) {};
   bool GraphicPrinter;
	UnicodeString GraphicFontName;
	int GraphicFontSize;

   int GraphicHeaderX;
   int GraphicHeaderY;
   int GraphicHeaderWidth;
   int GraphicHeaderHeight;
	UnicodeString GraphicHeaderFile;

   int GraphicReceiptX;
   int GraphicReceiptY;
   int GraphicPageCountX;
   int GraphicPageCountY;
   int GraphicReceiptMaxLines;

   int GraphicBackGroundX;
   int GraphicBackGroundY;
   int GraphicBackGroundWidth;
   int GraphicBackGroundHeight;
	UnicodeString GraphicBackGroundFile;

};

#endif
