object frmBetaAgreement: TfrmBetaAgreement
  Left = 168
  Top = 136
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Beta Version'
  ClientHeight = 358
  ClientWidth = 317
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object RichEdit1: TRichEdit
    Left = 8
    Top = 8
    Width = 297
    Height = 305
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Lines.Strings = (
      'Beta Release Software Notice'
      ''
      'This software is released only as a beta version for '
      'final operational testing in a commercial context.'
      ''
      'Until it has been fully tested it is believed to contain '
      'defects and a primary purpose of this beta testing is to '
      'obtain feedback on software performance and the '
      'identification of defects. Any user is advised to '
      'safeguard important data, to use caution and not to '
      'rely in any way on the correct functioning or '
      'performance of the software.'
      ''
      'Provision of any software under this agreement is '
      'experimental and shall not create any obligation for '
      'IQWorks Ltd to continue to develop, productise, '
      'support, repair, offer for sale or in any other way '
      'continue to provide or develop this software.'
      ''
      'The software is provided "as is" without any express or '
      'implied warranty of any kind including warranties of '
      'merchantability or fitness for any particular purpose. '
      'In no event shall IQWorks Ltd or its suppliers be liable '
      'for any damages whatsoever (including, without '
      'limitation, damages for loss of profits, business '
      'interruption, loss of information) arising out of the use '
      'of or inability to use the software, even if IQWorks Ltd '
      'has been advised of the possibility of such damages.')
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object Button1: TButton
    Left = 224
    Top = 320
    Width = 73
    Height = 25
    Caption = 'I Agree'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 144
    Top = 320
    Width = 73
    Height = 25
    Caption = 'I Disgree'
    TabOrder = 2
    OnClick = Button2Click
  end
end
