object frmMain: TfrmMain
  Left = 316
  Top = 108
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'IQWorks Code Generator'
  ClientHeight = 474
  ClientWidth = 544
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  Scaled = False
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pcLicences: TPageControl
    Left = 8
    Top = 8
    Width = 529
    Height = 465
    ActivePage = tsSingleLicence
    TabIndex = 0
    TabOrder = 0
    OnChange = pcLicencesChange
    object tsSingleLicence: TTabSheet
      Caption = 'Single Licence'
      DesignSize = (
        521
        437)
      object Label12: TLabel
        Left = 8
        Top = 356
        Width = 80
        Height = 13
        Anchors = [akLeft, akBottom]
        Caption = 'Registration Key:'
      end
      object Panel1: TPanel
        Left = 8
        Top = 248
        Width = 505
        Height = 41
        Anchors = [akLeft, akBottom]
        TabOrder = 2
        object Label3: TLabel
          Left = 72
          Top = 12
          Width = 3
          Height = 13
          Caption = '-'
        end
        object Label6: TLabel
          Left = 144
          Top = 12
          Width = 3
          Height = 13
          Caption = '-'
        end
        object Label7: TLabel
          Left = 216
          Top = 12
          Width = 3
          Height = 13
          Caption = '-'
        end
        object Label8: TLabel
          Left = 288
          Top = 12
          Width = 3
          Height = 13
          Caption = '-'
        end
        object Label10: TLabel
          Left = 360
          Top = 12
          Width = 3
          Height = 13
          Caption = '-'
        end
        object Label11: TLabel
          Left = 432
          Top = 12
          Width = 3
          Height = 13
          Caption = '-'
        end
        object Edit1: TEdit
          Left = 8
          Top = 8
          Width = 57
          Height = 22
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Courier New'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnChange = Edit1Change
          OnKeyDown = Edit1KeyDown
          OnKeyPress = Edit1KeyPress
        end
        object Edit2: TEdit
          Left = 80
          Top = 8
          Width = 57
          Height = 22
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Courier New'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnChange = Edit1Change
          OnKeyDown = Edit2KeyDown
          OnKeyPress = Edit1KeyPress
        end
        object Edit3: TEdit
          Left = 152
          Top = 8
          Width = 57
          Height = 22
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Courier New'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          OnChange = Edit1Change
          OnKeyDown = Edit3KeyDown
          OnKeyPress = Edit1KeyPress
        end
        object Edit4: TEdit
          Left = 224
          Top = 8
          Width = 57
          Height = 22
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Courier New'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          OnChange = Edit1Change
          OnKeyDown = Edit4KeyDown
          OnKeyPress = Edit1KeyPress
        end
        object Edit5: TEdit
          Left = 296
          Top = 8
          Width = 57
          Height = 22
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Courier New'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          OnChange = Edit1Change
          OnKeyDown = Edit5KeyDown
          OnKeyPress = Edit1KeyPress
        end
        object Edit6: TEdit
          Left = 368
          Top = 8
          Width = 57
          Height = 22
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Courier New'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
          OnChange = Edit1Change
          OnKeyDown = Edit6KeyDown
          OnKeyPress = Edit1KeyPress
        end
        object Edit7: TEdit
          Left = 440
          Top = 8
          Width = 57
          Height = 22
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Courier New'
          Font.Style = []
          ParentFont = False
          TabOrder = 6
          OnChange = Edit1Change
          OnKeyDown = Edit7KeyDown
          OnKeyPress = Edit1KeyPress
        end
      end
      object edRegKey: TEdit
        Left = 8
        Top = 376
        Width = 505
        Height = 22
        Anchors = [akLeft, akBottom]
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Courier New'
        Font.Style = []
        ParentColor = True
        ParentFont = False
        ReadOnly = True
        TabOrder = 4
      end
      object BitBtn1: TBitBtn
        Left = 8
        Top = 304
        Width = 81
        Height = 33
        Anchors = [akLeft, akBottom]
        Caption = 'Encode'
        TabOrder = 3
        OnClick = BitBtn1Click
      end
      object chbAddToMulti: TCheckBox
        Left = 8
        Top = 224
        Width = 157
        Height = 17
        Alignment = taLeftJustify
        Anchors = [akLeft, akBottom]
        Caption = 'Add To Multiple Licences'
        TabOrder = 1
      end
      object Panel2: TPanel
        Left = 8
        Top = 8
        Width = 505
        Height = 137
        TabOrder = 0
        object Label2: TLabel
          Left = 8
          Top = 8
          Width = 91
          Height = 13
          Caption = 'Max POS Licences'
        end
        object Label1: TLabel
          Left = 8
          Top = 40
          Width = 92
          Height = 13
          Caption = 'Max Palm Licences'
        end
        object Label9: TLabel
          Left = 8
          Top = 72
          Width = 97
          Height = 13
          Caption = 'Max Office Licences'
        end
        object edPOSLicences: TEdit
          Left = 136
          Top = 8
          Width = 81
          Height = 21
          TabOrder = 0
          OnChange = edPOSLicencesChange
        end
        object edPalmLicences: TEdit
          Left = 136
          Top = 40
          Width = 81
          Height = 21
          TabOrder = 1
          OnChange = edPOSLicencesChange
        end
        object chbKitchenModule: TCheckBox
          Left = 224
          Top = 8
          Width = 130
          Height = 17
          Alignment = taLeftJustify
          Caption = 'ChefMate Module'
          TabOrder = 2
        end
        object cbRooms: TCheckBox
          Left = 224
          Top = 56
          Width = 130
          Height = 17
          Alignment = taLeftJustify
          Caption = 'Rooms Module'
          TabOrder = 3
        end
        object cbTurnAroundTimes: TCheckBox
          Left = 224
          Top = 32
          Width = 130
          Height = 17
          Alignment = taLeftJustify
          Caption = 'Time Tracking'
          TabOrder = 4
        end
        object edOfficeLicences: TEdit
          Left = 136
          Top = 72
          Width = 81
          Height = 21
          TabOrder = 5
          OnChange = edPOSLicencesChange
        end
        object cbMembership: TCheckBox
          Left = 224
          Top = 80
          Width = 130
          Height = 17
          Alignment = taLeftJustify
          Caption = 'Membership Module'
          TabOrder = 6
        end
        object cbMYOB: TCheckBox
          Left = 368
          Top = 8
          Width = 130
          Height = 17
          Alignment = taLeftJustify
          Caption = 'MYOB Module'
          TabOrder = 7
        end
        object cbMenuMateDirect: TCheckBox
          Left = 368
          Top = 32
          Width = 130
          Height = 17
          Alignment = taLeftJustify
          Caption = 'MenuMate Direct'
          TabOrder = 8
        end
        object cbIntergratedEftPos: TCheckBox
          Left = 368
          Top = 56
          Width = 130
          Height = 17
          Alignment = taLeftJustify
          Caption = 'Intergrated Eftpos'
          TabOrder = 9
        end
        object cbExpires: TCheckBox
          Left = 224
          Top = 104
          Width = 129
          Height = 17
          Alignment = taLeftJustify
          Caption = 'Expires'
          TabOrder = 10
        end
        object dtpExpiry: TDateTimePicker
          Left = 368
          Top = 102
          Width = 130
          Height = 21
          CalAlignment = dtaLeft
          Date = 38187.6782432523
          Time = 38187.6782432523
          DateFormat = dfShort
          DateMode = dmComboBox
          Kind = dtkDate
          ParseInput = False
          TabOrder = 11
        end
        object chPhoenixSystem: TCheckBox
          Left = 367
          Top = 80
          Width = 130
          Height = 17
          Alignment = taLeftJustify
          Caption = 'Phoenix Hotel System'
          TabOrder = 12
        end
      end
      object btnClear: TBitBtn
        Left = 432
        Top = 304
        Width = 81
        Height = 33
        Anchors = [akLeft, akBottom]
        Caption = 'Clear'
        TabOrder = 5
        OnClick = btnClearClick
      end
    end
    object tsMultipleLicences: TTabSheet
      Caption = 'Multiple Licences'
      ImageIndex = 1
      DesignSize = (
        521
        437)
      object Label5: TLabel
        Left = 8
        Top = 208
        Width = 70
        Height = 13
        Anchors = [akLeft, akBottom]
        Caption = 'Customer Keys'
      end
      object Label4: TLabel
        Left = 8
        Top = 320
        Width = 82
        Height = 13
        Anchors = [akLeft, akBottom]
        Caption = 'Registration Keys'
      end
      object memCustomerKeys: TMemo
        Left = 8
        Top = 224
        Width = 281
        Height = 89
        Anchors = [akLeft, akBottom]
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Courier New'
        Font.Style = []
        ParentFont = False
        ScrollBars = ssVertical
        TabOrder = 0
        OnChange = memCustomerKeysChange
      end
      object memRegistrationKeys: TMemo
        Left = 8
        Top = 336
        Width = 281
        Height = 89
        Anchors = [akLeft, akBottom]
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Courier New'
        Font.Style = []
        ParentFont = False
        ScrollBars = ssVertical
        TabOrder = 1
      end
    end
  end
end
