{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRpRenderHTML;

interface

{$I RpVer.inc}
{$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED OFF}{$ENDIF}

uses
  Windows,
  Types, QGraphics,
  Classes, SysUtils, QRpRender;

type
  TRvRenderHTML = class(TRPRenderStream)
  protected
    FCacheDir: string;
    FGenerator: integer;
    ffMaxY: double;
    FOffsetX: integer;
    FOffsetY: integer;
    FPageURL: string;
    FTemplateHead: string;
    FTemplatePost: string;
    FTemplatePre: string;
    FServerMode: boolean;
    FUsingInternalStream: boolean;
    FLastFileNameIndex: integer;
    //
    function ColorToRGBString(AColor: TColor): string;
    procedure DoGifDiv(const AX, AY, AWidth, AHeight: single; AColor: TColor);
    function GetFontStyle(AFont: TFont): string;
    function GetTemplate: string;
  	Function HTMLColor(const colr: TColor): string;
    function HTMLText(const psText: string): string;
    function InchesToPixels(const AInches: double): integer;
    function NewCacheFile(AExtension: string): string;
    function ProcessParams(const AHTML: string): string;
    procedure SetDefaultTemplate;
    procedure SetTemplate(const AValue: string);
    procedure	ToJPEGFile(AGraphic: TGraphic; const AFileName: string);
    // WriteDiv must be the only item that writes out DIVs as it handles offsets, etc
    procedure WriteDiv(const AText: string; const AX1, AY1: double; const AWidth: double = 0;
     const AHeight: double = 0);
    function ProcessURL(AValue: string): string;
    function PadIntToStr(ANumber: integer): string;
    property LastFileNameIndex: integer read FLastFileNameIndex write FLastFileNameIndex;

  public
    constructor Create(AOwner: TComponent); override;
    procedure PrintRender(NDRStream: TStream; OutputFileName: TFileName); override;
    procedure CenterText(const psText: string; const pfX, pfY: double); override;
    class procedure CreateColorGif(AStream: TStream; AColor: TColor);
    procedure LeftText(const psText: string; const pfX, pfY: double); override;
    procedure LineTo(const pfX1, pfY1: double); override;
    procedure PrintBitmapRect(const AX1, AY1, AX2, AY2: double; AGraphic:
      QGraphics.TBitmap);
     override;
    procedure StretchDraw(const Rect: TRect; AGraphic: TGraphic); override;
    procedure PrintSpaces(const psText: string; const pfX, pfY, pfWidth: double); override;
    procedure PrintRightWidth(var AX, AY: double; AText: string; Width: double); override;
    procedure Rectangle(const pfX1, pfY1, pfX2, pfY2: double); override;
    procedure FillRect(const pRect: TRect); override;
    procedure RightText(const psText: string; const pfX, pfY: double); override;
    //
    procedure DocBegin; override;
    procedure PageBegin; override;
    procedure PageEnd; override;
    //
    property PageURL: string read FPageURL write FPageURL;
    property Template: string read GetTemplate write SetTemplate;
  published
    property OnDecodeImage;
    property CacheDir: string read FCacheDir write FCacheDir;
    property ServerMode: boolean read FServerMode write FServerMode;
  end;
  TRpRenderHTML = TRvRenderHTML; // Rave 4.0 compatibility


implementation

uses
  Math,
  QRpMBCS,
  QRpDefine;

const
  PPI = 96;
  Pixel = 1 / 96;

{ TRvRenderHTML }

procedure TRvRenderHTML.DocBegin;
begin
  LastFileNameIndex := 0;
end;

procedure TRvRenderHTML.PageBegin;
var
  LExt: string;
  LBasePathName: string;

  procedure PrintPageNavigation;
  begin
    PrintLn('<SCRIPT Language=Javascript>');
    if ServerMode then begin
      PrintLn('var GPageURL = "' + PageURL + '";');
    end else begin
      PrintLn('var GPageURL = "' + ProcessURL(PageURL) + '";');
    end; { else }
    PrintLn('var GPageNo = ' + IntToStr(Converter.PageNo) + ';');
    PrintLn('var GPageCount = ' + IntToStr(Converter.PageCount) + ';');
    PrintLn('');
    PrintLn('function GotoPage(APageNo) {');
    PrintLn('  location.href = GPageURL.replace(/{%PAGENO%}/,APageNo);');
    PrintLn('}');
    PrintLn('');
    PrintLn('function NextPage() {');
    PrintLn('  if (GPageNo < GPageCount)');
    PrintLn('    {GotoPage(GPageNo + 1);}');
    PrintLn('  else');
    PrintLn('    {window.alert("This is the last page.")}');
    PrintLn('}');
    PrintLn('');
    PrintLn('function PrevPage() {');
    PrintLn('  if (GPageNo > 1)');
    PrintLn('    {GotoPage(GPageNo - 1);}');
    PrintLn('  else');
    PrintLn('    {window.alert("This is the first page.")}');
    PrintLn('}');
    PrintLn('');
    PrintLn('</SCRIPT>');
  end;

begin
  ffMaxY := 0;
  if AnsiPos(';',OutputFileName) > 0 then begin
    OutputFileName := MBCSCopy(OutputFileName, 0, AnsiPos(';',OutputFileName) - 1);
  end; { if }
  LExt := ExtractFileExt(OutputFileName);
  LBasePathName := ExtractFilePath(OutputFileName) + ExtractFileName(OutputFileName);
  SetLength(LBasePathname,Length(LBasePathname) - Length(LExt));
  if (LExt = '') or (LExt = '.') then begin
    LExt := MBCSCopy(FileExtension, AnsiPos('.',FileExtension),Length(FileExtension));
    if AnsiPos(';',LExt) > 0 then begin
      LExt := MBCSCopy(LExt, 0, AnsiPos(';',LExt) - 1);
    end; { if }
  end; { if }
  if not ServerMode then begin
    PageURL := ExtractFilename(LBasePathname + '{%PAGENO%}' + LExt);
  end;
  if ActiveStream = nil then begin
    FUsingInternalStream := true;
    ActiveStream := TFileStream.Create(LBasePathName + IntToStr(Converter.PageNo) + LExt,fmCreate or fmShareDenyWrite);
  end; { if }
  if Length(FTemplateHead) > 0 then begin
    PrintLn(ProcessParams(FTemplateHead));
    PrintPageNavigation;
    Print(ProcessParams(FTemplatePre));
  end else begin
    Print(ProcessParams(FTemplatePre));
    PrintPageNavigation;
    PrintLn('</HEAD>');
    PrintLn('<BODY>');
  end; { else }
end;

procedure TRvRenderHTML.PageEnd;
begin
  Print(ProcessParams(FTemplatePost));
  if FUsingInternalStream then begin
    FreeAndNil(FActiveStream);
    FUsingInternalStream := false;
  end; { if }
  NDRStream := nil;
end;

function TRvRenderHTML.InchesToPixels(const AInches: double): integer;
begin
  Result := Round(AInches * PPI);
end;

procedure TRvRenderHTML.LeftText;
begin
  WriteDiv(HTMLText(psText), pfX, pfY - Converter.FontData.Height, Length(psText));
end;

procedure TRvRenderHTML.PrintSpaces(const psText: string; const pfX, pfY, pfWidth: double);
var
  CurrWord: string;
  Spaces: integer;
  SpaceWidth: double;
  SpacesStr: string;
  I1: integer;
  LText: string;
  XPos, YPos: double;
begin
{ Count the number of spaces in Text }
  LText := psText;
  Spaces := CountSpaces(LText);
  if Spaces = 0 then begin { Process each letter as a word }
  { Insert spaces in between each letter pair }
    I1 := 2;
    while I1 <= Length(LText) do begin
      if (LText[I1 - 1] <> ' ') and (LText[I1] <> ' ') then begin { Insert Space }
        Insert(' ',LText,I1);
        Inc(Spaces);
        Inc(I1);
      end; { if }
      Inc(I1);
    end; { while }
  end; { if }
  if Spaces = 0 then begin
    Spaces := 1;
  end; { if }

{ Figure out the width for each internal space }
  SpaceWidth := Converter.RPTFTextWidth(' ');

  SpaceWidth := SpaceWidth + ((pfWidth - Converter.RPTFTextWidth(LText)) / Spaces);

{ Print first word left aligned, middle and last words right aligned }
  CurrWord := GetFirstWord(LText);
  XPos := pfX;
  YPos := pfY;
  LeftText(CurrWord, XPos, YPos);
  AdjustXY(XPos,YPos,Converter.RPTFTextWidth(CurrWord));
  while LText <> '' do begin
    Spaces := Length(GetSpaces(LText));
    CurrWord := GetFirstWord(LText);
    SpacesStr := '';
    // Need to print SpacesStr so underlining works
    for I1 := 1 to Spaces do begin
      SpacesStr := SpacesStr + '&nbsp;';
    end; { for }
    PrintRightWidth(XPos,YPos,SpacesStr + CurrWord,Converter.RPTFTextWidth(CurrWord) + (Spaces * SpaceWidth));
  end; { while }
end;

procedure TRvRenderHTML.Rectangle(const pfX1, pfY1, pfX2, pfY2: double);
var
  LLeft, LTop, LWidth, LHeight: single;
begin
  LLeft := Min(pfX1, pfX2);
  LTop := Min(pfY1, pfY2);
  LWidth := Max(pfX1, pfX2) - LLeft;
  LHeight := Max(pfY1, pfY2) - LTop;
  DoGifDiv(LLeft, LTop, LWidth, LHeight, Converter.Brush.Color);
  DoGifDiv(LLeft, LTop, Pixel, LHeight, Converter.Pen.Color);
  DoGifDiv(LLeft, LTop + LHeight, LWidth, Pixel, Converter.Pen.Color);
  DoGifDiv(LLeft + LWidth, LTop, Pixel, LHeight, Converter.Pen.Color);
  DoGifDiv(LLeft, LTop, LWidth, Pixel, Converter.Pen.Color);
end;

procedure TRvRenderHTML.PrintRightWidth(var AX, AY: double; AText: string;
  Width: double);
var
  SpaceWidth: double;
begin
{ Print a block of spaces to fill the front space width }
  SpaceWidth := Width - Converter.RPTFTextWidth(AText);

{ Print Text right aligned }
  AdjustXY(AX,AY,SpaceWidth);
  AdjustXY(AX,AY,Converter.RPTFTextWidth(AText));
  RightText(AText,AX,AY);
end;

procedure TRvRenderHTML.WriteDiv(const AText: string; const AX1, AY1: double;
 const AWidth: double = 0; const AHeight: double = 0);
var
  s: string;
begin
  ffMaxY := Max(AY1 + AHeight, ffMaxY);
  s := '';
  if AWidth > 0 then begin
    s := s + 'width:' + IntToStr(InchesToPixels(AWidth)) + ';';
  end;
  if AHeight > 0 then begin
    s := s + 'height:' + IntToStr(InchesToPixels(AHeight)) + ';';
  end;
  PrintLn('<DIV STYLE="left:' + IntToStr(InchesToPixels(AX1) + FOffsetX) + ';top:'
   + IntToStr(InchesToPixels(AY1) + FOffsetY) + ';' + s + GetFontStyle(Converter.Font) + '">'
   + AText + '</DIV>');
end;

Function TRvRenderHTML.HTMLColor;
begin
  // MBCS-SAFE
	Result := IntToHex(ColorToRGB(colr), 6);
	Result := Result[5] + Result[6] + Result[3] + Result[4] + Result[1] + Result[2];
end;

function TRvRenderHTML.HTMLText(const psText: string): string;
begin
  {TODO: Convert special chars in psText}
  result := psText;
end;

function TRvRenderHTML.GetFontStyle(AFont: TFont): string;
begin
  Result := 'font:';
  if fsItalic in AFont.Style then begin
    Result := Result + 'italic ';
  end else begin
    Result := Result + 'normal ';
  end;
  if fsBold in AFont.Style then begin
    Result := Result + 'bold ';
  end else begin
    Result := Result + 'normal ';
  end;
  Result := Result + IntToStr(AFont.Size) + 'pt ''' + AFont.Name + '''';
  if fsUnderline in AFont.Style then begin
    Result := Result + ';text-decoration:underline';
  end;
  if AFont.Color <> clNone then begin
    Result := Result + ';color:#' + ColorToRGBString(AFont.Color);
  end;
end;

function TRvRenderHTML.ColorToRGBString(AColor: TColor): string;
begin
  Result := IntToHex(ColorToRGB(AColor), 6);
  Result := Result[5] + Result[6] + Result[3] + Result[4] + Result[1] + Result[2];
end;

procedure TRvRenderHTML.LineTo(const pfX1, pfY1: double);
begin
  if (pfX1 = Converter.CurrX) or (pfY1 = Converter.CurrY) then begin
    DoGifDiv(Converter.CurrX, Converter.CurrY, pfX1 - Converter.CurrX, pfY1 - Converter.CurrY
     , Converter.Pen.Color);
  end;
end;

procedure TRvRenderHTML.PrintBitmapRect(const AX1, AY1, AX2, AY2: double;
 AGraphic:
  QGraphics.TBitmap);
var
  LFilename: string;
  LPathURL: string;
begin
  LFilename := NewCacheFile('jpg');
  ToJPegFile(AGraphic, LFilename);
  if ServerMode then begin
    LPathURL := '/Cache/' + ExtractFilename(LFilename);
  end else begin
    LPathURL := CacheDir;
    if LPathURL = '' then begin
      LPathURL := GetCurrentDir;
    end;
    if not (LPathURL[Length(LPathURL)] in ['/', '\']) then begin
      LPathURL := LPathURL + '/';
    end;
    LPathURL := ProcessURL(LPathURL + ExtractFilename(LFilename));
  end;
  //TODO: Scale image during call to ToJPegFile instead of using browsers scaling
  WriteDiv('<IMG Src="' + LPathURL + '" width='
   + IntToStr(InchesToPixels(AX2 - AX1)) + ' height=' + IntToStr(InchesToPixels(AY2 - AY1)) + '>'
   , AX1, AY1, AX2 - AX1, AY2 - AY1);
end;

procedure TRvRenderHTML.StretchDraw(const Rect: TRect; AGraphic: TGraphic);
var
  LFilename: string;
  LPathURL: string;
  Bitmap:
    QGraphics.TBitmap;
  Rect1: TRect;
  MetafileDPI: integer;
begin
  Bitmap :=
    QGraphics.TBitmap.Create;
  try
    Rect1.Left := 0;
    Rect1.Top := 0;
    Rect1.Right := Rect.Right - Rect.Left;
    MetafileDPI := 300;
    Rect1.Right := Round(Rect1.Right / 72 * MetafileDPI);
    Rect1.Bottom := Rect.Bottom - Rect.Top;
    Rect1.Bottom := Round(Rect1.Bottom / 72 * MetafileDPI);
    Bitmap.Width := Rect1.Right;
    Bitmap.Height := Rect1.Bottom;
    Bitmap.Canvas.StretchDraw(Rect1, AGraphic);
    LFilename := NewCacheFile('jpg');
    if not ServerMode then begin
      SetLength(LFilename,Pos('.',LFilename));
      LFilename := LFilename + 'jpg';
    end; { if }
    ToJPegFile(Bitmap, LFilename);
  finally
    Bitmap.Free;
  end; { tryf }
  if ServerMode then begin
    LPathURL := '/Cache/' + ExtractFilename(LFilename);
  end else begin
    LPathURL := CacheDir;
    if LPathURL = '' then begin
      LPathURL := GetCurrentDir;
    end;
    if not (LPathURL[Length(LPathURL)] in ['/', '\']) then begin
      LPathURL := LPathURL + '/';
    end;
    LPathURL := ProcessURL(LPathURL + ExtractFilename(LFilename));
  end;
  //TODO: Scale image during call to ToJPegFile instead of using browsers scaling
  WriteDiv('<IMG Src="' + LPathURL + '" width='
   + IntToStr(InchesToPixels((Rect.Right / 72) - (Rect.Left / 72)))
   + ' height=' + IntToStr(InchesToPixels((Rect.Bottom / 72) - (Rect.Top / 72))) + '>'
   , Rect.Left / 72, Rect.Top / 72, (Rect.Right / 72) - (Rect.Left / 72)
   , (Rect.Bottom / 72) - (Rect.Top / 72));
end;

function TRvRenderHTML.NewCacheFile(AExtension: string): string;
const
	PathDelim = '\';
var
  ActiveCacheDir: string;
begin
  SetLength(Result,260);
  ActiveCacheDir := CacheDir;
  if ActiveCacheDir = '' then begin
    ActiveCacheDir := GetCurrentDir();
  end;
  repeat
    Result := ActiveCacheDir + PathDelim + Copy(PageURL,1,Pos('{',PageURL) - 1) + PadIntToStr(LastFileNameIndex) + '.' + AExtension;
    Inc(FLastFileNameIndex);
  until not (FileExists(Result));
end;

procedure TRvRenderHTML.ToJPEGFile(AGraphic: TGraphic; const AFileName: string);
begin
//!!PORT!!
//  with TNDGraphic.Create do try
//    Assign(AGraphic);
//    Format := gfJPEG;
//    SaveToFile(AFilename);
//  finally Free end;
end;

class procedure TRvRenderHTML.CreateColorGif(AStream: TStream; AColor: TColor);
const
  Prefix: array[1..13] of byte = ($47,$49,$46,$38,$39,$61,$02,$00,$02,$00,$B3,$00,$00);
  Suffix: array[1..62] of byte = ($00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
   $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
   $00,$00,$00,$00,$00,$00,$00,$2C,$00,$00,$00,$00,$02,$00,$02,$00,$00,$04,$03,$10,$48,$10,$00,$3B);
var
  LByte: byte;
  LColor: longint;
begin
  with AStream do begin
    WriteBuffer(Prefix, Length(Prefix));
    LColor := ColorToRGB(AColor);
    LByte := (LColor and $000000FF);
    WriteBuffer(LByte, SizeOf(LByte));
    LByte := (LColor and $0000FF00) shr 8;
    WriteBuffer(LByte, SizeOf(LByte));
    LByte := (LColor and $00FF0000) shr 16;
    WriteBuffer(LByte, SizeOf(LByte));
    WriteBuffer(Suffix, Length(Suffix));
  end;
end;

procedure TRvRenderHTML.DoGifDiv(const AX, AY, AWidth, AHeight: single; AColor: TColor);
var
  PathURL: string;
  ImageStream: TMemoryStream;
begin
  if ServerMode then begin
    PathURL := '/Color/' + HTMLColor(AColor) + '.gif';
  end else begin
    PathURL := CacheDir;
    if PathURL = '' then begin
      PathURL := GetCurrentDir;
    end; { if }
    if (PathURL[Length(PathURL)] <> '/') and
       (PathURL[Length(PathURL)] <> '\') then begin
      PathURL := PathURL + '/';
    end; { if }
    PathURL := PathURL + HTMLColor(AColor) + '.gif';
    PathURL := ProcessURL(PathURL);
    ImageStream := TMemoryStream.Create;
    try
      CreateColorGif(ImageStream, AColor);
      ImageStream.SaveToFile(ExpandFileName(PathURL));
    finally
      ImageStream.Free;
    end; { tryf }
  end;
  WriteDiv('<IMG SRC="' + PathURL + '" WIDTH='
   + IntToStr(Max(InchesToPixels(AWidth), 1)) + ' HEIGHT='
   + IntToStr(Max(InchesToPixels(AHeight), 1)) + '>', AX, AY, AWidth, AHeight);
end;

procedure TRvRenderHTML.CenterText(const psText: string; const pfX, pfY: double);
begin
  WriteDiv('<P ALIGN=CENTER>' + HTMLText(psText), 0, pfY - Converter.FontData.Height - Pixel, pfX * 2);
end;

procedure TRvRenderHTML.RightText(const psText: string; const pfX, pfY: double);
begin
//  WriteDiv('<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0><TR><TD WIDTH='
//   + IntToStr(InchesToPixels(pfX)) + '><P ALIGN=RIGHT>' + HTMLText(psText)
//   + '</TD></TR></TABLE>', 0, pfY - Converter.FontHeight - Pixel);
  // This works better, but does not work properly in NS - The above table doesnt work in IE
  // it loses the font info.
  WriteDiv('<P ALIGN=RIGHT>' + HTMLText(psText), 0, pfY - Converter.FontData.Height, pfX);
end;

constructor TRvRenderHTML.Create(AOwner: TComponent);
begin
  inherited;
  FLastFileNameIndex := 0;
  FUsingInternalStream := false;
  SetDefaultTemplate;
  DisplayName := 'Web Page (HTML)';
  FileExtension := '*.html;*.htm';
end;

function TRvRenderHTML.GetTemplate: string;
begin
  // MBCS-SAFE
  Result := FTemplateHead + FTemplatePre + '{%Report%}' + FTemplatePost;
end;

procedure TRvRenderHTML.SetTemplate(const AValue: string);
var
  i: integer;
  s: string;
begin
  if AValue = '' then begin
    SetDefaultTemplate;
  end else begin
    //TODO: Pos on uppercase is not efficient.
    i := AnsiPos('{%REPORT%}', AnsiUppercase(AValue));
    if i = 0 then begin
      raise Exception.Create('{%Report%} tag not found.');
    end;
    FTemplatePre := MBCSCopy(AValue, 1, i - 1);
    FTemplatePost := MBCSCopy(AValue, i + 10, MaxInt);
    //
    //TODO: Pos on uppercase is not efficient.
    i := AnsiPos('</HEAD', AnsiUppercase(FTemplatePre));
    if i = 0 then begin
      raise Exception.Create('</HEAD> tag not found.');
    end;
    FTemplateHead := MBCSCopy(FTemplatePre, 1, i - 1);
    FTemplatePre := MBCSCopy(FTemplatePre, i, MaxInt);
    // Extract offsets
    i := Pos('<RAVE ', AValue);
    if i > 0 then begin
      s := MBCSCopy(AValue, i + 6, MaxInt);
      i := AnsiPos('>', s);
      if i > 0 then begin
        SetLength(s, i - 1);
        with TStringList.Create do try
          CommaText := StringReplace(s, ' ', ',', [rfReplaceAll]);
          FOffsetX := StrToInt(Values['OffsetX']);
          FOffsetY := StrToInt(Values['OffsetY']);
        finally Free; end;
      end;
    end;
  end;
end;

function TRvRenderHTML.ProcessParams(const AHTML: string): string;
begin
  Result := AHTML;
  Result := StringReplace(Result, '{%PageNo%}', IntToStr(Converter.PageNo), [rfReplaceAll
   , rfIgnoreCase]);
  Result := StringReplace(Result, '{%TotalPages%}', IntToStr(Converter.PageCount), [rfReplaceAll
   , rfIgnoreCase]);
  Result := StringReplace(Result, '{%MaxY%}', IntToStr(InchesToPixels(ffMaxY + 1) + FOffsetY)
   , [rfReplaceAll, rfIgnoreCase]);
  Result := StringReplace(Result, '{%PrevPageURL%}', 'javascript:PrevPage();', [rfReplaceAll, rfIgnoreCase]);
  Result := StringReplace(Result, '{%NextPageURL%}', 'javascript:NextPage();', [rfReplaceAll, rfIgnoreCase]);
  Result := StringReplace(Result, '{%FirstPageURL%}', 'javascript:GotoPage(1);', [rfReplaceAll
   , rfIgnoreCase]);
  Result := StringReplace(Result, '{%LastPageURL%}', 'javascript:NextPage('
   + IntToStr(Converter.PageCount) + ');', [rfReplaceAll, rfIgnoreCase]);
end;

procedure TRvRenderHTML.FillRect(const pRect: TRect);
begin
  Rectangle(pRect.Left / 72, pRect.Top / 72, pRect.Right / 72, pRect.Bottom / 72);
end;

function TRvRenderHTML.PadIntToStr(ANumber: integer): string;
var
  I1: integer;
begin
  Result := Format('%4d', [ANumber]);
  for I1 := Length(Result) downto 1 do begin
    if Result[I1] = ' ' then begin
      Result[I1] := '0';
    end; { if }
  end; { for }
end;

function TRvRenderHTML.ProcessURL(AValue: string): string;
begin
  Result := StringReplace(AValue, '\', '/', [rfReplaceAll]);
end;

procedure TRvRenderHTML.SetDefaultTemplate;
begin
  FTemplateHead := '';
  with TStringList.Create do try
    {TODO: Make the prev/next page buttons "pretty"}
    Add('<HTML><HEAD>');
    Add('<STYLE TYPE="text/css">');
    Add('<!--');
    Add('DIV {position:absolute}');
    Add('-->');
    Add('</STYLE>');
    Add('</HEAD>');
    Add('<BODY>');
    FTemplatePre := Text;
    Clear;
    Add('<DIV style="COLOR: #000000; FONT: 10pt ''Arial''; LEFT: 0px; TOP: {%MaxY%}px">');
    Add('<HR><P ALIGN=CENTER>');
    Add('<A HREF="{%PrevPageURL%}">Previous Page</A>');
    Add(' --------- ');
    Add('<A HREF="{%NextPageURL%}">Next Page</A>');
    Add('</P></DIV></BODY></HTML>');
    FTemplatePost := Text;
  finally Free; end;
end;

{$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED OFF}{$ENDIF}
procedure TRvRenderHTML.PrintRender(NDRStream: TStream;
                                    OutputFileName: TFileName);
var
  I1: integer;
  LTotalPages: integer;
  LExt: string;
  LBasePathName: string;
  LRender: TRvRenderHTML;
begin
  LExt := ExtractFileExt(OutputFileName);
  LBasePathName := ExtractFilePath(OutputFileName) + ExtractFileName(OutputFileName);
  SetLength(LBasePathname,Length(LBasePathname) - Length(LExt));
  if (LExt = '') or (LExt = '.') then begin
    LExt := MBCSCopy(FileExtension, AnsiPos('.',FileExtension),Length(FileExtension));
    if AnsiPos(';',LExt) > 0 then begin
      LExt := MBCSCopy(LExt, 0, AnsiPos(';',LExt) - 1);
    end; { if }
  end; { if }
  LTotalPages := 1;
  I1 := 1;
  while I1 <= LTotalPages do begin
    NDRStream.Position := 0;
    LRender := TRvRenderHTML.Create(self);
    LRender.OutputFileName := OutputFileName;
    LRender.CacheDir := CacheDir;
    LRender.ServerMode := ServerMode;

    LRender.InitFileStream(LBasePathname + IntToStr(I1) + LExt);
    try
      LRender.PageURL := ExtractFilename(LBasePathname + '{%PAGENO%}' + LExt);
      with TRPConverter.Create(NDRStream, LRender) do try
        Generate(I1);
        LTotalPages := PageCount;
      finally
        Free;
      end; { with }
    finally
      LRender.Free;
    end; { tryf }
    Inc(I1);
  end; { while }
end;
{$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED ON}{$ENDIF}

end.