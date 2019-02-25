object frmHappyHourSettings: TfrmHappyHourSettings
  Tag = 1024
  Left = 430
  Top = 145
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Happy Hour Settings'
  ClientHeight = 456
  ClientWidth = 667
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -17
  Font.Name = 'Arial'
  Font.Style = [fsBold]
  OldCreateOrder = False
  Position = poScreenCenter
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 19
  object Panel24: TPanel
    Left = 9
    Top = 8
    Width = 507
    Height = 440
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object GroupBox1: TGroupBox
      Left = 6
      Top = 8
      Width = 501
      Height = 276
      Caption = 'Profile Info'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 4342338
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      object Label12: TLabel
        Left = 6
        Top = 160
        Width = 63
        Height = 17
        Caption = 'Start Time'
        Font.Charset = ANSI_CHARSET
        Font.Color = 4342338
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label13: TLabel
        Left = 6
        Top = 230
        Width = 57
        Height = 17
        Caption = 'End Time'
        Font.Charset = ANSI_CHARSET
        Font.Color = 4342338
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label4: TLabel
        Left = 6
        Top = 32
        Width = 114
        Height = 17
        Caption = 'Happy Hour Profile'
        Font.Charset = ANSI_CHARSET
        Font.Color = 4342338
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label9: TLabel
        Left = 77
        Top = 67
        Width = 25
        Height = 17
        Caption = 'Day'
        Font.Charset = ANSI_CHARSET
        Font.Color = 4342338
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label2: TLabel
        Left = 164
        Top = 69
        Width = 39
        Height = 17
        Caption = 'Month'
        Font.Charset = ANSI_CHARSET
        Font.Color = 4342338
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label3: TLabel
        Left = 247
        Top = 67
        Width = 27
        Height = 17
        Caption = 'Year'
        Font.Charset = ANSI_CHARSET
        Font.Color = 4342338
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label5: TLabel
        Left = 77
        Top = 133
        Width = 30
        Height = 17
        Caption = 'Hour'
        Font.Charset = ANSI_CHARSET
        Font.Color = 4342338
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label6: TLabel
        Left = 164
        Top = 133
        Width = 40
        Height = 17
        Caption = 'Minute'
        Font.Charset = ANSI_CHARSET
        Font.Color = 4342338
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label7: TLabel
        Left = 323
        Top = 133
        Width = 41
        Height = 17
        Caption = 'AM/PM'
        Font.Charset = ANSI_CHARSET
        Font.Color = 4342338
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label1: TLabel
        Left = 6
        Top = 98
        Width = 29
        Height = 17
        Caption = 'Date'
        Font.Charset = ANSI_CHARSET
        Font.Color = 4342338
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label8: TLabel
        Left = 77
        Top = 199
        Width = 30
        Height = 17
        Caption = 'Hour'
        Font.Charset = ANSI_CHARSET
        Font.Color = 4342338
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label10: TLabel
        Left = 164
        Top = 199
        Width = 40
        Height = 17
        Caption = 'Minute'
        Font.Charset = ANSI_CHARSET
        Font.Color = 4342338
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label11: TLabel
        Left = 323
        Top = 199
        Width = 41
        Height = 17
        Caption = 'AM/PM'
        Font.Charset = ANSI_CHARSET
        Font.Color = 4342338
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object lbStartSecond: TLabel
        Left = 247
        Top = 133
        Width = 47
        Height = 18
        Caption = 'Second'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = 4342338
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object lbEndSecond: TLabel
        Left = 247
        Top = 199
        Width = 47
        Height = 18
        Caption = 'Second'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = 4342338
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object lbDay: TCheckListBox
        Left = 390
        Top = 155
        Width = 103
        Height = 106
        OnClickCheck = lbDayClickCheck
        Ctl3D = True
        Font.Charset = ANSI_CHARSET
        Font.Color = 4342338
        Font.Height = -12
        Font.Name = 'Tahoma'
        Font.Style = []
        ItemHeight = 14
        Items.Strings = (
          'Sunday'
          'Monday'
          'Tuesday'
          'Wednesday'
          'Thursday'
          'Friday'
          'Saturday')
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 0
        OnClick = lbDayClick
      end
      object cbHappyHourProfile: TComboBox
        Left = 140
        Top = 34
        Width = 240
        Height = 24
        Style = csDropDownList
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
        OnChange = cbHappyHourProfileChange
        OnDblClick = cbHappyHourProfileDblClick
      end
      object BtnDate: TTouchBtn
        Left = 77
        Top = 89
        Width = 60
        Height = 40
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
        OnMouseClick = BtnDateMouseClick
      end
      object BtnMonth: TTouchBtn
        Left = 164
        Top = 89
        Width = 60
        Height = 40
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
        OnMouseClick = BtnMonthMouseClick
      end
      object BtnYear: TTouchBtn
        Left = 247
        Top = 88
        Width = 60
        Height = 40
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
        OnMouseClick = BtnYearMouseClick
      end
      object BtnReset: TTouchBtn
        Left = 323
        Top = 87
        Width = 60
        Height = 40
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'Clear'
        ButtonColor = 2200566
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = BtnResetMouseClick
      end
      object BtnStartHour: TTouchBtn
        Left = 77
        Top = 155
        Width = 60
        Height = 40
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
        OnMouseClick = BtnStartHourMouseClick
      end
      object BtnStartMinute: TTouchBtn
        Left = 164
        Top = 155
        Width = 60
        Height = 40
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
        OnMouseClick = BtnStartMinuteMouseClick
      end
      object BtnStartAMPM: TTouchBtn
        Left = 320
        Top = 155
        Width = 60
        Height = 40
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
        OnMouseClick = BtnStartAMPMMouseClick
      end
      object BtnEndHour: TTouchBtn
        Left = 77
        Top = 221
        Width = 60
        Height = 40
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
        OnMouseClick = BtnEndHourMouseClick
      end
      object BtnEndMinute: TTouchBtn
        Left = 164
        Top = 221
        Width = 60
        Height = 40
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
        OnMouseClick = BtnEndMinuteMouseClick
      end
      object BtnEndAMPM: TTouchBtn
        Left = 320
        Top = 221
        Width = 60
        Height = 40
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
        OnMouseClick = BtnEndAMPMMouseClick
      end
      object BtnStartSecond: TTouchBtn
        Left = 247
        Top = 155
        Width = 60
        Height = 40
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        ButtonColor = 9933970
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = BtnStartSecondMouseClick
      end
      object BtnEndSecond: TTouchBtn
        Left = 247
        Top = 221
        Width = 60
        Height = 40
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        ButtonColor = 9933970
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = BtnEndSecondMouseClick
      end
    end
    object GroupBox2: TGroupBox
      Left = 6
      Top = 290
      Width = 290
      Height = 90
      Caption = 'Profile Settings'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 4342338
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      object cbMembersOnly: TCheckBox
        Left = 3
        Top = 26
        Width = 270
        Height = 25
        Caption = 'Happy Hour is for Members Only'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = 4342338
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnClick = cbMembersOnlyClick
      end
      object cbNoPointsEarnedDuring: TCheckBox
        Left = 3
        Top = 57
        Width = 270
        Height = 25
        Caption = 'No Points Earned During Happy Hour'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = 4342338
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnClick = cbNoPointsEarnedDuringClick
      end
    end
    object GroupBox3: TGroupBox
      Left = 326
      Top = 290
      Width = 173
      Height = 60
      Caption = 'Price Level'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 4342338
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      object csPriceLevel: TComboBox
        Left = 11
        Top = 22
        Width = 134
        Height = 24
        Style = csDropDownList
        BiDiMode = bdLeftToRight
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentBiDiMode = False
        ParentFont = False
        TabOrder = 0
        OnChange = csPriceLevelChange
      end
    end
    object GroupBox4: TGroupBox
      Left = 326
      Top = 356
      Width = 173
      Height = 77
      Caption = ' Priority'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 4342338
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 3
      object BtnPriority: TTouchBtn
        Left = 11
        Top = 23
        Width = 105
        Height = 40
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -14
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = '0'
        ButtonColor = 9933970
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = BtnPriorityMouseClick
      end
    end
  end
  object Panel2: TPanel
    Left = 522
    Top = 8
    Width = 137
    Height = 440
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object TouchBtn1: TTouchBtn
      Left = 8
      Top = 12
      Width = 122
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
      OnMouseClick = btnOkClick
    end
    object TouchBtn2: TTouchBtn
      Left = 8
      Top = 372
      Width = 122
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
    object TouchBtn3: TTouchBtn
      Left = 8
      Top = 83
      Width = 122
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Add'
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnADDProfileClick
    end
    object TouchBtn5: TTouchBtn
      Left = 8
      Top = 155
      Width = 122
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Edit'
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnEDITProfileClick
    end
    object TouchBtn4: TTouchBtn
      Left = 8
      Top = 227
      Width = 122
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Delete '
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnDeleteProfileClick
    end
  end
end
