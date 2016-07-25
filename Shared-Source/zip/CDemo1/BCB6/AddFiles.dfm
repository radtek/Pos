object AddFile: TAddFile
  Left = 315
  Top = 188
  Width = 617
  Height = 441
  Caption = 'Add Files'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -10
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 173
    Top = 0
    Width = 436
    Height = 407
    Align = alClient
    TabOrder = 0
    object Panel5: TPanel
      Left = 1
      Top = 1
      Width = 104
      Height = 405
      Align = alLeft
      TabOrder = 0
      object Bevel1: TBevel
        Left = 3
        Top = 27
        Width = 99
        Height = 142
      end
      object Label2: TLabel
        Left = 28
        Top = 8
        Width = 44
        Height = 13
        Caption = 'Options'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object AddFileBut: TButton
        Left = 11
        Top = 192
        Width = 85
        Height = 21
        Caption = '&Add File ==>'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnClick = AddFileButClick
      end
      object RemoveBut: TButton
        Left = 11
        Top = 266
        Width = 85
        Height = 21
        Caption = '&Remove <=='
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
        OnClick = RemoveButClick
      end
      object OKBut: TButton
        Left = 11
        Top = 316
        Width = 85
        Height = 21
        Caption = 'START &NOW'
        Default = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 3
        OnClick = OKButClick
      end
      object CancelBut: TButton
        Left = 11
        Top = 349
        Width = 85
        Height = 21
        Cancel = True
        Caption = '&Cancel'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
        OnClick = CancelButClick
      end
      object DirNameCB: TCheckBox
        Left = 6
        Top = 32
        Width = 95
        Height = 14
        Caption = 'Save DirNames'
        TabOrder = 5
      end
      object RecurseCB: TCheckBox
        Left = 6
        Top = 65
        Width = 91
        Height = 14
        Caption = 'Recurse Dirs'
        TabOrder = 6
      end
      object AddDirBut: TButton
        Left = 11
        Top = 222
        Width = 85
        Height = 21
        Caption = 'Add &Dir  ==>'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnClick = AddDirButClick
      end
      object EncryptCB: TCheckBox
        Left = 6
        Top = 48
        Width = 87
        Height = 17
        Caption = 'Encrypt Files'
        TabOrder = 7
      end
      object DiskSpanCB: TCheckBox
        Left = 6
        Top = 77
        Width = 75
        Height = 17
        Caption = 'Span disk'
        TabOrder = 8
        OnClick = DiskSpanCBClick
      end
      object VolSizeBut: TButton
        Left = 6
        Top = 120
        Width = 93
        Height = 21
        Caption = 'Set max. vol. size'
        Enabled = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 9
        OnClick = VolSizeButClick
      end
      object FreeDisk1But: TButton
        Left = 6
        Top = 144
        Width = 93
        Height = 21
        Caption = 'Set free on disk 1'
        Enabled = False
        TabOrder = 10
        OnClick = FreeDisk1ButClick
      end
      object AddBtn: TButton
        Left = 6
        Top = 97
        Width = 93
        Height = 21
        Caption = 'Action: Add'
        TabOrder = 11
        OnClick = AddBtnClick
      end
    end
    object Panel6: TPanel
      Left = 105
      Top = 1
      Width = 330
      Height = 405
      Align = alClient
      TabOrder = 1
      object SelectedList: TListBox
        Left = 1
        Top = 27
        Width = 328
        Height = 349
        TabStop = False
        Align = alClient
        ItemHeight = 13
        MultiSelect = True
        TabOrder = 0
      end
      object Panel7: TPanel
        Left = 1
        Top = 1
        Width = 328
        Height = 26
        Align = alTop
        TabOrder = 1
        object Label1: TLabel
          Left = 53
          Top = 7
          Width = 72
          Height = 13
          Caption = 'Files to Add:'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -12
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object SortBut: TButton
          Left = 150
          Top = 6
          Width = 63
          Height = 16
          Caption = '&Sort'
          TabOrder = 0
          OnClick = SortButClick
        end
      end
      object Panel9: TPanel
        Left = 1
        Top = 376
        Width = 328
        Height = 28
        Align = alBottom
        TabOrder = 2
        object Label3: TLabel
          Left = 4
          Top = 8
          Width = 45
          Height = 13
          Caption = '&Wildcard:'
        end
        object WildEdit: TEdit
          Left = 52
          Top = 4
          Width = 141
          Height = 21
          TabOrder = 0
        end
        object AddWildBttn: TButton
          Left = 196
          Top = 4
          Width = 46
          Height = 21
          Caption = 'Add ^'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -12
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 1
          OnClick = AddWildBttnClick
        end
        object AddWildPathBttn: TButton
          Left = 244
          Top = 4
          Width = 81
          Height = 21
          Caption = 'Add + path ^'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -12
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 2
          OnClick = AddWildPathBttnClick
        end
      end
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 173
    Height = 407
    Align = alLeft
    TabOrder = 1
    object Panel3: TPanel
      Left = 1
      Top = 1
      Width = 171
      Height = 23
      Align = alTop
      TabOrder = 0
      object DriveComboBox1: TDriveComboBox
        Left = 3
        Top = 3
        Width = 157
        Height = 19
        DirList = DirectoryListBox1
        TabOrder = 0
      end
    end
    object FileListBox1: TFileListBox
      Left = 1
      Top = 193
      Width = 171
      Height = 213
      Align = alClient
      ItemHeight = 16
      MultiSelect = True
      ShowGlyphs = True
      TabOrder = 3
    end
    object Panel4: TPanel
      Left = 1
      Top = 24
      Width = 171
      Height = 145
      Align = alTop
      TabOrder = 1
      object DirectoryListBox1: TDirectoryListBox
        Left = 1
        Top = 1
        Width = 169
        Height = 143
        Align = alClient
        FileList = FileListBox1
        ItemHeight = 16
        TabOrder = 0
      end
    end
    object Panel8: TPanel
      Left = 1
      Top = 169
      Width = 171
      Height = 24
      Align = alTop
      TabOrder = 2
      object SelectAllBut: TButton
        Left = 43
        Top = 4
        Width = 81
        Height = 17
        Caption = 'Select All Files'
        TabOrder = 0
        OnClick = SelectAllButClick
      end
    end
  end
  object PopupMenu1: TPopupMenu
    AutoPopup = False
    Left = 238
    Top = 377
    object Add1: TMenuItem
      Tag = 1
      Caption = 'Add'
      Checked = True
      Default = True
      RadioItem = True
      OnClick = Add1Click
    end
    object Update1: TMenuItem
      Tag = 2
      Caption = 'Update'
      RadioItem = True
      OnClick = Add1Click
    end
    object Freshen1: TMenuItem
      Tag = 3
      Caption = 'Freshen'
      RadioItem = True
      OnClick = Add1Click
    end
    object Move1: TMenuItem
      Tag = 4
      Caption = 'Move'
      RadioItem = True
      OnClick = Add1Click
    end
  end
end
