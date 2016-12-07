object frmCaptureCustomerDetails: TfrmCaptureCustomerDetails
  Left = 0
  Top = 0
  Anchors = [akLeft, akTop, akRight, akBottom]
  BorderStyle = bsNone
  Caption = 'SC/PWD Details'
  ClientHeight = 815
  ClientWidth = 1135
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDefault
  WindowState = wsMaximized
  PixelsPerInch = 96
  TextHeight = 13
  object PanelDetails: TPanel
    Left = 240
    Top = 186
    Width = 769
    Height = 505
    Caption = 'PanelDetails'
    TabOrder = 0
    object GroupBox1: TGroupBox
      Left = 12
      Top = 10
      Width = 741
      Height = 479
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
        Top = 55
        Width = 141
        Height = 19
        Caption = 'Customer Name :'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object LabelAddress: TLabel
        Left = 8
        Top = 103
        Width = 76
        Height = 19
        Caption = 'Address :'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object LabelTIN: TLabel
        Left = 8
        Top = 151
        Width = 41
        Height = 19
        Caption = 'TIN :'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object LabelBusinessStyle: TLabel
        Left = 8
        Top = 199
        Width = 127
        Height = 19
        Caption = 'Business Style :'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object LabelSCPWDNO: TLabel
        Left = 8
        Top = 247
        Width = 87
        Height = 19
        Caption = 'SC/PWD #'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object edCustomerName: TEdit
        Left = 200
        Top = 55
        Width = 350
        Height = 22
        MaxLength = 75
        TabOrder = 0
        OnClick = CaptureCustomerDetails
      end
      object edAddress: TEdit
        Tag = 1
        Left = 200
        Top = 103
        Width = 350
        Height = 22
        TabOrder = 1
        OnClick = CaptureCustomerDetails
      end
      object edTIN: TEdit
        Tag = 2
        Left = 200
        Top = 151
        Width = 350
        Height = 22
        MaxLength = 50
        TabOrder = 2
        OnClick = CaptureCustomerDetails
      end
      object edBusinessStyle: TEdit
        Tag = 3
        Left = 200
        Top = 199
        Width = 350
        Height = 22
        MaxLength = 50
        TabOrder = 3
        OnClick = CaptureCustomerDetails
      end
      object edSCPWDNO: TEdit
        Tag = 4
        Left = 200
        Top = 247
        Width = 350
        Height = 22
        MaxLength = 50
        TabOrder = 4
        OnClick = CaptureCustomerDetails
      end
      object btnOK: TTouchBtn
        Tag = 5
        Left = 8
        Top = 331
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
        Tag = 6
        Left = 450
        Top = 331
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
