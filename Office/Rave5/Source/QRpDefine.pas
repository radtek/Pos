{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRpDefine;

interface

{.$DEFINE DBG} { Used for data connection thread debugging }

{$I RpVer.inc}

uses
{$IFDEF DEMO}
  Dialogs,
{$ENDIF}
  Windows,
  Qt, QGraphics, QTypes,
  Math, DB, SysUtils, Classes;

{ Status format characters }
{ %% - % character }
{ %p - Current Page }
{ %f - First Page }
{ %l - Last Page }
{ %d - Printer Device Name }
{ %r - Printer Driver Name }
{ %t - Printer Port }
{ %c - Current Pass - used for printers that don't support multiple pages }
{ %s - Total Passes - used for printers that don't support multiple pages }
{ %0 through %9 - Status Text Line }

const
{$IFDEF DEMO}
  RunMode = '/Demo';
{$ELSE}
  RunMode = '';
{$ENDIF}

{$IFDEF VER120}
  CompVer = '4';
{$ENDIF}
{$IFDEF VER125}
  CompVer = '4';
{$ENDIF}
{$IFDEF VER130}
  CompVer = '5';
{$ENDIF}
{$IFDEF VER140}
  CompVer = '6';
{$ENDIF}
{$IFDEF VER150}
  CompVer = '7';
{$ENDIF}

  LibType = 'CLX';

  RvMajorVersion = '5';
  RvMinorVersion = '1';
  RvSubVersion = '2';

  RPVersion = RvMajorVersion + '.' + RvMinorVersion + '.' + RvSubVersion + ' (' + LibType + CompVer + RunMode + ')'; { Current revision number }

  NA = -32768; { n/a value }

  BOXLINENONE = 0;
  BOXLINELEFT = 1;
  BOXLINERIGHT = 2;
  BOXLINELEFTRIGHT = 3;
  BOXLINETOP = 4;
  BOXLINELEFTTOP = 5;
  BOXLINERIGHTTOP = 6;
  BOXLINENOBOTTOM = 7;
  BOXLINEBOTTOM = 8;
  BOXLINELEFTBOTTOM = 9;
  BOXLINERIGHTBOTTOM = 10;
  BOXLINENOTOP = 11;
  BOXLINETOPBOTTOM = 12;
  BOXLINENORIGHT = 13;
  BOXLINENOLEFT = 14;
  BOXLINEALL = 15;

  BOXLINEHORIZ = BOXLINETOPBOTTOM; { Don't use, replaced by BOXLINETOPBOTTOM }
  BOXLINEVERT = BOXLINELEFTRIGHT; { Don't use, replaced by BOXLINELEFTRIGHT }

  MAXSAVEFONT = 10;
  MAXSAVETABS = 10;
  MAXSAVEPOS = 10;

  MAXWORDLEN = 65520;
  MAXBUFSIZE = 2000000000;

  MINPREVIEWZOOM = 10.0;
  MAXPREVIEWZOOM = 200.0;

{ ReportPrinter Commands }
{ GDI commands }
  rpcArc = 1;
  rpcBrushCopy = 2;
  rpcChord = 3;
  rpcCopyRect = 4;
  rpcDraw = 5;
  rpcDrawFocusRect = 6;
  rpcEllipse = 7;
  rpcFillRect = 8;
  rpcFloodFill = 9;
  rpcFrameRect = 10;
  rpcLineTo = 11;
  rpcMoveTo = 12;
  rpcPie = 13;
  rpcPolygon = 14;
  rpcPolyline = 15;
  rpcPrintBitmap = 16;
  rpcPrintBitmapRect = 17;
  rpcRectangle = 18;
  rpcTabRectangle = 19;
  rpcRoundRect = 20;
  rpcStretchDraw = 21;
  rpcTextRect = 22;
  rpcCenterText = 23;
  rpcLeftText = 24;
  rpcRightText = 25;
  rpcSetTextBKMode = 26;
  rpcSetBKColor = 27;
  rpcSetScaleX = 28;
  rpcSetScaleY = 29;
  rpcPrintData = 30;
  rpcPrintDataStream = 31;
  rpcPrintSpaces = 32;
  rpcBlockText = 33;
  rpcTransBitmap = 34;
  rpcSetFrameMode = 35;
  rpcPrintImageRect = 36;
  rpcVariableData = 37;

{ Brush commands }
  rpcSetBrush = 40;
  rpcSetBrushColor = 41;
  rpcSetBrushStyle = 42;
  rpcSetBrushBitmap = 43;

{ Font commands }
  rpcSetFont = 50;
  rpcSetFontName = 51;
  rpcSetFontColor = 52;
  rpcSetFontPitch = 53;
  rpcSetFontHeight = 54;
  rpcSetFontBold = 55;
  rpcSetFontUnderline = 56;
  rpcSetFontItalic = 57;
  rpcSetFontStrikeout = 58;
  rpcSetFontSuperscript = 59;
  rpcSetFontSubscript = 60;
  rpcSetFontRotation = 61;
  rpcSetFontAlign = 62;
  rpcSetFontCharset = 63;

{ Pen commands }
  rpcSetPen = 70;
  rpcSetPenColor = 71;
  rpcSetPenMode = 72;
  rpcSetPenStyle = 73;
  rpcSetPenWidth = 74;

{ Misc commands }
  rpcNewPage = 80;
  rpcJobHeader11 = 81; { Old ReportPrinter 1.1 Header }
  rpcEndPage = 82;
  rpcResetLineHeight = 83;
  rpcAllowPreviewOnly = 84;
  rpcAllowPrinterOnly = 85;
  rpcAllowAll = 86;
  rpcSetOrientation = 87;
  rpcSetBin = 88;
  rpcJobHeader20i = 89; { Old ReportPriner Pro 2.0a-i Header }
  rpcJobHeader = 90; { ReportPriner Pro 2.0j- Header }

{ All user created commands should start here }
  rpcUser = 200;

  FontString =
   'abcdefg hijklmnop qrstuv wxyz ABCDEFG HIJKLMNOP QRSTUV WXYZ 0123456789';
 {$EXTERNALSYM FontString}

  RTFIgnoreDest: array[1..32] of string[10] = (
   'author','buptim','comment','creatim','doccomm',
   'footer','footerf','footerl','footerr','footnote',
   'ftncn','ftnsep','ftnsepc','header','headerf',
   'headerl','headerr','info','keywords','operator',
   'pict','printim','private1','revtim','rxe',
   'stylesheet','subject','tc','title','txe','xe','*');

{ *** RPTF Constants *** }
  RPTFPrefix = #1; { Begins an RPTF formatting block }
  RPTFInfix = #2; { Separates commands within an RPTF formatting blocks }
  RPTFSeparator = #3; { Used to separate parameters }
  RPTFSuffix = #4; { Ends an RPTF formatting block }

  RPTFOff = '0';
  RPTFOn = '1';
  RPTFLeft = 'L';
  RPTFCenter = 'C';
  RPTFRight = 'R';
  RPTFBlock = 'B';

{ RPTF Commands }
  RPTFIgnore = ' ';
  RPTFBold = 'B';
  RPTFItalic = 'I';
  RPTFUnderline = 'U';
  RPTFStrikeout = 'K';
  RPTFSuperscript = 'P';
  RPTFSubscript = 'S';
  RPTFName = 'N';
  RPTFSize = 'Z';
  RPTFColor = 'C';
  RPTFPitch = 'H';
  RPTFResetParagraph = 'R';
  RPTFResetStyle = 'r';
  RPTFJustify = 'J';
  RPTFFirstIndent = 'F';
  RPTFLeftIndent = 'L';
  RPTFRightIndent = 'G';

{ RPTF Macros }
{$IFDEF BCB}
  (*$HPPEMIT '#define BoldOn "\x01\x42\x31\x04"' *)
  (*$HPPEMIT '#define BoldOff "\x01\x42\x30\x04"' *)
  (*$HPPEMIT '#define ItalicOn "\x01\x49\x31\x04"' *)
  (*$HPPEMIT '#define ItalicOff "\x01\x49\x30\x04"' *)
  (*$HPPEMIT '#define UnderlineOn "\x01\x55\x31\x04"' *)
  (*$HPPEMIT '#define UnderlineOff "\x01\x55\x30\x04"' *)
  (*$HPPEMIT '#define StrikeoutOn "\x01\x4B\x31\x04"' *)
  (*$HPPEMIT '#define StrikeoutOff "\x01\x4B\x30\x04"' *)
  (*$HPPEMIT '#define SuperscriptOn "\x01\x50\x31\x04"' *)
  (*$HPPEMIT '#define SuperscriptOff "\x01\x50\x30\x04"' *)
  (*$HPPEMIT '#define SubscriptOn "\x01\x53\x31\x04"' *)
  (*$HPPEMIT '#define SubscriptOff "\x01\x53\x30\x04"' *)
  (*$HPPEMIT '#define JustifyLeft "\x01\x4A\x4C\x04"' *)
  (*$HPPEMIT '#define JustifyCenter "\x01\x4A\x43\x04"' *)
  (*$HPPEMIT '#define JustifyRight "\x01\x4A\x52\x04"' *)
  (*$HPPEMIT '#define JustifyBlock "\x01\x4A\x42\x04"' *)
  (*$HPPEMIT '#undef ResetPrinter' *)
{$ELSE}
  BoldOn = RPTFPrefix + RPTFBold + RPTFOn + RPTFSuffix;
  BoldOff = RPTFPrefix + RPTFBold + RPTFOff + RPTFSuffix;
  ItalicOn = RPTFPrefix + RPTFItalic + RPTFOn + RPTFSuffix;
  ItalicOff = RPTFPrefix + RPTFItalic + RPTFOff + RPTFSuffix;
  UnderlineOn = RPTFPrefix + RPTFUnderline + RPTFOn + RPTFSuffix;
  UnderlineOff = RPTFPrefix + RPTFUnderline + RPTFOff + RPTFSuffix;
  StrikeoutOn = RPTFPrefix + RPTFStrikeout + RPTFOn + RPTFSuffix;
  StrikeoutOff = RPTFPrefix + RPTFStrikeout + RPTFOff + RPTFSuffix;
  SuperscriptOn = RPTFPrefix + RPTFSuperscript + RPTFOn + RPTFSuffix;
  SuperscriptOff = RPTFPrefix + RPTFSuperscript + RPTFOff + RPTFSuffix;
  SubscriptOn = RPTFPrefix + RPTFSubscript + RPTFOn + RPTFSuffix;
  SubscriptOff = RPTFPrefix + RPTFSubscript + RPTFOff + RPTFSuffix;
  JustifyLeft = RPTFPrefix + RPTFJustify + RPTFLeft + RPTFSuffix;
  JustifyCenter = RPTFPrefix + RPTFJustify + RPTFCenter + RPTFSuffix;
  JustifyRight = RPTFPrefix + RPTFJustify + RPTFRight + RPTFSuffix;
  JustifyBlock = RPTFPrefix + RPTFJustify + RPTFBlock + RPTFSuffix;
{$ENDIF}

type
  TRavePaperSize = type integer;
  TPaperSizeName = record
    Name: string;
    PaperSize: TRavePaperSize;
    PaperWidth: double;
    PaperHeight: double;
  end; { TPaperSizeName }

const

{ paper selections }

  {$EXTERNALSYM DMPAPER_LETTER}
  DMPAPER_LETTER      = 1;  { Letter 8 12 x 11 in               }
  {$EXTERNALSYM DMPAPER_FIRST}
  DMPAPER_FIRST       = DMPAPER_LETTER;
  {$EXTERNALSYM DMPAPER_LETTERSMALL}
  DMPAPER_LETTERSMALL = 2;  { Letter Small 8 12 x 11 in         }
  {$EXTERNALSYM DMPAPER_TABLOID}
  DMPAPER_TABLOID     = 3;  { Tabloid 11 x 17 in                }
  {$EXTERNALSYM DMPAPER_LEDGER}
  DMPAPER_LEDGER      = 4;  { Ledger 17 x 11 in                 }
  {$EXTERNALSYM DMPAPER_LEGAL}
  DMPAPER_LEGAL       = 5;  { Legal 8 12 x 14 in                }
  {$EXTERNALSYM DMPAPER_STATEMENT}
  DMPAPER_STATEMENT   = 6;  { Statement 5 12 x 8 12 in          }
  {$EXTERNALSYM DMPAPER_EXECUTIVE}
  DMPAPER_EXECUTIVE   = 7;  { Executive 7 14 x 10 12 in         }
  {$EXTERNALSYM DMPAPER_A3}
  DMPAPER_A3      = 8;      { A3 297 x 420 mm                     }
  {$EXTERNALSYM DMPAPER_A4}
  DMPAPER_A4      = 9;      { A4 210 x 297 mm                     }
  {$EXTERNALSYM DMPAPER_A4SMALL}
  DMPAPER_A4SMALL = 10;     { A4 Small 210 x 297 mm               }
  {$EXTERNALSYM DMPAPER_A5}
  DMPAPER_A5      = 11;     { A5 148 x 210 mm                     }
  {$EXTERNALSYM DMPAPER_B4}
  DMPAPER_B4      = 12;     { B4 (JIS) 250 x 354                  }
  {$EXTERNALSYM DMPAPER_B5}
  DMPAPER_B5      = 13;     { B5 (JIS) 182 x 257 mm               }
  {$EXTERNALSYM DMPAPER_FOLIO}
  DMPAPER_FOLIO   = 14;     { Folio 8 12 x 13 in                  }
  {$EXTERNALSYM DMPAPER_QUARTO}
  DMPAPER_QUARTO  = 15;     { Quarto 215 x 275 mm                 }
  {$EXTERNALSYM DMPAPER_10X14}
  DMPAPER_10X14   = 16;     { 10x14 in                            }
  {$EXTERNALSYM DMPAPER_11X17}
  DMPAPER_11X17   = 17;     { 11x17 in                            }
  {$EXTERNALSYM DMPAPER_NOTE}
  DMPAPER_NOTE    = 18;     { Note 8 12 x 11 in                   }
  {$EXTERNALSYM DMPAPER_ENV_9}
  DMPAPER_ENV_9   = 19;     { Envelope #9 3 78 x 8 78             }
  {$EXTERNALSYM DMPAPER_ENV_10}
  DMPAPER_ENV_10  = 20;     { Envelope #10 4 18 x 9 12            }
  {$EXTERNALSYM DMPAPER_ENV_11}
  DMPAPER_ENV_11  = 21;     { Envelope #11 4 12 x 10 38           }
  {$EXTERNALSYM DMPAPER_ENV_12}
  DMPAPER_ENV_12  = 22;     { Envelope #12 4 \276 x 11            }
  {$EXTERNALSYM DMPAPER_ENV_14}
  DMPAPER_ENV_14  = 23;     { Envelope #14 5 x 11 12              }
  {$EXTERNALSYM DMPAPER_CSHEET}
  DMPAPER_CSHEET  = 24;     { C size sheet                        }
  {$EXTERNALSYM DMPAPER_DSHEET}
  DMPAPER_DSHEET  = 25;     { D size sheet                        }
  {$EXTERNALSYM DMPAPER_ESHEET}
  DMPAPER_ESHEET  = 26;     { E size sheet                        }
  {$EXTERNALSYM DMPAPER_ENV_DL}
  DMPAPER_ENV_DL  = 27;     { Envelope DL 110 x 220mm             }
  {$EXTERNALSYM DMPAPER_ENV_C5}
  DMPAPER_ENV_C5  = 28;     { Envelope C5 162 x 229 mm            }
  {$EXTERNALSYM DMPAPER_ENV_C3}
  DMPAPER_ENV_C3  = 29;     { Envelope C3  324 x 458 mm           }
  {$EXTERNALSYM DMPAPER_ENV_C4}
  DMPAPER_ENV_C4  = 30;     { Envelope C4  229 x 324 mm           }
  {$EXTERNALSYM DMPAPER_ENV_C6}
  DMPAPER_ENV_C6  = 31;     { Envelope C6  114 x 162 mm           }
  {$EXTERNALSYM DMPAPER_ENV_C65}
  DMPAPER_ENV_C65  = 32;    { Envelope C65 114 x 229 mm           }
  {$EXTERNALSYM DMPAPER_ENV_B4}
  DMPAPER_ENV_B4   = 33;    { Envelope B4  250 x 353 mm           }
  {$EXTERNALSYM DMPAPER_ENV_B5}
  DMPAPER_ENV_B5   = 34;    { Envelope B5  176 x 250 mm           }
  {$EXTERNALSYM DMPAPER_ENV_B6}
  DMPAPER_ENV_B6   = 35;    { Envelope B6  176 x 125 mm           }
  {$EXTERNALSYM DMPAPER_ENV_ITALY}
  DMPAPER_ENV_ITALY          = 36;  { Envelope 110 x 230 mm               }
  {$EXTERNALSYM DMPAPER_ENV_MONARCH}
  DMPAPER_ENV_MONARCH        = 37;  { Envelope Monarch 3.875 x 7.5 in     }
  {$EXTERNALSYM DMPAPER_ENV_PERSONAL}
  DMPAPER_ENV_PERSONAL       = 38;  { 6 34 Envelope 3 58 x 6 12 in        }
  {$EXTERNALSYM DMPAPER_FANFOLD_US}
  DMPAPER_FANFOLD_US         = 39;  { US Std Fanfold 14 78 x 11 in        }
  {$EXTERNALSYM DMPAPER_FANFOLD_STD_GERMAN}
  DMPAPER_FANFOLD_STD_GERMAN = 40;  { German Std Fanfold 8 12 x 12 in    }
  {$EXTERNALSYM DMPAPER_FANFOLD_LGL_GERMAN}
  DMPAPER_FANFOLD_LGL_GERMAN = 41;  { German Legal Fanfold 8 12 x 13 in  }
  {$EXTERNALSYM DMPAPER_ISO_B4}
  DMPAPER_ISO_B4             = 42;  { B4 (ISO) 250 x 353 mm               }
  {$EXTERNALSYM DMPAPER_JAPANESE_POSTCARD}
  DMPAPER_JAPANESE_POSTCARD  = 43;  { Japanese Postcard 100 x 148 mm      }
  {$EXTERNALSYM DMPAPER_9X11}
  DMPAPER_9X11               = 44;  { 9 x 11 in                           }
  {$EXTERNALSYM DMPAPER_10X11}
  DMPAPER_10X11              = 45;  { 10 x 11 in                          }
  {$EXTERNALSYM DMPAPER_15X11}
  DMPAPER_15X11              = 46;  { 15 x 11 in                          }
  {$EXTERNALSYM DMPAPER_ENV_INVITE}
  DMPAPER_ENV_INVITE         = 47;  { Envelope Invite 220 x 220 mm        }
  {$EXTERNALSYM DMPAPER_RESERVED_48}
  DMPAPER_RESERVED_48        = 48;  { RESERVED--DO NOT USE                }
  {$EXTERNALSYM DMPAPER_RESERVED_49}
  DMPAPER_RESERVED_49        = 49;  { RESERVED--DO NOT USE                }
  {$EXTERNALSYM DMPAPER_LETTER_EXTRA}
  DMPAPER_LETTER_EXTRA       = 50;  { Letter Extra 9 \275 x 12 in         }
  {$EXTERNALSYM DMPAPER_LEGAL_EXTRA}
  DMPAPER_LEGAL_EXTRA        = 51;  { Legal Extra 9 \275 x 15 in          }
  {$EXTERNALSYM DMPAPER_TABLOID_EXTRA}
  DMPAPER_TABLOID_EXTRA      = 52;  { Tabloid Extra 11.69 x 18 in         }
  {$EXTERNALSYM DMPAPER_A4_EXTRA}
  DMPAPER_A4_EXTRA           = 53;  { A4 Extra 9.27 x 12.69 in            }
  {$EXTERNALSYM DMPAPER_LETTER_TRANSVERSE}
  DMPAPER_LETTER_TRANSVERSE  = 54;  { Letter Transverse 8 \275 x 11 in    }
  {$EXTERNALSYM DMPAPER_A4_TRANSVERSE}
  DMPAPER_A4_TRANSVERSE      = 55;  { A4 Transverse 210 x 297 mm          }
  {$EXTERNALSYM DMPAPER_LETTER_EXTRA_TRANSVERSE}
  DMPAPER_LETTER_EXTRA_TRANSVERSE = 56;     { Letter Extra Transverse 9\275 x 12 in  }
  {$EXTERNALSYM DMPAPER_A_PLUS}
  DMPAPER_A_PLUS        = 57;     { SuperASuperAA4 227 x 356 mm       }
  {$EXTERNALSYM DMPAPER_B_PLUS}
  DMPAPER_B_PLUS        = 58;     { SuperBSuperBA3 305 x 487 mm       }
  {$EXTERNALSYM DMPAPER_LETTER_PLUS}
  DMPAPER_LETTER_PLUS   = 59;     { Letter Plus 8.5 x 12.69 in          }
  {$EXTERNALSYM DMPAPER_A4_PLUS}
  DMPAPER_A4_PLUS       = 60;     { A4 Plus 210 x 330 mm                }
  {$EXTERNALSYM DMPAPER_A5_TRANSVERSE}
  DMPAPER_A5_TRANSVERSE = 61;     { A5 Transverse 148 x 210 mm          }
  {$EXTERNALSYM DMPAPER_B5_TRANSVERSE}
  DMPAPER_B5_TRANSVERSE = 62;     { B5 (JIS) Transverse 182 x 257 mm    }
  {$EXTERNALSYM DMPAPER_A3_EXTRA}
  DMPAPER_A3_EXTRA      = 63;     { A3 Extra 322 x 445 mm               }
  {$EXTERNALSYM DMPAPER_A5_EXTRA}
  DMPAPER_A5_EXTRA      = $40;    { A5 Extra 174 x 235 mm               }
  {$EXTERNALSYM DMPAPER_B5_EXTRA}
  DMPAPER_B5_EXTRA      = 65;     { B5 (ISO) Extra 201 x 276 mm         }
  {$EXTERNALSYM DMPAPER_A2}
  DMPAPER_A2            = 66;     { A2 420 x 594 mm                     }
  {$EXTERNALSYM DMPAPER_A3_TRANSVERSE}
  DMPAPER_A3_TRANSVERSE = 67;     { A3 Transverse 297 x 420 mm          }
  {$EXTERNALSYM DMPAPER_A3_EXTRA_TRANSVERSE}
  DMPAPER_A3_EXTRA_TRANSVERSE = 68;     { A3 Extra Transverse 322 x 445 mm    }

  {$EXTERNALSYM DMPAPER_DBL_JAPANESE_POSTCARD}
  DMPAPER_DBL_JAPANESE_POSTCARD = 69; { Japanese Double Postcard 200 x 148 mm }

  {$EXTERNALSYM DMPAPER_A6}
  DMPAPER_A6                  = 70;  { A6 105 x 148 mm                 }

  {$EXTERNALSYM DMPAPER_JENV_KAKU2          }
  DMPAPER_JENV_KAKU2          = 71;  { Japanese Envelope Kaku #2       }
  {$EXTERNALSYM DMPAPER_JENV_KAKU3          }
  DMPAPER_JENV_KAKU3          = 72;  { Japanese Envelope Kaku #3       }
  {$EXTERNALSYM DMPAPER_JENV_CHOU3          }
  DMPAPER_JENV_CHOU3          = 73;  { Japanese Envelope Chou #3       }
  {$EXTERNALSYM DMPAPER_JENV_CHOU4          }
  DMPAPER_JENV_CHOU4          = 74;  { Japanese Envelope Chou #4       }
  {$EXTERNALSYM DMPAPER_LETTER_ROTATED      }
  DMPAPER_LETTER_ROTATED      = 75;  { Letter Rotated 11 x 8 1/2 11 in }
  {$EXTERNALSYM DMPAPER_A3_ROTATED          }
  DMPAPER_A3_ROTATED          = 76;  { A3 Rotated 420 x 297 mm         }
  {$EXTERNALSYM DMPAPER_A4_ROTATED          }
  DMPAPER_A4_ROTATED          = 77;  { A4 Rotated 297 x 210 mm         }
  {$EXTERNALSYM DMPAPER_A5_ROTATED          }
  DMPAPER_A5_ROTATED          = 78;  { A5 Rotated 210 x 148 mm         }
  {$EXTERNALSYM DMPAPER_B4_JIS_ROTATED      }
  DMPAPER_B4_JIS_ROTATED      = 79;  { B4 (JIS) Rotated 364 x 257 mm   }
  {$EXTERNALSYM DMPAPER_B5_JIS_ROTATED      }
  DMPAPER_B5_JIS_ROTATED      = 80;  { B5 (JIS) Rotated 257 x 182 mm   }
  {$EXTERNALSYM DMPAPER_JAPANESE_POSTCARD_ROTATED }
  DMPAPER_JAPANESE_POSTCARD_ROTATED = 81; { Japanese Postcard Rotated 148 x 100 mm }
  {$EXTERNALSYM DMPAPER_DBL_JAPANESE_POSTCARD_ROTATED }
  DMPAPER_DBL_JAPANESE_POSTCARD_ROTATED = 82; { Double Japanese Postcard Rotated 148 x 200 mm }
  {$EXTERNALSYM DMPAPER_A6_ROTATED          }
  DMPAPER_A6_ROTATED          = 83;  { A6 Rotated 148 x 105 mm         }
  {$EXTERNALSYM DMPAPER_JENV_KAKU2_ROTATED  }
  DMPAPER_JENV_KAKU2_ROTATED  = 84;  { Japanese Envelope Kaku #2 Rotated}
  {$EXTERNALSYM DMPAPER_JENV_KAKU3_ROTATED  }
  DMPAPER_JENV_KAKU3_ROTATED  = 85;  { Japanese Envelope Kaku #3 Rotated}
  {$EXTERNALSYM DMPAPER_JENV_CHOU3_ROTATED  }
  DMPAPER_JENV_CHOU3_ROTATED  = 86;  { Japanese Envelope Chou #3 Rotated}
  {$EXTERNALSYM DMPAPER_JENV_CHOU4_ROTATED  }
  DMPAPER_JENV_CHOU4_ROTATED  = 87;  { Japanese Envelope Chou #4 Rotated}
  {$EXTERNALSYM DMPAPER_B6_JIS              }
  DMPAPER_B6_JIS              = 88;  { B6 (JIS) 128 x 182 mm           }
  {$EXTERNALSYM DMPAPER_B6_JIS_ROTATED      }
  DMPAPER_B6_JIS_ROTATED      = 89;  { B6 (JIS) Rotated 182 x 128 mm   }
  {$EXTERNALSYM DMPAPER_12X11               }
  DMPAPER_12X11               = 90;  { 12 x 11 in                      }
  {$EXTERNALSYM DMPAPER_JENV_YOU4           }
  DMPAPER_JENV_YOU4           = 91;  { Japanese Envelope You #4        }
  {$EXTERNALSYM DMPAPER_JENV_YOU4_ROTATED   }
  DMPAPER_JENV_YOU4_ROTATED   = 92;  { Japanese Envelope You #4 Rotated}
  {$EXTERNALSYM DMPAPER_P16K                }
  DMPAPER_P16K                = 93;  { PRC 16K 146 x 215 mm            }
  {$EXTERNALSYM DMPAPER_P32K                }
  DMPAPER_P32K                = 94;  { PRC 32K 97 x 151 mm             }
  {$EXTERNALSYM DMPAPER_P32KBIG             }
  DMPAPER_P32KBIG             = 95;  { PRC 32K(Big) 97 x 151 mm        }
  {$EXTERNALSYM DMPAPER_PENV_1              }
  DMPAPER_PENV_1              = 96;  { PRC Envelope #1 102 x 165 mm    }
  {$EXTERNALSYM DMPAPER_PENV_2              }
  DMPAPER_PENV_2              = 97;  { PRC Envelope #2 102 x 176 mm    }
  {$EXTERNALSYM DMPAPER_PENV_3              }
  DMPAPER_PENV_3              = 98;  { PRC Envelope #3 125 x 176 mm    }
  {$EXTERNALSYM DMPAPER_PENV_4              }
  DMPAPER_PENV_4              = 99;  { PRC Envelope #4 110 x 208 mm    }
  {$EXTERNALSYM DMPAPER_PENV_5              }
  DMPAPER_PENV_5              = 100; { PRC Envelope #5 110 x 220 mm    }
  {$EXTERNALSYM DMPAPER_PENV_6              }
  DMPAPER_PENV_6              = 101; { PRC Envelope #6 120 x 230 mm    }
  {$EXTERNALSYM DMPAPER_PENV_7              }
  DMPAPER_PENV_7              = 102; { PRC Envelope #7 160 x 230 mm    }
  {$EXTERNALSYM DMPAPER_PENV_8              }
  DMPAPER_PENV_8              = 103; { PRC Envelope #8 120 x 309 mm    }
  {$EXTERNALSYM DMPAPER_PENV_9              }
  DMPAPER_PENV_9              = 104; { PRC Envelope #9 229 x 324 mm    }
  {$EXTERNALSYM DMPAPER_PENV_10             }
  DMPAPER_PENV_10             = 105; { PRC Envelope #10 324 x 458 mm   }
  {$EXTERNALSYM DMPAPER_P16K_ROTATED        }
  DMPAPER_P16K_ROTATED        = 106; { PRC 16K Rotated                 }
  {$EXTERNALSYM DMPAPER_P32K_ROTATED        }
  DMPAPER_P32K_ROTATED        = 107; { PRC 32K Rotated                 }
  {$EXTERNALSYM DMPAPER_P32KBIG_ROTATED     }
  DMPAPER_P32KBIG_ROTATED     = 108; { PRC 32K(Big) Rotated            }
  {$EXTERNALSYM DMPAPER_PENV_1_ROTATED      }
  DMPAPER_PENV_1_ROTATED      = 109; { PRC Envelope #1 Rotated 165 x 102 mm}
  {$EXTERNALSYM DMPAPER_PENV_2_ROTATED      }
  DMPAPER_PENV_2_ROTATED      = 110; { PRC Envelope #2 Rotated 176 x 102 mm}
  {$EXTERNALSYM DMPAPER_PENV_3_ROTATED      }
  DMPAPER_PENV_3_ROTATED      = 111; { PRC Envelope #3 Rotated 176 x 125 mm}
  {$EXTERNALSYM DMPAPER_PENV_4_ROTATED      }
  DMPAPER_PENV_4_ROTATED      = 112; { PRC Envelope #4 Rotated 208 x 110 mm}
  {$EXTERNALSYM DMPAPER_PENV_5_ROTATED      }
  DMPAPER_PENV_5_ROTATED      = 113; { PRC Envelope #5 Rotated 220 x 110 mm}
  {$EXTERNALSYM DMPAPER_PENV_6_ROTATED      }
  DMPAPER_PENV_6_ROTATED      = 114; { PRC Envelope #6 Rotated 230 x 120 mm}
  {$EXTERNALSYM DMPAPER_PENV_7_ROTATED      }
  DMPAPER_PENV_7_ROTATED      = 115; { PRC Envelope #7 Rotated 230 x 160 mm}
  {$EXTERNALSYM DMPAPER_PENV_8_ROTATED      }
  DMPAPER_PENV_8_ROTATED      = 116; { PRC Envelope #8 Rotated 309 x 120 mm}
  {$EXTERNALSYM DMPAPER_PENV_9_ROTATED      }
  DMPAPER_PENV_9_ROTATED      = 117; { PRC Envelope #9 Rotated 324 x 229 mm}
  {$EXTERNALSYM DMPAPER_PENV_10_ROTATED     }
  DMPAPER_PENV_10_ROTATED     = 118; { PRC Envelope #10 Rotated 458 x 324 mm }
  {$EXTERNALSYM DMPAPER_LAST}
  DMPAPER_LAST                = DMPAPER_PENV_10_ROTATED;
  {$EXTERNALSYM DMPAPER_USER}
  DMPAPER_USER                = $100;

{ bin selections }

  {$EXTERNALSYM DMBIN_UPPER}
  DMBIN_UPPER = 1;
  {$EXTERNALSYM DMBIN_FIRST}
  DMBIN_FIRST = DMBIN_UPPER;
  {$EXTERNALSYM DMBIN_ONLYONE}
  DMBIN_ONLYONE = 1;
  {$EXTERNALSYM DMBIN_LOWER}
  DMBIN_LOWER = 2;
  {$EXTERNALSYM DMBIN_MIDDLE}
  DMBIN_MIDDLE = 3;
  {$EXTERNALSYM DMBIN_MANUAL}
  DMBIN_MANUAL = 4;
  {$EXTERNALSYM DMBIN_ENVELOPE}
  DMBIN_ENVELOPE = 5;
  {$EXTERNALSYM DMBIN_ENVMANUAL}
  DMBIN_ENVMANUAL = 6;
  {$EXTERNALSYM DMBIN_AUTO}
  DMBIN_AUTO = 7;
  {$EXTERNALSYM DMBIN_TRACTOR}
  DMBIN_TRACTOR = 8;
  {$EXTERNALSYM DMBIN_SMALLFMT}
  DMBIN_SMALLFMT = 9;
  {$EXTERNALSYM DMBIN_LARGEFMT}
  DMBIN_LARGEFMT = 10;
  {$EXTERNALSYM DMBIN_LARGECAPACITY}
  DMBIN_LARGECAPACITY = 11;
  {$EXTERNALSYM DMBIN_CASSETTE}
  DMBIN_CASSETTE = 14;
  {$EXTERNALSYM DMBIN_FORMSOURCE}
  DMBIN_FORMSOURCE = 15;
  {$EXTERNALSYM DMBIN_LAST}
  DMBIN_LAST = DMBIN_FORMSOURCE;
  {$EXTERNALSYM DMBIN_USER}
  DMBIN_USER = $100;   { device specific bins start here }

type
  TMetafile = class(TGraphic)
  protected
    FImageData: TMemoryStream;
    FWidth: integer;
    FHeight: integer;
    FHandle: HENHMETAFILE;
    procedure Draw(ACanvas: TCanvas; const Rect: TRect); override;
    function GetEmpty: Boolean; override;
    function GetHeight: integer; override;
    function GetWidth: integer; override;
    procedure SetHeight(Value: integer); override;
    procedure SetWidth(Value: integer); override;
    procedure ReadData(Stream: TStream); override;
    procedure WriteData(Stream: TStream); override;
  public
    constructor Create; override;
    destructor Destroy; override;
    procedure Assign(Source: TPersistent); override;
    procedure LoadFromMimeSource(MimeSource: TMimeSource); override;
    procedure SaveToMimeSource(MimeSource: TClxMimeSource); override;
    procedure LoadFromStream(AStream: TStream); override;
    procedure SaveToStream(AStream: TStream); override;
    property ImageData: TMemoryStream read FImageData write FImageData;
    property Handle: HENHMETAFILE read FHandle;
  end;

const
  PaperSizeNameCnt = 43;
  PaperSizeName: array[1..PaperSizeNameCnt] of TPaperSizeName = (
   (Name:'Default'; PaperSize:-1;PaperWidth:0;PaperHeight:0),
   (Name:'Custom'; PaperSize:DMPAPER_USER;PaperWidth:0;PaperHeight:0),
   (Name:'Letter, 8 1/2- by 11-inches'; PaperSize:DMPAPER_LETTER;PaperWidth:8.5;PaperHeight:11.0),
   (Name:'Legal, 8 1/2- by 14-inches'; PaperSize:DMPAPER_LEGAL;PaperWidth:8.5;PaperHeight:14.0),
   (Name:'A4 Sheet, 210- by 297-millimeters'; PaperSize:DMPAPER_A4;PaperWidth:210/25.4;PaperHeight:297/25.4),
   (Name:'C Sheet, 17- by 22-inches'; PaperSize:DMPAPER_CSHEET;PaperWidth:17.0;PaperHeight:22.0),
   (Name:'D Sheet, 22- by 34-inches'; PaperSize:DMPAPER_DSHEET;PaperWidth:22.0;PaperHeight:34.0),
   (Name:'E Sheet, 34- by 44-inches'; PaperSize:DMPAPER_ESHEET;PaperWidth:34.0;PaperHeight:44.0),
   (Name:'Letter Small, 8 1/2- by 11-inches'; PaperSize:DMPAPER_LETTERSMALL;PaperWidth:8.5;PaperHeight:11.0),
   (Name:'Tabloid, 11- by 17-inches'; PaperSize:DMPAPER_TABLOID;PaperWidth:11.0;PaperHeight:17.0),
   (Name:'Ledger, 17- by 11-inches'; PaperSize:DMPAPER_LEDGER;PaperWidth:17.0;PaperHeight:11.0),
   (Name:'Statement, 5 1/2- by 8 1/2-inches'; PaperSize:DMPAPER_STATEMENT;PaperWidth:5.5;PaperHeight:8.5),
   (Name:'Executive, 7 1/4- by 10 1/2-inches'; PaperSize:DMPAPER_EXECUTIVE;PaperWidth:7.25;PaperHeight:10.5),
   (Name:'A3 sheet, 297- by 420-millimeters'; PaperSize:DMPAPER_A3;PaperWidth:297/25.4;PaperHeight:420/25.4),
   (Name:'A4 small sheet, 210- by 297-millimeters'; PaperSize:DMPAPER_A4SMALL;PaperWidth:210/25.4;PaperHeight:297/25.4),
   (Name:'A5 sheet, 148- by 210-millimeters'; PaperSize:DMPAPER_A5;PaperWidth:148/25.4;PaperHeight:210/25.4),
   (Name:'B4 sheet, 250- by 354-millimeters'; PaperSize:DMPAPER_B4;PaperWidth:250/25.4;PaperHeight:354/25.4),
   (Name:'B5 sheet, 182- by 257-millimeter paper'; PaperSize:DMPAPER_B5;PaperWidth:182/25.4;PaperHeight:257/25.4),
   (Name:'Folio, 8 1/2- by 13-inch paper'; PaperSize:DMPAPER_FOLIO;PaperWidth:8.5;PaperHeight:13.0),
   (Name:'Quarto, 215- by 275-millimeter paper'; PaperSize:DMPAPER_QUARTO;PaperWidth:215/25.4;PaperHeight:275/25.4),
   (Name:'10- by 14-inch sheet'; PaperSize:DMPAPER_10X14;PaperWidth:10.0;PaperHeight:14.0),
   (Name:'11- by 17-inch sheet'; PaperSize:DMPAPER_11X17;PaperWidth:11.0;PaperHeight:17.0),
   (Name:'Note, 8 1/2- by 11-inches'; PaperSize:DMPAPER_NOTE;PaperWidth:8.5;PaperHeight:11.0),
   (Name:'#9 Envelope, 3 7/8- by 8 7/8-inches'; PaperSize:DMPAPER_ENV_9;PaperWidth:3.875;PaperHeight:8.875),
   (Name:'#10 Envelope, 4 1/8- by 9 1/2-inches'; PaperSize:DMPAPER_ENV_10;PaperWidth:4.125;PaperHeight:9.5),
   (Name:'#11 Envelope, 4 1/2- by 10 3/8-inches'; PaperSize:DMPAPER_ENV_11;PaperWidth:4.5;PaperHeight:10.375),
   (Name:'#12 Envelope, 4 3/4- by 11-inches'; PaperSize:DMPAPER_ENV_12;PaperWidth:4.75;PaperHeight:11.0),
   (Name:'#14 Envelope, 5- by 11 1/2-inches'; PaperSize:DMPAPER_ENV_14;PaperWidth:5.0;PaperHeight:11.5),
   (Name:'DL Envelope, 110- by 220-millimeters'; PaperSize:DMPAPER_ENV_DL;PaperWidth:110/25.4;PaperHeight:220/25.4),
   (Name:'C5 Envelope, 162- by 229-millimeters'; PaperSize:DMPAPER_ENV_C5;PaperWidth:162/25.4;PaperHeight:229/25.4),
   (Name:'C3 Envelope,  324- by 458-millimeters'; PaperSize:DMPAPER_ENV_C3;PaperWidth:324/25.4;PaperHeight:458/25.4),
   (Name:'C4 Envelope,  229- by 324-millimeters'; PaperSize:DMPAPER_ENV_C4;PaperWidth:229/25.4;PaperHeight:324/25.4),
   (Name:'C6 Envelope,  114- by 162-millimeters'; PaperSize:DMPAPER_ENV_C6;PaperWidth:114/25.4;PaperHeight:162/25.4),
   (Name:'C65 Envelope, 114- by 229-millimeters'; PaperSize:DMPAPER_ENV_C65;PaperWidth:114/25.4;PaperHeight:229/25.4),
   (Name:'B4 Envelope,  250- by 353-millimeters'; PaperSize:DMPAPER_ENV_B4;PaperWidth:250/25.4;PaperHeight:353/25.4),
   (Name:'B5 Envelope,  176- by 250-millimeters'; PaperSize:DMPAPER_ENV_B5;PaperWidth:176/25.4;PaperHeight:250/25.4),
   (Name:'B6 Envelope,  176- by 125-millimeters'; PaperSize:DMPAPER_ENV_B6;PaperWidth:176/25.4;PaperHeight:125/25.4),
   (Name:'Italy Envelope, 110- by 230-millimeters'; PaperSize:DMPAPER_ENV_ITALY;PaperWidth:110/25.4;PaperHeight:230/25.4),
   (Name:'Monarch Envelope, 3 7/8- by 7 1/2-inches'; PaperSize:DMPAPER_ENV_MONARCH;PaperWidth:3.875;PaperHeight:7.5),
   (Name:'6 3/4 Envelope, 3 5/8- by 6 1/2-inches'; PaperSize:DMPAPER_ENV_PERSONAL;PaperWidth:3.626;PaperHeight:6.5),
   (Name:'US Std Fanfold, 14 7/8- by 11-inches'; PaperSize:DMPAPER_FANFOLD_US;PaperWidth:14.875;PaperHeight:12.0),
   (Name:'German Std Fanfold, 8 1/2- by 12-inches'; PaperSize:DMPAPER_FANFOLD_STD_GERMAN;PaperWidth:8.5;PaperHeight:12.0),
   (Name:'German Legal Fanfold, 8 1/2- by 13-inches'; PaperSize:DMPAPER_FANFOLD_LGL_GERMAN;PaperWidth:8.5;PaperHeight:13.0)
  );


type
  EReportPrinter = class(Exception);

  TPrintPageEvent = function(    Sender: TObject;
                             var PageNum: integer): boolean of object;
  TDecodeImageEvent = procedure(Sender: TObject;
                                ImageStream: TStream;
                                ImageType: string;
                                Bitmap: TBitmap) of object;

  TVersion = string;
  TTitleString = string;
  TFormatString = string;
  TDeviceState = (dsNone,dsIC,dsDC);
  TAccuracyMethod = (amPositioning,amAppearance);
  TMarginMethod = (mmScaled,mmFixed);
  TRulerType = (rtNone,rtHorizCm,rtVertCm,rtBothCm,rtHorizIn,rtVertIn,rtBothIn);
  TPrintJustify = (pjCenter,pjLeft,pjRight,pjBlock);
  TTabJustify = (tjCenter,tjLeft,tjRight,tjBlock,tjNone);
  TPrintJustifyVert = (pjTop,pjMiddle,pjBottom);
  TPrintUnits = (unInch,unMM,unCM,unPoint,unUser);
  TLineHeightMethod = (lhmLinesPerInch,lhmFont,lhmUser);
  TBKMode = (bkTransparent,bkOpaque);
  TDestination = (destPrinter,destPreview,destFile,destNone,destCustomFile);
  TStreamMode = (smMemory,smFile,smUser,smTempFile);
  TOrientation = (poPortrait,poLandScape,poDefault);
  TDuplex = (dupSimplex,dupVertical,dupHorizontal);
  TReportEventType = (reBeginPage,reEndPage);
  TFontAlign = (faBaseline,faTop,faBottom);
  TReportDest = (rdPreview,rdPrinter,rdFile);
  TSystemSetup = (ssAllowSetup,ssAllowCopies,ssAllowCollate,ssAllowDuplex,
   ssAllowDestPreview,ssAllowDestPrinter,ssAllowDestFile,ssAllowPrinterSetup,
   ssAllowPreviewSetup);
  TSystemOption = (soUseFiler,soWaitForOK,soShowStatus,soAllowPrintFromPreview,
   soPreviewModal,soNoGenerate);
  TBoxLines = (blNone,blLeft,blRight,blLeftRight,blTop,blLeftTop,blRightTop,
   blNoBottom,blBottom,blLeftBottom,blRightBottom,blNoTop,blTopBottom,blNoRight,
   blNoLeft,blAll);
  TMacroID = (midCurrDateShort,midCurrDateLong,midCurrDateUS,midCurrDateInter,
   midCurrTimeShort,midCurrTimeLong,midCurrTimeAMPM,midCurrTime24,midFirstPage,
   midLastPage,midTotalPages,midCurrentPage,midRelativePage,
   midPrinterName,midDriverName,midPortName,
   midUser01,midUser02,midUser03,midUser04,midUser05,midUser06,midUser07,
   midUser08,midUser09,midUser10,midUser11,midUser12,midUser13,midUser14,
   midUser15,midUser16,midUser17,midUser18,midUser19,midUser20);
  TRTFTokenType = (ttControlWord,ttControlSymbol,ttGroupBegin,ttGroupEnd,ttText);
  TRTFTokenTypeSet = set of TRTFTokenType;
  PCharArray = ^TCharArray;
  TCharArray = array[0..MAXBUFSIZE] of char;
  TFrameMode = (fmInside,fmSplit,fmOutside);

  TSystemSetups = set of TSystemSetup;
  TSystemOptions = set of TSystemOption;

  TPosition = record
    X: double;
    Y: double;
  end; { TPosition }

  PPositionNode = ^TPositionNode;
  TPositionNode = record
    X: double;
    Y: double;
    Next: PPositionNode;
  end; { TPositionNode }

  PTab = ^TTab;
  TTab = record
    Pos: double; { X Position of tab in inches }
    Justify: TPrintJustify; { Tab justification }
    Width: double; { Width of Tab Box in inches }
    Margin: double; { Left or right margin for Tab Box }
    Left: boolean; { Draw line on left? }
    Right: boolean; { Draw line on right? }
    Top: boolean; { Draw line on top? }
    Bottom: boolean; { Draw line on bottom? }
    Shade: byte; { Percent shading for background }
    RefCount: word; { How many times is this tab list referenced? }
    Next: PTab; { Next tab in list }
    Prev: PTab; { Previous tab in list }
  end; { TTab }

  TSaveTab = record
    Head: PTab;
    Tail: PTab;
  end; { TSaveTab }

  PTabNode = ^TTabNode;
  TTabNode = record
    Head: PTab;
    Tail: PTab;
    Next: PTabNode;
  end; { TTabNode }

  TSaveBrush = record
    Color: TColor;
    Style: TBrushStyle;
    Bitmap: boolean;
  end; { TSaveBrush }

  TSaveFont = record
    Name: string[60];
    Color: TColor;
    Pitch: TFontPitch;
    Size: double; { Point size of normal font }
    Height: double; { Height of normal font in inches }
    Width: double; { Average width of normal font character in inches }
    Ascent: double; { Ascent height of true font in inches }
    Descent: double; { Descent height of true font in inches }
    InternalLeading: double; { Internal Leading height of true font in inches }
    Bold: boolean;
    Underline: boolean;
    Italic: boolean;
    Strikeout: boolean;
    Subscript: boolean; { Is the true font subscript? }
    Superscript: boolean; { Is the true font superscript? }
    Rotation: smallint; { Rotation in degrees }
    Align: TFontAlign; { Alignment for this font }
    Index: word; { Index for this font }
    Handle: Windows.HFont;
    CharSet: integer; //!!PORT!! RPDEFINE01
  end; { TSaveFont }

  PFontNode = ^TFontNode;
  TFontNode = record
    Font: TSaveFont;
    Next: PFontNode;
  end; { TFontNode }

  TSavePen = record
    Color: TColor;
    Mode: TPenMode;
    Style: TPenStyle;
    Width: double;
  end; { TSavePen }

  TFormatPara = record
    Justify: TPrintJustify;
    FirstIndent: double;
    LeftIndent: double;
    RightIndent: double;
  end; { TFormatPara }

  TFormatStyle = record
    Bold: boolean;
    Italic: boolean;
    Underline: boolean;
    Strikeout: boolean;
    Subscript: boolean;
    Superscript: boolean;
    Pitch: TFontPitch;
    Color: TColor;
  end; { TFormatStyle }

  PFormatState = ^TFormatState;
  TFormatState = record
    Name: string[40];
    Size: double;
    Style: TFormatStyle;
    Para: TFormatPara;
    Prev: PFormatState;
  end; { TFormatState }

  PLineList = ^TLineList;
  TLineList = record
    P1,P2: double; { Start and End points, P1 must be <= P2 }
    C1: double; { Common value }
    Color: TColor;
    Mode: TPenMode;
    Style: TPenStyle;
    Width: integer;
    Next: PLineList;
  end; { TLineList }

  TRpComponent = class(TComponent)
  protected
    FVersion: TVersion; { Internal revision number }
    procedure SetVersion(Value: TVersion);
  public
    constructor Create(AOwner: TComponent); override;
  published
    property Version: TVersion read FVersion write SetVersion stored false;
  end; { TRpComponent }

  TRPStringObject = class
    FValue: string;
  public
    constructor Create(InitValue: string);
    property Value: string read FValue write FValue;
  end; { TRPStringObject }

  PRPDevMode = PDeviceMode;

  procedure RaiseError(ErrorMessage: string);
  function ShadeToColor(ShadeColor: TColor;
                        ShadePercent: byte): TColor;
  function MoneyToLongName(Value: extended): string;

{$IFDEF DEMO}
  procedure DisplayWarning;
  function Validate: boolean;
{$ENDIF}

type
  TRPDataType = (dtString,dtInteger,dtBoolean,dtFloat,dtCurrency,dtBCD,dtDate,
   dtTime,dtDateTime,dtBlob,dtMemo,dtGraphic,dtInt64);
  TRPDataTypeSet = set of TRPDataType;

const
  TRPDataSetType: array[low(TFieldType)..high(TFieldType)] of TRPDataType =
   (
    {ftUnknown=}dtString,
    {ftString=}dtString,
    {ftSmallint=}dtInteger,
    {ftInteger=}dtInteger,
    {ftWord=}dtInteger,
    {ftBoolean=}dtBoolean,
    {ftFloat=}dtFloat,
    {ftCurrency=}dtCurrency,
    {ftBCD=}dtBCD,
    {ftDate=}dtDate,
    {ftTime=}dtTime,
    {ftDateTime=}dtDateTime,
    {ftBytes=}dtBlob,
    {ftVarBytes=}dtBlob,
    {ftAutoInc=}dtInteger,
    {ftBlob=}dtBlob,
    {ftMemo=}dtMemo,
    {ftGraphic=}dtGraphic,
    {ftFmtMemo=}dtBlob,
    {ftParadoxOle=}dtBlob,
    {ftDBaseOle=}dtBlob,
    {ftTypedBinary=}dtBlob,
    {ftCursor=}dtBlob,
    {ftFixedChar=}dtString,
    {ftWideString=}dtString,
    {ftLargeint=}dtInt64,
    {ftADT=}dtBlob,
    {ftArray=}dtBlob,
    {ftReference=}dtBlob,
    {ftDataSet=}dtBlob{$IFDEF LEVEL5},
    {ftOraBlob=}dtBlob,
    {ftOraClob=}dtBlob,
    {ftVariant=}dtBlob,
    {ftInterface=}dtBlob,
    {ftIDispatch=}dtBlob,
    {ftGuid=}dtString{$IFDEF LEVEL6},
    {ftTimeStamp}dtDateTime,
    {ftFMTBcd}dtBCD
    {$ENDIF}{$ENDIF}
  );

const
  FileMapSize = 65536;
  FileMapName = 'RAVEBUFFER';
  AltFileMapName = 'RAVEBUFFERALT';
  EVENTPREPARED = 0;

  DATAFIRST = 1;
  DATANEXT = 2;
  DATAEOF = 3;
  DATAGETCOLS = 4;
  DATAGETROW = 5;
  DATASETFILTER = 6;
  DATAGETSORTS = 7;
  DATASETSORT = 8;
  DATAOPEN = 9;
  DATARESTORE = 10;
  DATAACKNOWLEDGE = 11;
  DATAFREEALTBUF = 12;

var
  DataID: string = '';

const
  ControllerMutexName = 'RAVECONTROLER';
  CompletedEventName = 'RAVECOMPLETED';
  AcknowledgeRTEventName = 'RAVERTACK_';
  AcknowledgeDTEventName = 'RAVEDTACK_';
  ConnectEventName = 'RAVECONNECT';
  ErrorEventName = 'RAVEERROR';
  DisconnectEventName = 'RAVEDISCONNECT';
  DataRTEventName = 'RAVERTDATA_';
  DataDTEventName = 'RAVEDTDATA_';

type
  TRaveConnectionError = (ceAbort,ceInform,cePrompt);

  function InitEvent(EventName: string): THandle;
  function InitManualEvent(EventName: string): THandle;
  function InitMutex(MutexName: string): THandle;
  function InitFileMap(MapName: string;
                       MapSize: integer): THandle;
  function InitFileBuf(FileMap: THandle): PChar;
{$IFDEF DBG}
const
  DebugMapSize = 500000;
  DebugMapName = 'RAVEDEBUGBUF';

  procedure WriteDebug(Value: string;
                       DebugBuf: PChar);
  function InitDebugMap: THandle;
{$ENDIF}
function GetRValue(AColor: TColor): byte;
function GetGValue(AColor: TColor): byte;
function GetBValue(AColor: TColor): byte;
function RGB(RValue: byte;
             GValue: byte;
             BValue: byte): TColor;

  function DefaultTrans(const Value: string): string;
  function VCLToCLXCharSet(ACharSet: integer): TFontCharSet;
  function CLXToVCLCharSet(ACharSet: TFontCharSet): integer;
  function ConvertCharset(Charset: byte): byte;

var
  Trans: function(const Value: string): string;

{$IFNDEF LEVEL5}
  procedure FreeAndNil(var Item);
  function IncludeTrailingBackslash(const S: string): string;
  function SameText(const S1, S2: string): Boolean;
  function AnsiSameText(const S1, S2: string): Boolean;
{$ENDIF}

procedure CloseEvent(AEvent: THandle;
                     Force: boolean = false);
procedure CloseMutex(AMutex: THandle;
                     Force: boolean = false);
procedure CloseFileMap(AFileMap: THandle;
                       AFileBuf: PChar);

implementation

{$IFNDEF LEVEL5}
procedure FreeAndNil(var Item);
var
  SaveItem: TObject;
begin { FreeAndNil }
  SaveItem := TObject(Item);
  TObject(Item) := nil;
  SaveItem.Free;
end;  { FreeAndNil }

function IncludeTrailingBackslash(const S: string): string;
begin
  if S[Length(S)] = '\' then begin
    Result := S;
  end else begin
  Result := S + '\';
  end; { else }
end;

function SameText(const S1, S2: string): Boolean;
begin
  Result := CompareText(S1,S1) = 0;
end;

function AnsiSameText(const S1, S2: string): Boolean;
begin
  Result := AnsiCompareText(S1,S2) = 0;
end;
{$ENDIF}

function DefaultTrans(const Value: string): string;
begin { DefaultTrans }
  Result := Value;
end;  { DefaultTrans }


procedure CloseEvent(AEvent: THandle;
                     Force: boolean = false);
begin { CloseEvent }
  CloseHandle(AEvent);
end;  { CloseEvent }

procedure CloseMutex(AMutex: THandle;
                     Force: boolean = false);
begin { CloseMutex }
  CloseHandle(AMutex);
end;  { CloseMutex }

procedure CloseFileMap(AFileMap: THandle;
                       AFileBuf: PChar);
begin { CloseFileMap }
  UnmapViewOfFile(AFileBuf);
  CloseHandle(AFileMap);
end;  { CloseFileMap }

function InitEvent(EventName: string): THandle;
begin { InitEvent }
  EventName := DataID + EventName;
  if EventName = '' then begin
    Result := 0;
  end else begin
  // Open event if it exists, otherwise return 0
    Result := OpenEvent(EVENT_ALL_ACCESS,false,@EventName[1]);
    if Result = 0 then begin
    // Create event
      Result := CreateEvent(nil,false,false,@EventName[1]);
    end; { if }
  end; { else }
end;  { InitEvent }

function InitManualEvent(EventName: string): THandle;
begin { InitManualEvent }
  EventName := DataID + EventName;
  if EventName = '' then begin
    Result := 0;
  end else begin
    Result := OpenEvent(EVENT_ALL_ACCESS,false,@EventName[1]);
    if Result = 0 then begin
      Result := CreateEvent(nil,true,false,@EventName[1]);
    end; { if }
  end; { else }
end;  { InitManualEvent }

function InitMutex(MutexName: string): THandle;
begin { InitMutex }
  MutexName := DataID + MutexName;
  if MutexName = '' then begin
    Result := 0;
  end else begin
    Result := OpenMutex(MUTEX_ALL_ACCESS,false,@MutexName[1]);
    if Result = 0 then begin
      Result := CreateMutex(nil,false,@MutexName[1]);
    end; { if }
  end; { else }
end;  { InitMutex }

function InitFileMap(MapName: string;
                     MapSize: integer): THandle;
begin { InitFileMap }
  MapName := DataID + MapName;
  Result := OpenFileMapping(FILE_MAP_WRITE,false,@MapName[1]);
  if Result = 0 then begin
    Result := CreateFileMapping($FFFFFFFF,nil,PAGE_READWRITE,0,MapSize,@MapName[1]);
  end; { if }
end;  { InitFileMap }

function InitFileBuf(FileMap: THandle): PChar;

begin { InitFileBuf }
  Result := PChar(MapViewOfFile(FileMap,FILE_MAP_READ OR FILE_MAP_WRITE,0,0,0));
end;  { InitFileBuf }

{$IFDEF DBG}
procedure WriteDebug(Value: string;
                     DebugBuf: PChar);

var
 DebugPtr: PChar;
 Offset: integer;
 CS: TRTLCriticalSection;

begin { WriteDebug }
  InitializeCriticalSection(CS);
  try
    EnterCriticalSection(CS);
    Offset := integer(pointer(DebugBuf)^); { Get old offset }
    if (Offset + 128) < DebugMapSize then begin
      DebugPtr := DebugBuf + Offset + 4; { Move DebugPtr to write location }
      StrPCopy(DebugPtr,Value); { Write Value out }
      integer(pointer(DebugBuf)^) := Offset + Length(Value) + 1; { Update offset }
    end; { if }
  finally
    LeaveCriticalSection(CS);
    DeleteCriticalSection(CS);
  end; { with }
end;  { WriteDebug }

function InitDebugMap: THandle;

begin { InitDebugMap }
  Result := OpenFileMapping(FILE_MAP_WRITE,false,DebugMapName);
  if Result = 0 then begin
    Result := CreateFileMapping($FFFFFFFF,nil,PAGE_READWRITE,0,DebugMapSize,
     DebugMapName);
  end; { if }
end;  { InitDebugMap }
{$ENDIF}

procedure RaiseError(ErrorMessage: string);

begin { RaiseError }
  Raise EReportPrinter.Create(ErrorMessage);
end;  { RaiseError }

function GetRValue(AColor: TColor): byte;
begin { GetRValue }
  Result := byte(AColor);
end;  { GetRValue }

function GetGValue(AColor: TColor): byte;
begin { GetGValue }
  Result := byte(AColor shr 8);
end;  { GetGValue }

function GetBValue(AColor: TColor): byte;
begin { GetBValue }
  Result := byte(AColor shr 16);
end;  { GetBValue }

function RGB(RValue: byte;
             GValue: byte;
             BValue: byte): TColor;
begin { RGB }
  Result := RValue or (GValue shl 8) or (BValue shl 16);
end;  { RGB }

function ShadeToColor(ShadeColor: TColor;
                      ShadePercent: byte): TColor;

var
  Red: byte;
  Blue: byte;
  Green: byte;
  RGBVal: longint;

begin { ShadeToColor }
  if ShadePercent > 100 then begin
    ShadePercent := 100;
  end; { if }
  RGBVal := ColorToRGB(ShadeColor);
  Red := ((255 - GetRValue(RGBVal)) * (100 - ShadePercent)) div 100 +
   GetRValue(RGBVal);
  Green := ((255 - GetGValue(RGBVal)) * (100 - ShadePercent)) div 100 +
   GetGValue(RGBVal);
  Blue := GetBValue(RGBVal);
  Blue := ((255 - Blue) * (100 - ShadePercent)) div 100 + Blue;
  Result := RGB(Red,Green,Blue);
end;  { ShadeToColor }

function MoneyToLongName(Value: extended): string;

const
  Name1: array[0..9] of string = ('','One ','Two ','Three ','Four ','Five ',
  'Six ','Seven ','Eight ','Nine ');
  Name2: array[0..9] of string = ('Ten ','Eleven ','Twelve ','Thirteen ',
  'Fourteen ','Fifteen ','Sixteen ','Seventeen ','Eighteen ','Nineteen ');
  Name3: array[2..9] of string = ('Twenty ','Thirty ','Forty ','Fifty ',
  'Sixty ','Seventy ','Eighty ','Ninety ');
  Name4: array[0..4] of string = ('Trillion ','Billion ','Million ','Thousand ','');

  function MoneyToShortName(Value: longint;
                            AddName: string): string;
  { TODO : This is more problematic than just MBCS }
  var
    I1: longint;

  begin { MoneyToShortName }
    Result := '';
    if Value = 0 then Exit;
    I1 := Value div 100;
    if I1 > 0 then begin
      Result := Name1[I1] + 'Hundred ';
    end; { if }
    I1 := (Value div 10) mod 10;
    if I1 = 1 then begin
      Result := Result + Name2[Value mod 10];
    end else begin
      if I1 >= 2 then begin
        Result := Result + Name3[I1];
      end; { if }
      Result := Result + Name1[Value mod 10];
    end; { else }
    Result := Result + AddName;
  end;  { MoneyToShortName }

var
  I1: integer;
  S1: string;

begin { MoneyToLongName }
  S1 := FloatToStrF(Abs(Value),ffFixed,18,2);
  if Length(S1) > 18 then begin
    Result := '(overflow)';
    Exit;
  end; { if }
  while Length(S1) < 18 do begin
    S1 := '0' + S1;
  end; { while }
  if Value < 1.0 then begin
    Result := 'Zero ';
  end else begin
    Result := '';
    for I1 := 0 to 4 do begin
      Result := Result + MoneyToShortName(StrToInt(Copy(S1,I1 * 3 + 1,3)),Name4[I1]);
    end; { for }
  end; { else }
  Result := Result + 'and ' + Copy(S1,17,2) + '/100';
end;  { MoneyToLongName }

constructor TRpComponent.Create(AOwner: TComponent);

begin { Create }
  inherited Create(AOwner);
  FVersion := RPVersion;
end;  { Create }

procedure TRpComponent.SetVersion(Value: TVersion);

begin { SetVersion }
{ Don't do anything since version was set in Create }
end;  { SetVersion }

constructor TRpStringObject.Create(InitValue: string);

begin { Create }
  inherited Create;
  FValue := InitValue;
end;  { Create }

{ The file DEMOUTIL.PAS is not included nor is it required
  for the full version of Rave Reports }

{$IFDEF DEMO}
{$I ..\NONDIST\DEMOUTIL.PAS}
{$ENDIF}

{ TMetafile }

constructor TMetafile.Create;
begin
  FImageData := TMemoryStream.Create;
end;

destructor TMetafile.Destroy;
begin
  FreeAndNil(FImageData);
  if FHandle <> 0 then begin
    DeleteEnhMetaFile(FHandle);
  end; { if }
end;

procedure TMetafile.Assign(Source: TPersistent);
begin
  if Assigned(Source) and (Source is TMetafile) then begin
    TMetaFile(Source).ImageData.Position := 0;
    LoadFromStream(TMetaFile(Source).ImageData);
  end else begin
    inherited Assign(Source);
  end; { else }
end;

procedure TMetafile.Draw(ACanvas: TCanvas; const Rect: TRect);
begin
  PlayEnhMetaFile(QPainter_handle(ACanvas.Handle),FHandle,Rect);
end;

function TMetafile.GetEmpty: Boolean;
begin
  Result := FImageData.Size <= 0;
end;

function TMetafile.GetHeight: integer;
begin
  Result := FHeight;
end;

function TMetafile.GetWidth: integer;
begin
  Result := FWidth;
end;

procedure TMetafile.LoadFromMimeSource(MimeSource: TMimeSource);
begin
  inherited;
end;

procedure TMetafile.LoadFromStream(AStream: TStream);
var
  EMH: TEnhMetaheader;
begin
  FImageData.Size := 0;
  FImageData.CopyFrom(AStream,AStream.Size - AStream.Position);
  FImageData.Position := 0;
  FImageData.ReadBuffer(EMH,SizeOf(EMH));
  FHandle := SetEnhMetaFileBits(EMH.nBytes,FImageData.Memory);
  FWidth := EMH.rclFrame.Right - EMH.rclFrame.Left;
  FHeight := EMH.rclFrame.Bottom - EMH.rclFrame.Top;
end;

procedure TMetafile.SaveToMimeSource(MimeSource: TClxMimeSource);
begin
  inherited;
end;

procedure TMetafile.SaveToStream(AStream: TStream);
begin
  FImageData.Position := 0;
  FImageData.SaveToStream(AStream);
end;

procedure TMetafile.SetHeight(Value: integer);
begin
  inherited;
end;

procedure TMetafile.SetWidth(Value: integer);
begin
  inherited;
end;

procedure TMetafile.ReadData(Stream: TStream);
begin
  Stream.Position := Stream.Position + 4; // Throw away length
  LoadFromStream(Stream);
end;

procedure TMetafile.WriteData(Stream: TStream);
var
  I1: integer;
begin
  I1 := FImageData.Size;
  Stream.Write(I1,SizeOf(I1));
  SaveToStream(Stream);
end;

function VCLToCLXCharSet(ACharSet: integer): TFontCharSet;
begin
  case ACharSet of
    0: Result := fcsLatin1; // ANSI_CHARSET
    1: Result := fcsAnyCharSet; // DEFAULT_CHARSET
    2: Result := fcsUnicode; // SYMBOL_CHARSET
    77: Result := fcsLatin1; // MAC_CHARSET
    128: Result := fcsSet_Ja; // SHIFTJIS_CHARSET
    129: Result := fcsSet_Ko; // HANGEUL_CHARSET
    130: Result := fcsSet_Ko; // JOHAB_CHARSET
    134: Result := fcsSet_Zh_TW; // GB2312_CHARSET
    136: Result := fcsSet_Zh; // CHINESEBIG5_CHARSET
    161: Result := fcsISO_8859_7; // GREEK_CHARSET
    162: Result := fcsUnicode; // TURKISH_CHARSET
    163: Result := fcsUnicode; // VIETNAMESE_CHARSET
    177: Result := fcsISO_8859_8; // HEBREW_CHARSET
    178: Result := fcsISO_8859_6; // ARABIC_CHARSET
    186: Result := fcsUnicode; // BALTIC_CHARSET
    204: Result := fcsKOI8R; // RUSSIAN_CHARSET
    222: Result := fcsSet_Th_TH; // THAI_CHARSET
    238: Result := fcsUnicode; // EASTEREUROPE_CHARSET
    255: Result := fcsLatin1; // OEM_CHARSET
    else Result := fcsLatin1;
  end; { case }
end;

function CLXToVCLCharSet(ACharSet: TFontCharSet): integer;
begin
  case ACharSet of
    fcsLatin1: Result := 0;
    fcsAnyCharSet: Result := 1;
    fcsLatin2: Result := 0;
    fcsLatin3: Result := 0;
    fcsLatin4: Result := 0;
    fcsISO_8859_5: Result := 0;
    fcsISO_8859_6: Result := 178;
    fcsISO_8859_7: Result := 161;
    fcsISO_8859_8: Result := 177;
    fcsLatin5: Result := 0;
    fcsLatin6: Result := 0;
    fcsISO_8859_11: Result := 0;
    fcsISO_8859_12: Result := 0;
    fcsLatin7: Result := 0;
    fcsLatin8: Result := 0;
    fcsLatin9: Result := 0;
    fcsKOI8R: Result := 204;
    fcsSet_Ja: Result := 128;
    fcsSet_Ko: Result := 129;
    fcsSet_Th_TH: Result := 222;
    fcsSet_Zh: Result := 0;
    fcsSet_Zh_TW: Result := 0;
    fcsUnicode: Result := 0;
  {$IFDEF Level7}
    fcsSet_GBK: Result := 0;
    fcsSet_Big5: Result := 0;
    fcsTSCII: Result := 0;
    fcsKOI8U: Result := 0;
    fcsCP1251: Result := 0;
    fcsPT154: Result := 0;
  {$ENDIF}
    fcsDefaultCharSet: Result := 1;
    else Result := 0;
  end; { case }
end;

function SystemCharset: byte;
begin
  case GetACP of
    874: Result := Windows.THAI_CHARSET;
    932: Result := Windows.SHIFTJIS_CHARSET;
    936: Result := Windows.GB2312_CHARSET;
    949: Result := Windows.HANGEUL_CHARSET;
    950: Result := Windows.CHINESEBIG5_CHARSET;
    1200: Result := Windows.DEFAULT_CHARSET;
    1250: Result := Windows.EASTEUROPE_CHARSET;
    1251: Result := Windows.RUSSIAN_CHARSET;
    1252: Result := Windows.DEFAULT_CHARSET;
    1253: Result := Windows.GREEK_CHARSET;
    1254: Result := Windows.TURKISH_CHARSET;
    1255: Result := Windows.HEBREW_CHARSET;
    1256: Result := Windows.ARABIC_CHARSET;
    1257: Result := Windows.BALTIC_CHARSET;
    else Result := Windows.DEFAULT_CHARSET;
  end;
end;

function ConvertCharset(Charset: byte): byte;
begin
  if Charset = Windows.DEFAULT_CHARSET then begin
    case GetUserDefaultLCID of
      $0401: Result := Windows.ARABIC_CHARSET; // Saudi Arabia (Arabic)
      $0404: Result := Windows.CHINESEBIG5_CHARSET; // Traditional Chinese (Taiwan)
      $0405: Result := Windows.EASTEUROPE_CHARSET; // Czechoslovakia
      $0408: Result := Windows.GREEK_CHARSET; // Greek (Greece)
      $040D: Result := Windows.HEBREW_CHARSET; // Israel (Hebrew)
      $040E: Result := Windows.EASTEUROPE_CHARSET; // Hungary (Hungarian)
      $0411: Result := Windows.SHIFTJIS_CHARSET; // Japan
      $0412: Result := Windows.HANGEUL_CHARSET; // Korea
      $0415: Result := Windows.EASTEUROPE_CHARSET; // Poland (Polish)
      $0418: Result := Windows.EASTEUROPE_CHARSET; // Romania (Romanian)
      $0419: Result := Windows.RUSSIAN_CHARSET; // Russia (Russian)
      $041A: Result := Windows.EASTEUROPE_CHARSET; // Croatia (Croatian)
      $041B: Result := Windows.EASTEUROPE_CHARSET; // Slovakia (Slovak)
      $041E: Result := Windows.THAI_CHARSET; // Thailand (Tha)
      $041F: Result := Windows.TURKISH_CHARSET; // Turkey (Turkish)
      $0424: Result := Windows.EASTEUROPE_CHARSET; // Slovenia (Slovenian)
      $042A: Result := Windows.VIETNAMESE_CHARSET; // Vietnam (Vietnamese)
      $0804: Result := Windows.GB2312_CHARSET;  // Simplified Chinese  (People's Republic of China)
      else Result := SystemCharset;
    end; { case }
  end else begin
    Result := Charset;
  end; { else }
end;

initialization
  Trans := DefaultTrans;
end.