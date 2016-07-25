object DlADOConfigureForm: TDlADOConfigureForm
  Left = 353
  Top = 202
  Width = 396
  Height = 276
  BorderIcons = [biSystemMenu]
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
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 388
    Height = 208
    ActivePage = TabSheet2
    Align = alClient
    TabOrder = 0
    object TabSheet2: TTabSheet
      Caption = 'Connection'
      ImageIndex = 1
      object Label1: TLabel
        Left = 24
        Top = 120
        Width = 95
        Height = 13
        Caption = 'Connection Timeout'
      end
      object Label2: TLabel
        Left = 208
        Top = 120
        Width = 88
        Height = 13
        Caption = 'Command Timeout'
      end
      object radoDataLinkFile: TRadioButton
        Tag = 2203
        Left = 2
        Top = 13
        Width = 226
        Height = 20
        Caption = 'Use Data &Link File'
        Checked = True
        TabOrder = 0
        TabStop = True
        OnClick = radoParamsClick
      end
      object cmboDataLinkFile: TComboBox
        Tag = 2206
        Left = 20
        Top = 34
        Width = 332
        Height = 21
        ItemHeight = 13
        Sorted = True
        TabOrder = 1
      end
      object radoConnectionString: TRadioButton
        Tag = 2202
        Left = 2
        Top = 65
        Width = 226
        Height = 20
        Caption = 'Use &Connection String'
        TabOrder = 2
        OnClick = radoParamsClick
      end
      object editConnectionString: TEdit
        Tag = 2205
        Left = 20
        Top = 86
        Width = 332
        Height = 21
        TabOrder = 3
      end
      object butnBrowseDataLinkFile: TButton
        Left = 352
        Top = 35
        Width = 19
        Height = 19
        Caption = '...'
        TabOrder = 4
        OnClick = butnBrowseDataLinkFileClick
      end
      object butnBuildConnectionString: TButton
        Left = 352
        Top = 87
        Width = 19
        Height = 19
        Caption = '...'
        TabOrder = 5
        OnClick = butnBuildConnectionStringClick
      end
      object editConnectionTimeout: TEdit
        Left = 24
        Top = 136
        Width = 161
        Height = 21
        TabOrder = 6
        Text = '15'
      end
      object editCommandTimeout: TEdit
        Left = 208
        Top = 136
        Width = 161
        Height = 21
        TabOrder = 7
        Text = '30'
      end
    end
    object TabSheet1: TTabSheet
      Caption = 'Test'
      ImageIndex = 1
      object Label6: TLabel
        Left = 8
        Top = 48
        Width = 46
        Height = 13
        Caption = 'Table List'
      end
      object Button1: TButton
        Left = 104
        Top = 16
        Width = 137
        Height = 25
        Caption = 'Test Connection'
        TabOrder = 0
        OnClick = Button1Click
      end
      object lboxTables: TListBox
        Left = 8
        Top = 64
        Width = 361
        Height = 108
        ItemHeight = 13
        TabOrder = 1
      end
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 208
    Width = 388
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    DesignSize = (
      388
      41)
    object butnOk: TButton
      Left = 224
      Top = 8
      Width = 75
      Height = 25
      Action = actnOk
      Anchors = [akTop, akRight]
      Default = True
      ModalResult = 1
      TabOrder = 0
    end
    object butnCancel: TButton
      Left = 304
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Cancel = True
      Caption = '&Cancel'
      ModalResult = 2
      TabOrder = 1
    end
  end
  object ActionList1: TActionList
    Left = 80
    Top = 200
    object actnOk: TAction
      Caption = '&Ok'
      OnUpdate = actnOkUpdate
    end
  end
end
