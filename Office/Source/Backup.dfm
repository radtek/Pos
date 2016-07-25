object frmBackup: TfrmBackup
  Left = 322
  Top = 185
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'MenuMate Backup'
  ClientHeight = 338
  ClientWidth = 483
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnCloseQuery = FormCloseQuery
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object pnlBackup: TPanel
    Left = 0
    Top = 0
    Width = 483
    Height = 338
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object PageControl1: TPageControl
      Left = 8
      Top = 8
      Width = 465
      Height = 281
      ActivePage = tsDataBackup
      TabIndex = 0
      TabOrder = 0
      OnChanging = PageControl1Changing
      object tsDataBackup: TTabSheet
        Caption = 'Data Backup'
        object Label6: TLabel
          Left = 8
          Top = 160
          Width = 87
          Height = 16
          Caption = 'Backup files to'
        end
        object lbeProgress: TLabel
          Left = 8
          Top = 224
          Width = 9
          Height = 16
          Caption = '...'
        end
        object lbeProgressCap: TLabel
          Left = 8
          Top = 208
          Width = 58
          Height = 16
          Caption = 'Progress:'
        end
        object edDestPath: TEdit
          Left = 8
          Top = 176
          Width = 201
          Height = 22
          Ctl3D = False
          ParentCtl3D = False
          TabOrder = 0
        end
        object btnSelectDestPath: TBitBtn
          Left = 209
          Top = 176
          Width = 24
          Height = 22
          TabOrder = 1
          OnClick = btnSelectDestPathClick
          Glyph.Data = {
            76010000424D7601000000000000760000002800000020000000100000000100
            0400000000000001000000000000000000001000000010000000000000000000
            800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00555555555555
            55555555FFFFFFFFFF55555000000000055555577777777775F55500B8B8B8B8
            B05555775F555555575F550F0B8B8B8B8B05557F75F555555575550BF0B8B8B8
            B8B0557F575FFFFFFFF7550FBF0000000000557F557777777777500BFBFBFBFB
            0555577F555555557F550B0FBFBFBFBF05557F7F555555FF75550F0BFBFBF000
            55557F75F555577755550BF0BFBF0B0555557F575FFF757F55550FB700007F05
            55557F557777557F55550BFBFBFBFB0555557F555555557F55550FBFBFBFBF05
            55557FFFFFFFFF7555550000000000555555777777777755555550FBFB055555
            5555575FFF755555555557000075555555555577775555555555}
          NumGlyphs = 2
        end
        object GroupBox1: TGroupBox
          Left = 248
          Top = 16
          Width = 201
          Height = 201
          TabOrder = 2
          object Label12: TLabel
            Left = 24
            Top = 0
            Width = 143
            Height = 16
            Caption = 'Backup From Database'
            OnClick = Label12Click
          end
          object btnFullDataBackup: TBitBtn
            Left = 16
            Top = 24
            Width = 169
            Height = 57
            Caption = 'Full Data Backup'
            TabOrder = 0
            OnClick = btnFullDataBackupClick
          end
          object btnMMDataBackup: TBitBtn
            Left = 16
            Top = 96
            Width = 169
            Height = 41
            Caption = 'MenuMate Data Backup'
            TabOrder = 1
            OnClick = btnMMDataBackupClick
          end
          object btnStockDataBackup: TBitBtn
            Left = 16
            Top = 144
            Width = 169
            Height = 41
            Caption = 'Stock Data Backup'
            TabOrder = 2
            OnClick = btnStockDataBackupClick
          end
        end
        object GroupBox2: TGroupBox
          Left = 8
          Top = 16
          Width = 225
          Height = 137
          TabOrder = 3
          object Label9: TLabel
            Left = 16
            Top = 24
            Width = 97
            Height = 16
            Caption = 'Pickup files from'
          end
          object Label11: TLabel
            Left = 24
            Top = 0
            Width = 122
            Height = 16
            Caption = 'Pickup Backup Files'
            OnClick = Label11Click
          end
          object edPickupPath: TEdit
            Left = 16
            Top = 43
            Width = 169
            Height = 22
            Ctl3D = False
            ParentCtl3D = False
            TabOrder = 0
          end
          object btnSelectSourcePath: TBitBtn
            Left = 185
            Top = 43
            Width = 24
            Height = 22
            TabOrder = 1
            OnClick = btnSelectSourcePathClick
            Glyph.Data = {
              76010000424D7601000000000000760000002800000020000000100000000100
              0400000000000001000000000000000000001000000010000000000000000000
              800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
              FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00555555555555
              55555555FFFFFFFFFF55555000000000055555577777777775F55500B8B8B8B8
              B05555775F555555575F550F0B8B8B8B8B05557F75F555555575550BF0B8B8B8
              B8B0557F575FFFFFFFF7550FBF0000000000557F557777777777500BFBFBFBFB
              0555577F555555557F550B0FBFBFBFBF05557F7F555555FF75550F0BFBFBF000
              55557F75F555577755550BF0BFBF0B0555557F575FFF757F55550FB700007F05
              55557F557777557F55550BFBFBFBFB0555557F555555557F55550FBFBFBFBF05
              55557FFFFFFFFF7555550000000000555555777777777755555550FBFB055555
              5555575FFF755555555557000075555555555577775555555555}
            NumGlyphs = 2
          end
          object btnPickupCopy: TBitBtn
            Left = 16
            Top = 80
            Width = 169
            Height = 41
            Caption = 'Copy'
            TabOrder = 2
            OnClick = btnPickupCopyClick
          end
        end
        object radPickup: TRadioButton
          Left = 16
          Top = 16
          Width = 17
          Height = 17
          TabOrder = 4
          OnClick = radPickupClick
        end
        object radBackup: TRadioButton
          Left = 256
          Top = 16
          Width = 17
          Height = 17
          TabOrder = 5
          OnClick = radPickupClick
        end
      end
      object tsSystemBackup: TTabSheet
        Caption = 'System Backup'
        ImageIndex = 1
        object Label8: TLabel
          Left = 136
          Top = 16
          Width = 87
          Height = 16
          Caption = 'Backup files to'
        end
        object ListBox1: TListBox
          Left = 8
          Top = 48
          Width = 441
          Height = 145
          ItemHeight = 16
          MultiSelect = True
          TabOrder = 0
        end
        object btnAddSysFile: TBitBtn
          Left = 232
          Top = 208
          Width = 105
          Height = 33
          Caption = 'Add a file'
          TabOrder = 1
          OnClick = btnAddSysFileClick
        end
        object btnRemoveSysFile: TBitBtn
          Left = 344
          Top = 208
          Width = 105
          Height = 33
          Caption = 'Remove file'
          TabOrder = 2
          OnClick = btnRemoveSysFileClick
        end
        object edSysDestPath: TEdit
          Left = 240
          Top = 16
          Width = 185
          Height = 22
          Ctl3D = False
          ParentCtl3D = False
          TabOrder = 3
        end
        object btnSelectSysDestPath: TBitBtn
          Left = 425
          Top = 16
          Width = 24
          Height = 22
          TabOrder = 5
          OnClick = btnSelectSysDestPathClick
          Glyph.Data = {
            76010000424D7601000000000000760000002800000020000000100000000100
            0400000000000001000000000000000000001000000010000000000000000000
            800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00555555555555
            55555555FFFFFFFFFF55555000000000055555577777777775F55500B8B8B8B8
            B05555775F555555575F550F0B8B8B8B8B05557F75F555555575550BF0B8B8B8
            B8B0557F575FFFFFFFF7550FBF0000000000557F557777777777500BFBFBFBFB
            0555577F555555557F550B0FBFBFBFBF05557F7F555555FF75550F0BFBFBF000
            55557F75F555577755550BF0BFBF0B0555557F575FFF757F55550FB700007F05
            55557F557777557F55550BFBFBFBFB0555557F555555557F55550FBFBFBFBF05
            55557FFFFFFFFF7555550000000000555555777777777755555550FBFB055555
            5555575FFF755555555557000075555555555577775555555555}
          NumGlyphs = 2
        end
        object btnSystemBackup: TBitBtn
          Left = 8
          Top = 200
          Width = 121
          Height = 41
          Caption = 'System Backup'
          TabOrder = 4
          OnClick = btnSystemBackupClick
        end
      end
      object tsSetup: TTabSheet
        Caption = 'Data Path Setup'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImageIndex = 2
        ParentFont = False
        object Label3: TLabel
          Left = 16
          Top = 224
          Width = 154
          Height = 16
          Caption = 'Stock backup file path'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label1: TLabel
          Left = 16
          Top = 104
          Width = 161
          Height = 41
          AutoSize = False
          Caption = 'Server'#39's Stock backup path'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          WordWrap = True
        end
        object Label7: TLabel
          Left = 16
          Top = 184
          Width = 161
          Height = 41
          AutoSize = False
          Caption = 'MenuMate backup file path'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          WordWrap = True
        end
        object Label4: TLabel
          Left = 16
          Top = 144
          Width = 425
          Height = 41
          AutoSize = False
          Caption = 
            'This is the path visible from this machine to the backup file ab' +
            'ove. e.g. \\SERVER\Backup'
          WordWrap = True
        end
        object Label2: TLabel
          Left = 16
          Top = 8
          Width = 425
          Height = 57
          AutoSize = False
          Caption = 
            'These are the paths in which to store the backup files, as seen ' +
            'by the Interbase server. e.g. C:\Backup\. It is usually set to t' +
            'he same path as the database file itself.'
          WordWrap = True
        end
        object Label5: TLabel
          Left = 16
          Top = 64
          Width = 161
          Height = 41
          AutoSize = False
          Caption = 'Server'#39's MenuMate backup path'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          WordWrap = True
        end
        object btnDataPath: TBitBtn
          Left = 417
          Top = 224
          Width = 24
          Height = 22
          TabOrder = 5
          OnClick = btnDataPathClick
          Glyph.Data = {
            76010000424D7601000000000000760000002800000020000000100000000100
            0400000000000001000000000000000000001000000010000000000000000000
            800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00555555555555
            55555555FFFFFFFFFF55555000000000055555577777777775F55500B8B8B8B8
            B05555775F555555575F550F0B8B8B8B8B05557F75F555555575550BF0B8B8B8
            B8B0557F575FFFFFFFF7550FBF0000000000557F557777777777500BFBFBFBFB
            0555577F555555557F550B0FBFBFBFBF05557F7F555555FF75550F0BFBFBF000
            55557F75F555577755550BF0BFBF0B0555557F575FFF757F55550FB700007F05
            55557F557777557F55550BFBFBFBFB0555557F555555557F55550FBFBFBFBF05
            55557FFFFFFFFF7555550000000000555555777777777755555550FBFB055555
            5555575FFF755555555557000075555555555577775555555555}
          NumGlyphs = 2
        end
        object edDataBackupPath: TEdit
          Left = 184
          Top = 224
          Width = 233
          Height = 22
          Ctl3D = False
          ParentCtl3D = False
          TabOrder = 4
        end
        object Edit1: TEdit
          Left = 184
          Top = 104
          Width = 257
          Height = 22
          Ctl3D = False
          ParentCtl3D = False
          TabOrder = 1
        end
        object BitBtn2: TBitBtn
          Left = 417
          Top = 184
          Width = 24
          Height = 22
          TabOrder = 3
          OnClick = BitBtn2Click
          Glyph.Data = {
            76010000424D7601000000000000760000002800000020000000100000000100
            0400000000000001000000000000000000001000000010000000000000000000
            800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00555555555555
            55555555FFFFFFFFFF55555000000000055555577777777775F55500B8B8B8B8
            B05555775F555555575F550F0B8B8B8B8B05557F75F555555575550BF0B8B8B8
            B8B0557F575FFFFFFFF7550FBF0000000000557F557777777777500BFBFBFBFB
            0555577F555555557F550B0FBFBFBFBF05557F7F555555FF75550F0BFBFBF000
            55557F75F555577755550BF0BFBF0B0555557F575FFF757F55550FB700007F05
            55557F557777557F55550BFBFBFBFB0555557F555555557F55550FBFBFBFBF05
            55557FFFFFFFFF7555550000000000555555777777777755555550FBFB055555
            5555575FFF755555555557000075555555555577775555555555}
          NumGlyphs = 2
        end
        object Edit3: TEdit
          Left = 184
          Top = 184
          Width = 233
          Height = 22
          Ctl3D = False
          ParentCtl3D = False
          TabOrder = 2
        end
        object Edit2: TEdit
          Left = 184
          Top = 64
          Width = 257
          Height = 22
          Ctl3D = False
          ParentCtl3D = False
          TabOrder = 0
        end
      end
    end
    object btnOk: TBitBtn
      Left = 384
      Top = 296
      Width = 89
      Height = 33
      TabOrder = 1
      OnClick = btnOkClick
      Kind = bkClose
    end
    object ProgressBar: TProgressBar
      Left = 8
      Top = 296
      Width = 369
      Height = 32
      Min = 0
      Max = 1000
      TabOrder = 2
    end
  end
  object SaveDialog1: TSaveDialog
    Left = 104
    Top = 296
  end
  object ZipBuilder1: TZipBuilder
    AddCompLevel = 9
    AddStoreSuffixes = [assGIF, assPNG, assZ, assZIP, assZOO, assARC, assLZH, assARJ, assTAZ, assTGZ, assLHA, assRAR, assACE, assCAB, assGZ, assGZIP, assJAR]
    VersionInfo = '1.78'
    OnProgress = ZipBuilder1Progress
    OnMessage = ZipBuilder1Message
    SFXCaption = 'Self-extracting Archive'
    SFXOptions = [SFXCheckSize]
    SFXPath = 'ZipSFX.bin'
    Left = 24
    Top = 296
  end
  object OpenDialog1: TOpenDialog
    Options = [ofReadOnly, ofOverwritePrompt, ofHideReadOnly, ofAllowMultiSelect, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Left = 312
    Top = 296
  end
end
