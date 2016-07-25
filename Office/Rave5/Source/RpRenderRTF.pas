{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RpRenderRTF;

interface

{$I RpVer.inc}
{$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED OFF}{$ENDIF}

uses
  Windows,
  Graphics, JPEG, Dialogs,
  Classes, SysUtils, RpRender, RpDefine;

type
  TImageOutput = (ioHigherQuality,ioSmallerSize);
  TImageEncoding = (ieBinary,ieHex);

  TRvRenderRTF = class(TRPRenderStream)
  protected
    FontTable: TStringList;
    ColorTable: TStringList;
    UseFinal: boolean;
    WorkFile: TMemoryStream;
    WorkFileName: string;
    LastGraphicX: double;
    LastGraphicY: double;
    FImageOutput: TImageOutput;
    FImageEncoding: TImageEncoding;

    function IsCJKFont: boolean;
    procedure WriteFont;
    procedure WriteColor;
    procedure WriteStr(Value: string);
    function GetFontIndex: integer;
    function GetRTFColor(const AColor: TColor): string;
    function GetColorIndex(const AColor: TColor): integer;
    function ContainsWideChars(AText: string): boolean;
    procedure WriteText(Text: string);
    procedure WriteLine(Value: string);
    procedure WriteGraphicHeader;
    procedure WriteGraphicRect(X1,Y1,X2,Y2: double);
    procedure WriteXTwips(Command: string; Value: double);
    procedure WriteYTwips(Command: string; Value: double);
    procedure WriteGraphicPen;
    procedure WriteGraphicBrush;
    procedure PageBegin; override;
    procedure PageEnd; override;
    procedure LeftText(const AText: string; const X1,Y1: double); override;
    procedure CenterText(const AText: string; const X1,Y1: double); override;
    procedure RightText(const AText: string; const X1,Y1: double); override;
    procedure PrintSpaces(const AText: string; const AX,AY, AWidth: double); override;
    procedure PrintRightWidth(var AX,AY: double; AText: string; Width: double); override;
    procedure TextRect(Rect: TRect; X1,Y1: double;AText: string); override;
    procedure Rectangle(const X1, Y1, X2, Y2: double); override;
    procedure RoundRect(const X1,Y1,X2,Y2,X3,Y3: double); override;
    procedure MoveTo(const X1, Y1: double); override;
    procedure LineTo(const X1,Y1: double); override;
    procedure Ellipse(const X1,Y1,X2,Y2: double); override;
    procedure WriteImageHeader;
    procedure WriteImageFooter;
    procedure PrintBitmapRect(const X1,Y1,X2,Y2: double; AGraphic:
      Graphics.TBitmap); override;
    procedure WritePictType;
    procedure WritePictSize(Metafile: TMetafile);
    procedure WritePictData(AWidth, AHeight: double; AGraphic: TGraphic);
    procedure MetafileToHex(AMetafile: TMetafile);
    procedure MetafileToBin(AMetafile: TMetafile);
    procedure StretchDraw(const ARect: TRect; AGraphic: TGraphic); override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure PrintRender(NDRStream: TStream; OutputFileName: TFileName); override;
    procedure DocBegin; override;
    procedure DocEnd; override;
  published
    property OnDecodeImage;
    property ImageOutput: TImageOutput read FImageOutput write FImageOutput default ioHigherQuality;
    property ImageEncoding: TImageEncoding read FImageEncoding write FImageEncoding default ieHex;
  end;

implementation

{ TRvRenderRTF }

procedure TRvRenderRTF.CenterText(const AText: string; const X1,Y1: double);
var
  TextWidth: double;
begin
  inherited;
  if AText = '' then Exit;

  TextWidth := Converter.RPTFTextWidth(AText);
  LeftText(AText,X1 - (TextWidth / 2),Y1);
end;

constructor TRvRenderRTF.Create(AOwner: TComponent);
begin
  inherited;
  DisplayName := 'Rich Text Format (RTF)';
  FileExtension := '*.rtf';
  FImageOutput := ioHigherQuality;
  FImageEncoding := ieHex;
end;

destructor TRvRenderRTF.Destroy;
begin
  inherited;
end;

procedure TRvRenderRTF.DocBegin;
begin { DocBegin }
  inherited;

  FontTable := TStringList.Create;
  ColorTable := TStringList.Create;

{ Create work file }
  WorkFile := TMemoryStream.Create;
  UseFinal := false;
end;

procedure TRvRenderRTF.DocEnd;
var
  I1: integer;
begin { DocEnd }
  try
  { Transfer items to final file along with fonts and colors }
    UseFinal := true;

  { Create OutputStream if not exists }
    if FOutputStream = nil then begin
      InitFileStream(OutputFileName);
    end; { if }

  { Write the header }
    WriteStr('{\rtf1\ansi');

  { Write the font table }
    WriteStr('{\fonttbl');
    for I1 := 0 to (FontTable.Count - 1) do begin
      WriteStr('{\f' + IntToStr(I1) + FontTable[I1] + ';}');
    end; { for }
    WriteStr('}');

  { Write the color table }
    WriteStr('{\colortbl');
    for I1 := 0 to (ColorTable.Count - 1) do begin
      WriteStr(ColorTable[I1]);
    end; { for }
    WriteStr('}');

  { Document width/height }
  with Converter do begin
    WriteStr('\paperw' + IntToStr(Round(1440 * PaperWidth)));
    WriteStr('\paperh' + IntToStr(Round(1440 * PaperHeight)));
    if ReportHeader.Orientation = poLandscape then begin
      WriteStr('\landscape');
    end; { if }
  end; { if }

  WriteStr('\lytprtmet');

  { Document View Mode:  Page Layout View }
    WriteStr ('\viewkind1');

  { Transfer all items in WorkFile to FinalFile }
    WorkFile.Position := 0;
    WorkFile.SaveToStream(ActiveStream);
    FreeAndNil(FontTable);
    FreeAndNil(ColorTable);

  { Finish off RTF stream }
    WriteStr('}');

  finally
  { Close/Erase work file }
    FreeAndNil(WorkFile);
    FreeFileStream;
  end; { try }
end;  { DocEnd }

procedure TRvRenderRTF.WriteStr(Value: string);
begin { WriteStr }
  if Value = '' then Exit;
  if UseFinal then begin
    OutputStream.WriteBuffer(Value[1],Length(Value));
  end else begin
    WorkFile.WriteBuffer(Value[1],Length(Value));
  end; { else }
end;  { WriteStr }

function TRvRenderRTF.GetFontIndex: integer;
var
  I1: integer;
  S1: string;

  function CJKFontName: string;
  var
    I1: integer;
  begin
    Result := '';
    for I1 := 1 to Length(Converter.Font.Name) do begin
      Result := Result + Format('\''%s', [IntToHex(Ord(Converter.Font.Name[I1]),2)]);
    end;
  end;

begin { GetFontIndex }
  S1 := '\fcharset' + IntToStr((Converter.Font.Charset)) + '\fprq0 ';
  if ContainsWideChars(Converter.Font.Name) then begin
    S1 := S1 + CJKFontName;
  end else begin
    S1 := S1 + Converter.Font.Name;
  end;
  Result := -1;
  for I1 := 0 to (FontTable.Count - 1) do begin
    if UpperCase(FontTable[I1]) = UpperCase(S1) then begin
      Result := I1;
      Break;
    end; { if }
  end; { for }
  if Result < 0 then begin
    FontTable.Add(S1);
    Result := FontTable.Count - 1;
  end; { if }
end;  { GetFontIndex }

procedure TRvRenderRTF.WriteFont;
begin { WriteFont }
  with Converter.Font do begin
  { Write out font characteristics }
    WriteStr('\f' + IntToStr(GetFontIndex) + '\fs' + IntToStr(Round(Size * 2)));
    if fsBold in Style then begin
      WriteStr('\b');
    end else begin
      WriteStr('\b0');
    end; { else }
    if fsItalic in Style then begin
      WriteStr('\i');
    end else begin
      WriteStr('\i0');
    end; { else }
    if fsUnderline in Style then begin
      WriteStr('\ul');
    end else begin
      WriteStr('\ul0');
    end; { else }
    if fsStrikeout in Style then begin
      WriteStr('\strike');
    end else begin
      WriteStr('\strike0');
    end; { else }
    if Converter.FontData.Superscript then begin
      WriteStr('\super');
    end else if Converter.FontData.Subscript then begin
      WriteStr('\sub');
    end else begin
      WriteStr('\nosupersub');
    end; { else }
  end; { with }
end;  { WriteFont }

function TRvRenderRTF.IsCJKFont: boolean;
begin
  case ConvertCharset((Converter.Font.Charset)) of
    Windows.SHIFTJIS_CHARSET,
    Windows.HANGEUL_CHARSET,
    Windows.GB2312_CHARSET,
    Windows.CHINESEBIG5_CHARSET,
    Windows.JOHAB_CHARSET: begin
      Result := true;
    end else begin
      Result := false;
    end
  end; { case }
end;

function TRvRenderRTF.ContainsWideChars(AText: string): boolean;
var
  I1: integer;
begin
  Result := false;
  if IsCJKFont then begin
    for I1 := 1 to Length(AText) do begin
      if Ord(AText[I1]) >= $80 then begin
        Result := true;
        Break;
      end; { if }
    end; { for }
  end; { if }
end;


procedure TRvRenderRTF.WriteText(Text: string);
var
  I1: integer;
  S1: string;
  Ch1: array[1..3] of WideChar;
  Ch2: array[1..3] of AnsiChar;
begin { WriteText }
  if ContainsWideChars(Text) then begin
    S1 := '';
    I1 := 0;
    while I1 < Length(Text) do begin
      Inc(I1);
      if (Ord(Text[I1]) >= $80) and (I1 < Length(Text)) then begin
        //StringToWideChar(Copy(Text,I1,2),@Ch,2);
        MultiByteToWideChar(GetACP,0,@Text[I1],2,@Ch1,sizeof(Ch1));
        WideCharToMultiByte(GetACP,0,@Ch1, -1, @Ch2, sizeof(Ch2), nil, nil);
        S1 := S1 + '\uc2\u' + IntToStr(Ord(Ch1[1])) + '\''' + IntToHex(Ord(Ch2[1]),2) + '\''' + IntToHex(Ord(Ch2[2]),2);
        Inc(I1);
      end else begin


      // Method 2 - preferred
        if Text[I1] in ['\','{','}'] then begin
          S1 := S1 + '\';
        end; { if }
        S1 := S1 + Text[I1];

      end; { else }
    end; { while }
  end else begin
  { Look for any special characters }
    I1 := 0;
    while I1 < Length(Text) do begin
      Inc(I1);
      if Text[I1] in ['\','{','}'] then begin
        Insert('\',Text,I1);
        Inc(I1);
      end; { if }
    end; { while }
    S1 := Text;
  end; { else }
  WriteStr(S1);
end;  { WriteText }


procedure TRvRenderRTF.LeftText(const AText: string; const X1,Y1: double);
var
  Rect: TRect;
  FontHeight: double;
begin { LeftText }
  FontHeight := Converter.Font.Size / 72;
  Rect.Left := Round(X1 * 72);
  Rect.Top := Round((Y1 - FontHeight) * 72);
  Rect.Right := Round((X1 + Converter.RPTFTextWidth(AText)) * 72) + 1;
  Rect.Bottom := Round((Y1 + ((FontHeight * 1.25) - FontHeight)) * 72);
  TextRect(Rect,X1,Y1,AText);
//  WriteText(AText);
end;  { LeftText }

procedure TRvRenderRTF.PageEnd;
begin
  inherited;
end;

{$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED OFF}{$ENDIF}
procedure TRvRenderRTF.PrintRender(NDRStream: TStream;
  OutputFileName: TFileName);
var
  LExtension: string;
begin
  if Pos('.',OutputFileName) < 1 then begin
    LExtension := Copy(FileExtension, Pos('.',FileExtension),Length(FileExtension));
    if Pos(';',LExtension) > 0 then begin
      LExtension := Copy(LExtension, 0, Pos(';',LExtension) - 1);
    end; { if }

    OutputFileName := OutputFileName + LExtension;
  end; { if }
  InitFileStream(OutputFileName);
  with TRPConverter.Create(NDRStream, self) do try
    Generate;
  finally
    Free;
  end; { with }
end;
{$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED ON}{$ENDIF}

procedure TRvRenderRTF.PrintSpaces(const AText: string; const AX,AY,
  AWidth: double);
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
  LText := AText;
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

  SpaceWidth := SpaceWidth + ((AWidth - Converter.RPTFTextWidth(LText)) / Spaces);

{ Print first word left aligned, middle and last words right aligned }
  CurrWord := GetFirstWord(LText);
  XPos := AX;
  YPos := AY;
  LeftText(CurrWord, XPos, YPos);
  AdjustXY(XPos,YPos,Converter.RPTFTextWidth(CurrWord));
  while LText <> '' do begin
    SpacesStr := GetSpaces(LText);
    Spaces := Length(SpacesStr);
    CurrWord := GetFirstWord(LText);
    // Need to print SpacesStr so underlining works
    PrintRightWidth(XPos,YPos,SpacesStr + CurrWord,Converter.RPTFTextWidth(CurrWord) + (Spaces * SpaceWidth));
  end; { while }
end;

procedure TRvRenderRTF.PrintRightWidth(var AX, AY: double; AText: string;
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

procedure TRvRenderRTF.RightText(const AText: string; const X1,Y1: double);
begin
  inherited;

  if AText = '' then Exit;

  LeftText(AText,X1 - Converter.RPTFTextWidth(AText), Y1);
end;

procedure TRvRenderRTF.PageBegin;
begin
  if Converter.PageNo > 1 then begin
    WriteStr('\page ');
  end;
end;

procedure TRvRenderRTF.TextRect(Rect: TRect; X1, Y1: double; AText: string);
begin
  WriteGraphicHeader;
  WriteStr('\dptxbx\dptxbxmar0');
  WriteStr('{\dptxbxtext {\f' + IntToStr(GetFontIndex) + '\fs' +
   IntToStr(Round(Converter.Font.Size * 2)) + ' ');
  WriteFont;
  WriteColor;
  WriteText(AText);
  WriteStr('\par}}');
  WriteGraphicRect(Rect.Left/72,Rect.Top/72,Rect.Right/72,Rect.Bottom/72);
  WriteStr('\dplinehollow\dplinecor0\dplinecog0\dplinecob0\dplinew1');
  WriteStr('\dpfillfgcr255\dpfillfgcg255\dpfillfgcb255');
  WriteStr('\dpfillbgcr255\dpfillbgcg255\dpfillbgcb255\dpfillpat0');
  WriteStr('}');
end;

procedure TRvRenderRTF.WriteGraphicHeader;
begin
  WriteLine('');
  WriteStr('{\*\do\dolock\dobxpage\dobypage');
end;

procedure TRvRenderRTF.WriteGraphicRect(X1, Y1, X2, Y2: double);
begin
  if X1 < X2 then begin
    WriteXTwips('\dpx',X1);
  end else begin
    WriteXTwips('\dpx',X2);
  end; { else }
  if Y1 < Y2 then begin
    WriteYTwips('\dpy',Y1);
  end else begin
    WriteYTwips('\dpy',Y2);
  end; { else }
  WriteXTwips('\dpxsize',Abs(X1 - X2));
  WriteYTwips('\dpysize',Abs(Y1 - Y2));
end;

procedure TRvRenderRTF.WriteLine(Value: string);
begin
  WriteStr(Value + #13#10);
end;

procedure TRvRenderRTF.WriteXTwips(Command: string; Value: double);
begin
  WriteStr(Command + IntToStr(Round(Value * 1440)));
end;

procedure TRvRenderRTF.WriteYTwips(Command: string; Value: double);
begin
  WriteStr(Command + IntToStr(Round(Value * 1440)));
end;

function TRvRenderRTF.GetColorIndex(const AColor: TColor): integer;
var
  I1: integer;
  CurrColor: string;
begin { GetColorIndex }
  Result := -1;
  CurrColor := GetRTFColor(AColor);
  for I1 := 0 to (ColorTable.Count - 1) do begin

    if ColorTable[I1] = CurrColor then begin
      Result := I1 + 1;
      Break;
    end; { if }
  end; { for }
  if Result < 0 then begin
    ColorTable.Add(CurrColor);
    Result := ColorTable.Count;
  end; { if }
end;  { GetColorIndex }

function TRvRenderRTF.GetRTFColor(const AColor: TColor): string;
var
  sColor: string;
  iRed: integer;
  iGreen: integer;
  iBlue: integer;
begin
  Result := ';';
  if AColor >= 0 then begin
    sColor := IntToHex(AColor, 8);
  end else begin
    sColor := IntToHex(clBlack, 8);
  end; { else }
  iBlue := StrToInt('$' + Copy(sColor, 3,2));
  iGreen := StrToInt('$' + Copy(sColor, 5,2));
  iRed := StrToInt('$' + Copy(sColor, 7, 2));
  sColor := Format(';\red%d\green%d\blue%d', [iRed, iGreen, iBlue]);
  Result := sColor;
end;

procedure TRvRenderRTF.WriteColor;
begin
  WriteStr('\cf' + IntToStr(GetColorIndex(Converter.Font.Color)));
  WriteStr('\cb' + IntToStr(GetColorIndex(Converter.Brush.Color)));
  WriteStr(' ');
end;

procedure TRvRenderRTF.Rectangle(const X1,Y1,X2,Y2: double);
begin
  WriteGraphicHeader;
  WriteStr('\dprect');
  WriteGraphicRect(X1,Y1,X2,Y2);
  WriteGraphicPen;
  WriteGraphicBrush;
  WriteStr('}');
end;

procedure TRvRenderRTF.WriteGraphicBrush;
begin { WriteGraphicBrush }
  WriteStr('\dpfillfgcr255\dpfillfgcg255\dpfillfgcb255');
  WriteStr('\dpfillbgcr' + IntToStr(GetRValue(Converter.Brush.Color)));
  WriteStr('\dpfillbgcg' + IntToStr(GetGValue(Converter.Brush.Color)));
  WriteStr('\dpfillbgcb' + IntToStr(GetBValue(Converter.Brush.Color)));
  WriteStr('\dpfillpat');
  case Converter.Brush.Style of
    bsClear: WriteStr('0');
    bsSolid: WriteStr('1');
    bsHorizontal: WriteStr('20');
    bsVertical: WriteStr('21');
    bsFDiagonal: WriteStr('22');
    bsBDiagonal: WriteStr('23');
    bsCross: WriteStr('24');
    bsDiagCross: WriteStr('25');
  end; { case }
end;  { WriteGraphicBrush }

procedure TRvRenderRTF.WriteGraphicPen;
begin { WriteGraphicPen }
  case Converter.Pen.Style of
    psSolid: WriteStr('\dplinesolid');
    psDash: WriteStr('\dplinedash');
    psDot: WriteStr('\dplinedot');
    psDashDot: WriteStr('\dplinedado');
    psDashDotDot: WriteStr('\dplinedadodo');
    psClear: WriteStr('\dplinehollow');
  end; { case }
  WriteStr('\dplinecor' + IntToStr(GetRValue(Converter.Pen.Color)));
  WriteStr('\dplinecog' + IntToStr(GetGValue(Converter.Pen.Color)));
  WriteStr('\dplinecob' + IntToStr(GetBValue(Converter.Pen.Color)));
  WriteXTwips('\dplinew',Converter.Pen.Width/72);
end;  { WriteGraphicPen }

procedure TRvRenderRTF.LineTo(const X1,Y1: double);
var
  CompareX: boolean;
  CompareY: boolean;
begin { LineTo }
  WriteGraphicHeader;
  WriteStr('\dpline');
  CompareX := X1 <= LastGraphicX;
  CompareY := Y1 <= LastGraphicY;
  WriteXTwips('\dpptx',0);
  if CompareX = CompareY then begin
    WriteYTwips('\dppty',0);
    WriteXTwips('\dpptx',Abs(X1 - LastGraphicX));
    WriteYTwips('\dppty',Abs(Y1 - LastGraphicY));
  end else begin
    WriteYTwips('\dppty',Abs(Y1 - LastGraphicY));
    WriteXTwips('\dpptx',Abs(X1 - LastGraphicX));
    WriteYTwips('\dppty',0);
  end; { else }
  WriteGraphicRect(X1,Y1,LastGraphicX,LastGraphicY);
  WriteGraphicPen;
  WriteStr('}');
  MoveTo(X1,Y1);
end;  { LineTo }

procedure TRvRenderRTF.MoveTo(const X1,Y1: double);
begin { MoveTo }
  LastGraphicX := X1;
  LastGraphicY := Y1;
end;  { MoveTo }

procedure TRvRenderRTF.Ellipse(const X1, Y1, X2, Y2: double);
begin { Ellipse }
  WriteGraphicHeader;
  WriteStr('\dpellipse');
  WriteGraphicRect(X1,Y1,X2,Y2);
  WriteGraphicPen;
  WriteGraphicBrush;
  WriteStr('}');
end;  { Ellipse }

procedure TRvRenderRTF.RoundRect(const X1, Y1, X2, Y2, X3, Y3: double);
begin { RoundRect }
  WriteGraphicHeader;
  WriteStr('\dprect\dproundr');
  WriteGraphicRect(X1,Y1,X2,Y2);
  WriteGraphicPen;
  WriteGraphicBrush;
  WriteStr('}');
end;  { RoundRect }

procedure TRvRenderRTF.WritePictType;
begin
  if ImageOutput = ioSmallerSize then begin
    WriteStr('\emfblip');
  end else begin
    WriteStr('\wmetafile8');
  end; { else }
end;

procedure TRvRenderRTF.WritePictSize(Metafile: TMetafile);
begin
  WriteStr('\picw');
  WriteStr(IntToStr(Metafile.MMWidth));
  WriteStr('\pich');
  WriteStr(IntToStr(Metafile.MMHeight));

  WriteXTwips('\picwgoal',Metafile.Width);
  WriteYTwips('\pichgoal',Metafile.Height);
end;

procedure TRvRenderRTF.MetafileToHex(AMetafile: TMetafile);
var
  ImageStream: TMemoryStream;
  HexStr: string;
  I1: integer;
  I2: byte;
  DataOffset: byte;
begin
  ImageStream := TMemoryStream.Create;
  try
    AMetafile.SaveToStream(ImageStream);
    WritePictSize(AMetafile);
    WriteStr(' ');
    if ImageOutput = ioSmallerSize then begin
      DataOffset := 0;
    end else begin
      DataOffset := 22;
    end; { else }

    ImageStream.Position := DataOffset;
    for I1 := 0 to ImageStream.Size - DataOffset - 1 do begin
      ImageStream.Read(I2,1);
      HexStr := IntToHex(I2,2);
      WriteStr(HexStr);
      if (I1 + 1) mod 32 = 0 then begin
        WriteLine('');
      end; // if
    end;
  finally
    FreeAndNil(ImageStream);
  end; { tryf }
end;

procedure TRvRenderRTF.MetafileToBin(AMetafile: TMetafile);
var
  ImageStream: TMemoryStream;
begin
    ImageStream := TMemoryStream.Create;
    try
      AMetafile.SaveToStream(ImageStream);
      if ImageOutput = ioSmallerSize then begin
        ImageStream.Position := 0;
      end else begin
        ImageStream.Position := 22;
      end; { else }
      WritePictSize(AMetafile);
      WriteStr('\bin');
      WriteStr(IntToStr(ImageStream.Size - ImageSTream.Position));
      WriteStr(' ');
      if UseFinal then begin
        OutputStream.CopyFrom(ImageStream,ImageStream.Size - ImageStream.Position);
      end else begin
        WorkFile.CopyFrom(ImageStream,ImageStream.Size - ImageStream.Position);
      end; { else }

    finally
      FreeAndNil(ImageStream);
    end; { tryf }
end;

procedure TRvRenderRTF.WritePictData(AWidth, AHeight: double; AGraphic: TGraphic);
var
  Metafile: TMetafile;
  MetafileCanvas: TMetafileCanvas;
  IW,IH: integer;
begin
  Metafile := TMetafile.Create;

  try
    if ImageOutput = ioSmallerSize then begin
      Metafile.Enhanced := true;
    end else begin
      Metafile.Enhanced := false;
    end; { else }
    IW := Round(AWidth * 102);
    IH := Round(AHeight * 109);
    Metafile.Width := IW;
    Metafile.Height := IH;

    MetafileCanvas := TMetafileCanvas.Create(Metafile,0);
    try
      MetafileCanvas.StretchDraw(Rect(0,0,IW,IH),AGraphic);
    finally
      MetafileCanvas.Free;
    end; { tryf }
    if ImageEncoding = ieBinary then begin
      MetafileToBin(Metafile);
    end else begin
      MetafileToHex(Metafile);
    end; { else }
  finally
    Metafile.Free;
  end; { tryf }
end;

procedure TRvRenderRTF.PrintBitmapRect(const X1, Y1, X2, Y2: double; AGraphic:
  Graphics.TBitmap);
begin
  WriteImageHeader;
  WritePictType;
  WritePictData(Abs(X1-X2),Abs(Y1-Y2), AGraphic);
  WriteStr('}\par}}');
  WriteLine('');
  WriteGraphicRect(X1,Y1,X2,Y2);
  WriteImageFooter;
end;

procedure TRvRenderRTF.WriteImageHeader;
begin
  WriteLine('');
  WriteLine('');
  WriteStr('{\*\do\dobxpage\dobypage\dptxbx\dptxlrtb');
  WriteLine('');
  WriteStr('{\dptxbxtext\pard\plain \ql ');
  WriteStr('\li0\ri0\widctlpar\aspalpha\aspnum\faauto\adjustright');
  WriteStr('\rin0\lin0\itap0 ');
  WriteStr('\fs24\lang1033\langfe1033\cgrid\langnp1033\langfenp1033');
  WriteStr('{');

  WriteLine('');
  WriteStr('{\pict ');
end;

procedure TRvRenderRTF.WriteImageFooter;
begin
  WriteStr('\dplinehollow\dplinecor0\dplinecog0\dplinecob0\dplinew1');
  WriteStr('\dpfillfgcr255\dpfillfgcg255\dpfillfgcb255');
  WriteStr('\dpfillbgcr255\dpfillbgcg255\dpfillbgcb255\dpfillpat0');
  WriteStr('}');
end;

procedure TRvRenderRTF.StretchDraw(const ARect: TRect; AGraphic: TGraphic);
var
  X1,Y1,X2,Y2: double;
begin
  X1 := ARect.Left / XDPI;
  Y1 := ARect.Top / YDPI;
  X2 := ARect.Right / XDPI;
  Y2 := ARect.Bottom / YDPI;
  WriteImageHeader;
  WritePictType;
  WritePictData(Abs(X1-X2),Abs(Y1-Y2), AGraphic);
  WriteStr('}\par}}');
  WriteLine('');
  WriteGraphicRect(X1,Y1,X2,Y2);
  WriteImageFooter;
end;
end.