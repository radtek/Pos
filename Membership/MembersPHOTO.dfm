object frmMembersPhoto: TfrmMembersPhoto
  Left = 100
  Top = 44
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'frmMembersPhoto'
  ClientHeight = 467
  ClientWidth = 582
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  KeyPreview = True
  OldCreateOrder = False
  OnCreate = FormCreate
  OnKeyUp = FormKeyUp
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object lblFinished: TLabel
    Left = 240
    Top = 96
    Width = 3
    Height = 13
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 233
    Height = 467
    Align = alLeft
    BevelOuter = bvNone
    TabOrder = 0
    object pnlCountdown: TPanel
      Left = 0
      Top = 0
      Width = 233
      Height = 270
      Align = alClient
      BevelOuter = bvNone
      BorderWidth = 8
      TabOrder = 0
      object GroupBox3: TGroupBox
        Left = 8
        Top = 8
        Width = 217
        Height = 182
        Align = alTop
        Caption = 'Time '
        TabOrder = 0
        object RxClock: TRxClock
          Left = 2
          Top = 15
          Width = 213
          Height = 165
          AlarmEnabled = True
          BevelInner = bvNone
          BevelOuter = bvNone
          TwelveHour = True
          Align = alClient
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -24
          Font.Name = 'MS Serif'
          Font.Style = [fsBold]
          ParentFont = False
          OnAlarm = RxClockAlarm
        end
      end
      object GroupBox4: TGroupBox
        Left = 8
        Top = 202
        Width = 217
        Height = 60
        Align = alClient
        Caption = 'Start Time '
        TabOrder = 1
        object pnlStartTime: TPanel
          Left = 2
          Top = 15
          Width = 213
          Height = 43
          Align = alClient
          BevelOuter = bvNone
          Caption = 'pnlStartTime'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -24
          Font.Name = 'MS Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
        end
      end
      object Panel3: TPanel
        Left = 8
        Top = 190
        Width = 217
        Height = 12
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 2
      end
    end
    object pnlStart: TPanel
      Left = 168
      Top = 24
      Width = 233
      Height = 392
      BevelOuter = bvNone
      BorderWidth = 8
      TabOrder = 1
      Visible = False
      object GroupBox1: TGroupBox
        Left = 8
        Top = 8
        Width = 217
        Height = 124
        Align = alTop
        Caption = 'Countdown '
        TabOrder = 0
        object lblTimer: TLabel
          Left = 2
          Top = 15
          Width = 213
          Height = 107
          Align = alClient
          Alignment = taCenter
          AutoSize = False
          Caption = '55'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -64
          Font.Name = 'Georgia'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          WordWrap = True
        end
      end
      object GroupBox2: TGroupBox
        Left = 8
        Top = 144
        Width = 217
        Height = 240
        Align = alClient
        Caption = 'History List '
        TabOrder = 1
        object ListBox: TListBox
          Left = 2
          Top = 15
          Width = 213
          Height = 223
          Align = alClient
          BorderStyle = bsNone
          Color = clBtnFace
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ItemHeight = 24
          ParentFont = False
          TabOrder = 0
        end
      end
      object Panel2: TPanel
        Left = 8
        Top = 132
        Width = 217
        Height = 12
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 2
      end
    end
    object Panel5: TPanel
      Left = 0
      Top = 270
      Width = 233
      Height = 197
      Align = alBottom
      BevelOuter = bvNone
      BorderWidth = 8
      Caption = 'Panel5'
      TabOrder = 2
      object GroupBox6: TGroupBox
        Left = 8
        Top = 8
        Width = 217
        Height = 181
        Align = alClient
        Caption = 'End Time '
        TabOrder = 0
        object pnlEndTime: TPanel
          Left = 2
          Top = 15
          Width = 213
          Height = 164
          Align = alClient
          BevelOuter = bvNone
          Caption = 'pnlEndTime'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -24
          Font.Name = 'MS Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
        end
      end
    end
  end
  object Panel4: TPanel
    Left = 233
    Top = 0
    Width = 349
    Height = 467
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 8
    Caption = 'Panel4'
    TabOrder = 1
    object gbxPhoto: TGroupBox
      Left = 8
      Top = 8
      Width = 333
      Height = 451
      Align = alClient
      Caption = 'Members Photograph '
      Color = clBtnFace
      ParentColor = False
      TabOrder = 0
      object Image2: TImage
        Left = 138
        Top = 55
        Width = 329
        Height = 434
        Stretch = True
        Visible = False
      end
      object Image1: TImage
        Left = 114
        Top = 80
        Width = 287
        Height = 297
        Stretch = True
      end
      object lblLuckyMember: TLabel
        Left = 2
        Top = 15
        Width = 247
        Height = 38
        Alignment = taCenter
        Caption = 'Lucky Member'
        Color = clYellow
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -32
        Font.Name = 'Georgia'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        Layout = tlCenter
        Visible = False
      end
      object lblLuckyMemberBottom: TLabel
        Left = 4
        Top = 79
        Width = 274
        Height = 38
        Alignment = taCenter
        Caption = 'Congratulations '
        Color = clYellow
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -32
        Font.Name = 'Georgia'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        Layout = tlCenter
        Visible = False
      end
      object pnlDrawn: TPanel
        Left = 24
        Top = 136
        Width = 233
        Height = 161
        BevelInner = bvLowered
        BevelWidth = 2
        TabOrder = 0
        Visible = False
        object Label1: TLabel
          Left = 32
          Top = 76
          Width = 78
          Height = 13
          Caption = 'Member Number'
        end
        object Label2: TLabel
          Left = 31
          Top = 23
          Width = 177
          Height = 29
          Caption = 'Lucky Member '
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -24
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object edtLuckyMember: TwwDBEdit
          Left = 121
          Top = 72
          Width = 73
          Height = 21
          Hint = 'Lucky Member'
          Picture.PictureMask = '#*#'
          TabOrder = 0
          UnboundDataType = wwDefault
          UnboundAlignment = taRightJustify
          WantReturns = False
          WordWrap = False
        end
        object btnOK: TButton
          Left = 130
          Top = 113
          Width = 87
          Height = 25
          Caption = 'OK'
          TabOrder = 1
          OnClick = btnOKClick
        end
        object btnCancel: TButton
          Left = 26
          Top = 113
          Width = 87
          Height = 25
          Caption = 'Cancel'
          TabOrder = 2
          OnClick = btnCancelClick
        end
      end
    end
  end
  object MemberTimer: TTimer
    Enabled = False
    OnTimer = MemberTimerTimer
    Left = 144
    Top = 16
  end
  object labelTimer: TTimer
    Enabled = False
    OnTimer = labelTimerTimer
    Left = 104
    Top = 16
  end
end
