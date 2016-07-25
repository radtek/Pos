{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvPERpt;

interface

procedure RaveRegister;

implementation

uses
  RvClass, RvCsRpt, RvProj, RvTool, RvToolEvents, RpDefine;

type
  TRaveBandEditor = class(TRaveComponentEditor)
    procedure AddToMenu(AddMenuItem: TAddMenuItemProc); override;
    procedure RunFromMenu(ID: integer); override;
  end; { TRaveBandEditor }

procedure RaveRegister;

begin { RaveRegister }
  RegisterRaveComponentEditor(TRaveBand,TRaveBandEditor);
end;  { RaveRegister }

(*****************************************************************************}
( class TRaveDataBandEditor
(*****************************************************************************)

procedure TRaveBandEditor.AddToMenu(AddMenuItem: TAddMenuItemProc);

begin { AddToMenu }
  AddMenuItem(nil,Trans('Create'),1,0);
  AddMenuItem(nil,Trans('Body Header'),10,1);
  AddMenuItem(nil,Trans('Group Header'),11,1);
  AddMenuItem(nil,Trans('Row Header'),12,1);
  AddMenuItem(nil,Trans('Detail Band'),13,1);
  AddMenuItem(nil,Trans('Row Footer'),14,1);
  AddMenuItem(nil,Trans('Group Footer'),15,1);
  AddMenuItem(nil,Trans('Body Footer'),16,1);
end;  { AddToMenu }

procedure TRaveBandEditor.RunFromMenu(ID: integer);

var
  Band: TRaveBand;
  DataBand: TRaveIterateBand;

  procedure ProcessBand(Band: TRaveBand;
                        PrintLoc: TBandPrintLoc);

  var
    I1: integer;
    InsertPoint: integer;

  begin { ProcessBand }
    Band.BandStyle.PrintLoc := [PrintLoc];
    InsertPoint := Band.ControllerBand.ChildIndex;
    if not (PrintLoc in [plBodyHeader,plGroupHeader,plRowHeader]) then begin
      Inc(InsertPoint);
    end; { if }
    for I1 := 1 to (Band.ChildIndex - InsertPoint) do begin
      Band.MoveBehind;
    end; { for }
  end;  { ProcessBand }

begin { RunFromMenu }
  if (ID < 10) or (ID > 16) then Exit;

  if Instance is TRaveIterateBand then begin
    DataBand := TRaveIterateBand(Instance);
  end else begin
    DataBand := TRaveBand(Instance).ControllerBand;
    if not Assigned(DataBand) then Exit;
  end; { else }

  if ID = 13 then begin
    Band := TRaveDataBand.Create(Instance.Owner);
  end else begin
    Band := TRaveBand.Create(Instance.Owner);
  end; { else }
  ProjectManager.DataChanged := true;
  Band.Name := TRaveProjectManager(Band.Project).GetUniqueName({Trans-}'Band',Band.Owner as TRaveComponent,false);
  Band.Parent := DataBand.Parent;
  Band.ControllerBand := DataBand;
  case ID of
    10: begin
      ProcessBand(Band,plBodyHeader);
    end;
    11: begin
      ProcessBand(Band,plGroupHeader);
    end;
    12: begin
      ProcessBand(Band,plRowHeader);
    end;
    13: begin
      ProcessBand(Band,plDetail);
    end;
    14: begin
      ProcessBand(Band,plRowFooter);
    end;
    15: begin
      ProcessBand(Band,plGroupFooter);
    end;
    16: begin
      ProcessBand(Band,plBodyFooter);
    end;
  end; { case }
  PageDesignerEventHandler.AfterNewComponent(Band);
  AddComponent(Band);
  CurrentDesigner.ClearSelection;
  CurrentDesigner.SelectControl(Band);
end;  { RunFromMenu }

initialization
  RegisterProc({Trans-}'RVCL',RaveRegister);
end.