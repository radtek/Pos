object frmMain: TfrmMain
  Left = 284
  Top = 156
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Registration'
  ClientHeight = 360
  ClientWidth = 591
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pcRegistration: TPageControl
    Left = 8
    Top = 8
    Width = 577
    Height = 345
    ActivePage = TabSheet1
    TabOrder = 0
    OnChange = pcRegistrationChange
    OnChanging = pcRegistrationChanging
    object tsConnection: TTabSheet
      Caption = 'Connection'
      ImageIndex = 3
      object Label4: TLabel
        Left = 8
        Top = 192
        Width = 69
        Height = 13
        Caption = 'Connecting to:'
      end
      object GroupBox2: TGroupBox
        Left = 8
        Top = 48
        Width = 409
        Height = 73
        TabOrder = 0
        object Label3: TLabel
          Left = 16
          Top = 32
          Width = 75
          Height = 13
          Caption = 'Company Name'
        end
        object cbOfficeConnections: TComboBox
          Left = 120
          Top = 28
          Width = 273
          Height = 21
          Style = csDropDownList
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          Sorted = True
          TabOrder = 0
          OnChange = cbOfficeConnectionsChange
        end
      end
      object edConnectionName: TEdit
        Left = 96
        Top = 188
        Width = 321
        Height = 21
        ParentColor = True
        ReadOnly = True
        TabOrder = 1
      end
      object rbUseMenuMateSettings: TRadioButton
        Left = 16
        Top = 24
        Width = 161
        Height = 17
        Caption = 'Use MenuMate Settings'
        Checked = True
        TabOrder = 2
        TabStop = True
        OnClick = rbUseMenuMateSettingsClick
      end
      object rbUseOfficeSettings: TRadioButton
        Left = 16
        Top = 47
        Width = 113
        Height = 17
        Caption = 'Use Office Settings'
        TabOrder = 3
        OnClick = rbUseOfficeSettingsClick
      end
    end
    object tsInformation: TTabSheet
      Caption = 'Information'
      DesignSize = (
        569
        317)
      object Panel5: TPanel
        Left = 304
        Top = 8
        Width = 257
        Height = 297
        Anchors = [akLeft, akTop, akBottom]
        TabOrder = 0
        object lbFaxInfo4: TLabel
          Left = 112
          Top = 32
          Width = 75
          Height = 13
          Caption = '+64 3 343 1202'
        end
        object lbFaxInfo3: TLabel
          Left = 16
          Top = 32
          Width = 31
          Height = 13
          Caption = 'Phone'
        end
        object lbFaxInfo5: TLabel
          Left = 16
          Top = 64
          Width = 17
          Height = 13
          Caption = 'Fax'
        end
        object lbFaxInfo6: TLabel
          Left = 112
          Top = 64
          Width = 75
          Height = 13
          Caption = '+64 3 343 1118'
        end
        object lbFaxInfo8: TLabel
          Left = 112
          Top = 96
          Width = 72
          Height = 13
          Caption = '2B Michelle Rd'
        end
        object lbFaxInfo7: TLabel
          Left = 16
          Top = 96
          Width = 41
          Height = 13
          Caption = 'Location'
        end
        object lbFaxInfo9: TLabel
          Left = 112
          Top = 112
          Width = 46
          Height = 13
          Caption = 'Sockburn'
        end
        object lbFaxInfo10: TLabel
          Left = 112
          Top = 128
          Width = 59
          Height = 13
          Caption = 'Christchurch'
        end
        object lbFaxInfo11: TLabel
          Left = 112
          Top = 144
          Width = 64
          Height = 13
          Caption = 'New Zealand'
        end
        object lbFaxInfo13: TLabel
          Left = 112
          Top = 176
          Width = 66
          Height = 13
          Caption = 'P.O Box 7472'
        end
        object lbFaxInfo12: TLabel
          Left = 16
          Top = 176
          Width = 70
          Height = 13
          Caption = 'Postal Address'
        end
        object lbFaxInfo14: TLabel
          Left = 112
          Top = 192
          Width = 50
          Height = 13
          Caption = 'Sydenham'
        end
        object lbFaxInfo16: TLabel
          Left = 112
          Top = 256
          Width = 127
          Height = 13
          Caption = 'registration@iqworks.co.nz'
        end
        object lbFaxInfo15: TLabel
          Left = 16
          Top = 256
          Width = 28
          Height = 13
          Caption = 'E-mail'
        end
        object Label14: TLabel
          Left = 8
          Top = 8
          Width = 136
          Height = 13
          Caption = 'Registration Information'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label15: TLabel
          Left = 112
          Top = 208
          Width = 59
          Height = 13
          Caption = 'Christchurch'
        end
        object Label16: TLabel
          Left = 112
          Top = 224
          Width = 64
          Height = 13
          Caption = 'New Zealand'
        end
      end
      object Panel1: TPanel
        Left = 8
        Top = 8
        Width = 289
        Height = 297
        Anchors = [akLeft, akTop, akBottom]
        TabOrder = 1
        object Label46: TLabel
          Left = 16
          Top = 32
          Width = 75
          Height = 13
          Caption = 'Company Name'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label9: TLabel
          Left = 16
          Top = 104
          Width = 94
          Height = 14
          Caption = 'Total Palm Licences'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Label44: TLabel
          Left = 16
          Top = 80
          Width = 93
          Height = 14
          Caption = 'Total POS Licences'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Label12: TLabel
          Left = 8
          Top = 56
          Width = 112
          Height = 13
          Caption = 'Product Information'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label13: TLabel
          Left = 8
          Top = 8
          Width = 120
          Height = 13
          Caption = 'Customer Information'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label6: TLabel
          Left = 16
          Top = 128
          Width = 102
          Height = 14
          Caption = 'Total Office Licences'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbCustomerName: TComboBox
          Left = 128
          Top = 28
          Width = 145
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          Sorted = True
          TabOrder = 0
          OnChange = cbCustomerNameChange
        end
        object edPalmLicences: TEdit
          Left = 128
          Top = 100
          Width = 145
          Height = 21
          AutoSize = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          OnChange = edPalmLicencesChange
        end
        object edPOSLicences: TEdit
          Left = 128
          Top = 76
          Width = 145
          Height = 21
          AutoSize = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnChange = edPOSLicencesChange
        end
        object edOffice: TEdit
          Left = 128
          Top = 124
          Width = 145
          Height = 21
          AutoSize = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          OnChange = edOfficeChange
        end
      end
    end
    object TabSheet1: TTabSheet
      Caption = 'Modules'
      ImageIndex = 4
      DesignSize = (
        569
        317)
      object Panel3: TPanel
        Left = 8
        Top = 8
        Width = 289
        Height = 297
        Anchors = [akLeft, akTop, akBottom]
        TabOrder = 0
        object Label22: TLabel
          Left = 8
          Top = 8
          Width = 109
          Height = 13
          Caption = 'Module Information'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object chbKitchenModule: TCheckBox
          Left = 8
          Top = 129
          Width = 270
          Height = 17
          Alignment = taLeftJustify
          Caption = 'ChefMate'
          TabOrder = 0
          OnClick = chbKitchenModuleClick
        end
        object chbRoomsModule: TCheckBox
          Left = 8
          Top = 181
          Width = 270
          Height = 17
          Alignment = taLeftJustify
          Caption = 'Rooms'
          TabOrder = 1
          OnClick = cbRoomsClick
        end
        object chbTimeTracking: TCheckBox
          Left = 8
          Top = 104
          Width = 270
          Height = 17
          Alignment = taLeftJustify
          Caption = 'Time Tracking'
          TabOrder = 2
          OnClick = chbTimeTrackingClick
        end
        object cbMembership: TCheckBox
          Left = 8
          Top = 52
          Width = 270
          Height = 17
          Alignment = taLeftJustify
          Caption = 'Loyalty'
          TabOrder = 3
          OnClick = cbMembershipClick
        end
        object cbMYOB: TCheckBox
          Left = 8
          Top = 78
          Width = 270
          Height = 17
          Alignment = taLeftJustify
          Caption = 'Accounting Interface'
          TabOrder = 4
          OnClick = cbMYOBClick
        end
        object cbIntaMate: TCheckBox
          Left = 8
          Top = 207
          Width = 270
          Height = 17
          Alignment = taLeftJustify
          Caption = 'IntaMate'
          TabOrder = 5
          OnClick = cbIntaMateClick
        end
        object cbIntergratedEftpos: TCheckBox
          Left = 8
          Top = 27
          Width = 270
          Height = 17
          Alignment = taLeftJustify
          Caption = 'Eftpos'
          TabOrder = 6
          OnClick = cbIntergratedEftposClick
        end
        object cbPhoenixHS: TCheckBox
          Left = 8
          Top = 155
          Width = 270
          Height = 17
          Alignment = taLeftJustify
          Caption = 'Property Managment'
          TabOrder = 7
          OnClick = cbPhoenixHSClick
        end
      end
      object Panel4: TPanel
        Left = 304
        Top = 8
        Width = 257
        Height = 297
        Anchors = [akLeft, akTop, akBottom]
        TabOrder = 1
        object Label23: TLabel
          Left = 112
          Top = 32
          Width = 66
          Height = 13
          Caption = '0800 657 300'
        end
        object Label24: TLabel
          Left = 16
          Top = 32
          Width = 31
          Height = 13
          Caption = 'Phone'
        end
        object Label25: TLabel
          Left = 16
          Top = 64
          Width = 17
          Height = 13
          Caption = 'Fax'
        end
        object Label26: TLabel
          Left = 112
          Top = 64
          Width = 72
          Height = 13
          Caption = '+64 3 3657302'
        end
        object Label27: TLabel
          Left = 112
          Top = 96
          Width = 59
          Height = 13
          Caption = '25 Carlyle St'
        end
        object Label28: TLabel
          Left = 16
          Top = 96
          Width = 41
          Height = 13
          Caption = 'Location'
        end
        object Label30: TLabel
          Left = 112
          Top = 112
          Width = 59
          Height = 13
          Caption = 'Christchurch'
        end
        object Label31: TLabel
          Left = 112
          Top = 128
          Width = 64
          Height = 13
          Caption = 'New Zealand'
        end
        object Label35: TLabel
          Left = 112
          Top = 256
          Width = 127
          Height = 13
          Caption = 'registration@iqworks.co.nz'
        end
        object Label36: TLabel
          Left = 16
          Top = 256
          Width = 28
          Height = 13
          Caption = 'E-mail'
        end
        object Label37: TLabel
          Left = 8
          Top = 8
          Width = 136
          Height = 13
          Caption = 'Registration Information'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
      end
    end
    object tsIndividualLicences: TTabSheet
      Caption = 'Individual Licences'
      ImageIndex = 1
      DesignSize = (
        569
        317)
      object tvLicences: TTreeView
        Left = 8
        Top = 8
        Width = 433
        Height = 297
        Anchors = [akLeft, akTop, akBottom]
        Images = imlTreeImages
        Indent = 19
        ReadOnly = True
        TabOrder = 0
        OnDblClick = tvLicencesDblClick
        OnDeletion = tvLicencesDeletion
      end
      object btnUnregister: TButton
        Left = 448
        Top = 272
        Width = 113
        Height = 33
        Anchors = [akLeft, akBottom]
        Caption = 'Unregister'
        TabOrder = 3
        OnClick = btnUnregisterClick
      end
      object btnRemoveKey: TButton
        Left = 448
        Top = 232
        Width = 113
        Height = 33
        Anchors = [akLeft, akBottom]
        Caption = 'Remove Key'
        TabOrder = 2
        OnClick = btnRemoveKeyClick
      end
      object btnEnterKey: TButton
        Left = 448
        Top = 8
        Width = 113
        Height = 33
        Anchors = [akLeft, akBottom]
        Caption = 'Enter Key'
        TabOrder = 1
        OnClick = btnEnterKeyClick
      end
    end
    object tsMultiLicences: TTabSheet
      Caption = 'Multiple Licences'
      ImageIndex = 2
      DesignSize = (
        569
        317)
      object Label1: TLabel
        Left = 8
        Top = 160
        Width = 82
        Height = 13
        Caption = 'Registration Keys'
      end
      object Label2: TLabel
        Left = 8
        Top = 8
        Width = 70
        Height = 13
        Caption = 'Customer Keys'
      end
      object memCustomerKeys: TMemo
        Left = 8
        Top = 24
        Width = 425
        Height = 129
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Courier New'
        Font.Style = []
        ParentColor = True
        ParentFont = False
        ReadOnly = True
        ScrollBars = ssVertical
        TabOrder = 0
        WordWrap = False
      end
      object memRegKeys: TMemo
        Left = 8
        Top = 176
        Width = 425
        Height = 129
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Courier New'
        Font.Style = []
        ParentFont = False
        ScrollBars = ssVertical
        TabOrder = 1
      end
      object Panel2: TPanel
        Left = 440
        Top = 24
        Width = 121
        Height = 281
        Anchors = [akLeft, akTop, akBottom]
        BevelOuter = bvLowered
        TabOrder = 2
        DesignSize = (
          121
          281)
        object btnSave: TButton
          Left = 8
          Top = 8
          Width = 105
          Height = 33
          Caption = 'Save To File'
          TabOrder = 0
          OnClick = btnSaveClick
        end
        object btnPrint: TButton
          Left = 8
          Top = 48
          Width = 105
          Height = 33
          Caption = 'Print'
          TabOrder = 1
          OnClick = btnPrintClick
        end
        object btnRefresh: TBitBtn
          Left = 8
          Top = 96
          Width = 105
          Height = 33
          Caption = 'Refresh'
          DoubleBuffered = True
          ParentDoubleBuffered = False
          TabOrder = 2
          Visible = False
          OnClick = btnRefreshClick
        end
        object btnApply: TBitBtn
          Left = 8
          Top = 240
          Width = 105
          Height = 33
          Anchors = [akLeft, akBottom]
          Caption = 'Apply'
          DoubleBuffered = True
          ParentDoubleBuffered = False
          TabOrder = 3
          OnClick = btnApplyClick
        end
      end
    end
  end
  object PrintDialog: TPrintDialog
    Left = 704
    Top = 144
  end
  object imlTreeImages: TImageList
    Left = 640
    Top = 32
    Bitmap = {
      494C010103000400180010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000001000000001002000000000000010
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000505050006060
      6000606060002020200000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000004040400060606000808080008080800080808000606060004040
      4000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000AFAFAF00DFDFDF00DFDFDF00DFDF
      DF00DFDFDF00DFDFDF00DFDFDF00DFDFDF00DFDFDF00DFDFDF0090909000BFBF
      BF00BFBFBF009090900060606000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000002020
      20002020600020209F000000BF000000BF000000BF0020209F00404080008080
      8000606060002020200000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000020202000BFBFBF00404040007F7F
      7F007F7F7F00404040007F7F7F007F7F7F00404040007F7F7F007F7F7F000000
      000000000000000000009F9F9F00404040000000000000000000000000000000
      00009090900080808000B0B0B000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000020002020
      9F000000FF000000FF000000FF000000FF000000FF000000FF000000FF002020
      9F00606080008080800020202000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000040404000AFAFAF008F8F
      8F0070707000BFBFBF009F9F9F0080808000BFBFBF009F9F9F00808080009090
      9000AFAFAF00AFAFAF009F9F9F00707070000000000000000000000000003090
      3000008000006080600080808000B0B0B0000000000000000000000000000000
      00000000000000000000000000000000000000000000000020000000DF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000DF006060800060606000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000040404000DFDFDF0090B0
      900090B09000C0C0C000C0C0C000808080004040400040404000404040004040
      400070707000909090008080800020202000000000000000000030CF30000080
      000000800000008000006080600080808000B0B0B00000000000000000000000
      0000000000000000000000000000000000000000000000007F000000FF000000
      FF003F3FFF000000FF000000FF000000FF000000FF000000FF003F3FFF000000
      FF000000FF0020209F0080808000404040000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000040404000EFEFEF00DFDF
      DF00DFDFDF00DFDFDF00DFDFDF00DFDFDF00DFDFDF00DFDFDF00DFDFDF00DFDF
      DF00DFDFDF00AFAFAF008080800000000000000000000000000000BF00000080
      00000080000000800000008000006080600080808000B0B0B000000000000000
      000000000000000000000000000000000000000040000000FF000000FF003F3F
      FF0000000000BFBFFF000000FF000000FF000000FF00BFBFFF00000000003F3F
      FF000000FF000000FF0040408000606060000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000404040004040
      4000404040004040400040404000404040004040400040404000404040004040
      400040404000909090009090900000000000000000000000000000BF00000080
      0000008000000080000000800000008000006080600090909000000000000000
      00000000000000000000000000000000000000007F000000FF000000FF000000
      FF00BFBFFF0000000000BFBFFF000000FF00BFBFFF0000000000BFBFFF000000
      FF000000FF000000FF0020209F00808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000009F9F
      9F00A0A0A000A0A0A000A0A0A000A0A0A000A0A0A000A0A0A000A0A0A000A0A0
      A00090909000404040009090900000000000000000000000000000BF00000080
      00000080000020DF2000009F0000008000002080200080808000A0A0A0000000
      0000000000000000000000000000000000000000BF000000FF000000FF000000
      FF000000FF00BFBFFF00000000000000000000000000BFBFFF000000FF000000
      FF000000FF000000FF000000BF00808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF00BF9F9F00BF7F7F00BF7F7F00BF7F7F00BF7F7F00BF7F7F00BF7F7F00DFBF
      BF00A0A0A000404040000000000000000000000000000000000090CF900000BF
      0000008000000000000030EF3000009F0000008000002080200080808000B0B0
      B000000000000000000000000000000000000000BF000000FF000000FF000000
      FF000000FF000000FF000000000000000000000000000000FF000000FF000000
      FF000000FF000000FF000000BF00808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF0060606000FF000000FF000000FF000000FF000000FF000000FF000000BF7F
      7F00A0A0A00040404000000000000000000000000000000000000000000090CF
      900000DF0000000000000000000030EF3000009F000000800000608060008080
      8000B0B0B0000000000000000000000000000000BF000000FF000000FF000000
      FF000000FF00BFBFFF00000000000000000000000000BFBFFF000000FF000000
      FF000000FF000000FF000000BF00606060000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF0060606000FF000000FF000000FF000000FF000000FF000000FF000000BF7F
      7F00A0A0A0004040400000000000000000000000000000000000000000000000
      00000000000000000000000000000000000030EF3000009F0000008000006080
      60009090900000000000000000000000000000007F000000FF000000FF000000
      FF00BFBFFF0000000000BFBFFF000000FF00BFBFFF0000000000BFBFFF000000
      FF000000FF000000FF0020209F00404040000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF0060606000FF3F0000FF000000FF000000FF000000FF000000FF000000BF7F
      7F00A0A0A0004040400000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000030EF3000009F00002080
      200080808000A0A0A0000000000000000000000040000000FF000000FF003F3F
      FF0000000000BFBFFF000000FF000000FF000000FF00BFBFFF00000000003F3F
      FF000000FF000000FF0020206000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF0060606000FF3F0000FF000000FF000000FF000000FF000000FF000000BF7F
      7F00A0A0A0004040400000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000030EF3000009F
      0000208020008080800000000000000000000000000000007F000000FF000000
      FF003F3FFF000000FF000000FF000000FF000000FF000000FF003F3FFF000000
      FF000000FF0020209F0020202000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF0080808000404040004040400040404000404040004040400040404000BF9F
      9F00A0A0A0004040400000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000030EF
      3000009F000060806000000000000000000000000000000020000000DF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000DF000000200000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000007F7F
      7F00DFDFDF00DFDFDF00DFDFDF00DFDFDF00DFDFDF00DFDFDF00DFDFDF00DFDF
      DF00AFAFAF004040400000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000060DF600090B0900000000000000000000000000000000000000020000000
      7F000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      7F00000020000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000070707000A0A0A000A0A0A000A0A0A000A0A0A000A0A0A000A0A0A000A0A0
      A000AFAFAF004040400000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000400000007F000000BF000000BF000000BF0000007F00000040000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000100000000100010000000000800000000000000000000000
      000000000000000000000000FFFFFF000001FFFFF80F00000000FFFFE0030000
      001CF1FFC00100008000E0FF800100008000C07F800000008000C03F08200000
      C000C03F04400000E000C01F03800000E003C40F03800000E003E60703800000
      E003FF0704400000E003FF8308210000E003FFC380010000E003FFE380030000
      E003FFF3C0070000F003FFFFF01F000000000000000000000000000000000000
      000000000000}
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'txt'
    FileName = 'MMReg.txt'
    Filter = 'Al Files (*.*)|*.*|Text Files (*.txt)|*.txt'
    Left = 704
    Top = 32
  end
end
