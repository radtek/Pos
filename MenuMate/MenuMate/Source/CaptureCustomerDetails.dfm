object frmCaptureDiscountDetails: TfrmCaptureDiscountDetails
  Left = 0
  Top = 0
  Caption = 'SC/PWD Details'
  ClientHeight = 372
  ClientWidth = 368
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object PanelDetails: TPanel
    Left = 0
    Top = 0
    Width = 376
    Height = 377
    Caption = 'PanelDetails'
    TabOrder = 0
    object GroupBox1: TGroupBox
      Left = 8
      Top = 0
      Width = 357
      Height = 361
      Caption = 'Details Required'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      object LabelCustomerName: TLabel
        Left = 8
        Top = 35
        Width = 90
        Height = 14
        Caption = 'Cutomer Name'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object LabelAddress: TLabel
        Left = 8
        Top = 83
        Width = 81
        Height = 14
        Caption = 'LabelAddress'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object LabelTIN: TLabel
        Left = 8
        Top = 131
        Width = 20
        Height = 14
        Caption = 'TIN'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object LabelBusinessStyle: TLabel
        Left = 8
        Top = 179
        Width = 87
        Height = 14
        Caption = 'Business Style'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object LabelSCPWDNO: TLabel
        Left = 8
        Top = 227
        Width = 65
        Height = 14
        Caption = 'SC/PWD #'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object edCustomerName: TEdit
        Left = 8
        Top = 55
        Width = 313
        Height = 22
        TabOrder = 0
        OnClick = edCustomerNameClick
      end
      object edAddress: TEdit
        Left = 8
        Top = 103
        Width = 313
        Height = 22
        TabOrder = 1
        OnClick = edAddressClick
      end
      object edTIN: TEdit
        Left = 8
        Top = 151
        Width = 313
        Height = 22
        TabOrder = 2
        OnClick = edTinClick
      end
      object edBusinessStyle: TEdit
        Left = 8
        Top = 199
        Width = 313
        Height = 22
        TabOrder = 3
        OnClick = edBusinessStyleClick
      end
      object edSCPWDNO: TEdit
        Left = 8
        Top = 247
        Width = 313
        Height = 22
        TabOrder = 4
        OnClick = edSCPWDNOClick
      end
      object btnOK: TTouchBtn
        Left = 8
        Top = 291
        Width = 100
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        ParentColor = True
        Caption = 'OK'
        ButtonColor = 18944
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnOKClick
      end
      object btnCancel: TTouchBtn
        Left = 221
        Top = 291
        Width = 100
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'Cancel'
        ButtonColor = 2200566
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnCancelClick
      end
    end
  end
end
