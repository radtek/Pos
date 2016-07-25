{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvPEBand;

interface

uses
  Windows, Messages,
  SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, RvCsRpt, RvClass, RvUtil, RvTool, RpDefine, RvToolUtil;

type
  TBandStyleDialog = class(TForm)
    Panel2: TPanel;
    ScrollBox2: TScrollBox;
    GroupBox2: TGroupBox;
    BodyHeaderCB: TCheckBox;
    GroupHeaderCB: TCheckBox;
    RowHeaderCB: TCheckBox;
    DetailCB: TCheckBox;
    RowFooterCB: TCheckBox;
    GroupFooterCB: TCheckBox;
    BodyFooterCB: TCheckBox;
    GroupBox1: TGroupBox;
    FirstCB: TCheckBox;
    NewPageCB: TCheckBox;
    NewColumnCB: TCheckBox;
    Button1: TButton;
    Button2: TButton;
    Panel1: TPanel;
    Label1: TLabel;
    Panel3: TPanel;
    ScrollBox1: TScrollBox;
    Image1: TImage;
    procedure CheckBoxClick(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormKeyPress(Sender: TObject; var Key: Char);
  protected
    FBandStyle: TRaveBandStyle;
    FBand: TRaveBand;
    FRegion: TRaveRegion;
    NoDraw: boolean;

    procedure SetBandStyle(Value: TRaveBandStyle);
    procedure SetBand(Value: TRaveBand);
    procedure DrawBands;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;

    property BandStyle: TRaveBandStyle read FBandStyle write SetBandStyle;
    property Band: TRaveBand read FBand write SetBand;
    property Region: TRaveRegion read FRegion;
  end;

var
  BandStyleDialog: TBandStyleDialog;

  procedure RaveRegister;

implementation

{$R *.dfm}

type
  TRaveBandStylePropertyEditor = class(TRavePropertyEditor)
  protected
    function GetOptions: TPropertyOptionsSet; override;
    procedure Edit; override;
    function GetValue: string; override;
  end; { TRaveBandStylePropertyEditor }

  TRaveControllerBandPropertyEditor = class(TRaveComponentPropertyEditor)
  protected
    function GetOptions: TPropertyOptionsSet; override;
    function Include(Value: TComponent;
                     Data: longint): boolean; override;
  end; { TRaveControllerBandPropertyEditor }

  procedure RaveRegister;

  begin { RaveRegister }
  {TransOff}
    RegisterRavePropertyEditor(TypeInfo(TRaveBandStyle),nil,'',
     TRaveBandStylePropertyEditor);
    RegisterRavePropertyEditor(TypeInfo(TRaveIterateBand),TRaveBand,'ControllerBand',
     TRaveControllerBandPropertyEditor);
  {TransOn}
  end;  { RaveRegister }

(*****************************************************************************}
( class TRaveBandStylePropertyEditor
(*****************************************************************************)

  function TRaveBandStylePropertyEditor.GetOptions: TPropertyOptionsSet;

  begin { GetOptions }
    Result := [poReadOnly,poEditor];
  end;  { GetOptions }

  function TRaveBandStylePropertyEditor.GetValue: string;

  begin { GetValue }
    Result := '(' + Trans('Band Styles') + ')';
  end;  { GetValue }

  procedure TRaveBandStylePropertyEditor.Edit;

  var
    I1: integer;

  begin { Edit }
    with TBandStyleDialog.Create(Application) do try
    { Init dialog contents }
      BandStyle := TRaveBandStyle(GetOrdValue(0));
      Band := Instance[0] as TRaveBand;
      if ShowModal = mrOK then begin
      { Copy from dialog contents }
        for I1 := 0 to InstCount - 1 do begin
          TRaveBandStyle(GetOrdValue(I1)).Assign(BandStyle);
          if Instance[I1] is TRaveControl then begin
            TRaveControl(Instance[I1]).Invalidate;
          end; { if }
        end; { for }
        Modified;
      end; { if }
    finally
      Free;
    end; { with }
  end;  { Edit }

(*****************************************************************************}
( class TRaveControllerBandPropertyEditor
(*****************************************************************************)

  function TRaveControllerBandPropertyEditor.GetOptions: TPropertyOptionsSet;

  begin { GetOptions }
    Result := [poListing,poMultiSelect];
  end;  { GetOptions }

  function TRaveControllerBandPropertyEditor.Include(Value: TComponent;
                                                     Data: longint): boolean;

  var
    Band: TRaveIterateBand;

    function IsCircular(ControllerBand: TRaveIterateBand): boolean;

    begin { IsCircular }
      Result := (ControllerBand = Instance[0]);
      if not Result and Assigned(ControllerBand.ControllerBand) then begin
        Result := IsCircular(ControllerBand.ControllerBand);
      end; { if }
    end;  { IsCircular }

  begin { Include }
    if Value is TRaveIterateBand then begin
      Band := TRaveIterateBand(Value);
      Result := (Band <> Instance[0]) and (Band.Parent = Instance[0].Parent) and
       (not Assigned(Band.ControllerBand) or not IsCircular(Band.ControllerBand));
    end else begin
      Result := false;
    end; { else }
  end;  { Include }

(*****************************************************************************}
( class TRaveBandDialog
(*****************************************************************************)

  constructor TBandStyleDialog.Create(AOwner: TComponent);

  begin { Create }
    inherited Create(AOwner);
    FBandStyle := TRaveBandStyle.Create;
  end;  { Create }

  destructor TBandStyleDialog.Destroy;

  begin { Destroy }
    FreeAndNil(FBandStyle);
    inherited Destroy;
  end;  { Destroy }

  procedure TBandStyleDialog.SetBandStyle(Value: TRaveBandStyle);

  begin { SetBandStyle }
    NoDraw := true;
    FBandStyle.Assign(Value);
    with FBandStyle do begin
      BodyHeaderCB.Checked := plBodyHeader in PrintLoc;
      GroupHeaderCB.Checked := plGroupHeader in PrintLoc;
      RowHeaderCB.Checked := plRowHeader in PrintLoc;
      DetailCB.Checked := plDetail in PrintLoc;
      RowFooterCB.Checked := plRowFooter in PrintLoc;
      GroupFooterCB.Checked := plGroupFooter in PrintLoc;
      BodyFooterCB.Checked := plBodyFooter in PrintLoc;
      FirstCB.Checked := poFirst in PrintOcc;
      NewPageCB.Checked := poNewPage in PrintOcc;
      NewColumnCB.Checked := poNewColumn in PrintOcc;
    end; { with }
    NoDraw := false;
  end;  { SetBandStyle }

  procedure TBandStyleDialog.SetBand(Value: TRaveBand);

  begin { SetBand }
    FBand := Value;
    FRegion := FBand.Parent as TRaveRegion;
  end;  { SetBand }

  procedure TBandStyleDialog.DrawBands;

  const
    MaxLines = 250;

  var
    LineCount: integer;
    DoPrintBand: boolean;
    Rect: TRect;
    X1: integer;
    Y1: integer;
    BandList: TList;
    IterateCount: integer;

    procedure ExecuteLoc(ControllerBand: TRaveIterateBand;
                         PrintLoc: TBandPrintLoc;
                         Level: integer);

    const
      PrintLocCh: array[TBandPrintLoc] of char = ('B','G','R','M','D','r','g','b');

    var
      BandSearchRec: TBandSearchRec;
      SubBand: TRaveBand;
      SubBandDetail: PBandDetail;
      I1: integer;
      S1: string;

    begin { ExecuteLoc }
      SubBand := Region.FirstSubBand(BandSearchRec,ControllerBand,PrintLoc);
      while Assigned(SubBand) do begin
        SubBandDetail := nil;
        for I1 := 0 to BandList.Count - 1 do begin
          SubBandDetail := PBandDetail(BandList[I1]);
          if SubBandDetail.Band = SubBand then Break;
        end; { for }

        if SubBand is TRaveIterateBand then begin
          ExecuteLoc(TRaveIterateBand(SubBand),plBodyHeader,Level);
          ExecuteLoc(TRaveIterateBand(SubBand),plGroupHeader,Level);
          for I1 := 1 to IterateCount do begin
            ExecuteLoc(TRaveIterateBand(SubBand),plRowHeader,Level);
            with Image1.Picture.Bitmap.Canvas do begin
            { Check LineCount }
              Inc(LineCount); { Band Print }
              if LineCount >= MaxLines then begin
                if DoPrintBand then begin
                  Font.Name := {Trans-}'Arial';
                  Font.Size := 8;
                  Font.Color := clBlack;
                  Font.Style := [];
                  TextOut(X1,Y1,' ' + Trans('It''s all pretty much the same from here.'));
                end; { if }
                Exit;
              end; { if }

            { Output Line Contents }
              if DoPrintBand then begin
                X1 := 5 + Level * 12;
              { Print diamond }
                Brush.Color := BandColor[SubBandDetail.MasterColor];
                Polygon([Point(X1 + 6,Y1),
                 Point(X1 + 12,Y1 + 6),
                 Point(X1 + 6,Y1 + 12),
                 Point(X1,Y1 + 6)]);
                Inc(X1,16);

              { Print text }
                Brush.Color := clWhite;
                Font.Name := {Trans-}'Arial';
                Font.Size := 8;
                Font.Color := clBlack;
                if SubBand = Band then begin
                  Font.Style := [fsBold,fsUnderline];
                end else begin
                  Font.Style := [];
                end; { else }
                S1 := SubBand.Name + {Trans-}' (';
                if Assigned(SubBand.ControllerBand) then begin
                  S1 := S1 + Trans('Detail') + ')';
                end else begin
                  S1 := S1 + Trans('Master') + ')';
                end; { else }
                TextOut(X1,Y1,S1);
                Inc(Y1,16);
              end; { if }
            end; { with }
            ExecuteLoc(TRaveIterateBand(SubBand),plDetail,Level + 1);
            ExecuteLoc(TRaveIterateBand(SubBand),plRowFooter,Level);
          end;
          ExecuteLoc(TRaveIterateBand(SubBand),plGroupFooter,Level);
          ExecuteLoc(TRaveIterateBand(SubBand),plBodyFooter,Level);
        end else begin
          with Image1.Picture.Bitmap.Canvas do begin
          { Check LineCount }
            Inc(LineCount); { Band Print }
            if LineCount >= MaxLines then begin
              if DoPrintBand then begin
                Font.Name := {Trans-}'Arial';
                Font.Size := 8;
                Font.Color := clBlack;
                Font.Style := [];
                TextOut(X1,Y1,' ' + Trans('It''s all pretty much the same from here.'));
              end; { if }
              Exit;
            end; { if }

          { Output line contents }
            if DoPrintBand then begin
              X1 := 5 + Level * 12;
            { Print symbol - up/down arrow or diamond }
              if SubBandDetail.PointerColor = 0 then begin
                Brush.Color := BandColor[SubBandDetail.MasterColor];
                Polygon([Point(X1 + 6,Y1),
                 Point(X1 + 12,Y1 + 6),
                 Point(X1 + 6,Y1 + 12),
                 Point(X1,Y1 + 6)]);
              end else begin
                Brush.Color := BandColor[SubBandDetail.PointerColor];
                if PrintLoc in [plBodyHeader,plGroupHeader,plRowHeader] then begin
                  Polygon([Point(X1 + 2,Y1),Point(X1 + 10,Y1),Point(X1 + 6,Y1 + 12)]);
                end else begin
                  Polygon([Point(X1 + 6,Y1),Point(X1 + 2,Y1 + 12),Point(X1 + 10,Y1 + 12)]);
                end; { else }
              end; { if }
              Inc(X1,16);

              Brush.Color := clWhite;
              Font.Name := {Trans-}'Arial';
              Font.Size := 8;
              Font.Color := clBlack;
              if SubBand = Band then begin
                Font.Style := [fsBold,fsUnderline];
              end else begin
                Font.Style := [];
              end; { else }
              if SubBandDetail.PointerColor = 0 then begin
                S1 := SubBand.Name + {Trans-}' (' + Trans('Master') + ')';
              end else begin
                S1 := SubBand.Name + {Trans-}' (' + PrintLocCh[PrintLoc] + ')';
              end; { else }
              TextOut(X1,Y1,S1);
              Inc(Y1,16);
            end; { if }
          end; { with }
        end; { else }
        SubBand := Region.NextSubBand(BandSearchRec);
      end; { while }
    end;  { ExecuteLoc }

  begin { DrawBands }
    if NoDraw then Exit;

  { Figure out height needed for image }
    Region.ReplaceBand := Band;
    Region.ReplaceBandStyle := BandStyle;
    BandList := CreateBandList(Region);
    DoPrintBand := false;
    LineCount := 0;
    IterateCount := 3;
    ExecuteLoc(nil,plBodyHeader,0);
    Image1.Picture.Bitmap.Height := LineCount * 16 + 5;
    Image1.Picture.Bitmap.Width := 400;
    ScrollBox1.VertScrollBar.Range := Image1.Picture.Bitmap.Height;
    ScrollBox1.VertScrollBar.Position := 0;
    Image1.Left := 0;
    Image1.Top := 0;
    Application.ProcessMessages;
    Label1.Caption := Trans(Format({Trans+}'Band Display for %0:s: %1:s',[Region.Name,Band.Name]));

  { Draw bands }
    with Image1.Picture.Bitmap.Canvas do begin
      Brush.Style := bsSolid;
      Brush.Color := clWhite;
      Pen.Style := psSolid;
      Pen.Width := 1;
      Pen.Color := clBlack;
      Pen.Mode := pmCopy;
      Rect := Image1.ClientRect;
      FillRect(Rect);
      DoPrintBand := true;
      Y1 := 2;
      LineCount := 0;
      ExecuteLoc(nil,plBodyHeader,0);
    end; { with }

    Region.ReplaceBand := nil;
    Region.ReplaceBandStyle := nil;
    FreeBandList(BandList);
  end;  { DrawBands }

  procedure TBandStyleDialog.CheckBoxClick(Sender: TObject);


  begin
    if NoDraw then Exit;

    with FBandStyle do begin
      PrintLoc := [];
      PrintOcc := [];
      if BodyHeaderCB.Checked then begin
        PrintLoc := PrintLoc + [plBodyHeader];
      end; { if }
      if GroupHeaderCB.Checked then begin
        PrintLoc := PrintLoc + [plGroupHeader];
      end; { if }
      if RowHeaderCB.Checked then begin
        PrintLoc := PrintLoc + [plRowHeader];
      end; { if }
      if DetailCB.Checked then begin
        PrintLoc := PrintLoc + [plDetail];
      end; { if }
      if RowFooterCB.Checked then begin
        PrintLoc := PrintLoc + [plRowFooter];
      end; { if }
      if GroupFooterCB.Checked then begin
        PrintLoc := PrintLoc + [plGroupFooter];
      end; { if }
      if BodyFooterCB.Checked then begin
        PrintLoc := PrintLoc + [plBodyFooter];
      end; { if }
      if FirstCB.Checked then begin
        PrintOcc := PrintOcc + [poFirst];
      end; { if }
      if NewPageCB.Checked then begin
        PrintOcc := PrintOcc + [poNewPage];
      end; { if }
      if NewColumnCB.Checked then begin
        PrintOcc := PrintOcc + [poNewColumn];
      end; { if }
    end; { with }
    DrawBands;
  end;

  procedure TBandStyleDialog.FormShow(Sender: TObject);
  begin
    RestoreToolWin(self);
    DrawBands;
  end;

  procedure TBandStyleDialog.FormCreate(Sender: TObject);
  begin
    FormTrans(self);
    PrepareToolWin(self);
  end;

  procedure TBandStyleDialog.FormDestroy(Sender: TObject);
  begin
    SaveToolWin(self);
  end;

procedure TBandStyleDialog.FormKeyPress(Sender: TObject; var Key: Char);
begin
  if Key = #27 then begin { Escape }
    ModalResult := mrCancel;
  end; { if }
end;

initialization
  RegisterProc({Trans-}'RVCL',RaveRegister);
end.