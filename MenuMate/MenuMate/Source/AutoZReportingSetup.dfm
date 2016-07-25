object frmAutoZReportingSetup: TfrmAutoZReportingSetup
  Left = 0
  Top = 0
  Caption = 'frmAutoZReportingSetup'
  ClientHeight = 355
  ClientWidth = 774
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object btnCheck4: TTouchBtn
    Tag = 3
    Left = 581
    Top = 198
    ParentColor = True
    Caption = 'Check 4'
    ButtonColor = clBtnFace
    LatchedColor = clBtnFace
    DisabledButtonColor = clBtnFace
    LatchingProperties = [lkLatchColor, lkStayDown]
    OnMouseClick = CheckTimeSelected
  end
  object btnCheck3: TTouchBtn
    Tag = 2
    Left = 390
    Top = 198
    ParentColor = True
    Caption = 'Check 3'
    ButtonColor = clBtnFace
    LatchedColor = clBtnFace
    DisabledButtonColor = clBtnFace
    LatchingProperties = [lkLatchColor, lkStayDown]
    OnMouseClick = CheckTimeSelected
  end
  object btnCheck2: TTouchBtn
    Tag = 1
    Left = 199
    Top = 198
    ParentColor = True
    Caption = 'Check 2'
    ButtonColor = clBtnFace
    LatchedColor = clBtnFace
    DisabledButtonColor = clBtnFace
    LatchingProperties = [lkLatchColor, lkStayDown]
    OnMouseClick = CheckTimeSelected
  end
  object btnCheck1: TTouchBtn
    Left = 8
    Top = 198
    ParentColor = True
    Caption = 'Check 1'
    ButtonColor = clBtnFace
    LatchedColor = clBtnFace
    DisabledButtonColor = clBtnFace
    LatchingProperties = [lkLatchColor, lkStayDown]
    OnMouseClick = CheckTimeSelected
  end
  object ttCheckTime: TTouchTime
    Left = 248
    Top = 8
    Width = 281
    Height = 121
    BorderWidth = 4
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -35
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    IndicatorDelay = 100
    GridBorderWidth = 0
    ButtonGapWidth = 4
    ButtonGapHeight = 4
    ScrollHourUp.Font.Charset = DEFAULT_CHARSET
    ScrollHourUp.Font.Color = clWindowText
    ScrollHourUp.Font.Height = -11
    ScrollHourUp.Font.Name = 'Tahoma'
    ScrollHourUp.Font.Style = []
    ScrollHourUp.LatchedFont.Charset = DEFAULT_CHARSET
    ScrollHourUp.LatchedFont.Color = clWindowText
    ScrollHourUp.LatchedFont.Height = -11
    ScrollHourUp.LatchedFont.Name = 'Tahoma'
    ScrollHourUp.LatchedFont.Style = []
    ScrollHourUp.GlyphAlpha = 255
    ScrollHourUp.Color = -16777201
    ScrollHourUp.FontColor = -16777208
    ScrollHourUp.LatchedColor = -16777201
    ScrollHourUp.DisabledColor = -16777201
    ScrollHourUp.LatchedFontColor = -16777208
    ScrollHourDown.Font.Charset = DEFAULT_CHARSET
    ScrollHourDown.Font.Color = clWindowText
    ScrollHourDown.Font.Height = -11
    ScrollHourDown.Font.Name = 'Tahoma'
    ScrollHourDown.Font.Style = []
    ScrollHourDown.LatchedFont.Charset = DEFAULT_CHARSET
    ScrollHourDown.LatchedFont.Color = clWindowText
    ScrollHourDown.LatchedFont.Height = -11
    ScrollHourDown.LatchedFont.Name = 'Tahoma'
    ScrollHourDown.LatchedFont.Style = []
    ScrollHourDown.GlyphAlpha = 255
    ScrollHourDown.Color = -16777201
    ScrollHourDown.FontColor = -16777208
    ScrollHourDown.LatchedColor = -16777201
    ScrollHourDown.DisabledColor = -16777201
    ScrollHourDown.LatchedFontColor = -16777208
    ScrollMinuteUp.Font.Charset = DEFAULT_CHARSET
    ScrollMinuteUp.Font.Color = clWindowText
    ScrollMinuteUp.Font.Height = -11
    ScrollMinuteUp.Font.Name = 'Tahoma'
    ScrollMinuteUp.Font.Style = []
    ScrollMinuteUp.LatchedFont.Charset = DEFAULT_CHARSET
    ScrollMinuteUp.LatchedFont.Color = clWindowText
    ScrollMinuteUp.LatchedFont.Height = -11
    ScrollMinuteUp.LatchedFont.Name = 'Tahoma'
    ScrollMinuteUp.LatchedFont.Style = []
    ScrollMinuteUp.GlyphAlpha = 255
    ScrollMinuteUp.Color = -16777201
    ScrollMinuteUp.FontColor = -16777208
    ScrollMinuteUp.LatchedColor = -16777201
    ScrollMinuteUp.DisabledColor = -16777201
    ScrollMinuteUp.LatchedFontColor = -16777208
    ScrollMinuteDown.Font.Charset = DEFAULT_CHARSET
    ScrollMinuteDown.Font.Color = clWindowText
    ScrollMinuteDown.Font.Height = -11
    ScrollMinuteDown.Font.Name = 'Tahoma'
    ScrollMinuteDown.Font.Style = []
    ScrollMinuteDown.LatchedFont.Charset = DEFAULT_CHARSET
    ScrollMinuteDown.LatchedFont.Color = clWindowText
    ScrollMinuteDown.LatchedFont.Height = -11
    ScrollMinuteDown.LatchedFont.Name = 'Tahoma'
    ScrollMinuteDown.LatchedFont.Style = []
    ScrollMinuteDown.GlyphAlpha = 255
    ScrollMinuteDown.Color = -16777201
    ScrollMinuteDown.FontColor = -16777208
    ScrollMinuteDown.LatchedColor = -16777201
    ScrollMinuteDown.DisabledColor = -16777201
    ScrollMinuteDown.LatchedFontColor = -16777208
    InitialDelay = 300
    RepeatDelay = 100
    ButtonWidth = 60
    DisplayFormat = 'HH:nn'
  end
  object btnEmailZ: TTouchBtn
    Left = 390
    Top = 135
    ParentColor = True
    Caption = 'Email Z Report'
    ButtonColor = clBtnFace
    LatchedColor = clBtnFace
    DisabledButtonColor = clBtnFace
    Enabled = False
    LatchingType = ltCheckbox
    LatchingProperties = [lkLatchColor, lkStayDown]
  end
  object btnPrintZ: TTouchBtn
    Left = 199
    Top = 135
    ParentColor = True
    Caption = 'Print Z Report'
    ButtonColor = clBtnFace
    LatchedColor = clBtnFace
    DisabledButtonColor = clBtnFace
    Enabled = False
    LatchingType = ltCheckbox
    LatchingProperties = [lkLatchColor, lkStayDown]
  end
  object btnResetCheckTime: TTouchBtn
    Left = 199
    Top = 288
    ParentColor = True
    Caption = 'Reset selected check time'
    ButtonColor = clBtnFace
    LatchedColor = clBtnFace
    DisabledButtonColor = clBtnFace
    Enabled = False
    LatchingProperties = [lkLatchColor, lkStayDown]
    OnMouseClick = ResetCheckTimeSelected
  end
  object btnResetAllCheckTimes: TTouchBtn
    Tag = 1
    Left = 390
    Top = 288
    ParentColor = True
    Caption = 'Reset all check times'
    ButtonColor = clBtnFace
    LatchedColor = clBtnFace
    DisabledButtonColor = clBtnFace
    Enabled = False
    LatchingProperties = [lkLatchColor, lkStayDown]
    OnMouseClick = ResetCheckTimeSelected
  end
  object btnSaveAndClose: TTouchBtn
    Left = 8
    Top = 288
    ParentColor = True
    Caption = 'Save and Close'
    ButtonColor = clBtnFace
    LatchedColor = clBtnFace
    DisabledButtonColor = clBtnFace
    LatchingProperties = [lkLatchColor, lkStayDown]
    OnMouseClick = Close
  end
  object btnDiscardAndClose: TTouchBtn
    Tag = 1
    Left = 581
    Top = 288
    ParentColor = True
    Caption = 'Discard and Close'
    ButtonColor = clBtnFace
    LatchedColor = clBtnFace
    DisabledButtonColor = clBtnFace
    LatchingProperties = [lkLatchColor, lkStayDown]
    OnMouseClick = Close
  end
  object btnAuthenticateAs: TTouchBtn
    Left = 581
    Top = 8
    ParentColor = True
    Caption = 'Authenticate as...'
    ButtonColor = clBtnFace
    LatchedColor = clBtnFace
    DisabledButtonColor = clBtnFace
    LatchingProperties = [lkLatchColor, lkStayDown]
    OnMouseClick = SelectUserToAuthenticate
  end
end
