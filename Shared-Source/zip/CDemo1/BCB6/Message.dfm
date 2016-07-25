object MsgForm: TMsgForm
  Left = 373
  Top = 184
  Width = 580
  Height = 440
  Caption = 'Zip Messages'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -10
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 0
    Top = 363
    Width = 572
    Height = 24
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 2
    object DismissBut: TButton
      Left = 369
      Top = 3
      Width = 118
      Height = 20
      Caption = '&Dismiss this Window'
      TabOrder = 1
      OnClick = DismissButClick
    end
    object CancelBut: TButton
      Left = 192
      Top = 3
      Width = 104
      Height = 20
      Caption = '&Cancel Operation'
      TabOrder = 0
      OnClick = CancelButClick
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 572
    Height = 29
    Align = alTop
    TabOrder = 0
    object FileBeingZipped: TLabel
      Left = 259
      Top = 8
      Width = 105
      Height = 13
      Alignment = taRightJustify
      Caption = 'File being manipulated'
    end
    object ProgressBar1: TProgressBar
      Left = 376
      Top = 8
      Width = 177
      Height = 13
      Min = 1
      Max = 10001
      Position = 1
      Step = 100
      TabOrder = 0
    end
  end
  object RichEdit1: TRichEdit
    Left = 0
    Top = 29
    Width = 572
    Height = 334
    Align = alClient
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 1
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 387
    Width = 572
    Height = 19
    Panels = <
      item
        Bevel = pbNone
        Width = 110
      end
      item
        Bevel = pbNone
        Width = 80
      end
      item
        Style = psOwnerDraw
        Width = 150
      end>
    SimplePanel = False
  end
end
