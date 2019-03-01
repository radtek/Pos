object frmServingTime: TfrmServingTime
  Left = 0
  Top = 0
  Caption = 'frmServingTime'
  ClientHeight = 194
  ClientWidth = 401
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnlMain: TPanel
    Left = 0
    Top = 0
    Width = 401
    Height = 193
    Caption = 'pnlMain'
    TabOrder = 0
    object btnCancel: TTouchBtn
      Left = 264
      Top = 132
      Width = 121
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCancelMouseClick
    end
    object btnClose: TTouchBtn
      Left = 264
      Top = 12
      Width = 121
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Close'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCloseMouseClick
    end
    object GroupBox1: TGroupBox
      Left = 1
      Top = 3
      Width = 258
      Height = 187
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 4342338
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      object lbeHour: TLabel
        Left = 20
        Top = 40
        Width = 35
        Height = 17
        Caption = 'Hour'
        Font.Charset = ANSI_CHARSET
        Font.Color = 4342338
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object lbeMinute: TLabel
        Left = 95
        Top = 40
        Width = 47
        Height = 17
        Caption = 'Minute'
        Font.Charset = ANSI_CHARSET
        Font.Color = 4342338
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object lbeSecond: TLabel
        Left = 170
        Top = 40
        Width = 51
        Height = 17
        Caption = 'Second'
        Font.Charset = ANSI_CHARSET
        Font.Color = 4342338
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object BtnServingTimeHour: TTouchBtn
        Left = 20
        Top = 62
        Width = 64
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        ButtonColor = 9933970
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = BtnServingTimeHourMouseClick
      end
      object BtnServingTimeMinute: TTouchBtn
        Left = 94
        Top = 62
        Width = 64
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        ButtonColor = 9933970
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = BtnServingTimeMinuteMouseClick
      end
      object BtnServingTimeSecond: TTouchBtn
        Left = 168
        Top = 62
        Width = 64
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        ButtonColor = 9933970
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = BtnServingTimeSecondMouseClick
      end
    end
  end
end
