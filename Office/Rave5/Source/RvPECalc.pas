{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvPECalc;

interface

  procedure RaveRegister;

implementation

uses
  Classes, SysUtils, TypInfo, RvDefine, RvClass, RvProj, RvTool, RpDefine, RvCsData;

type
  TRaveControllerPropertyEditor = class(TRaveComponentPropertyEditor)
  protected
    function Include(Value: TComponent;
                     Data: longint): boolean; override;
  end; { TRaveControllerPropertyEditor }

  TRaveInitializerPropertyEditor = class(TRaveComponentPropertyEditor)
  protected
    function Include(Value: TComponent;
                     Data: longint): boolean; override;
  end; { TRaveInitializerPropertyEditor }

  TRaveCalcVarPropertyEditor = class(TRaveComponentPropertyEditor)
  protected
    function GetOptions: TPropertyOptionsSet; override;
    function Include(Value: TComponent;
                     Data: longint): boolean; override;
  end; { TRaveCalcVarPropertyEditor }

  TRaveDestParamPropertyEditor = class(TRaveStringPropertyEditor)
  protected
    function GetOptions: TPropertyOptionsSet; override;
    procedure GetValues(ValueList: TStrings); override;
  end; { TRaveDestParamPropertyEditor }

  TRaveDestPIVarPropertyEditor = class(TRaveStringPropertyEditor)
  protected
    function GetOptions: TPropertyOptionsSet; override;
    procedure GetValues(ValueList: TStrings); override;
  end; { TRaveDestPIVarPropertyEditor }

  procedure RaveRegister;

  begin { RaveRegister }
  {TransOff}
    RegisterRavePropertyEditor(TypeInfo(TRaveComponent),TRaveCalcText,'Controller',
     TRaveControllerPropertyEditor);
    RegisterRavePropertyEditor(TypeInfo(TRaveComponent),TRaveCalcTotal,'Controller',
     TRaveControllerPropertyEditor);
    RegisterRavePropertyEditor(TypeInfo(TRaveComponent),TRaveCalcTotal,'Initializer',
     TRaveInitializerPropertyEditor);

    RegisterRavePropertyEditor(TypeInfo(TRaveCalcVariable),TRaveCalcTotal,'SrcCalcVar',
     TRaveCalcVarPropertyEditor);
    RegisterRavePropertyEditor(TypeInfo(TRaveCalcVariable),TRaveCalcOp,'Src1CalcVar',
     TRaveCalcVarPropertyEditor);
    RegisterRavePropertyEditor(TypeInfo(TRaveCalcVariable),TRaveCalcOp,'Src2CalcVar',
     TRaveCalcVarPropertyEditor);

    RegisterRavePropertyEditor(TypeInfo(string),TRaveCalcOp,'DestParam',
     TRaveDestParamPropertyEditor);
    RegisterRavePropertyEditor(TypeInfo(string),TRaveCalcTotal,'DestParam',
     TRaveDestParamPropertyEditor);
    RegisterRavePropertyEditor(TypeInfo(string),TRaveCalcOp,'DestPIVar',
     TRaveDestPIVarPropertyEditor);
    RegisterRavePropertyEditor(TypeInfo(string),TRaveCalcTotal,'DestPIVar',
     TRaveDestPIVarPropertyEditor);
  {TransOn}
  end;  { RaveRegister }

(*****************************************************************************}
( class TRaveControllerPropertyEditor
(*****************************************************************************)

  function TRaveControllerPropertyEditor.Include(Value: TComponent;
                                                 Data: longint): boolean;

  begin { Include }
    Result := (Value is TRaveComponent) and TRaveComponent(Value).Habla({Trans-}
     'CalcNewData');
  end;  { Include }

(*****************************************************************************}
( class TRaveInitializerPropertyEditor
(*****************************************************************************)

  function TRaveInitializerPropertyEditor.Include(Value: TComponent;
                                                  Data: longint): boolean;

  begin { Include }
    Result := (Value is TRaveComponent) and TRaveComponent(Value).Habla({Trans-}
     'CalcInitData');
  end;  { Include }

(*****************************************************************************}
( class TRaveCalcVarPropertyEditor
(*****************************************************************************)

  function TRaveCalcVarPropertyEditor.GetOptions: TPropertyOptionsSet;

  begin { GetOptions }
    Result := inherited GetOptions - [poMultiSelect];
  end;  { GetOptions }

  function TRaveCalcVarPropertyEditor.Include(Value: TComponent;
                                              Data: longint): boolean;

  begin { Include }
    Result := (Value <> Instance[0]) and (Value is TRaveCalcVariable);
  end;  { Include }

(*****************************************************************************}
( class TRaveDestParamPropertyEditor
(*****************************************************************************)

  function TRaveDestParamPropertyEditor.GetOptions: TPropertyOptionsSet;

  begin { GetOptions }
    Result := [poMultiSelect,poListing];
  end;  { GetOptions }

  procedure TRaveDestParamPropertyEditor.GetValues(ValueList: TStrings);

  var
    I1: integer;

  begin { GetValues }
    ValueList.Assign(ProjectManager.Parameters);
    if not Instance[0].Page.Global then begin
      ValueList.AddStrings((Instance[0].Page.Parent as TRaveReport).Parameters);
      ValueList.AddStrings(Instance[0].Page.Parameters);
    end; { if }
    for I1 := 0 to (ProjectManager.GlobalPageList.Count - 1) do begin
      ValueList.AddStrings(TRavePage(ProjectManager.GlobalPageList[I1]).Parameters);
    end; { for }
  end;  { GetValues }

(*****************************************************************************}
( class TRaveDestPIVarPropertyEditor
(*****************************************************************************)

  function TRaveDestPIVarPropertyEditor.GetOptions: TPropertyOptionsSet;

  begin { GetOptions }
    Result := [poMultiSelect,poListing];
  end;  { GetOptions }

  procedure TRaveDestPIVarPropertyEditor.GetValues(ValueList: TStrings);

  var
    I1: integer;

  begin { GetValues }
    ValueList.Assign(ProjectManager.PIVars);
    if not Instance[0].Page.Global then begin { Add report PIVars }
      ValueList.AddStrings((Instance[0].Page.Parent as TRaveReport).PIVars);
      ValueList.AddStrings(Instance[0].Page.PIVars);
    end; { if }
    for I1 := 0 to (ProjectManager.GlobalPageList.Count - 1) do begin
      ValueList.AddStrings(TRavePage(ProjectManager.GlobalPageList[I1]).PIVars);
    end; { for }
  end;  { GetValues }

initialization
  RegisterProc({Trans-}'RVCL',RaveRegister);
end.