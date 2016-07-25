{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRpMBCS;

interface

{$I RpVer.inc}

uses
  Classes, SysUtils;

function MBCSNextChar(const AStr: PChar): PChar;
function MBCSCopy(AString: string; APos, ACount: integer): string;
procedure MBCSDelete(var AString: string; APos, ACount: integer);

implementation

function MBCSNextChar(const AStr: PChar): PChar;
begin
  Result := StrNextChar(AStr);
end;

function MBCSCopy(AString: string; APos, ACount: integer): string;
var
  LChar, LNext: PChar;
begin
  Result := '';
  LChar := @AString[1];
  if Assigned(LChar) then begin
    while APos > 1 do begin
      LChar := MBCSNextChar(LChar);
      Dec(APos);
      if LChar^ = #0 then begin
        Exit;
      end;
    end;
    LNext := LChar;
    while (ACount > 0) and (LNext^ <> #0) do begin
      LNext := MBCSNextChar(LNext);
      Dec(ACount);
    end;
    SetString(Result, LChar, LNext - LChar);
  end;
end;

procedure MBCSDelete(var AString: string; APos, ACount: integer);
var
  LStart, LCurrent: PChar;
  LTemp: string;
  LChars: integer;
begin
  LStart := @AString[1];
  LCurrent := @AString[1];
  LChars := 0;
  if Assigned(LCurrent) then begin
    while APos > 1 do begin
      LCurrent := MBCSNextChar(LCurrent);
      Dec(APos);
      Inc(LChars);
      if LCurrent^ = #0 then begin
        Exit;
      end;
    end;
    LTemp := MBCSCopy(LStart, 1, LChars);
    while (ACount > 0) and (LCurrent^ <> #0) do begin
      LCurrent := MBCSNextChar(LCurrent);
      Dec(ACount);
    end;
    LTemp := LTemp + LCurrent;
    AString := LTemp;
  end;
end;


end.