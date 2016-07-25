{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvLESystem;

interface

uses
  Windows, Graphics, Dialogs, Controls,
  Classes, SysUtils, RpDefine, RvDefine, RvClass, RvData, RvProj, RvCsStd,
  RvCsData, RvCsRpt, RvCsBars, RvCsDraw, RpBars, RvDirectDataView, RvDataField,
  RvDatabase, RvDriverDataView, RpBase, RvSecurity;

  procedure RaveRegister;

implementation

type
  TRaveFunction = procedure;
  TRaveFuncItem = record
    A: pointer;
    B: string;
    C: string;
  end; { TRaveFuncItem }

  procedure Beep;
  begin { Beep }
    MessageBeep($FFFF);
  end;  { Beep }

{$I RvLESystem.inc}

  procedure RaveRegister;

  var
    I1: integer;

  begin { RaveRegister }
  // Register all Rave interface functions
    for I1 := 1 to RaveFuncItemCount do begin
      with RaveFuncItems[I1] do begin
        RegisterRaveFunc(A^,B,C);
      end; { with }
    end; { for }
  end;  { RaveRegister }

initialization
  RegisterProc({Trans-}'RVCL',RaveRegister);
end.