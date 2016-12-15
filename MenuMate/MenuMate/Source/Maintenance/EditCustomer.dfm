object frmEditCustomer: TfrmEditCustomer
  Tag = 1024
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsNone
  BorderWidth = 5
  Caption = 'Edit Customer'
  ClientHeight = 560
  ClientWidth = 834
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  WindowState = wsMaximized
  OnKeyDown = FormKeyDown
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnlButtons: TPanel
    Left = 697
    Top = 0
    Width = 137
    Height = 560
    Align = alRight
    BevelOuter = bvNone
    BorderWidth = 5
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object btnClose: TTouchBtn
      Tag = -1
      Left = 5
      Top = 5
      Width = 122
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -14
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnOkClick
    end
    object tbtnUserProfile: TTouchBtn
      Left = 5
      Top = 96
      Width = 122
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -14
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'User Profile'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnUserProfileMouseClick
    end
    object tbtnPalmProfile: TTouchBtn
      Left = 5
      Top = 168
      Width = 122
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -14
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Discount Profile'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnPalmProfileMouseClick
    end
    object tbtnPoints: TTouchBtn
      Left = 5
      Top = 240
      Width = 122
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -14
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Points Profile'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnPointsMouseClick
    end
    object tbtnCancel: TTouchBtn
      Tag = -1
      Left = 5
      Top = 498
      Width = 127
      Align = alBottom
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -14
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
    object btnGroups: TTouchBtn
      Left = 5
      Top = 384
      Width = 122
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -14
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Groups'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnGroupsMouseClick
    end
    object btnAccountProfile: TTouchBtn
      Left = 5
      Top = 313
      Width = 123
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -14
      Font.Name = 'Tahoma'
      Font.Style = []
      Color = clGreen
      ParentFont = False
      Caption = 'Account Profile'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnAccountProfileMouseClick
    end
  end
  object pgControl: TPageControl
    Left = 0
    Top = 0
    Width = 697
    Height = 560
    ActivePage = tsPoints
    Align = alClient
    TabOrder = 1
    object tsUser: TTabSheet
      TabVisible = False
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Panel1: TPanel
        Left = 0
        Top = 0
        Width = 699
        Height = 560
        Align = alClient
        BevelOuter = bvNone
        Color = 14342874
        ParentBackground = False
        TabOrder = 0
        object lbeSmartCardSpace: TLabel
          Left = 0
          Top = 543
          Width = 699
          Height = 17
          Align = alBottom
          Caption = 'Smart Card Space Used.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clGreen
          Font.Height = -14
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ExplicitWidth = 149
        end
        object GroupBox2: TGroupBox
          Left = 0
          Top = 0
          Width = 590
          Height = 390
          Caption = 
            'Contact Details (Fields marked with * are mandatory to be entere' +
            'd)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = 4342338
          Font.Height = -14
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          object Panel3: TPanel
            Left = 3
            Top = 21
            Width = 496
            Height = 365
            BevelOuter = bvNone
            BorderWidth = 5
            ParentBackground = False
            ParentColor = True
            TabOrder = 0
            DesignSize = (
              496
              365)
            object lblFirstname: TLabel
              Left = 12
              Top = 0
              Width = 100
              Height = 15
              AutoSize = False
              Caption = 'First Name *:'
              Font.Charset = ANSI_CHARSET
              Font.Color = 4342338
              Font.Height = -14
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object lblLasttname: TLabel
              Left = 12
              Top = 25
              Width = 100
              Height = 15
              AutoSize = False
              Caption = 'Last Name *: '
              Font.Charset = ANSI_CHARSET
              Font.Color = 4342338
              Font.Height = -14
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object lblPhone: TLabel
              Left = 12
              Top = 50
              Width = 52
              Height = 15
              AutoSize = False
              Caption = 'Phone : '
              Font.Charset = ANSI_CHARSET
              Font.Color = 4342338
              Font.Height = -14
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object lblMobile: TLabel
              Left = 12
              Top = 100
              Width = 110
              Height = 15
              AutoSize = False
              Caption = 'Mobile Phone : '
              Font.Charset = ANSI_CHARSET
              Font.Color = 4342338
              Font.Height = -14
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object lblLocation: TLabel
              Left = 12
              Top = 140
              Width = 120
              Height = 15
              AutoSize = False
              Caption = 'Location Address : '
              Font.Charset = ANSI_CHARSET
              Font.Color = 4342338
              Font.Height = -14
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object lblAddress: TLabel
              Left = 12
              Top = 222
              Width = 115
              Height = 20
              AutoSize = False
              Caption = 'Mailing Address : '
              Font.Charset = ANSI_CHARSET
              Font.Color = 4342338
              Font.Height = -14
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object lblEmail: TLabel
              Left = 12
              Top = 75
              Width = 100
              Height = 15
              AutoSize = False
              Caption = 'Email *:'
              Font.Charset = ANSI_CHARSET
              Font.Color = 4342338
              Font.Height = -14
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object lblMemberNumber: TLabel
              Left = 12
              Top = 263
              Width = 120
              Height = 15
              AutoSize = False
              Caption = 'Member Number : '
              Font.Charset = ANSI_CHARSET
              Font.Color = 4342338
              Font.Height = -14
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object lblKnownas: TLabel
              Left = 12
              Top = 288
              Width = 73
              Height = 15
              AutoSize = False
              Caption = 'Known As : '
              Font.Charset = ANSI_CHARSET
              Font.Color = 4342338
              Font.Height = -14
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object lblSex: TLabel
              Left = 12
              Top = 313
              Width = 36
              Height = 15
              AutoSize = False
              Caption = 'Sex : '
              Font.Charset = ANSI_CHARSET
              Font.Color = 4342338
              Font.Height = -14
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object lblNote: TLabel
              Left = 12
              Top = 338
              Width = 42
              Height = 15
              AutoSize = False
              Caption = 'Note : '
              Font.Charset = ANSI_CHARSET
              Font.Color = 4342338
              Font.Height = -14
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object cbNoEmail: TCheckBox
              Left = 350
              Top = 72
              Width = 147
              Height = 25
              Caption = 'Email not available'
              TabOrder = 12
              OnClick = cbNoEmailMouseClick
            end
            object edFirstName: TEdit
              Left = 170
              Top = 0
              Width = 175
              Height = 20
              Anchors = [akLeft]
              AutoSelect = False
              AutoSize = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -15
              Font.Name = 'Tahoma'
              Font.Style = []
              MaxLength = 20
              ParentFont = False
              TabOrder = 0
            end
            object edLastName: TEdit
              Left = 170
              Top = 25
              Width = 175
              Height = 20
              Anchors = [akLeft, akBottom]
              AutoSelect = False
              AutoSize = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -15
              Font.Name = 'Tahoma'
              Font.Style = []
              MaxLength = 20
              ParentFont = False
              TabOrder = 1
            end
            object edPhone: TEdit
              Left = 170
              Top = 50
              Width = 175
              Height = 20
              Anchors = [akLeft, akBottom]
              AutoSelect = False
              AutoSize = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -15
              Font.Name = 'Tahoma'
              Font.Style = []
              MaxLength = 20
              ParentFont = False
              TabOrder = 2
            end
            object edMobile: TEdit
              Left = 170
              Top = 100
              Width = 175
              Height = 20
              Anchors = [akLeft, akBottom]
              AutoSelect = False
              AutoSize = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -15
              Font.Name = 'Tahoma'
              Font.Style = []
              MaxLength = 20
              ParentFont = False
              TabOrder = 4
            end
            object reLocationAddress: TRichEdit
              Left = 170
              Top = 125
              Width = 175
              Height = 53
              Anchors = [akLeft, akBottom]
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -13
              Font.Name = 'Tahoma'
              Font.Style = []
              MaxLength = 150
              ParentFont = False
              TabOrder = 5
            end
            object cbCopyAddress: TCheckBox
              Left = 170
              Top = 180
              Width = 397
              Height = 25
              Caption = 'Mailing Address is same as Location Address'
              TabOrder = 6
              OnClick = cbCopyAddressClick
            end
            object reAddress: TRichEdit
              Left = 170
              Top = 205
              Width = 175
              Height = 53
              Anchors = [akLeft, akBottom]
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -13
              Font.Name = 'Tahoma'
              Font.Style = []
              MaxLength = 150
              ParentFont = False
              TabOrder = 7
            end
            object edEmail: TEdit
              Left = 170
              Top = 75
              Width = 175
              Height = 20
              Anchors = [akLeft, akBottom]
              AutoSelect = False
              AutoSize = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -15
              Font.Name = 'Tahoma'
              Font.Style = []
              MaxLength = 55
              ParentFont = False
              TabOrder = 3
            end
            object edMemberNumber: TEdit
              Left = 170
              Top = 263
              Width = 175
              Height = 20
              Anchors = [akLeft, akBottom]
              AutoSelect = False
              AutoSize = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -15
              Font.Name = 'Tahoma'
              Font.Style = []
              MaxLength = 20
              ParentFont = False
              TabOrder = 8
            end
            object edKnownas: TEdit
              Left = 170
              Top = 288
              Width = 175
              Height = 20
              Anchors = [akLeft, akBottom]
              AutoSelect = False
              AutoSize = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -15
              Font.Name = 'Tahoma'
              Font.Style = []
              MaxLength = 20
              ParentFont = False
              TabOrder = 9
            end
            object edSex: TEdit
              Left = 170
              Top = 313
              Width = 175
              Height = 20
              Anchors = [akLeft, akBottom]
              AutoSelect = False
              AutoSize = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -15
              Font.Name = 'Tahoma'
              Font.Style = []
              MaxLength = 20
              ParentFont = False
              TabOrder = 10
            end
            object edNote: TEdit
              Left = 170
              Top = 338
              Width = 175
              Height = 20
              Anchors = [akLeft, akBottom]
              AutoSelect = False
              AutoSize = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -15
              Font.Name = 'Tahoma'
              Font.Style = []
              MaxLength = 20
              ParentFont = False
              TabOrder = 11
            end
          end
        end
        object pnlKeyboard: TPanel
          Left = 2
          Top = 392
          Width = 865
          Height = 150
          TabOrder = 0
          object TouchKeyboard1: TMMTouchKeyboard
            Left = 1
            Top = 1
            Width = 863
            Height = 148
            Align = alClient
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            GridBorderWidth = 0
            ButtonGapWidth = 2
            ButtonGapHeight = 2
            ButtonColor = 6052441
            OnClick = TouchKeyboard1Click
          end
        end
        object Panel5: TPanel
          Left = 600
          Top = 0
          Width = 270
          Height = 390
          BevelOuter = bvNone
          ParentColor = True
          TabOrder = 1
          object GroupBox1: TGroupBox
            Left = 0
            Top = 0
            Width = 270
            Height = 80
            Align = alTop
            Caption = 'Date Of Birth'
            Font.Charset = ANSI_CHARSET
            Font.Color = 4342338
            Font.Height = -14
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentBackground = False
            ParentFont = False
            TabOrder = 0
            object pnlDateOfbirth: TPanel
              Left = 12
              Top = 25
              Width = 155
              Height = 35
              TabOrder = 0
              object DateTimePicker1: TDateTimePicker
                Left = 0
                Top = 0
                Width = 155
                Height = 38
                Date = 40757.392338425900000000
                Time = 40757.392338425900000000
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clBlack
                Font.Height = -25
                Font.Name = 'Tahoma'
                Font.Style = []
                ParentFont = False
                TabOrder = 0
                OnChange = DateTimePicker1OnChange
              end
            end
          end
          object GroupBox5: TGroupBox
            Left = 0
            Top = 80
            Width = 270
            Height = 47
            Align = alTop
            Caption = 'Points'
            Color = 14342874
            Font.Charset = ANSI_CHARSET
            Font.Color = 4342338
            Font.Height = -14
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentBackground = False
            ParentColor = False
            ParentFont = False
            TabOrder = 1
            object lbePoints: TLabel
              Left = 12
              Top = 20
              Width = 28
              Height = 17
              Caption = '0.00'
            end
          end
          object GroupBox3: TGroupBox
            Left = 0
            Top = 127
            Width = 270
            Height = 97
            Align = alTop
            Caption = 'Credit Details'
            Color = 14342874
            Font.Charset = ANSI_CHARSET
            Font.Color = 4342338
            Font.Height = -14
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentBackground = False
            ParentColor = False
            ParentFont = False
            TabOrder = 2
            object Label7: TLabel
              Left = 12
              Top = 40
              Width = 72
              Height = 17
              Caption = 'Allowed Tab'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = 4342338
              Font.Height = -14
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object tbtnAllowedTab: TTouchBtn
              Left = 140
              Top = 26
              Width = 122
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -14
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
              ParentColor = True
              Caption = 'Not Allowed Tab'
              ButtonColor = 6052441
              LatchedColor = clNavy
              DisabledButtonColor = clBtnFace
              LatchingType = ltCheckbox
              LatchingProperties = [lkLatchColor, lkStayDown]
              OnMouseClick = tbtnAllowedTabClick
            end
          end
          object GroupBox4: TGroupBox
            Left = 0
            Top = 224
            Width = 270
            Height = 166
            Align = alTop
            Caption = 'Card Details'
            Color = 14342874
            Font.Charset = DEFAULT_CHARSET
            Font.Color = 4342338
            Font.Height = -14
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentBackground = False
            ParentColor = False
            ParentFont = False
            TabOrder = 3
            object btnSwipe: TTouchBtn
              Left = 12
              Top = 32
              Width = 122
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -14
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
              ParentColor = True
              Caption = 'Swipe Card(s)'
              ButtonColor = 6052441
              LatchedColor = clBtnFace
              DisabledButtonColor = clBtnFace
              LatchingProperties = [lkLatchColor, lkStayDown]
              OnMouseClick = btnSwipeClick
            end
            object btnRemoveCard: TTouchBtn
              Left = 140
              Top = 32
              Width = 122
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -14
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
              ParentColor = True
              Caption = 'Remove Card'
              ButtonColor = 6052441
              LatchedColor = clBtnFace
              DisabledButtonColor = clBtnFace
              LatchingProperties = [lkLatchColor, lkStayDown]
              OnMouseClick = btnRemoveCardClick
            end
            object tbtnClearAllCards: TTouchBtn
              Left = 140
              Top = 96
              Width = 122
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -14
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
              ParentColor = True
              Caption = 'Clear All Cards'
              ButtonColor = 6052441
              LatchedColor = clBtnFace
              DisabledButtonColor = clBtnFace
              LatchingProperties = [lkLatchColor, lkStayDown]
              OnMouseClick = tbtnClearAllCardsClick
            end
            object tbProximity: TTouchBtn
              Left = 12
              Top = 96
              Width = 122
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -14
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
              ParentColor = True
              Caption = 'Proximity Card'
              ButtonColor = 6052441
              LatchedColor = clBtnFace
              DisabledButtonColor = clBtnFace
              LatchingProperties = [lkLatchColor, lkStayDown]
              OnMouseClick = tbProximityMouseClick
            end
          end
        end
      end
    end
    object tsDiscounts: TTabSheet
      Caption = 'tsDiscounts'
      ImageIndex = 1
      TabVisible = False
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Panel2: TPanel
        Left = 0
        Top = 0
        Width = 699
        Height = 560
        Align = alClient
        Color = 14342874
        ParentBackground = False
        TabOrder = 0
        object GroupBox6: TGroupBox
          Left = 1
          Top = 1
          Width = 697
          Height = 558
          Align = alClient
          Caption = 'Contacts Discounts'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clMaroon
          Font.Height = -14
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          object tgDiscounts: TTouchGrid
            Left = 2
            Top = 19
            Width = 693
            Height = 537
            Align = alClient
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -14
            Font.Name = 'Tahoma'
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
              04000000370000006C39B8D92FC48AD2FB3FDD0000003C40844C61D72497FC3F
              DD0000003C40844C61D72497FC3FDD0000003C40844C61D72497FC3F00008000
              008000000000800006000000540061006800FFFFFF000B000000000000000000
              00000000000000000000FFFFFF0006000000540061006800FFFFFF000B000000
              00000000000000000000000000000000FFFFFF00000000000100000001000000
              01000000010000000100000001000000FF000000640000000100000000000000
              0100000000000000000000000000800000800000000080000600000054006100
              6800FFFFFF000B00000000000000000000000000000000000000FFFFFF000600
              0000540061006800FFFFFF000B00000000000000000000000000000000000000
              FFFFFF0000000000010000000100000001000000010000000100000001000000
              FF00000064000000010000000000000001000000000000000000000000008000
              008000000000800006000000540061006800FFFFFF000B000000000000000000
              00000000000000000000FFFFFF0006000000540061006800FFFFFF000B000000
              00000000000000000000000000000000FFFFFF00000000000100000001000000
              01000000010000000100000001000000FF000000640000000100000000000000
              010000000000000000000000}
          end
        end
      end
    end
    object tsPoints: TTabSheet
      Caption = 'tsPoints'
      ImageIndex = 2
      TabVisible = False
      object TPanel
        Left = 0
        Top = 0
        Width = 689
        Height = 550
        Align = alClient
        BorderWidth = 6
        Color = 14342874
        ParentBackground = False
        TabOrder = 0
        object Panel4: TPanel
          Left = 7
          Top = 7
          Width = 675
          Height = 34
          Align = alTop
          BevelOuter = bvLowered
          Caption = 'Points Settings'
          Color = 2200566
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -14
          Font.Name = 'Arial'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 0
        end
        object tcbeprNoPointsRedemption: TTouchBtn
          Left = 7
          Top = 47
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -14
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'No Points Redemption'
          ButtonColor = 6052441
          LatchedColor = clGreen
          DisabledButtonColor = clBtnFace
          LatchingType = ltCheckbox
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tcbeprNoPointsRedemptionClick
        end
        object tcbeprNoPointsPurchases: TTouchBtn
          Left = 7
          Top = 110
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -14
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'No Points Purchases'
          ButtonColor = 6052441
          LatchedColor = clGreen
          DisabledButtonColor = clBtnFace
          LatchingType = ltCheckbox
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tcbeprNoPointsPurchasesClick
        end
        object tcbeprEarnsPointsWhileRedeemingPoints: TTouchBtn
          Left = 7
          Top = 173
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -14
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Earns Points While Redeeming'
          ButtonColor = 6052441
          LatchedColor = clGreen
          DisabledButtonColor = clBtnFace
          LatchingType = ltCheckbox
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tcbeprEarnsPointsWhileRedeemingPointsClick
        end
        object tcbeprOnlyEarnsPointsWhileRedeemingPoints: TTouchBtn
          Left = 7
          Top = 236
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -14
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Only Earns While Redeeming'
          ButtonColor = 6052441
          LatchedColor = clGreen
          DisabledButtonColor = clBtnFace
          LatchingType = ltCheckbox
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tcbeprOnlyEarnsPointsWhileRedeemingPointsClick
        end
        object tcbeprNeverEarnsPoints: TTouchBtn
          Left = 7
          Top = 299
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -14
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Never Earns Points'
          ButtonColor = 6052441
          LatchedColor = clGreen
          DisabledButtonColor = clBtnFace
          LatchingType = ltCheckbox
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tcbeprNeverEarnsPointsClick
        end
        object tcbeprAllowedNegitive: TTouchBtn
          Left = 7
          Top = 362
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -14
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Allowed Negative Points'
          ButtonColor = 6052441
          LatchedColor = clGreen
          DisabledButtonColor = clBtnFace
          LatchingType = ltCheckbox
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tcbeprAllowedNegitiveClick
        end
        object tcbeprFinancial: TTouchBtn
          Left = 7
          Top = 425
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -14
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Financial'
          ButtonColor = 6052441
          LatchedColor = clGreen
          DisabledButtonColor = clBtnFace
          LatchingType = ltCheckbox
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tcbeprFinancialClick
        end
        object tcbeprAllowDiscounts: TTouchBtn
          Left = 7
          Top = 488
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -14
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Allow Discounts'
          ButtonColor = 6052441
          LatchedColor = clGreen
          DisabledButtonColor = clBtnFace
          LatchingType = ltCheckbox
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tcbeprAllowDiscountsClick
        end
      end
    end
    object tsGroups: TTabSheet
      Caption = 'tsGroups'
      ImageIndex = 3
      TabVisible = False
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Panel7: TPanel
        Left = 0
        Top = 0
        Width = 699
        Height = 560
        Align = alClient
        Color = 14342874
        ParentBackground = False
        TabOrder = 0
        object Panel8: TPanel
          Left = 1
          Top = 1
          Width = 424
          Height = 558
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 5
          Color = clBlack
          ParentBackground = False
          TabOrder = 0
          object CurrentGroupsGrid: TTouchGrid
            Left = 5
            Top = 39
            Width = 414
            Height = 514
            Align = alClient
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            DefaultButtonColor = clBtnFace
            DefaultButtonLatchedColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = CurrentGroupsGridMouseClick
            RowCount = 1
            ColCount = 2
            ParentGridColor = False
            GridColor = cl3DLight
            DefaultColWidth = 120
            DefaultRowHeight = 55
            VertScrollButtonPos = vsBelow
            VertScrollButtonHeight = 55
            HorizScrollButtonWidth = 70
            ScrollBtnUp.Font.Charset = DEFAULT_CHARSET
            ScrollBtnUp.Font.Color = clWindowText
            ScrollBtnUp.Font.Height = -11
            ScrollBtnUp.Font.Name = 'Tahoma'
            ScrollBtnUp.Font.Style = []
            ScrollBtnUp.ParentFont = False
            ScrollBtnUp.LatchedFont.Charset = DEFAULT_CHARSET
            ScrollBtnUp.LatchedFont.Color = clWindowText
            ScrollBtnUp.LatchedFont.Height = -11
            ScrollBtnUp.LatchedFont.Name = 'Tahoma'
            ScrollBtnUp.LatchedFont.Style = []
            ScrollBtnUp.GlyphAlpha = 255
            ScrollBtnUp.Color = 11587808
            ScrollBtnUp.FontColor = 0
            ScrollBtnUp.LatchedColor = -16777201
            ScrollBtnUp.DisabledColor = -16777201
            ScrollBtnUp.LatchedFontColor = -16777208
            ScrollBtnDown.Font.Charset = DEFAULT_CHARSET
            ScrollBtnDown.Font.Color = clWindowText
            ScrollBtnDown.Font.Height = -11
            ScrollBtnDown.Font.Name = 'Tahoma'
            ScrollBtnDown.Font.Style = []
            ScrollBtnDown.ParentFont = False
            ScrollBtnDown.LatchedFont.Charset = DEFAULT_CHARSET
            ScrollBtnDown.LatchedFont.Color = clWindowText
            ScrollBtnDown.LatchedFont.Height = -11
            ScrollBtnDown.LatchedFont.Name = 'Tahoma'
            ScrollBtnDown.LatchedFont.Style = []
            ScrollBtnDown.GlyphAlpha = 255
            ScrollBtnDown.Color = 11587808
            ScrollBtnDown.FontColor = 0
            ScrollBtnDown.LatchedColor = -16777201
            ScrollBtnDown.DisabledColor = -16777201
            ScrollBtnDown.LatchedFontColor = -16777208
            ScrollBtnLeft.Font.Charset = DEFAULT_CHARSET
            ScrollBtnLeft.Font.Color = clWindowText
            ScrollBtnLeft.Font.Height = -11
            ScrollBtnLeft.Font.Name = 'Tahoma'
            ScrollBtnLeft.Font.Style = []
            ScrollBtnLeft.ParentFont = False
            ScrollBtnLeft.LatchedFont.Charset = DEFAULT_CHARSET
            ScrollBtnLeft.LatchedFont.Color = clWindowText
            ScrollBtnLeft.LatchedFont.Height = -11
            ScrollBtnLeft.LatchedFont.Name = 'Tahoma'
            ScrollBtnLeft.LatchedFont.Style = []
            ScrollBtnLeft.GlyphAlpha = 255
            ScrollBtnLeft.Color = -16777201
            ScrollBtnLeft.FontColor = 0
            ScrollBtnLeft.LatchedColor = -16777201
            ScrollBtnLeft.DisabledColor = -16777201
            ScrollBtnLeft.LatchedFontColor = -16777208
            ScrollBtnRight.Font.Charset = DEFAULT_CHARSET
            ScrollBtnRight.Font.Color = clWindowText
            ScrollBtnRight.Font.Height = -11
            ScrollBtnRight.Font.Name = 'Tahoma'
            ScrollBtnRight.Font.Style = []
            ScrollBtnRight.ParentFont = False
            ScrollBtnRight.LatchedFont.Charset = DEFAULT_CHARSET
            ScrollBtnRight.LatchedFont.Color = clWindowText
            ScrollBtnRight.LatchedFont.Height = -11
            ScrollBtnRight.LatchedFont.Name = 'Tahoma'
            ScrollBtnRight.LatchedFont.Style = []
            ScrollBtnRight.GlyphAlpha = 255
            ScrollBtnRight.Color = -16777201
            ScrollBtnRight.FontColor = 0
            ScrollBtnRight.LatchedColor = -16777201
            ScrollBtnRight.DisabledColor = -16777201
            ScrollBtnRight.LatchedFontColor = -16777208
            Buttons = {
              04000000370000008EBA665A1C75CDB4FB3FF000000000000000000000000000
              32000000000000000000000000000F0000FF0F0000FF0F0000FF0D0000004D00
              53002000530061006E0073000000000C00000001000000000000000000000000
              000000000000000D0000004D0053002000530061006E0073000000000C000000
              0100000000000000000000000000000000000000000000000100000001000000
              01000000010000000100000001000000FF000000640000000100000000000000
              0100000000000000000000000F0000FF0F0000FF0F0000FF0D0000004D005300
              2000530061006E0073000000000C000000010000000000000000000000000000
              00000000000D0000004D0053002000530061006E0073000000000C0000000100
              0000000000000000000000000000000000000000000001000000010000000100
              0000010000000100000001000000FF0000006400000001000000000000000100
              00000000000000000000}
          end
          object Panel11: TPanel
            Left = 5
            Top = 5
            Width = 414
            Height = 34
            Align = alTop
            BevelOuter = bvLowered
            Caption = 'Groups'
            Color = 2200566
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -14
            Font.Name = 'Arial'
            Font.Style = []
            ParentBackground = False
            ParentFont = False
            TabOrder = 1
          end
        end
        object Panel9: TPanel
          Left = 425
          Top = 1
          Width = 273
          Height = 558
          Align = alClient
          BevelOuter = bvNone
          BorderWidth = 5
          Color = clBlack
          ParentBackground = False
          TabOrder = 1
          object AvailableGroupsGrid: TTouchGrid
            Left = 5
            Top = 39
            Width = 263
            Height = 514
            Align = alClient
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            DefaultButtonColor = clBtnFace
            DefaultButtonLatchedColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = AvailableGroupsGridMouseClick
            RowCount = 1
            ColCount = 2
            ParentGridColor = False
            GridColor = cl3DLight
            DefaultColWidth = 120
            DefaultRowHeight = 55
            VertScrollButtonPos = vsBelow
            VertScrollButtonHeight = 55
            HorizScrollButtonWidth = 70
            ScrollBtnUp.Font.Charset = DEFAULT_CHARSET
            ScrollBtnUp.Font.Color = clWindowText
            ScrollBtnUp.Font.Height = -11
            ScrollBtnUp.Font.Name = 'Tahoma'
            ScrollBtnUp.Font.Style = []
            ScrollBtnUp.ParentFont = False
            ScrollBtnUp.LatchedFont.Charset = DEFAULT_CHARSET
            ScrollBtnUp.LatchedFont.Color = clWindowText
            ScrollBtnUp.LatchedFont.Height = -11
            ScrollBtnUp.LatchedFont.Name = 'Tahoma'
            ScrollBtnUp.LatchedFont.Style = []
            ScrollBtnUp.GlyphAlpha = 255
            ScrollBtnUp.Color = 11587808
            ScrollBtnUp.FontColor = 0
            ScrollBtnUp.LatchedColor = -16777201
            ScrollBtnUp.DisabledColor = -16777201
            ScrollBtnUp.LatchedFontColor = -16777208
            ScrollBtnDown.Font.Charset = DEFAULT_CHARSET
            ScrollBtnDown.Font.Color = clWindowText
            ScrollBtnDown.Font.Height = -11
            ScrollBtnDown.Font.Name = 'Tahoma'
            ScrollBtnDown.Font.Style = []
            ScrollBtnDown.ParentFont = False
            ScrollBtnDown.LatchedFont.Charset = DEFAULT_CHARSET
            ScrollBtnDown.LatchedFont.Color = clWindowText
            ScrollBtnDown.LatchedFont.Height = -11
            ScrollBtnDown.LatchedFont.Name = 'Tahoma'
            ScrollBtnDown.LatchedFont.Style = []
            ScrollBtnDown.GlyphAlpha = 255
            ScrollBtnDown.Color = 11587808
            ScrollBtnDown.FontColor = 0
            ScrollBtnDown.LatchedColor = -16777201
            ScrollBtnDown.DisabledColor = -16777201
            ScrollBtnDown.LatchedFontColor = -16777208
            ScrollBtnLeft.Font.Charset = DEFAULT_CHARSET
            ScrollBtnLeft.Font.Color = clWindowText
            ScrollBtnLeft.Font.Height = -11
            ScrollBtnLeft.Font.Name = 'Tahoma'
            ScrollBtnLeft.Font.Style = []
            ScrollBtnLeft.ParentFont = False
            ScrollBtnLeft.LatchedFont.Charset = DEFAULT_CHARSET
            ScrollBtnLeft.LatchedFont.Color = clWindowText
            ScrollBtnLeft.LatchedFont.Height = -11
            ScrollBtnLeft.LatchedFont.Name = 'Tahoma'
            ScrollBtnLeft.LatchedFont.Style = []
            ScrollBtnLeft.GlyphAlpha = 255
            ScrollBtnLeft.Color = -16777201
            ScrollBtnLeft.FontColor = 0
            ScrollBtnLeft.LatchedColor = -16777201
            ScrollBtnLeft.DisabledColor = -16777201
            ScrollBtnLeft.LatchedFontColor = -16777208
            ScrollBtnRight.Font.Charset = DEFAULT_CHARSET
            ScrollBtnRight.Font.Color = clWindowText
            ScrollBtnRight.Font.Height = -11
            ScrollBtnRight.Font.Name = 'Tahoma'
            ScrollBtnRight.Font.Style = []
            ScrollBtnRight.ParentFont = False
            ScrollBtnRight.LatchedFont.Charset = DEFAULT_CHARSET
            ScrollBtnRight.LatchedFont.Color = clWindowText
            ScrollBtnRight.LatchedFont.Height = -11
            ScrollBtnRight.LatchedFont.Name = 'Tahoma'
            ScrollBtnRight.LatchedFont.Style = []
            ScrollBtnRight.GlyphAlpha = 255
            ScrollBtnRight.Color = -16777201
            ScrollBtnRight.FontColor = 0
            ScrollBtnRight.LatchedColor = -16777201
            ScrollBtnRight.DisabledColor = -16777201
            ScrollBtnRight.LatchedFontColor = -16777208
            Buttons = {
              040000003700000000000000000000000000F000000000000000000000000000
              32000000000000000000000000000F0000FF0F0000FF0F0000FF0D0000004D00
              53002000530061006E0073000000000C00000001000000000000000000000000
              000000000000000D0000004D0053002000530061006E0073000000000C000000
              0100000000000000000000000000000000000000000000000100000001000000
              01000000010000000100000001000000FF000000640000000100000000000000
              0100000000000000000000000F0000FF0F0000FF0F0000FF0D0000004D005300
              2000530061006E0073000000000C000000010000000000000000000000000000
              00000000000D0000004D0053002000530061006E0073000000000C0000000100
              0000000000000000000000000000000000000000000001000000010000000100
              0000010000000100000001000000FF0000006400000001000000000000000100
              00000000000000000000}
          end
          object Panel13: TPanel
            Left = 5
            Top = 5
            Width = 263
            Height = 34
            Align = alTop
            BevelOuter = bvLowered
            Caption = 'Available Groups'
            Color = 2200566
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -14
            Font.Name = 'Arial'
            Font.Style = []
            ParentBackground = False
            ParentFont = False
            TabOrder = 1
          end
        end
      end
    end
    object tsProfile: TTabSheet
      Caption = 'tsProfile'
      ImageIndex = 4
      TabVisible = False
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Panel10: TPanel
        Left = 0
        Top = 0
        Width = 699
        Height = 560
        Align = alClient
        Color = 14342874
        ParentBackground = False
        TabOrder = 0
        object CheckBoxCharges: TCheckBox
          Left = 24
          Top = 32
          Width = 185
          Height = 57
          Caption = 'Allow Charges'
          Color = clNavy
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
          TabOrder = 0
          OnClick = CheckBoxChargesMouseClick
        end
      end
    end
  end
  object Panel6: TPanel
    Left = 875
    Top = 0
    Width = 5
    Height = 584
    BevelOuter = bvNone
    Color = clBlack
    TabOrder = 2
  end
end
