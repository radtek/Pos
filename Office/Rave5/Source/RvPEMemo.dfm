object MemoEditor: TMemoEditor
  Left = 379
  Top = 224
  ClientWidth = 300
  ClientHeight = 240
  VertScrollBar.Range = 41
  ActiveControl = Memo1
  BorderStyle = bsSizeToolWin
  Caption = 'Memo Editor'
  Color = clBtnFace
  Constraints.MinHeight = 240
  Constraints.MinWidth = 300
  Font.Color = clBtnText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Pitch = fpVariable
  Font.Style = []
  ParentFont = False
  Scaled = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 75
  object Memo1: TMemo
    Left = 0
    Top = 0
    Width = 300
    Height = 199
    Align = alClient
    ScrollBars = ssVertical
    TabOrder = 0
    OnKeyPress = Memo1KeyPress
  end
  object Panel1: TPanel
    Left = 0
    Top = 199
    Width = 300
    Height = 41
    Align = alBottom
    TabOrder = 1
    object Button1: TButton
      Left = 128
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akRight]
      Caption = '&OK'
      Default = True
      ModalResult = 1
      TabOrder = 0
    end
    object Button2: TButton
      Left = 209
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akRight]
      Cancel = True
      Caption = '&Cancel'
      ModalResult = 2
      TabOrder = 1
    end
  end
end