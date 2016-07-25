#ifndef enumPrintingH
#define enumPrintingH

#define Max_LargeFont_NameSize 8
#define Max_SmallFont_NameSize 20
#define PRINTER_WIDTH_MIN 15
#define PRINTER_WIDTH_MAX 50


enum PrinterType { prnUnknown = 0 ,prnScreen = 1,A715 = 0x08, A714 = 0x0D}; // 714 Impact //715 Thermial .

enum PrintJobType{pjInit,pjReceiptReceipt,pjReceiptInvoice,pjKitchen,pjLoyaltyActivationReceipt};

enum PrintJobCommands{pjOpen = 0x80, pjClose, pjReset,pjNewLine,pjReverseFeed,
								pjTab,pjAdd,pjAddLines,pjAddLine,pjAddRightJustify,pjAddCenter,
								pjCutFull,pjCutPartial,pjDrawLine,pjTxtFont,pjKickDraw};

enum PrintJobFont{pjfntLarge = 0x01,pjfntBold = 0x02,pjfntUnderLined = 0x04,pjfntRed = 0x08};

enum PrintJobRequestType{epjRequest,epjResponse,epjRedirection};

#endif