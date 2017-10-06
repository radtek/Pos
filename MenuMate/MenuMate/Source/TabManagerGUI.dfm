object frmTabManager: TfrmTabManager
  Tag = 1024
  Left = 0
  Top = 0
  Align = alClient
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'Account Manager'
  ClientHeight = 768
  ClientWidth = 1024
  Color = 14342874
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  WindowState = wsMaximized
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pgControl: TPageControl
    Left = 0
    Top = 0
    Width = 1024
    Height = 768
    ActivePage = tsTabManager
    Align = alClient
    TabOrder = 0
    object tsTabManager: TTabSheet
      TabVisible = False
      DesignSize = (
        1016
        758)
      object pnlDisplay: TPanel
        AlignWithMargins = True
        Left = 378
        Top = 8
        Width = 388
        Height = 720
        Margins.Top = 8
        Margins.Bottom = 8
        Anchors = [akLeft, akTop, akBottom]
        Color = 14342874
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -27
        Font.Name = 'Arial'
        Font.Style = []
        ParentBackground = False
        ParentFont = False
        TabOrder = 0
        object lbeCredit: TLabel
          Left = 200
          Top = 235
          Width = 42
          Height = 19
          Caption = 'Credit'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object lbeCreditLimit: TLabel
          Left = 200
          Top = 189
          Width = 82
          Height = 19
          Caption = 'Credit Limit'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object lbeLockedReason: TLabel
          Left = 16
          Top = 360
          Width = 330
          Height = 340
          AutoSize = False
          Caption = 'Locked Reason'
          Color = 14342874
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentColor = False
          ParentFont = False
          WordWrap = True
        end
        object lbeIDType: TLabel
          Left = 200
          Top = 51
          Width = 17
          Height = 19
          Caption = 'ID'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object lbeIDNumber: TLabel
          Left = 200
          Top = 92
          Width = 79
          Height = 19
          Caption = 'ID Number'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object lbeIDExpire: TLabel
          Left = 200
          Top = 143
          Width = 44
          Height = 19
          Caption = 'Expiry'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label3: TLabel
          Left = 16
          Top = 328
          Width = 122
          Height = 19
          Caption = 'Locked Reason'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label4: TLabel
          Left = 16
          Top = 189
          Width = 96
          Height = 19
          Caption = 'Credit Limit'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lbeBalance: TLabel
          Left = 16
          Top = 235
          Width = 120
          Height = 19
          Caption = 'Balance Owing'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label6: TLabel
          Left = 16
          Top = 143
          Width = 51
          Height = 19
          Caption = 'Expiry'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label7: TLabel
          Left = 16
          Top = 97
          Width = 88
          Height = 19
          Caption = 'ID Number'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label8: TLabel
          Left = 16
          Top = 51
          Width = 19
          Height = 19
          Caption = 'ID'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label2: TLabel
          Left = 16
          Top = 281
          Width = 99
          Height = 19
          Caption = 'Total Points'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lbeTotalPoints: TLabel
          Left = 200
          Top = 276
          Width = 84
          Height = 19
          Caption = 'Total Points'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object lbeTabName: TStaticText
          Left = 10
          Top = 8
          Width = 360
          Height = 33
          Alignment = taCenter
          AutoSize = False
          BorderStyle = sbsSingle
          Caption = 'Tab Name'
          Color = 2200566
          Font.Charset = THAI_CHARSET
          Font.Color = clWhite
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentColor = False
          ParentFont = False
          TabOrder = 0
        end
      end
      object Panel2: TPanel
        AlignWithMargins = True
        Left = 5
        Top = 8
        Width = 368
        Height = 720
        Margins.Left = 5
        Margins.Top = 8
        Margins.Bottom = 8
        Anchors = [akLeft, akTop, akBottom]
        Color = 14342874
        Ctl3D = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -20
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentBackground = False
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 1
        object btnSeatsUp: TTouchBtn
          Left = 293
          Top = 190
          Width = 65
          Height = 65
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Color = clWhite
          ParentFont = False
          Caption = '/\'
          ButtonColor = clBtnFace
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseDown = btnSeatsUpMouseDown
        end
        object btnSeatsDown: TTouchBtn
          Left = 293
          Top = 637
          Width = 65
          Height = 65
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = '\/'
          ButtonColor = clBtnFace
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseDown = btnSeatsDownMouseDown
        end
        object btnShowNormalTabs: TTouchBtn
          Left = 8
          Top = 49
          Width = 110
          Height = 45
          Font.Charset = THAI_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Tabs'
          ButtonColor = 6052441
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnShowNormalTabsClick
        end
        object btnMemberTabs: TTouchBtn
          Left = 128
          Top = 49
          Width = 110
          Height = 45
          Font.Charset = THAI_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Member Tabs'
          ButtonColor = 6052441
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnMemberTabsClick
        end
        object lbePartyName: TStaticText
          Left = 8
          Top = 152
          Width = 350
          Height = 33
          Alignment = taCenter
          AutoSize = False
          BorderStyle = sbsSingle
          Caption = 'Party Name: Johnson'
          Color = 2200566
          DoubleBuffered = False
          Font.Charset = THAI_CHARSET
          Font.Color = clWhite
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentColor = False
          ParentDoubleBuffered = False
          ParentFont = False
          TabOrder = 0
        end
        object Label1: TStaticText
          Left = 8
          Top = 8
          Width = 350
          Height = 33
          Alignment = taCenter
          AutoSize = False
          BorderStyle = sbsSingle
          Caption = 'Select One'
          Color = 2200566
          Font.Charset = THAI_CHARSET
          Font.Color = clWhite
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentColor = False
          ParentFont = False
          TabOrder = 1
        end
        object btnStaffTabs: TTouchBtn
          Left = 244
          Top = 49
          Width = 110
          Height = 45
          Font.Charset = THAI_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Staff Tabs'
          ButtonColor = 6052441
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnStaffTabsClick
        end
        object pnlTabs: TPanel
          Left = 8
          Top = 192
          Width = 275
          Height = 510
          BevelOuter = bvNone
          BorderStyle = bsSingle
          ParentColor = True
          TabOrder = 7
          OnResize = pnlTabsResize
          object sbTabs: TScrollBox
            Left = 0
            Top = 0
            Width = 270
            Height = 505
            HorzScrollBar.Visible = False
            VertScrollBar.ButtonSize = 1
            VertScrollBar.Color = 10011108
            VertScrollBar.ParentColor = False
            VertScrollBar.Smooth = True
            VertScrollBar.Size = 1
            VertScrollBar.Style = ssFlat
            VertScrollBar.ThumbSize = 1
            VertScrollBar.Tracking = True
            AutoScroll = False
            BorderStyle = bsNone
            Font.Charset = THAI_CHARSET
            Font.Color = clBlack
            Font.Height = -20
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
        end
        object btnClippTab: TTouchBtn
          Left = 8
          Top = 100
          Width = 110
          Height = 45
          Font.Charset = THAI_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Clipp Tabs'
          ButtonColor = 6052441
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnClippTabMouseClick
        end
      end
      object Panel3: TPanel
        AlignWithMargins = True
        Left = 760
        Top = 8
        Width = 250
        Height = 720
        Margins.Top = 8
        Margins.Right = 5
        Margins.Bottom = 8
        Anchors = [akLeft, akTop, akBottom]
        Color = 14342874
        ParentBackground = False
        TabOrder = 2
        object btnClose: TTouchBtn
          Left = 10
          Top = 8
          Width = 230
          Height = 65
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Exit'
          ButtonColor = 2200566
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnCloseClick
        end
        object btnLockTab: TTouchBtn
          Left = 10
          Top = 430
          Width = 112
          Height = 65
          Font.Charset = THAI_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Lock / Unlock Tab'
          ButtonColor = 6052441
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnLockTabClick
        end
        object btnManInvoice: TTouchBtn
          Left = 10
          Top = 640
          Width = 112
          Height = 65
          BorderWidth = 1
          Font.Charset = THAI_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Manual Invoice'
          ButtonColor = 6052441
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnManInvoiceClick
        end
        object btnTabCredit: TTouchBtn
          Left = 10
          Top = 290
          Width = 112
          Height = 65
          Font.Charset = THAI_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Tab Credit'
          ButtonColor = 6052441
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnTabCreditClick
        end
        object btnAddNewTab: TTouchBtn
          Left = 10
          Top = 80
          Width = 112
          Height = 65
          Font.Charset = THAI_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Create'
          ButtonColor = 6052441
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnAddNewTabClick
        end
        object btnPermanent: TTouchBtn
          Left = 10
          Top = 500
          Width = 112
          Height = 65
          Font.Charset = THAI_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Make Permanent'
          ButtonColor = 6052441
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnPermanentClick
        end
        object btnRemoveTab: TTouchBtn
          Left = 10
          Top = 570
          Width = 112
          Height = 65
          Font.Charset = THAI_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Remove Empty Tab'
          ButtonColor = 6052441
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnRemoveTabClick
        end
        object btnChangeDetails: TTouchBtn
          Left = 10
          Top = 150
          Width = 112
          Height = 65
          Font.Charset = THAI_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Change Details'
          ButtonColor = 6052441
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnChangeDetailsClick
        end
        object btnPINTab: TTouchBtn
          Left = 10
          Top = 360
          Width = 112
          Height = 65
          Font.Charset = THAI_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Set PIN'
          ButtonColor = 6052441
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnPINTabClick
        end
        object TouchButton1: TTouchBtn
          Left = 10
          Top = 220
          Width = 112
          Height = 65
          Font.Charset = THAI_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Swipe Card'
          ButtonColor = 6052441
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = TouchButton1Click
        end
        object btnSubsidisedProfile: TTouchBtn
          Left = 128
          Top = 80
          Width = 112
          Height = 65
          Font.Charset = THAI_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Subsidized Profile'
          ButtonColor = 6052441
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnSubsidisedProfileMouseClick
        end
      end
    end
    object tsDiscounts: TTabSheet
      Caption = 'tsDiscounts'
      ImageIndex = 1
      TabVisible = False
      DesignSize = (
        1016
        758)
      object Panel4: TPanel
        Left = 0
        Top = 0
        Width = 1016
        Height = 758
        Anchors = [akLeft, akTop, akRight, akBottom]
        Color = 14342874
        ParentBackground = False
        TabOrder = 0
        DesignSize = (
          1016
          758)
        object GroupBox6: TGroupBox
          Left = 0
          Top = 0
          Width = 1016
          Height = 758
          Anchors = [akLeft, akTop, akRight, akBottom]
          Caption = 'Tab Discounts'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clMaroon
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          DesignSize = (
            1016
            758)
          object tgDiscounts: TTouchGrid
            Left = 10
            Top = 20
            Width = 860
            Height = 736
            Anchors = [akTop, akBottom]
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -19
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            BevelOuter = bvNone
            DefaultButtonColor = clNavy
            DefaultButtonLatchedColor = clGreen
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = tgDiscountsMouseClick
            RowCount = 1
            ColCount = 3
            DefaultColWidth = 120
            DefaultRowHeight = 55
            ButtonGapWidth = 5
            ButtonGapHeight = 5
            AutoButtonWidth = True
            VertScrollButtonPos = vsAboveAndBelow
            VertScrollButtonHeight = 55
            HorizScrollButtonWidth = 70
            ScrollBtnUp.Font.Charset = DEFAULT_CHARSET
            ScrollBtnUp.Font.Color = clWindowText
            ScrollBtnUp.Font.Height = -11
            ScrollBtnUp.Font.Name = 'MS Sans Serif'
            ScrollBtnUp.Font.Style = []
            ScrollBtnUp.ParentFont = False
            ScrollBtnUp.LatchedFont.Charset = DEFAULT_CHARSET
            ScrollBtnUp.LatchedFont.Color = clWindowText
            ScrollBtnUp.LatchedFont.Height = -11
            ScrollBtnUp.LatchedFont.Name = 'MS Sans Serif'
            ScrollBtnUp.LatchedFont.Style = []
            ScrollBtnUp.ParentLatchedFontColor = False
            ScrollBtnUp.GlyphAlpha = 255
            ScrollBtnUp.Color = -2147483633
            ScrollBtnUp.FontColor = 16777215
            ScrollBtnUp.LatchedColor = -2147483633
            ScrollBtnUp.DisabledColor = -2147483633
            ScrollBtnUp.LatchedFontColor = -2147483640
            ScrollBtnDown.Font.Charset = DEFAULT_CHARSET
            ScrollBtnDown.Font.Color = clWindowText
            ScrollBtnDown.Font.Height = -11
            ScrollBtnDown.Font.Name = 'MS Sans Serif'
            ScrollBtnDown.Font.Style = []
            ScrollBtnDown.ParentFont = False
            ScrollBtnDown.LatchedFont.Charset = DEFAULT_CHARSET
            ScrollBtnDown.LatchedFont.Color = clWindowText
            ScrollBtnDown.LatchedFont.Height = -11
            ScrollBtnDown.LatchedFont.Name = 'MS Sans Serif'
            ScrollBtnDown.LatchedFont.Style = []
            ScrollBtnDown.ParentLatchedFontColor = False
            ScrollBtnDown.GlyphAlpha = 255
            ScrollBtnDown.Color = -2147483633
            ScrollBtnDown.FontColor = 16777215
            ScrollBtnDown.LatchedColor = -2147483633
            ScrollBtnDown.DisabledColor = -2147483633
            ScrollBtnDown.LatchedFontColor = -2147483640
            ScrollBtnLeft.Font.Charset = DEFAULT_CHARSET
            ScrollBtnLeft.Font.Color = clWindowText
            ScrollBtnLeft.Font.Height = -11
            ScrollBtnLeft.Font.Name = 'MS Sans Serif'
            ScrollBtnLeft.Font.Style = []
            ScrollBtnLeft.ParentFont = False
            ScrollBtnLeft.LatchedFont.Charset = DEFAULT_CHARSET
            ScrollBtnLeft.LatchedFont.Color = clWindowText
            ScrollBtnLeft.LatchedFont.Height = -11
            ScrollBtnLeft.LatchedFont.Name = 'MS Sans Serif'
            ScrollBtnLeft.LatchedFont.Style = []
            ScrollBtnLeft.ParentLatchedFontColor = False
            ScrollBtnLeft.GlyphAlpha = 255
            ScrollBtnLeft.Color = -2147483633
            ScrollBtnLeft.FontColor = 16777215
            ScrollBtnLeft.LatchedColor = -2147483633
            ScrollBtnLeft.DisabledColor = -2147483633
            ScrollBtnLeft.LatchedFontColor = -2147483640
            ScrollBtnRight.Font.Charset = DEFAULT_CHARSET
            ScrollBtnRight.Font.Color = clWindowText
            ScrollBtnRight.Font.Height = -11
            ScrollBtnRight.Font.Name = 'MS Sans Serif'
            ScrollBtnRight.Font.Style = []
            ScrollBtnRight.ParentFont = False
            ScrollBtnRight.LatchedFont.Charset = DEFAULT_CHARSET
            ScrollBtnRight.LatchedFont.Color = clWindowText
            ScrollBtnRight.LatchedFont.Height = -11
            ScrollBtnRight.LatchedFont.Name = 'MS Sans Serif'
            ScrollBtnRight.LatchedFont.Style = []
            ScrollBtnRight.ParentLatchedFontColor = False
            ScrollBtnRight.GlyphAlpha = 255
            ScrollBtnRight.Color = -2147483633
            ScrollBtnRight.FontColor = 16777215
            ScrollBtnRight.LatchedColor = -2147483633
            ScrollBtnRight.DisabledColor = -2147483633
            ScrollBtnRight.LatchedFontColor = -2147483640
            Buttons = {
              04000000370000006C39B8D92FC48AD2FB3F140100003C40844C61D72497FC3F
              150100003C40844C61D72497FC3F150100003C40844C61D72497FC3F00008000
              00800000000080000D0000004D0053002000530061006E0073FFFFFF000E0000
              0000000000000000000000000000000000FFFFFF000D0000004D005300200053
              0061006E0073FFFFFF000E00000000000000000000000000000000000000FFFF
              FF0000000000010000000100000001000000010000000100000001000000FF00
              0000640000000100000000000000010000000000000000000000000080000080
              0000000080000D0000004D0053002000530061006E0073FFFFFF000E00000000
              000000000000000000000000000000FFFFFF000D0000004D0053002000530061
              006E0073FFFFFF000E00000000000000000000000000000000000000FFFFFF00
              00000000010000000100000001000000010000000100000001000000FF000000
              6400000001000000000000000100000000000000000000000000800000800000
              000080000D0000004D0053002000530061006E0073FFFFFF000E000000000000
              00000000000000000000000000FFFFFF000D0000004D0053002000530061006E
              0073FFFFFF000E00000000000000000000000000000000000000FFFFFF000000
              0000010000000100000001000000010000000100000001000000FF0000006400
              00000100000000000000010000000000000000000000}
          end
          object Panel1: TPanel
            Left = 875
            Top = 22
            Width = 140
            Height = 730
            Anchors = [akTop, akRight, akBottom]
            Color = 14342874
            ParentBackground = False
            TabOrder = 1
            DesignSize = (
              140
              730)
            object TouchBtn1: TTouchBtn
              Tag = -1
              Left = 10
              Top = 10
              Width = 120
              Anchors = [akTop]
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -19
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
              ParentColor = True
              Caption = 'Ok'
              ButtonColor = 18944
              LatchedColor = clBtnFace
              DisabledButtonColor = clBtnFace
              LatchingProperties = [lkLatchColor, lkStayDown]
              OnMouseClick = TouchBtn1MouseClick
            end
          end
        end
      end
    end
  end
end
