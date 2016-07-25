object frmQuickMemberCreation: TfrmQuickMemberCreation
  Left = 0
  Top = 0
  Caption = 'Quick Member Creation'
  ClientHeight = 474
  ClientWidth = 633
  Color = clWhite
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object pnlMemberDetails: TPanel
    Left = 8
    Top = 8
    Width = 617
    Height = 457
    TabOrder = 0
    object lblMemberDetails: TLabel
      Left = 16
      Top = 16
      Width = 584
      Height = 19
      Alignment = taCenter
      AutoSize = False
      Caption = 'Member details'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object btnName: TTouchBtn
      Tag = 0
      Left = 16
      Top = 41
      Width = 289
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = '* Name'
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = button_clicked
    end
    object btnMobilePhoneNumber: TTouchBtn
      Tag = 1
      Left = 311
      Top = 41
      Width = 289
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Mobile Phone'
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = button_clicked
    end
    object btnEmail: TTouchBtn
      Tag = 2
      Left = 16
      Top = 112
      Width = 584
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Email'
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = button_clicked
    end
    object gbDateOfBirth: TGroupBox
      Left = 16
      Top = 183
      Width = 584
      Height = 115
      Caption = ' Date of Birth '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      object btnYear: TTouchBtn
        Tag = 5
        Left = 424
        Top = 32
        Width = 144
        Height = 65
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'Year'
        ButtonColor = 9933970
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = button_clicked
      end
      object btnMonth: TTouchBtn
        Tag = 4
        Left = 159
        Top = 32
        Width = 259
        Height = 65
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'Month'
        ButtonColor = 9933970
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = button_clicked
      end
      object btnDay: TTouchBtn
        Tag = 3
        Left = 16
        Top = 32
        Width = 137
        Height = 65
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'Day'
        ButtonColor = 9933970
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = button_clicked
      end
    end
    object btnSave: TTouchBtn
      Tag = 6
      Left = 16
      Top = 304
      Width = 584
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Save'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = button_clicked
    end
    object btnCancel: TTouchBtn
      Tag = 7
      Left = 16
      Top = 375
      Width = 584
      Height = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      Color = clMaroon
      ParentFont = False
      Caption = 'Cancel'
      ButtonColor = clMaroon
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = button_clicked
    end
  end
end
