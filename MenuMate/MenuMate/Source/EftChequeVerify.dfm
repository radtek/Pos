object frmChequeVerfiy: TfrmChequeVerfiy
  Tag = 1024
  Left = 139
  Top = 272
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Cheque Verify'
  ClientHeight = 219
  ClientWidth = 676
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel27: TPanel
    Left = 8
    Top = 9
    Width = 513
    Height = 200
    Color = 14342874
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentFont = False
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 24
      Width = 148
      Height = 22
      Caption = 'Cheque Amount'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label2: TLabel
      Left = 8
      Top = 67
      Width = 208
      Height = 22
      Caption = 'Cheque Serial Number'
      Color = 14342874
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
    end
    object Label3: TLabel
      Left = 8
      Top = 110
      Width = 145
      Height = 22
      Caption = 'Cheque Branch'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label4: TLabel
      Left = 8
      Top = 153
      Width = 153
      Height = 22
      Caption = 'Cheque Account'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object edChqAmount: TEdit
      Left = 224
      Top = 19
      Width = 270
      Height = 32
      Color = clSilver
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ReadOnly = True
      TabOrder = 0
    end
    object edChqSerial: TEdit
      Left = 224
      Top = 62
      Width = 270
      Height = 32
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      OnMouseUp = edChqSerialMouseUp
    end
    object edChqBranch: TEdit
      Left = 224
      Top = 105
      Width = 270
      Height = 32
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      OnMouseUp = edChqBranchMouseUp
    end
    object edChqAccount: TEdit
      Left = 224
      Top = 148
      Width = 270
      Height = 32
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 3
      OnMouseUp = edChqAccountMouseUp
    end
  end
  object Panel2: TPanel
    Left = 532
    Top = 9
    Width = 137
    Height = 200
    BorderWidth = 4
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object TouchBtn1: TTouchBtn
      Left = 5
      Top = 5
      Width = 127
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnOkClick
      ExplicitLeft = -5
      ExplicitTop = 51
      ExplicitWidth = 185
    end
    object TouchBtn2: TTouchBtn
      Left = 5
      Top = 138
      Width = 127
      Align = alBottom
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = Panel1Click
      ExplicitLeft = -5
      ExplicitTop = 51
      ExplicitWidth = 185
    end
  end
end
