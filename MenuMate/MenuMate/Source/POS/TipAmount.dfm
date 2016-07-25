object frmTipAmount: TfrmTipAmount
  Left = 0
  Top = 0
  Caption = 'Enter Tip Amount'
  ClientHeight = 500
  ClientWidth = 530
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 529
    Height = 500
    TabOrder = 0
    object lbTipDisplay: TNumpadDisplay
      Left = 8
      Top = 8
      Width = 353
      Height = 82
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -40
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object numTipAmount: TTouchNumpad
      Left = 8
      Top = 96
      Width = 353
      Height = 393
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -21
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      GridBorderWidth = 8
      ButtonGapWidth = 2
      ButtonGapHeight = 2
      ButtonColor = 6052441
      CustomKey = True
      CustomKeyCaption = '00'
      NumpadDisplay = lbTipDisplay
    end
    object btnProcessTipOnReceipt: TTouchBtn
      Left = 367
      Top = 8
      Width = 154
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'OK'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnProcessTipOnReceiptMouseClick
    end
    object btnCancelTipping: TTouchBtn
      Left = 367
      Top = 71
      Width = 154
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
      OnMouseClick = btnCancelTippingMouseClick
    end
  end
end
