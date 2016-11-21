object frmSetup: TfrmSetup
  Tag = 1024
  Left = 2
  Top = 2
  Align = alClient
  Anchors = [akLeft, akTop, akRight]
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = 'Setup'
  ClientHeight = 749
  ClientWidth = 1030
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -15
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  Scaled = False
  WindowState = wsMaximized
  OnClose = FormClose
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  ExplicitWidth = 320
  ExplicitHeight = 240
  DesignSize = (
    1030
    749)
  PixelsPerInch = 96
  TextHeight = 18
  object lbVersion: TLabel
    Left = 0
    Top = 729
    Width = 46
    Height = 18
    Anchors = [akLeft, akBottom]
    Caption = 'version'
    ExplicitTop = 668
  end
  object PageControl: TPageControl
    Left = 0
    Top = 1
    Width = 918
    Height = 726
    ActivePage = tsMallSettings
    Anchors = [akLeft, akTop, akRight, akBottom]
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnChange = PageControlChange
    OnChanging = PageControlChanging
    object tsPrinters: TTabSheet
      Caption = 'I/O Settings'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label6: TLabel
        Left = 224
        Top = 32
        Width = 174
        Height = 18
        Caption = 'MenuMate DB IP Address '
      end
      object Panel6: TPanel
        Left = 0
        Top = 0
        Width = 910
        Height = 693
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 1
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        object Label20: TLabel
          Left = 9
          Top = 287
          Width = 82
          Height = 18
          Caption = 'Security Port'
        end
        object Label8: TLabel
          Left = 8
          Top = 232
          Width = 135
          Height = 18
          Caption = 'Barcode Reader Port'
        end
        object lblBarcodeFormat: TLabel
          Left = 8
          Top = 175
          Width = 180
          Height = 18
          Caption = 'Weight Scales Label Format'
        end
        object Label4: TLabel
          Left = 8
          Top = 60
          Width = 141
          Height = 18
          Caption = 'Proximity Reader Port'
        end
        object Label11: TLabel
          Left = 8
          Top = 342
          Width = 144
          Height = 18
          Caption = 'Customer Pole Display'
        end
        object Label1: TLabel
          Left = 8
          Top = 5
          Width = 114
          Height = 18
          Caption = 'Cash Drawer Port'
        end
        object Label3: TLabel
          Left = 8
          Top = 115
          Width = 121
          Height = 18
          Caption = 'Weight Scales Port'
        end
        object cbSecuritySerialPort: TComboBox
          Left = 9
          Top = 310
          Width = 329
          Height = 26
          Style = csDropDownList
          TabOrder = 19
          OnChange = cbSecuritySerialPortChange
          Items.Strings = (
            'None'
            'Comm 01'
            'Comm 02'
            'Comm 03'
            'Comm 04'
            'Comm 05'
            'Comm 06'
            'Comm 07'
            'Comm 08'
            'Comm 09'
            'Comm 10'
            'Comm 11'
            'Comm 12'
            'Comm 13'
            'Comm 14'
            'Comm 15'
            'Comm 16')
        end
        object cbBarCodePort: TComboBox
          Left = 8
          Top = 255
          Width = 329
          Height = 26
          Style = csDropDownList
          TabOrder = 1
          OnChange = cbBarCodePortChange
          Items.Strings = (
            'None'
            'Comm 01'
            'Comm 02'
            'Comm 03'
            'Comm 04'
            'Comm 05'
            'Comm 06'
            'Comm 07'
            'Comm 08'
            'Comm 09'
            'Comm 10'
            'Comm 11'
            'Comm 12'
            'Comm 13'
            'Comm 14'
            'Comm 15'
            'Comm 16')
        end
        object cbProxReader: TComboBox
          Left = 8
          Top = 83
          Width = 329
          Height = 26
          Style = csDropDownList
          TabOrder = 2
          OnChange = cbProxReaderChange
          Items.Strings = (
            'None'
            'Comm 01'
            'Comm 02'
            'Comm 03'
            'Comm 04'
            'Comm 05'
            'Comm 06'
            'Comm 07'
            'Comm 08'
            'Comm 09'
            'Comm 10'
            'Comm 11'
            'Comm 12'
            'Comm 13'
            'Comm 14'
            'Comm 15'
            'Comm 16')
        end
        object cbPoleDisplayMode: TComboBox
          Left = 8
          Top = 432
          Width = 329
          Height = 26
          Style = csDropDownList
          TabOrder = 3
          OnChange = cbPoleDisplayModeChange
          Items.Strings = (
            'No Emulation (Javlins)'
            'CD5220'
            'Epson Esc/POS'
            'UTC'
            'EMAX ADEX'
            'Toshiba')
        end
        object cbPoleDisplay: TComboBox
          Left = 8
          Top = 397
          Width = 329
          Height = 26
          Style = csDropDownList
          TabOrder = 4
          OnChange = cbPoleDisplayChange
          Items.Strings = (
            'None'
            'Comm 01'
            'Comm 02'
            'Comm 03'
            'Comm 04'
            'Comm 05'
            'Comm 06'
            'Comm 07'
            'Comm 08'
            'Comm 09'
            'Comm 10'
            'Comm 11'
            'Comm 12'
            'Comm 13'
            'Comm 14'
            'Comm 15'
            'Comm 16')
        end
        object cbUseHighChars: TCheckBox
          Left = 352
          Top = 32
          Width = 185
          Height = 17
          Caption = 'Use High Characters'
          TabOrder = 5
          OnClick = cbUseHighCharsClick
        end
        object cbCashDrawPort: TComboBox
          Left = 8
          Top = 28
          Width = 329
          Height = 26
          Style = csDropDownList
          TabOrder = 6
          OnChange = cbCashDrawPortChange
        end
        object edSerialKickCharsCount: TEdit
          Left = 543
          Top = 31
          Width = 57
          Height = 27
          AutoSize = False
          TabOrder = 7
          Text = '3'
        end
        object UpDown1: TUpDown
          Left = 600
          Top = 31
          Width = 15
          Height = 27
          Associate = edSerialKickCharsCount
          Max = 5000
          Position = 3
          TabOrder = 8
          OnClick = UpDown1Click
        end
        object gbPoleDisplay: TGroupBox
          Left = 8
          Top = 472
          Width = 633
          Height = 139
          Caption = 'Default Pole Display Top and Bottom Lines'
          TabOrder = 9
          object edTopLine: TEdit
            Left = 8
            Top = 21
            Width = 609
            Height = 26
            TabOrder = 0
            OnMouseUp = edTopLineMouseUp
          end
          object edBottomLine: TEdit
            Left = 8
            Top = 58
            Width = 609
            Height = 26
            TabOrder = 1
            OnMouseUp = edBottomLineMouseUp
          end
          object Button1: TButton
            Left = 8
            Top = 95
            Width = 193
            Height = 35
            Caption = 'Test Clear Command'
            TabOrder = 2
            OnClick = Button1Click
          end
        end
        object GroupBox1: TGroupBox
          Left = 8
          Top = 613
          Width = 633
          Height = 68
          TabOrder = 10
          object Label2: TLabel
            Left = 11
            Top = 30
            Width = 76
            Height = 18
            Caption = 'Export Path'
          end
          object btnSelectStockMasterPath: TTouchBtn
            Left = 488
            Top = 16
            Width = 129
            Height = 40
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -17
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Select'
            ButtonColor = clBtnFace
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseClick = btnSelectStockMasterPathClick
          end
          object edStockMasterExport: TEdit
            Left = 120
            Top = 24
            Width = 345
            Height = 26
            TabOrder = 1
            OnMouseUp = edStockMasterExportMouseUp
          end
        end
        object Configure: TButton
          Left = 344
          Top = 310
          Width = 161
          Height = 41
          Caption = 'Configure'
          TabOrder = 11
          OnClick = ConfigureClick
        end
        object cbProxFormat: TComboBox
          Left = 343
          Top = 83
          Width = 241
          Height = 26
          Style = csDropDownList
          TabOrder = 12
          OnChange = cbProxFormatChange
          Items.Strings = (
            'Standard'
            'Wiegand')
        end
        object cbWeighScales: TComboBox
          Left = 9
          Top = 140
          Width = 329
          Height = 26
          Style = csDropDownList
          TabOrder = 13
          OnChange = cbWeighScalesChange
          Items.Strings = (
            'None'
            'Comm 01'
            'Comm 02'
            'Comm 03'
            'Comm 04'
            'Comm 05'
            'Comm 06'
            'Comm 07'
            'Comm 08'
            'Comm 09'
            'Comm 10'
            'Comm 11'
            'Comm 12'
            'Comm 13'
            'Comm 14'
            'Comm 15'
            'Comm 16')
        end
        object cbBarcodeFormat: TComboBox
          Left = 9
          Top = 200
          Width = 329
          Height = 26
          Style = csDropDownList
          TabOrder = 18
          OnChange = cbBarcodeFormatChange
          Items.Strings = (
            'FF CCCCC PCD PPPP CD'
            'FF CCCCC PCD PPPPP CD'
            'F2 CCCCC XXXXXX CD'
            'FF CCCCC WWWWWW CD'
            'FF CCCCC WWWWWW PPPPPP CD')
        end
        object btnWeighScalesConfig: TButton
          Left = 608
          Top = 132
          Width = 97
          Height = 41
          Caption = 'Configure'
          TabOrder = 14
          OnClick = btnWeighScalesConfigClick
        end
        object tbtnProxSerialConfig: TButton
          Left = 608
          Top = 75
          Width = 97
          Height = 41
          Caption = 'Configure'
          TabOrder = 15
          OnClick = tbtnProxSerialConfigClick
        end
        object btnTestScales: TButton
          Left = 711
          Top = 132
          Width = 97
          Height = 41
          Caption = 'Test Scales'
          TabOrder = 16
          OnClick = btnTestScalesClick
        end
        object Button2: TButton
          Left = 343
          Top = 397
          Width = 161
          Height = 41
          Caption = 'Configure'
          TabOrder = 17
          OnClick = ConfigPoleDisplayClick
        end
        object cbScalesType: TComboBox
          Left = 344
          Top = 140
          Width = 241
          Height = 26
          Style = csDropDownList
          TabOrder = 21
          OnChange = cbScalesTypeChange
          Items.Strings = (
            'Wedderburn Type A Std Stream'
            'Towa Scale QUQA Stream'
            'Avery Berkel FX Series'
            'Kenko KK-300W')
        end
        object gbNewbookDisplay: TGroupBox
          Left = 344
          Top = 172
          Width = 409
          Height = 71
          Caption = 'PMS Setting'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          object lblNewBook: TLabel
            Left = 23
            Top = 27
            Width = 67
            Height = 18
            Caption = 'PMS Type'
          end
          object cbNewbookType: TComboBox
            Left = 127
            Top = 23
            Width = 241
            Height = 26
            Style = csDropDownList
            TabOrder = 0
            OnChange = cbNewbookTypeChange
            Items.Strings = (
              'None'
              'Strait'
              'Newbook ')
          end
        end
        object cbShowCustomerDisplay: TCheckBox
          Left = 9
          Top = 366
          Width = 240
          Height = 17
          Caption = 'Show Customer Display'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 20
          OnClick = cbShowCustomerDisplayClick
        end
      end
    end
    object tsApplications: TTabSheet
      Caption = 'Applications Settings'
      ImageIndex = 4
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object GroupBox4: TGroupBox
        Left = 8
        Top = 326
        Width = 889
        Height = 323
        Caption = 'Networking'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        object lbeMMDBIP: TLabel
          Left = 224
          Top = 32
          Width = 174
          Height = 18
          Caption = 'MenuMate DB IP Address '
        end
        object lbeMMDBPath: TLabel
          Left = 224
          Top = 56
          Width = 132
          Height = 18
          Caption = 'MenuMate DB Path '
        end
        object lbeMemDBIP: TLabel
          Left = 224
          Top = 108
          Width = 184
          Height = 18
          Caption = 'Membership DB IP Address '
        end
        object lbeMemDBPath: TLabel
          Left = 224
          Top = 133
          Width = 142
          Height = 18
          Caption = 'Membership DB Path '
        end
        object lbeStockDBIP: TLabel
          Left = 224
          Top = 192
          Width = 140
          Height = 18
          Caption = 'Stock DB IP Address '
        end
        object lbeStockDBPath: TLabel
          Left = 224
          Top = 216
          Width = 98
          Height = 18
          Caption = 'Stock DB Path '
        end
        object lbeSNTPIP: TLabel
          Left = 224
          Top = 272
          Width = 117
          Height = 18
          Caption = 'SNTP IP Address '
        end
        object tbtnReconfigMMDB: TTouchBtn
          Left = 8
          Top = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Reconfigure MenuMate DB'
          ButtonColor = clNavy
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbtnReconfigMMDBMouseClick
        end
        object tbtnReconfigMemDB: TTouchBtn
          Left = 8
          Top = 104
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Reconfigure Membership DB'
          ButtonColor = clNavy
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbtnReconfigMemDBMouseClick
        end
        object tbtnReconfigStockDB: TTouchBtn
          Left = 8
          Top = 184
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Reconfigure Stock DB'
          ButtonColor = clNavy
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbtnReconfigStockDBMouseClick
        end
        object tbtnReconfigSNTPIP: TTouchBtn
          Left = 8
          Top = 256
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Reconfigure Network Time Server'
          ButtonColor = clNavy
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbtnReconfigSNTPIPMouseClick
        end
        object tbtnIPSettingsRefresh: TTouchBtn
          Left = 760
          Top = 280
          Width = 121
          Height = 33
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Refresh'
          ButtonColor = clMaroon
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbtnIPSettingsRefreshMouseClick
        end
      end
      object tbtnStatusReport: TTouchBtn
        Left = 8
        Top = 16
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'Status Report'
        ButtonColor = clGreen
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = tbtnStatusReportMouseClick
      end
      object btnDebuggerTest: TTouchBtn
        Left = 8
        Top = 88
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'Test Debugger'
        ButtonColor = clMaroon
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnDebuggerTestMouseClick
      end
      object rgMembershipType: TRadioGroup
        Left = 3
        Top = 160
        Width = 318
        Height = 160
        Caption = 'Membership Type'
        Color = clBtnHighlight
        ItemIndex = 0
        Items.Strings = (
          'MenuMate Loyalty.'
          'MenuMate Club Membership'
          'E-Bet Gaming Membership'
          'Casino External Membership'
          'Thorlink Membership')
        ParentBackground = False
        ParentColor = False
        TabOrder = 3
        OnClick = rgMembershipTypeClick
      end
      object gbEmailDetails: TGroupBox
        Left = 326
        Top = 39
        Width = 355
        Height = 283
        Caption = 'Sender'#39's Email Credentials'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        object lbeEmailHost: TLabel
          Left = 9
          Top = 32
          Width = 68
          Height = 18
          Caption = 'Email Host'
        end
        object lbeEmailHostPort: TLabel
          Left = 9
          Top = 100
          Width = 99
          Height = 18
          Caption = 'Email Host Port'
        end
        object lbeEmailId: TLabel
          Left = 9
          Top = 170
          Width = 52
          Height = 18
          Caption = 'Email Id'
        end
        object lbeEmailPassword: TLabel
          Left = 9
          Top = 220
          Width = 99
          Height = 18
          Caption = 'Email Password'
        end
        object tbtnReconfigEmailHost: TTouchBtn
          Left = 116
          Top = 20
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Configure Email Host'
          ButtonColor = clNavy
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbtnReconfigEmailHostMouseClick
        end
        object tbtnReconfigEmailHostPort: TTouchBtn
          Left = 116
          Top = 85
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Configure Email Host Port'
          ButtonColor = clNavy
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbtnReconfigEmailHostPortMouseClick
        end
        object tbtnReconfigEmailId: TTouchBtn
          Left = 116
          Top = 150
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Configure Email Id'
          ButtonColor = clNavy
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbtnReconfigEmailIdMouseClick
        end
        object tbtnReconfigEmailPassword: TTouchBtn
          Left = 116
          Top = 215
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Configure Email Password'
          ButtonColor = clNavy
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = tbtnReconfigEmailPasswordMouseClick
        end
      end
    end
    object tsExport: TTabSheet
      Caption = 'Export Settings'
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object gbMall: TGroupBox
        Left = 16
        Top = 3
        Width = 737
        Height = 620
        Caption = 'Mall'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        object lbTenantNo: TLabel
          Left = 16
          Top = 94
          Width = 5
          Height = 18
        end
        object lbFileLocation: TLabel
          Left = 16
          Top = 152
          Width = 78
          Height = 18
          Caption = 'File Location'
        end
        object lbClassCode: TLabel
          Left = 16
          Top = 210
          Width = 71
          Height = 18
          Caption = 'Class Code'
        end
        object lbTradeCode: TLabel
          Left = 16
          Top = 268
          Width = 77
          Height = 18
          Caption = 'Trade Code'
        end
        object lbOutletNumber: TLabel
          Left = 16
          Top = 326
          Width = 96
          Height = 18
          Caption = 'Outlet Number'
        end
        object lbMallType: TLabel
          Left = 16
          Top = 36
          Width = 63
          Height = 18
          Caption = 'Mall Type'
        end
        object lbBranchCode: TLabel
          Left = 15
          Top = 384
          Width = 5
          Height = 18
        end
        object lbTerminalNumber: TLabel
          Left = 14
          Top = 442
          Width = 113
          Height = 18
          Caption = 'Terminal Number'
        end
        object lbSerialNumber: TLabel
          Left = 15
          Top = 500
          Width = 90
          Height = 18
          Caption = 'Serial Number'
        end
        object lbFTPPath: TLabel
          Left = 392
          Top = 151
          Width = 60
          Height = 18
          Caption = 'FTP Path'
        end
        object lbFTPUserName: TLabel
          Left = 392
          Top = 208
          Width = 105
          Height = 18
          Caption = 'FTP User Name'
        end
        object lbFTPPassword: TLabel
          Left = 392
          Top = 267
          Width = 92
          Height = 18
          Caption = 'FTP Password'
        end
        object lbFTPServer: TLabel
          Left = 392
          Top = 95
          Width = 73
          Height = 18
          Caption = 'FTP Server'
        end
        object lbConsolidated: TLabel
          Left = 392
          Top = 383
          Width = 146
          Height = 18
          Caption = 'Consolidated DB Paths'
        end
      end
      object cbMallLoc: TComboBox
        Left = 32
        Top = 64
        Width = 329
        Height = 26
        Style = csDropDownList
        TabOrder = 11
        OnChange = cbMallLocChange
      end
      object edTenantNo: TEdit
        Left = 31
        Top = 122
        Width = 330
        Height = 26
        TabOrder = 1
        OnMouseUp = edTenantNoMouseUp
      end
      object edMallPath: TEdit
        Left = 32
        Top = 180
        Width = 331
        Height = 26
        TabOrder = 2
        OnMouseUp = edMallPathMouseUp
      end
      object edClassCode: TEdit
        Left = 31
        Top = 238
        Width = 331
        Height = 26
        TabOrder = 3
        OnMouseUp = edClassCodeMouseUp
      end
      object edTradeCode: TEdit
        Left = 32
        Top = 296
        Width = 329
        Height = 26
        TabOrder = 4
        OnMouseUp = edTradeCodeMouseUp
      end
      object edOutletCode: TEdit
        Left = 32
        Top = 354
        Width = 329
        Height = 26
        TabOrder = 5
        OnMouseUp = edOutletCodeMouseUp
      end
      object edBranchCode: TEdit
        Left = 32
        Top = 412
        Width = 329
        Height = 26
        TabOrder = 6
        OnMouseUp = edBranchCodeMouseUp
      end
      object edTerminalNo: TEdit
        Left = 32
        Top = 470
        Width = 329
        Height = 26
        TabOrder = 7
        OnMouseUp = edTerminalNoMouseUp
      end
      object edSerialNo: TEdit
        Left = 32
        Top = 528
        Width = 329
        Height = 26
        TabOrder = 8
        OnMouseUp = edSerialNoMouseUp
      end
      object btnResendReport: TTouchBtn
        Left = 408
        Top = 33
        Width = 155
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'Resend Reports'
        ButtonColor = clMaroon
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnResendReportMouseClick
      end
      object btnRegenReport: TTouchBtn
        Left = 580
        Top = 33
        Width = 155
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'Regenerate Reports'
        ButtonColor = clMaroon
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnRegenerateReportMouseClick
      end
      object btnAssignSalesType: TTouchBtn
        Left = 32
        Top = 569
        Width = 329
        Height = 30
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentColor = True
        Caption = 'Assign Sales Type'
        ButtonColor = clMaroon
        LatchedColor = clBtnFace
        DisabledButtonColor = clBtnFace
        Enabled = False
        LatchingProperties = [lkLatchColor, lkStayDown]
        OnMouseClick = btnAssignSalesTypeMouseClick
      end
      object edFTPServer: TEdit
        Left = 408
        Top = 122
        Width = 330
        Height = 26
        TabOrder = 12
        OnMouseUp = edFTPServerMouseUp
      end
      object edFTPPath: TEdit
        Left = 408
        Top = 179
        Width = 330
        Height = 26
        TabOrder = 13
        OnMouseUp = edFTPPathMouseUp
      end
      object edFTPUserName: TEdit
        Left = 408
        Top = 235
        Width = 330
        Height = 26
        TabOrder = 14
        OnMouseUp = edFTPUserNameMouseUp
      end
      object edFTPPassword: TEdit
        Left = 408
        Top = 294
        Width = 330
        Height = 26
        TabOrder = 18
        OnMouseUp = edFTPPasswordMouseUp
      end
      object cbEnableConsolidatedRep: TCheckBox
        Left = 408
        Top = 355
        Width = 289
        Height = 20
        Caption = 'Enable Consolidated Report'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 17
        OnClick = cbEnableConsolidatedRepClick
      end
      object edConsolidatedDBPaths: TEdit
        Left = 408
        Top = 412
        Width = 330
        Height = 26
        TabOrder = 15
        OnMouseUp = edConsolidatedDBPathsMouseUp
      end
    end
    object tsMallSettings: TTabSheet
      Caption = 'Mall Export Settings'
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object gbMalls1: TGroupBox
        Left = 16
        Top = 3
        Width = 737
        Height = 620
        Caption = 'Mall'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        object lbTenantNo1: TLabel
          Left = 16
          Top = 94
          Width = 5
          Height = 18
        end
        object lbFileLocation1: TLabel
          Left = 16
          Top = 152
          Width = 78
          Height = 18
          Caption = 'File Location'
        end
        object lbClassCode1: TLabel
          Left = 16
          Top = 210
          Width = 71
          Height = 18
          Caption = 'Class Code'
        end
        object lbTradeCode1: TLabel
          Left = 16
          Top = 268
          Width = 77
          Height = 18
          Caption = 'Trade Code'
        end
        object lbOutletNumber1: TLabel
          Left = 16
          Top = 326
          Width = 96
          Height = 18
          Caption = 'Outlet Number'
        end
        object lbMallType1: TLabel
          Left = 16
          Top = 36
          Width = 63
          Height = 18
          Caption = 'Mall Type'
        end
        object lbTenantCode1: TLabel
          Left = 14
          Top = 94
          Width = 85
          Height = 18
          Caption = 'Tenant Code'
        end
        object lbBranchCode1: TLabel
          Left = 15
          Top = 384
          Width = 5
          Height = 18
        end
        object lbTerminalNumber1: TLabel
          Left = 14
          Top = 442
          Width = 113
          Height = 18
          Caption = 'Terminal Number'
        end
        object lbSerialNumber1: TLabel
          Left = 15
          Top = 500
          Width = 90
          Height = 18
          Caption = 'Serial Number'
        end
        object lbFTPPath1: TLabel
          Left = 392
          Top = 151
          Width = 60
          Height = 18
          Caption = 'FTP Path'
        end
        object lbFTPUserName1: TLabel
          Left = 392
          Top = 208
          Width = 105
          Height = 18
          Caption = 'FTP User Name'
        end
        object lbFTPPassword1: TLabel
          Left = 392
          Top = 267
          Width = 92
          Height = 18
          Caption = 'FTP Password'
        end
        object lbFTPServer1: TLabel
          Left = 392
          Top = 95
          Width = 73
          Height = 18
          Caption = 'FTP Server'
        end
        object lbConsolidated1: TLabel
          Left = 392
          Top = 383
          Width = 146
          Height = 18
          Caption = 'Consolidated DB Paths'
        end
        object cbMallLoc1: TComboBox
          Left = 16
          Top = 64
          Width = 329
          Height = 26
          Style = csDropDownList
          TabOrder = 0
          OnChange = cbMallLoc1Change
        end
        object edTenantNo1: TEdit
          Left = 16
          Top = 122
          Width = 330
          Height = 26
          Color = clInactiveCaptionText
          Enabled = False
          TabOrder = 1
          OnMouseUp = edTenantNo1MouseUp
        end
        object edMallPath1: TEdit
          Left = 16
          Top = 180
          Width = 331
          Height = 26
          Color = clInactiveCaptionText
          Enabled = False
          TabOrder = 2
          OnMouseUp = edMallPath1MouseUp
        end
        object edClassCode1: TEdit
          Left = 16
          Top = 238
          Width = 331
          Height = 26
          Color = clInactiveCaptionText
          Enabled = False
          TabOrder = 3
          OnMouseUp = edClassCode1MouseUp
        end
        object edTradeCode1: TEdit
          Left = 16
          Top = 296
          Width = 329
          Height = 26
          Color = clInactiveCaptionText
          Enabled = False
          TabOrder = 4
          OnMouseUp = edTradeCode1MouseUp
        end
        object edOutletCode1: TEdit
          Left = 16
          Top = 354
          Width = 329
          Height = 26
          Color = clInactiveCaptionText
          Enabled = False
          TabOrder = 5
          OnMouseUp = edOutletCode1MouseUp
        end
        object edBranchCode1: TEdit
          Left = 17
          Top = 410
          Width = 329
          Height = 26
          Color = clInactiveCaptionText
          Enabled = False
          TabOrder = 6
          OnMouseUp = edBranchCode1MouseUp
        end
        object edTerminalNo1: TEdit
          Left = 16
          Top = 470
          Width = 329
          Height = 26
          Color = clInactiveCaptionText
          Enabled = False
          TabOrder = 7
          OnMouseUp = edTerminalNo1MouseUp
        end
        object edSerialNo1: TEdit
          Left = 16
          Top = 528
          Width = 329
          Height = 26
          Color = clInactiveCaptionText
          Enabled = False
          TabOrder = 8
          OnMouseUp = edSerialNo1MouseUp
        end
        object btnResendReport1: TTouchBtn
          Left = 408
          Top = 33
          Width = 155
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Resend Reports'
          ButtonColor = clMaroon
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnResendReport1MouseClick
        end
        object btnRegenReport1: TTouchBtn
          Left = 580
          Top = 33
          Width = 155
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Regenerate Reports'
          ButtonColor = clMaroon
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnRegenerateReport1MouseClick
        end
        object btnAssignSalesType1: TTouchBtn
          Left = 32
          Top = 569
          Width = 329
          Height = 30
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ParentColor = True
          Caption = 'Assign Sales Type'
          ButtonColor = clMaroon
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          Enabled = False
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnAssignSalesType1MouseClick
        end
        object edFTPServer1: TEdit
          Left = 392
          Top = 122
          Width = 330
          Height = 26
          Color = clInactiveCaptionText
          TabOrder = 12
        end
        object edFTPPath1: TEdit
          Left = 392
          Top = 179
          Width = 330
          Height = 26
          Color = clInactiveCaptionText
          TabOrder = 13
        end
        object edFTPUserName1: TEdit
          Left = 392
          Top = 235
          Width = 330
          Height = 26
          Color = clInactiveCaptionText
          TabOrder = 14
        end
        object edFTPPassword1: TEdit
          Left = 392
          Top = 294
          Width = 330
          Height = 26
          Color = clInactiveCaptionText
          TabOrder = 16
        end
        object cbEnableConsolidatedRep1: TCheckBox
          Left = 392
          Top = 355
          Width = 289
          Height = 20
          Caption = 'Enable Consolidated Report'
          Color = clInactiveCaptionText
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
          TabOrder = 17
        end
        object edConsolidatedDBPaths1: TEdit
          Left = 392
          Top = 412
          Width = 330
          Height = 26
          Color = clInactiveCaptionText
          Enabled = False
          TabOrder = 15
        end
      end
    end
  end
  object pnlButtons: TPanel
    Left = 918
    Top = 8
    Width = 112
    Height = 719
    Anchors = [akTop, akRight, akBottom]
    BevelOuter = bvNone
    BorderWidth = 5
    UseDockManager = False
    TabOrder = 1
    object btnClose: TTouchBtn
      Left = 6
      Top = 18
      Width = 100
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Close'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnAutoRepeat = imgCloseClick
    end
  end
end
