object frmMMSecurityScreenSettings: TfrmMMSecurityScreenSettings
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'Security screen settings'
  ClientHeight = 717
  ClientWidth = 1024
  Color = clWhite
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 19
  object pLefSep: TPanel
    Left = 0
    Top = 5
    Width = 5
    Height = 712
    Align = alLeft
    BevelOuter = bvNone
    Color = clWhite
    ParentBackground = False
    TabOrder = 0
  end
  object pRightSep: TPanel
    Left = 1019
    Top = 5
    Width = 5
    Height = 712
    Align = alRight
    BevelOuter = bvNone
    Color = clWhite
    ParentBackground = False
    TabOrder = 1
  end
  object pBottomSep: TPanel
    Left = 0
    Top = 717
    Width = 1024
    Height = 0
    Align = alBottom
    BevelOuter = bvNone
    Color = clWhite
    ParentBackground = False
    TabOrder = 2
  end
  object pTopSep: TPanel
    Left = 0
    Top = 0
    Width = 1024
    Height = 5
    Align = alTop
    BevelOuter = bvNone
    Color = clWhite
    ParentBackground = False
    TabOrder = 3
  end
  object pMiddle: TPanel
    Left = 5
    Top = 5
    Width = 1014
    Height = 712
    Align = alClient
    BevelOuter = bvNone
    Color = clWhite
    ParentBackground = False
    TabOrder = 4
    object pBtnContainer: TPanel
      Left = 899
      Top = 0
      Width = 115
      Height = 712
      Align = alRight
      BevelOuter = bvNone
      TabOrder = 0
      object btnOK: TTouchBtn
        Left = 6
        Top = 6
        Width = 100
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'OK'
        ButtonColor = 18944
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnAutoRepeat = btnOKAutoRepeat
      end
      object btnCancel: TTouchBtn
        Left = 6
        Top = 651
        Width = 100
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'Cancel'
        ButtonColor = clMaroon
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnAutoRepeat = btnCancelAutoRepeat
      end
      object tbtnReload: TTouchBtn
        Left = 6
        Top = 74
        Width = 100
        Margins.Left = 8
        Margins.Top = 8
        Margins.Right = 8
        Margins.Bottom = 8
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'Reload'
        ButtonColor = 6315356
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = tbtnReloadMouseClick
      end
    end
    object pBtnSep: TPanel
      Left = 894
      Top = 0
      Width = 5
      Height = 712
      Align = alRight
      BevelOuter = bvNone
      Color = clWhite
      ParentBackground = False
      TabOrder = 1
    end
    object pControls: TPanel
      Left = 0
      Top = 0
      Width = 894
      Height = 712
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 2
      object Label1: TLabel
        Left = 6
        Top = 6
        Width = 177
        Height = 19
        Caption = 'Security screen filters'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label2: TLabel
        Left = 6
        Top = 31
        Width = 152
        Height = 16
        Caption = 'Select the filter criteria'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGray
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object gbLocationFilter: TGroupBox
        Left = 6
        Top = 54
        Width = 220
        Height = 650
        Caption = ' Locations '
        Color = clBtnFace
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentBackground = False
        ParentColor = False
        ParentFont = False
        TabOrder = 0
        object Bevel2: TBevel
          Left = 2
          Top = 88
          Width = 216
          Height = 2
          Align = alTop
          ExplicitWidth = 196
        end
        object pLocationScroll: TPanel
          Left = 2
          Top = 596
          Width = 216
          Height = 52
          Align = alBottom
          BevelOuter = bvNone
          TabOrder = 0
          object tbtnLocationScrollDown: TTouchBtn
            Left = 133
            Top = 1
            Width = 81
            Height = 50
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            ParentColor = True
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            ArrowType = atUp
            OnMouseClick = tbtnLocationScrollDownMouseClick
            InitialDelay = 250
            RepeatDelay = 60
          end
          object tbtnLocationScrollUp: TTouchBtn
            Left = 1
            Top = 1
            Width = 81
            Height = 50
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            ParentColor = True
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            ArrowType = atDown
            OnMouseClick = tbtnLocationScrollUpMouseClick
            InitialDelay = 250
            RepeatDelay = 60
          end
        end
        object sbLocationFilter: TScrollBox
          Left = 2
          Top = 90
          Width = 216
          Height = 506
          HorzScrollBar.Visible = False
          VertScrollBar.Visible = False
          Align = alClient
          BevelOuter = bvNone
          BorderStyle = bsNone
          TabOrder = 1
        end
        object Panel2: TPanel
          Left = 2
          Top = 21
          Width = 216
          Height = 67
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 2
          object cbLocationSelectAll: TCheckBox
            Left = 16
            Top = 12
            Width = 97
            Height = 17
            Caption = 'Select all'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = cbLocationSelectAllClick
          end
          object cbLocationUnselectAll: TCheckBox
            Left = 16
            Top = 42
            Width = 121
            Height = 17
            Caption = 'Unselect all'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnClick = cbLocationUnselectAllClick
          end
        end
      end
      object gbTerminalFilter: TGroupBox
        Left = 227
        Top = 54
        Width = 220
        Height = 650
        Caption = ' Terminals '
        Color = clBtnFace
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentBackground = False
        ParentColor = False
        ParentFont = False
        TabOrder = 1
        object Bevel1: TBevel
          Left = 2
          Top = 88
          Width = 216
          Height = 2
          Align = alTop
          ExplicitWidth = 196
        end
        object sbTerminalFilter: TScrollBox
          Left = 2
          Top = 90
          Width = 216
          Height = 506
          HorzScrollBar.Visible = False
          VertScrollBar.Smooth = True
          VertScrollBar.Style = ssFlat
          VertScrollBar.Visible = False
          Align = alClient
          BevelOuter = bvNone
          BorderStyle = bsNone
          TabOrder = 0
        end
        object pTerminalScroll: TPanel
          Left = 2
          Top = 596
          Width = 216
          Height = 52
          Align = alBottom
          BevelOuter = bvNone
          TabOrder = 1
          object tbtnTerminalScrollDown: TTouchBtn
            Left = 133
            Top = 1
            Width = 81
            Height = 50
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            ParentColor = True
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            ArrowType = atUp
            OnMouseClick = tbtnTerminalScrollDownMouseClick
            InitialDelay = 250
            RepeatDelay = 60
          end
          object tbtnTerminalScrollUp: TTouchBtn
            Left = 1
            Top = 1
            Width = 81
            Height = 50
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            ParentColor = True
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            ArrowType = atDown
            OnMouseClick = tbtnTerminalScrollUpMouseClick
            InitialDelay = 250
            RepeatDelay = 60
          end
        end
        object pSelectAllTerminals: TPanel
          Left = 2
          Top = 21
          Width = 216
          Height = 67
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 2
          object cbTerminalSelectAll: TCheckBox
            Left = 16
            Top = 12
            Width = 97
            Height = 17
            Caption = 'Select all'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = cbTerminalSelectAllClick
          end
          object cbTerminalUnselectAll: TCheckBox
            Left = 16
            Top = 42
            Width = 121
            Height = 17
            Caption = 'Unselect all'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnClick = cbTerminalUnselectAllClick
          end
        end
      end
      object gbCashierFilter: TGroupBox
        Left = 447
        Top = 54
        Width = 220
        Height = 650
        Caption = ' Cashiers '
        Color = clBtnFace
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentBackground = False
        ParentColor = False
        ParentFont = False
        TabOrder = 2
        object Bevel3: TBevel
          Left = 2
          Top = 88
          Width = 216
          Height = 2
          Align = alTop
          ExplicitWidth = 196
        end
        object pCashierScroll: TPanel
          Left = 2
          Top = 596
          Width = 216
          Height = 52
          Align = alBottom
          BevelOuter = bvNone
          TabOrder = 0
          object tbtnCashierScrollDown: TTouchBtn
            Left = 133
            Top = 1
            Width = 81
            Height = 50
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            ParentColor = True
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            ArrowType = atUp
            OnMouseClick = tbtnCashierScrollDownMouseClick
            InitialDelay = 250
            RepeatDelay = 60
          end
          object tbtnCashierScrollUp: TTouchBtn
            Left = 1
            Top = 1
            Width = 81
            Height = 50
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            ParentColor = True
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            ArrowType = atDown
            OnMouseClick = tbtnCashierScrollUpMouseClick
            InitialDelay = 250
            RepeatDelay = 60
          end
        end
        object sbCashierFilter: TScrollBox
          Left = 2
          Top = 90
          Width = 216
          Height = 506
          HorzScrollBar.Visible = False
          VertScrollBar.Visible = False
          Align = alClient
          BevelOuter = bvNone
          BorderStyle = bsNone
          TabOrder = 1
        end
        object Panel4: TPanel
          Left = 2
          Top = 21
          Width = 216
          Height = 67
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 2
          object cbCashierSelectAll: TCheckBox
            Left = 16
            Top = 12
            Width = 97
            Height = 17
            Caption = 'Select all'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = cbCashierSelectAllClick
          end
          object cbCashierUnselectAll: TCheckBox
            Left = 16
            Top = 42
            Width = 121
            Height = 17
            Caption = 'Unselect all'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnClick = cbCashierUnselectAllClick
          end
        end
      end
      object gbPaymentTypeFilter: TGroupBox
        Left = 668
        Top = 54
        Width = 220
        Height = 650
        Caption = ' Payment types '
        Color = clBtnFace
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentBackground = False
        ParentColor = False
        ParentFont = False
        TabOrder = 3
        object Bevel4: TBevel
          Left = 2
          Top = 88
          Width = 216
          Height = 2
          Align = alTop
          ExplicitWidth = 196
        end
        object pPaymentTypeScroll: TPanel
          Left = 2
          Top = 596
          Width = 216
          Height = 52
          Align = alBottom
          BevelOuter = bvNone
          TabOrder = 0
          object tbtnPaymentTypeScrollDown: TTouchBtn
            Left = 132
            Top = 1
            Width = 81
            Height = 50
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            ParentColor = True
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            ArrowType = atUp
            OnMouseClick = tbtnPaymentTypeScrollDownMouseClick
            InitialDelay = 250
            RepeatDelay = 60
          end
          object tbtnPaymentTypeScrollUp: TTouchBtn
            Left = 1
            Top = 1
            Width = 81
            Height = 50
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            ParentColor = True
            ButtonColor = 9933970
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            ArrowType = atDown
            OnMouseClick = tbtnPaymentTypeScrollUpMouseClick
            InitialDelay = 250
            RepeatDelay = 60
          end
        end
        object sbPaymentTypeFilter: TScrollBox
          Left = 2
          Top = 90
          Width = 216
          Height = 506
          HorzScrollBar.Visible = False
          VertScrollBar.Visible = False
          Align = alClient
          BevelOuter = bvNone
          BorderStyle = bsNone
          TabOrder = 1
        end
        object Panel6: TPanel
          Left = 2
          Top = 21
          Width = 216
          Height = 67
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 2
          object cbPaymentTypeSelectAll: TCheckBox
            Left = 16
            Top = 12
            Width = 97
            Height = 17
            Caption = 'Select all'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = cbPaymentTypeSelectAllClick
          end
          object cbPaymentTypeUnselectAll: TCheckBox
            Left = 16
            Top = 42
            Width = 121
            Height = 17
            Caption = 'Unselect all'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnClick = cbPaymentTypeUnselectAllClick
          end
        end
      end
    end
  end
end
