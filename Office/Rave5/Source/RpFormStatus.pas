{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RpFormStatus;

interface

uses
  Windows, Messages, Graphics, Controls, Forms, Dialogs, StdCtrls, Buttons,
  SysUtils, Classes, RpSystem, RpDefine;

type
  TRpStatusForm = class(TForm)
    CancelButton: TButton;
    StatusLabel: TLabel;
    procedure CancelButtonClick(Sender: TObject);
  private
  public
    ReportSystem: TRvSystem;
    FormClosed: boolean;
  end;

var
  RpStatusForm: TRpStatusForm;

implementation

{$R *.dfm}

procedure TRpStatusForm.CancelButtonClick(Sender: TObject);
begin
  with Sender as TButton do begin
    if ModalResult = mrCancel then begin
      ReportSystem.BaseReport.Abort;
    end else begin
      FormClosed := true;
    end; { if }
  end; { with }
end;

end.
