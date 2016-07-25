object frmNewUser: TfrmNewUser
  Tag = 1024
  Left = 183
  Top = 104
  BorderIcons = []
  BorderStyle = bsNone
  BorderWidth = 5
  Caption = 'New User'
  ClientHeight = 604
  ClientWidth = 886
  Color = clBlack
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Icon.Data = {
    0000010001002020100000000000E80200001600000028000000200000004000
    0000010004000000000080020000000000000000000000000000000000000000
    0000000080000080000000808000800000008000800080800000C0C0C0008080
    80000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF007777
    7777777777777008888888888880787878787878787870078888888888807788
    8888888888888007788888888880788888888888888880000000000778807788
    0000000000000000000000078880788800000000788800000000000778807788
    0000000078880000000000078880788800000000788800000000000778807788
    0000000078880000000000078880788800000000788800000000000778807788
    0000000078880007788888888880788800000000788800078888888888807788
    0000000078880007788888888880788800000000788800078800000000007788
    0000000078880007780000000000788800000000788800078800000000007788
    0000000078880007780000000000788800000000788800078800000000007788
    0000000078880007780000000000788800000000788800078888888888007788
    0000000078880007787878787800788800000000788800077777777777007788
    0000000078880000000000000000788800778888888800000000000000007788
    0078888888888888888888888000788800787878787878787878787870007788
    0077777777777777777777777000788800000000000000000000000000007788
    8888888888888888800000000000788888888888888888888000000000007787
    8787878787878787700000000000777777777777777777777000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    000000000000000000000000000000000000000000000000000000000000}
  OldCreateOrder = True
  Position = poScreenCenter
  Scaled = False
  WindowState = wsMaximized
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 20
  object pnlButtons: TPanel
    Left = 750
    Top = 0
    Width = 136
    Height = 604
    Align = alRight
    BorderWidth = 6
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object btnClose: TTouchBtn
      Tag = -1
      Left = 8
      Top = 8
      Width = 121
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnAcceptClick
    end
    object tbtnUserProfile: TTouchBtn
      Left = 8
      Top = 96
      Width = 122
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'User Profile'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbUserProfileClick
    end
    object tbtnPalmProfile: TTouchBtn
      Left = 6
      Top = 168
      Width = 122
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Palm Profile'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbPalmProfileClick
    end
    object tbtnPayroll: TTouchBtn
      Left = 6
      Top = 240
      Width = 122
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Payroll Interface'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbAcePayrollClick
    end
    object tbtnCancel: TTouchBtn
      Tag = -1
      Left = 7
      Top = 540
      Width = 122
      Align = alBottom
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = clMaroon
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCancelClick
    end
    object tbtnPointsPage: TTouchBtn
      Left = 6
      Top = 312
      Width = 122
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Points Profile'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnPointsPageMouseClick
    end
  end
  object pgControl: TPageControl
    Left = 0
    Top = 0
    Width = 824
    Height = 604
    ActivePage = tsUser
    Align = alLeft
    TabOrder = 1
    object tsUser: TTabSheet
      TabVisible = False
      object Panel1: TPanel
        Left = 0
        Top = 0
        Width = 816
        Height = 594
        Align = alClient
        BorderWidth = 6
        Color = 14342874
        ParentBackground = False
        TabOrder = 0
        object Label5: TLabel
          Left = 8
          Top = 48
          Width = 40
          Height = 18
          Caption = 'Name'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clMaroon
          Font.Height = -15
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label6: TLabel
          Left = 200
          Top = 48
          Width = 46
          Height = 18
          Caption = 'Initials'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clMaroon
          Font.Height = -15
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label7: TLabel
          Left = 8
          Top = 400
          Width = 50
          Height = 18
          Caption = 'Access'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clMaroon
          Font.Height = -15
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label18: TLabel
          Left = 8
          Top = 224
          Width = 81
          Height = 18
          Caption = 'Card Swipe'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clMaroon
          Font.Height = -15
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object btnName: TTouchBtn
          Left = 8
          Top = 72
          Width = 177
          Height = 65
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'Name'
          ButtonColor = clNavy
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnNameClick
        end
        object btnInitials: TTouchBtn
          Left = 200
          Top = 72
          Width = 177
          Height = 65
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'ABC'
          ButtonColor = clNavy
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnInitialsClick
        end
        object btnAccess: TTouchBtn
          Left = 8
          Top = 496
          Width = 177
          Height = 65
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'Administrator'
          ButtonColor = clNavy
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnAccessClick
        end
        object btnPIN: TTouchBtn
          Left = 8
          Top = 424
          Width = 177
          Height = 65
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'Set PIN Number'
          ButtonColor = clNavy
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnPINClick
        end
        object btnSwipe: TTouchBtn
          Left = 8
          Top = 320
          Width = 177
          Height = 65
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'Swipe Card(s)'
          ButtonColor = clNavy
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnSwipeClick
        end
        object tbProximity: TTouchBtn
          Left = 8
          Top = 248
          Width = 177
          Height = 65
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'Proximity Card'
          ButtonColor = clNavy
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbProximityClick
        end
        object Panel11: TPanel
          Left = 7
          Top = 7
          Width = 802
          Height = 34
          Align = alTop
          BevelOuter = bvLowered
          Caption = 'User Profile'
          Color = 2200566
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 7
        end
        object btnRemoveStaffCard: TTouchBtn
          Left = 200
          Top = 320
          Width = 177
          Height = 65
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'Remove Card'
          ButtonColor = clNavy
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnRemoveStaffCardClick
        end
        object tbtnClearAllCards: TTouchBtn
          Left = 392
          Top = 320
          Width = 177
          Height = 65
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'Clear All Cards'
          ButtonColor = clNavy
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbtnClearAllCardsClick
        end
        object btnAllowedTab: TTouchBtn
          Left = 200
          Top = 424
          Width = 177
          Height = 65
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'Allowed Tab'
          ButtonColor = clNavy
          LatchedColor = clGreen
          DisabledButtonColor = clBtnFace
          LatchingType = ltCheckbox
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnAllowedTabClick
        end
      end
    end
    object tsPayroll: TTabSheet
      ImageIndex = 1
      TabVisible = False
      object Panel3: TPanel
        Left = 0
        Top = 0
        Width = 816
        Height = 594
        Align = alClient
        BorderWidth = 6
        Color = 14342874
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentBackground = False
        ParentFont = False
        TabOrder = 0
        object PayrollID: TLabel
          Left = 8
          Top = 68
          Width = 73
          Height = 19
          Caption = 'Payroll ID'
          Color = 10011108
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
        end
        object Label1: TLabel
          Left = 8
          Top = 140
          Width = 110
          Height = 19
          Caption = 'Ordinary From'
          Color = 10011108
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
          Visible = False
        end
        object Label2: TLabel
          Left = 8
          Top = 172
          Width = 131
          Height = 19
          Caption = 'Time && Half From'
          Color = 10011108
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
          Visible = False
        end
        object Label3: TLabel
          Left = 8
          Top = 204
          Width = 140
          Height = 19
          Caption = 'Double Time From'
          Color = 10011108
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
          Visible = False
        end
        object Label4: TLabel
          Left = 8
          Top = 236
          Width = 110
          Height = 19
          Caption = 'User Def From'
          Color = 10011108
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
          Visible = False
        end
        object Label9: TLabel
          Left = 8
          Top = 268
          Width = 110
          Height = 19
          Caption = 'User Def From'
          Color = 10011108
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
          Visible = False
        end
        object Label10: TLabel
          Left = 8
          Top = 300
          Width = 110
          Height = 19
          Caption = 'User Def From'
          Color = 10011108
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
          Visible = False
        end
        object Label11: TLabel
          Left = 8
          Top = 112
          Width = 173
          Height = 19
          Caption = 'Pay Time Period Starts'
          Color = 10011108
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
          Visible = False
        end
        object Label12: TLabel
          Left = 336
          Top = 140
          Width = 69
          Height = 19
          Caption = 'Onwards'
          Color = 10011108
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
          Visible = False
        end
        object Label13: TLabel
          Left = 336
          Top = 172
          Width = 69
          Height = 19
          Caption = 'Onwards'
          Color = 10011108
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
          Visible = False
        end
        object Label14: TLabel
          Left = 336
          Top = 204
          Width = 69
          Height = 19
          Caption = 'Onwards'
          Color = 10011108
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
          Visible = False
        end
        object Label15: TLabel
          Left = 336
          Top = 236
          Width = 69
          Height = 19
          Caption = 'Onwards'
          Color = 10011108
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
          Visible = False
        end
        object Label16: TLabel
          Left = 336
          Top = 268
          Width = 69
          Height = 19
          Caption = 'Onwards'
          Color = 10011108
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
          Visible = False
        end
        object Label17: TLabel
          Left = 336
          Top = 300
          Width = 69
          Height = 19
          Caption = 'Onwards'
          Color = 10011108
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
          Visible = False
        end
        object Label19: TLabel
          Left = 28
          Top = 419
          Width = 90
          Height = 19
          Caption = 'Hourly Rate'
        end
        object btnPayrollID: TTouchBtn
          Left = 152
          Top = 57
          Width = 161
          Height = 49
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          ButtonColor = clWhite
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnPayrollIDClick
        end
        object btnAllpyPayrollTimesToAll: TTouchBtn
          Left = 8
          Top = 344
          Width = 145
          Height = 49
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Visible = False
          Caption = 'Apply Payroll Times To Staff'
          ButtonColor = clNavy
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnAllpyPayrollTimesToAllClick
        end
        object dtpStart6: TDateTimePicker
          Left = 152
          Top = 297
          Width = 160
          Height = 27
          Date = 37334.000000000000000000
          Time = 37334.000000000000000000
          ShowCheckbox = True
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Kind = dtkTime
          ParentFont = False
          TabOrder = 2
          Visible = False
        end
        object dtpStart5: TDateTimePicker
          Left = 152
          Top = 264
          Width = 160
          Height = 27
          Date = 37334.000000000000000000
          Time = 37334.000000000000000000
          ShowCheckbox = True
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Kind = dtkTime
          ParentFont = False
          TabOrder = 3
          Visible = False
        end
        object dtpStart4: TDateTimePicker
          Left = 152
          Top = 232
          Width = 160
          Height = 27
          Date = 37334.000000000000000000
          Time = 37334.000000000000000000
          ShowCheckbox = True
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Kind = dtkTime
          ParentFont = False
          TabOrder = 4
          Visible = False
        end
        object dtpStart3: TDateTimePicker
          Left = 152
          Top = 200
          Width = 160
          Height = 27
          Date = 37334.000000000000000000
          Time = 37334.000000000000000000
          ShowCheckbox = True
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Kind = dtkTime
          ParentFont = False
          TabOrder = 5
          Visible = False
        end
        object dtpStart2: TDateTimePicker
          Left = 152
          Top = 168
          Width = 160
          Height = 27
          Date = 37334.000000000000000000
          Time = 37334.000000000000000000
          ShowCheckbox = True
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Kind = dtkTime
          ParentFont = False
          TabOrder = 6
          Visible = False
        end
        object dtpStart1: TDateTimePicker
          Left = 152
          Top = 136
          Width = 160
          Height = 27
          Date = 37334.000000000000000000
          Time = 37334.000000000000000000
          ShowCheckbox = True
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Kind = dtkTime
          ParentFont = False
          TabOrder = 7
          Visible = False
        end
        object Panel4: TPanel
          Left = 7
          Top = 7
          Width = 802
          Height = 34
          Align = alTop
          BevelOuter = bvLowered
          Caption = 'Payroll Interface Settings'
          Color = 2200566
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 8
        end
        object btnHourRate: TTouchBtn
          Left = 9
          Top = 444
          Width = 144
          Height = 50
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          ButtonColor = clNavy
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnHourRateMouseClick
        end
      end
    end
    object tsPalm: TTabSheet
      ImageIndex = 2
      TabVisible = False
      object TPanel
        Left = 0
        Top = 0
        Width = 816
        Height = 594
        Align = alClient
        BorderWidth = 6
        Color = 14342874
        ParentBackground = False
        TabOrder = 0
        object Label8: TLabel
          Left = 264
          Top = 224
          Width = 137
          Height = 24
          AutoSize = False
          Caption = 'Palm Logoff (min)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clMaroon
          Font.Height = -15
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Panel5: TPanel
          Left = 7
          Top = 7
          Width = 802
          Height = 34
          Align = alTop
          BevelOuter = bvLowered
          Caption = 'Palm Settings'
          Color = 2200566
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 0
        end
        object cbAdvancedUser: TTouchBtn
          Left = 8
          Top = 56
          Height = 41
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'Advanced'
          ButtonColor = clNavy
          LatchedColor = clGreen
          DisabledButtonColor = clBtnFace
          LatchingType = ltCheckbox
          LatchingProperties = [lkLatchColor, lkStayDown]
        end
        object cbAutoPopup: TTouchBtn
          Left = 8
          Top = 104
          Height = 41
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'Auto Details'
          ButtonColor = clNavy
          LatchedColor = clGreen
          DisabledButtonColor = clBtnFace
          LatchingType = ltCheckbox
          LatchingProperties = [lkLatchColor, lkStayDown]
        end
        object cbScreenLock: TTouchBtn
          Left = 8
          Top = 152
          Height = 41
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'Screen Lock'
          ButtonColor = clNavy
          LatchedColor = clGreen
          DisabledButtonColor = clBtnFace
          LatchingType = ltCheckbox
          LatchingProperties = [lkLatchColor, lkStayDown]
        end
        object cbGetAllTables: TTouchBtn
          Left = 8
          Top = 200
          Height = 41
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'Get All Tables'
          ButtonColor = clNavy
          LatchedColor = clGreen
          DisabledButtonColor = clBtnFace
          LatchingType = ltCheckbox
          LatchingProperties = [lkLatchColor, lkStayDown]
        end
        object cbLargeFont: TTouchBtn
          Left = 8
          Top = 248
          Height = 41
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'Large Fonts'
          ButtonColor = clNavy
          LatchedColor = clGreen
          DisabledButtonColor = clBtnFace
          LatchingType = ltCheckbox
          LatchingProperties = [lkLatchColor, lkStayDown]
        end
        object btnLogoff: TTouchBtn
          Left = 264
          Top = 248
          Height = 41
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Color = clWhite
          ParentFont = False
          Caption = '30'
          ButtonColor = clNavy
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
        end
        object rgSoundLevel: TRadioGroup
          Left = 264
          Top = 56
          Width = 185
          Height = 137
          Caption = 'Palm Sound Level'
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Items.Strings = (
            'No Sound'
            'Low'
            'Medium'
            'High')
          ParentFont = False
          TabOrder = 2
        end
        object cbClassicMode: TTouchBtn
          Left = 8
          Top = 296
          Height = 41
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'Classic Mode'
          ButtonColor = clNavy
          LatchedColor = clGreen
          DisabledButtonColor = clBtnFace
          LatchingType = ltCheckbox
          LatchingProperties = [lkLatchColor, lkStayDown]
        end
      end
    end
    object tsPoints: TTabSheet
      Caption = 'tsPoints'
      ImageIndex = 3
      TabVisible = False
      object TPanel
        Left = 0
        Top = 0
        Width = 816
        Height = 594
        Align = alClient
        BorderWidth = 6
        Color = 14342874
        ParentBackground = False
        TabOrder = 0
        object Panel2: TPanel
          Left = 7
          Top = 7
          Width = 802
          Height = 34
          Align = alTop
          BevelOuter = bvLowered
          Caption = 'Points Settings'
          Color = 2200566
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentBackground = False
          ParentFont = False
          TabOrder = 0
        end
        object tcbeprNoPointsRedemption: TTouchBtn
          Left = 8
          Top = 56
          Width = 400
          Height = 41
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'Disable Points Redemption'
          ButtonColor = clNavy
          LatchedColor = clGreen
          DisabledButtonColor = clBtnFace
          LatchingType = ltCheckbox
          LatchingProperties = [lkLatchColor, lkStayDown]
        end
        object tcbeprNoPointsPurchases: TTouchBtn
          Left = 8
          Top = 108
          Width = 400
          Height = 41
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'Disable Points Purchase'
          ButtonColor = clNavy
          LatchedColor = clGreen
          DisabledButtonColor = clBtnFace
          LatchingType = ltCheckbox
          LatchingProperties = [lkLatchColor, lkStayDown]
        end
        object tcbeprOnlyEarnsPointsWhileRedeemingPoints: TTouchBtn
          Left = 8
          Top = 214
          Width = 400
          Height = 41
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'Only Earns Points while Redeeming Points'
          ButtonColor = clNavy
          LatchedColor = clGreen
          DisabledButtonColor = clBtnFace
          LatchingType = ltCheckbox
          LatchingProperties = [lkLatchColor, lkStayDown]
        end
        object tcbeprEarnsPointsWhileRedeemingPoints: TTouchBtn
          Left = 8
          Top = 161
          Width = 400
          Height = 41
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'Earns Points when Redeeming Points'
          ButtonColor = clNavy
          LatchedColor = clGreen
          DisabledButtonColor = clBtnFace
          LatchingType = ltCheckbox
          LatchingProperties = [lkLatchColor, lkStayDown]
        end
        object tcbeprNeverEarnsPoints: TTouchBtn
          Left = 8
          Top = 267
          Width = 400
          Height = 41
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'Never Earns Points'
          ButtonColor = clNavy
          LatchedColor = clGreen
          DisabledButtonColor = clBtnFace
          LatchingType = ltCheckbox
          LatchingProperties = [lkLatchColor, lkStayDown]
        end
        object tcbeprAllowedNegitive: TTouchBtn
          Left = 7
          Top = 319
          Width = 400
          Height = 41
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          ParentColor = True
          Caption = 'Allowed Negative Points'
          ButtonColor = clNavy
          LatchedColor = clGreen
          DisabledButtonColor = clBtnFace
          LatchingType = ltCheckbox
          LatchingProperties = [lkLatchColor, lkStayDown]
        end
      end
    end
  end
end
