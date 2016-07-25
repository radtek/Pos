(* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is TurboPower OfficePartner
 *
 * The Initial Developer of the Original Code is
 * TurboPower Software
 *
 * Portions created by the Initial Developer are Copyright (C) 2000-2002
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * ***** END LICENSE BLOCK ***** *)

unit ExSMail1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, OpShared, OpOlkXP, OpOutlk;

type
  TForm1 = class(TForm)
    btnCreateAnSend: TButton;
    OpOutlook1: TOpOutlook;
    procedure btnCreateAnSendClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.DFM}

uses
  ExSMail2, ExSMail3;

procedure TForm1.btnCreateAnSendClick(Sender: TObject);
var
  Dlg: TForm2;
begin
  if not OpOutlook1.Connected then
    OpOutlook1.Connected := True;
  Dlg := TForm2.Create( Self );
  try
    Dlg.Outlook := OpOutlook1;
    Dlg.ShowModal;
  finally
    Dlg.Free;
  end;
end;

end.
