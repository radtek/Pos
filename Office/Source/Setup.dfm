object frmSetup: TfrmSetup
  Tag = 1024
  Left = 810
  Top = 51
  BorderStyle = bsDialog
  Caption = 'Setup'
  ClientHeight = 567
  ClientWidth = 521
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object tcCompanyNames: TTabControl
    Left = 8
    Top = 8
    Width = 505
    Height = 513
    OwnerDraw = True
    TabOrder = 0
    Tabs.Strings = (
      'Company Name')
    TabIndex = 0
    OnChange = tcCompanyNamesChange
    OnChanging = tcCompanyNamesChanging
    OnDrawTab = tcCompanyNamesDrawTab
    object pcSettings: TPageControl
      Left = 16
      Top = 32
      Width = 345
      Height = 457
      ActivePage = tsMenuSettings
      MultiLine = True
      TabIndex = 4
      TabOrder = 0
      OnChange = pcSettingsChange
      object tsMMConnection: TTabSheet
        Caption = 'MenuMate Database'
        object Panel2: TPanel
          Left = 8
          Top = 8
          Width = 321
          Height = 361
          BevelOuter = bvNone
          BorderStyle = bsSingle
          Ctl3D = True
          ParentColor = True
          ParentCtl3D = False
          TabOrder = 0
          object Label2: TLabel
            Left = 16
            Top = 48
            Width = 48
            Height = 13
            Caption = 'File Path'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label11: TLabel
            Left = 16
            Top = 8
            Width = 38
            Height = 13
            Caption = 'Server'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object edMMDBName: TEdit
            Left = 16
            Top = 64
            Width = 289
            Height = 21
            Ctl3D = True
            ParentCtl3D = False
            TabOrder = 1
          end
          object GroupBox2: TGroupBox
            Left = 16
            Top = 140
            Width = 289
            Height = 157
            Ctl3D = True
            ParentCtl3D = False
            TabOrder = 0
            object Label4: TLabel
              Left = 16
              Top = 24
              Width = 82
              Height = 13
              Caption = 'Phonebook Entry'
            end
            object Label14: TLabel
              Left = 16
              Top = 56
              Width = 52
              Height = 13
              Caption = 'User Name'
            end
            object Label15: TLabel
              Left = 16
              Top = 84
              Width = 46
              Height = 13
              Caption = 'Password'
            end
            object cbMMRASEntry: TComboBox
              Left = 120
              Top = 20
              Width = 153
              Height = 21
              Style = csDropDownList
              Enabled = False
              ItemHeight = 0
              TabOrder = 0
              OnChange = cbMMRASEntryChange
              OnClick = cbMMRASEntryClick
            end
            object btnHelpMMRAS: TBitBtn
              Left = 192
              Top = 112
              Width = 81
              Height = 33
              TabOrder = 1
              OnClick = btnHelpRASClick
              Kind = bkHelp
            end
            object edMMRASUsername: TEdit
              Left = 120
              Top = 48
              Width = 153
              Height = 21
              Ctl3D = True
              Enabled = False
              ParentCtl3D = False
              TabOrder = 2
            end
            object edMMRASPassword: TEdit
              Left = 120
              Top = 76
              Width = 153
              Height = 21
              Ctl3D = True
              Enabled = False
              ParentCtl3D = False
              TabOrder = 3
            end
            object btnDialMM: TButton
              Left = 16
              Top = 112
              Width = 81
              Height = 33
              Caption = 'Dial'
              Enabled = False
              TabOrder = 4
              OnClick = btnDialMMClick
            end
            object btnHangUpMM: TButton
              Left = 104
              Top = 112
              Width = 81
              Height = 33
              Caption = 'Hang up'
              Enabled = False
              TabOrder = 5
              OnClick = btnHangUpMMClick
            end
          end
          object btnHelpMMDatabase: TBitBtn
            Left = 224
            Top = 96
            Width = 81
            Height = 33
            TabOrder = 2
            OnClick = btnHelpMMDatabaseClick
            Kind = bkHelp
          end
          object btnConnectMM: TButton
            Left = 16
            Top = 96
            Width = 113
            Height = 33
            Caption = 'Test Connection'
            TabOrder = 3
            OnClick = btnConnectCompanyClick
          end
          object StaticText1: TStaticText
            Left = 40
            Top = 138
            Width = 172
            Height = 17
            Alignment = taRightJustify
            Caption = 'Remote MenuMate Database '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
          end
          object chbMMRemote: TCheckBox
            Left = 24
            Top = 137
            Width = 17
            Height = 17
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
            OnClick = chbMMRemoteClick
          end
          object btnSweepMenuMate: TButton
            Left = 136
            Top = 96
            Width = 81
            Height = 33
            Caption = 'Sweep'
            TabOrder = 6
            OnClick = btnSweepMenuMateClick
          end
          object cbMMDBServer: TComboBox
            Left = 16
            Top = 24
            Width = 193
            Height = 21
            ItemHeight = 0
            TabOrder = 7
            OnDropDown = cbDBServerDropDown
          end
          object btnGetMMIP: TButton
            Left = 224
            Top = 16
            Width = 81
            Height = 29
            Caption = 'Get IP'
            TabOrder = 8
            OnClick = btnGetMMIPClick
          end
        end
      end
      object tsStockConnection: TTabSheet
        Caption = 'Stock Database'
        ImageIndex = 1
        object Panel4: TPanel
          Left = 8
          Top = 8
          Width = 321
          Height = 361
          BevelOuter = bvNone
          BorderStyle = bsSingle
          Ctl3D = True
          ParentColor = True
          ParentCtl3D = False
          TabOrder = 0
          object Label3: TLabel
            Left = 16
            Top = 48
            Width = 48
            Height = 13
            Caption = 'File Path'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label12: TLabel
            Left = 16
            Top = 8
            Width = 38
            Height = 13
            Caption = 'Server'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object edStockDBName: TEdit
            Left = 16
            Top = 64
            Width = 289
            Height = 21
            Ctl3D = True
            ParentCtl3D = False
            TabOrder = 0
          end
          object btnConnectStock: TButton
            Left = 16
            Top = 96
            Width = 113
            Height = 33
            Caption = 'Test Connection'
            TabOrder = 1
            OnClick = btnConnectCompanyClick
          end
          object btnHelpStockDatabase: TBitBtn
            Left = 224
            Top = 96
            Width = 81
            Height = 33
            TabOrder = 2
            OnClick = btnHelpStockDatabaseClick
            Kind = bkHelp
          end
          object GroupBox3: TGroupBox
            Left = 16
            Top = 140
            Width = 289
            Height = 157
            Ctl3D = True
            ParentCtl3D = False
            TabOrder = 3
            object Label5: TLabel
              Left = 16
              Top = 24
              Width = 82
              Height = 13
              Caption = 'Phonebook Entry'
            end
            object Label16: TLabel
              Left = 16
              Top = 84
              Width = 46
              Height = 13
              Caption = 'Password'
            end
            object Label17: TLabel
              Left = 16
              Top = 56
              Width = 52
              Height = 13
              Caption = 'User Name'
            end
            object cbStockRASEntry: TComboBox
              Left = 120
              Top = 20
              Width = 153
              Height = 21
              Style = csDropDownList
              Enabled = False
              ItemHeight = 0
              TabOrder = 0
              OnChange = cbStockRASEntryChange
              OnClick = cbStockRASEntryClick
            end
            object btnHelpStockRAS: TBitBtn
              Left = 192
              Top = 112
              Width = 81
              Height = 33
              TabOrder = 1
              OnClick = btnHelpRASClick
              Kind = bkHelp
            end
            object edStockRASUsername: TEdit
              Left = 120
              Top = 48
              Width = 153
              Height = 21
              Ctl3D = True
              Enabled = False
              ParentCtl3D = False
              TabOrder = 2
            end
            object edStockRASPassword: TEdit
              Left = 120
              Top = 76
              Width = 153
              Height = 21
              Ctl3D = True
              Enabled = False
              ParentCtl3D = False
              TabOrder = 3
            end
            object btnDialStock: TButton
              Left = 16
              Top = 112
              Width = 81
              Height = 33
              Caption = 'Dial'
              Enabled = False
              TabOrder = 4
              OnClick = btnDialStockClick
            end
            object btnHangUpStock: TButton
              Left = 104
              Top = 112
              Width = 81
              Height = 33
              Caption = 'Hang up'
              Enabled = False
              TabOrder = 5
              OnClick = btnHangUpStockClick
            end
          end
          object StaticText2: TStaticText
            Left = 40
            Top = 138
            Width = 144
            Height = 17
            Alignment = taRightJustify
            Caption = 'Remote Stock Database '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            OnClick = StaticText2Click
          end
          object chbStockRemote: TCheckBox
            Left = 24
            Top = 137
            Width = 17
            Height = 17
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
            OnClick = chbStockRemoteClick
          end
          object btnSweepStock: TButton
            Left = 136
            Top = 96
            Width = 81
            Height = 33
            Caption = 'Sweep'
            TabOrder = 6
            OnClick = btnSweepStockClick
          end
          object cbStockDBServer: TComboBox
            Left = 16
            Top = 24
            Width = 193
            Height = 21
            ItemHeight = 0
            TabOrder = 7
            OnDropDown = cbDBServerDropDown
          end
          object btnGetStockIPClick: TButton
            Left = 224
            Top = 16
            Width = 81
            Height = 29
            Caption = 'Get IP'
            TabOrder = 8
            OnClick = btnGetStockIPClickClick
          end
        end
      end
      object tsChefMateConnection: TTabSheet
        Caption = 'ChefMate Database'
        ImageIndex = 1
        object PanelChefMate4: TPanel
          Left = 8
          Top = 8
          Width = 321
          Height = 361
          BevelOuter = bvNone
          BorderStyle = bsSingle
          Ctl3D = True
          ParentColor = True
          ParentCtl3D = False
          TabOrder = 0
          object LabelChefMate3: TLabel
            Left = 16
            Top = 48
            Width = 48
            Height = 13
            Caption = 'File Path'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object LabelChefMate12: TLabel
            Left = 16
            Top = 8
            Width = 38
            Height = 13
            Caption = 'Server'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object edChefMateDBName: TEdit
            Left = 16
            Top = 64
            Width = 289
            Height = 21
            Ctl3D = True
            ParentCtl3D = False
            TabOrder = 0
          end
          object btnConnectChefMate: TButton
            Left = 16
            Top = 96
            Width = 113
            Height = 33
            Caption = 'Test Connection'
            TabOrder = 1
            OnClick = btnConnectCompanyClick
          end
          object btnHelpChefMateDatabase: TBitBtn
            Left = 224
            Top = 96
            Width = 81
            Height = 33
            TabOrder = 2
            OnClick = btnHelpChefMateDatabaseClick
            Kind = bkHelp
          end
          object GroupBoxChefMate3: TGroupBox
            Left = 16
            Top = 140
            Width = 289
            Height = 157
            Ctl3D = True
            ParentCtl3D = False
            TabOrder = 3
            object LabelChefMate5: TLabel
              Left = 16
              Top = 24
              Width = 82
              Height = 13
              Caption = 'Phonebook Entry'
            end
            object LabelChefMate16: TLabel
              Left = 16
              Top = 84
              Width = 46
              Height = 13
              Caption = 'Password'
            end
            object LabelChefMate17: TLabel
              Left = 16
              Top = 56
              Width = 52
              Height = 13
              Caption = 'User Name'
            end
            object cbChefMateRASEntry: TComboBox
              Left = 120
              Top = 20
              Width = 153
              Height = 21
              Style = csDropDownList
              Enabled = False
              ItemHeight = 0
              TabOrder = 0
              OnChange = cbChefMateRASEntryChange
              OnClick = cbChefMateRASEntryClick
            end
            object btnHelpChefMateRAS: TBitBtn
              Left = 192
              Top = 112
              Width = 81
              Height = 33
              TabOrder = 1
              OnClick = btnHelpRASClick
              Kind = bkHelp
            end
            object edChefMateRASUsername: TEdit
              Left = 120
              Top = 48
              Width = 153
              Height = 21
              Ctl3D = True
              Enabled = False
              ParentCtl3D = False
              TabOrder = 2
            end
            object edChefMateRASPassword: TEdit
              Left = 120
              Top = 76
              Width = 153
              Height = 21
              Ctl3D = True
              Enabled = False
              ParentCtl3D = False
              TabOrder = 3
            end
            object btnDialChefMate: TButton
              Left = 16
              Top = 112
              Width = 81
              Height = 33
              Caption = 'Dial'
              Enabled = False
              TabOrder = 4
              OnClick = btnDialChefMateClick
            end
            object btnHangUpChefMate: TButton
              Left = 104
              Top = 112
              Width = 81
              Height = 33
              Caption = 'Hang up'
              Enabled = False
              TabOrder = 5
              OnClick = btnHangUpChefMateClick
            end
          end
          object StaticTextChefMate2: TStaticText
            Left = 40
            Top = 138
            Width = 166
            Height = 17
            Alignment = taRightJustify
            Caption = 'Remote ChefMate Database '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            OnClick = StaticText2Click
          end
          object chbChefMateRemote: TCheckBox
            Left = 24
            Top = 137
            Width = 17
            Height = 17
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
            OnClick = chbChefMateRemoteClick
          end
          object btnSweepChefMate: TButton
            Left = 136
            Top = 96
            Width = 81
            Height = 33
            Caption = 'Sweep'
            TabOrder = 6
            OnClick = btnSweepChefMateClick
          end
          object cbChefMateDBServer: TComboBox
            Left = 16
            Top = 24
            Width = 193
            Height = 21
            ItemHeight = 0
            TabOrder = 7
            OnDropDown = cbDBServerDropDown
          end
          object btnGetChefMateIP: TButton
            Left = 224
            Top = 16
            Width = 81
            Height = 29
            Caption = 'Get IP'
            TabOrder = 8
            OnClick = btnGetChefMateIPClick
          end
        end
      end
      object tsHeader: TTabSheet
        Caption = 'Menu Editor Settings'
        ImageIndex = 2
        object Panel1: TPanel
          Left = 8
          Top = 8
          Width = 321
          Height = 361
          BevelOuter = bvNone
          BorderStyle = bsSingle
          Ctl3D = True
          ParentColor = True
          ParentCtl3D = False
          TabOrder = 0
          object Label1: TLabel
            Left = 16
            Top = 248
            Width = 289
            Height = 25
            AutoSize = False
            Caption = 
              'Enter the details of your company that you would like to appear ' +
              'in the header of your purchase orders.'
            Visible = False
            WordWrap = True
          end
          object Label6: TLabel
            Left = 16
            Top = 8
            Width = 107
            Height = 13
            Caption = 'Menu Commit Path'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label7: TLabel
            Left = 16
            Top = 48
            Width = 91
            Height = 13
            Caption = 'Menu Save Path'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label8: TLabel
            Left = 16
            Top = 108
            Width = 126
            Height = 13
            Caption = 'Default Menu TAX (%)'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label9: TLabel
            Left = 16
            Top = 132
            Width = 127
            Height = 13
            Caption = 'Default Stock TAX (%)'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label30: TLabel
            Left = 16
            Top = 156
            Width = 156
            Height = 13
            Caption = 'No. Of price Levels Required'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object memCompanyDetails: TMemo
            Left = 16
            Top = 280
            Width = 289
            Height = 25
            Ctl3D = True
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentCtl3D = False
            ParentFont = False
            ScrollBars = ssVertical
            TabOrder = 0
            Visible = False
            WordWrap = False
          end
          object edCommitPath: TEdit
            Left = 16
            Top = 24
            Width = 233
            Height = 21
            Ctl3D = True
            ParentCtl3D = False
            TabOrder = 1
          end
          object edMenuSavePath: TEdit
            Left = 16
            Top = 64
            Width = 233
            Height = 21
            Ctl3D = True
            ParentCtl3D = False
            TabOrder = 2
          end
          object btnBrowseMMSavePath: TBitBtn
            Left = 249
            Top = 64
            Width = 22
            Height = 21
            TabOrder = 3
            OnClick = btnBrowseMMSavePathClick
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
          object btnBrowseMMServerPath: TBitBtn
            Left = 249
            Top = 24
            Width = 22
            Height = 21
            TabOrder = 4
            OnClick = btnBrowseMMServerPathClick
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
          object btnHelpMenuCommit: TBitBtn
            Left = 280
            Top = 24
            Width = 25
            Height = 25
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
            OnClick = btnHelpMenuCommitClick
            Glyph.Data = {
              DE010000424DDE01000000000000760000002800000024000000120000000100
              0400000000006801000000000000000000001000000000000000000000000000
              80000080000000808000800000008000800080800000C0C0C000808080000000
              FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333336633
              3333333333333FF3333333330000333333364463333333333333388F33333333
              00003333333E66433333333333338F38F3333333000033333333E66333333333
              33338FF8F3333333000033333333333333333333333338833333333300003333
              3333446333333333333333FF3333333300003333333666433333333333333888
              F333333300003333333E66433333333333338F38F333333300003333333E6664
              3333333333338F38F3333333000033333333E6664333333333338F338F333333
              0000333333333E6664333333333338F338F3333300003333344333E666433333
              333F338F338F3333000033336664333E664333333388F338F338F33300003333
              E66644466643333338F38FFF8338F333000033333E6666666663333338F33888
              3338F3330000333333EE666666333333338FF33333383333000033333333EEEE
              E333333333388FFFFF8333330000333333333333333333333333388888333333
              0000}
            NumGlyphs = 2
          end
          object btnHelpMenuSave: TBitBtn
            Left = 280
            Top = 64
            Width = 25
            Height = 25
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
            OnClick = btnHelpMenuSaveClick
            Glyph.Data = {
              DE010000424DDE01000000000000760000002800000024000000120000000100
              0400000000006801000000000000000000001000000000000000000000000000
              80000080000000808000800000008000800080800000C0C0C000808080000000
              FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333336633
              3333333333333FF3333333330000333333364463333333333333388F33333333
              00003333333E66433333333333338F38F3333333000033333333E66333333333
              33338FF8F3333333000033333333333333333333333338833333333300003333
              3333446333333333333333FF3333333300003333333666433333333333333888
              F333333300003333333E66433333333333338F38F333333300003333333E6664
              3333333333338F38F3333333000033333333E6664333333333338F338F333333
              0000333333333E6664333333333338F338F3333300003333344333E666433333
              333F338F338F3333000033336664333E664333333388F338F338F33300003333
              E66644466643333338F38FFF8338F333000033333E6666666663333338F33888
              3338F3330000333333EE666666333333338FF33333383333000033333333EEEE
              E333333333388FFFFF8333330000333333333333333333333333388888333333
              0000}
            NumGlyphs = 2
          end
          object edDefaultMenuGST: TEdit
            Left = 216
            Top = 104
            Width = 57
            Height = 21
            TabOrder = 7
          end
          object edDefaultStockGST: TEdit
            Left = 216
            Top = 128
            Width = 57
            Height = 21
            TabOrder = 8
          end
          object edNoOfPriceLevels: TNumericEdit
            Left = 216
            Top = 152
            Width = 57
            Height = 21
            Alignment = taLeftJustify
            Precision = 1
            MaxLength = 2
            TabOrder = 9
            OnKeyPress = edNoOfPriceLevelsKeyPress
            DecimalPlaces = 0
          end
        end
      end
      object tsMenuSettings: TTabSheet
        Caption = 'Other Settings'
        ImageIndex = 3
        object Panel6: TPanel
          Left = 8
          Top = 8
          Width = 321
          Height = 361
          BevelOuter = bvNone
          BorderStyle = bsSingle
          Ctl3D = True
          ParentColor = True
          ParentCtl3D = False
          TabOrder = 0
          object Label10: TLabel
            Left = 8
            Top = 8
            Width = 72
            Height = 13
            Caption = 'TAX. Number'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label21: TLabel
            Left = 168
            Top = 8
            Width = 73
            Height = 13
            Caption = 'Barcode Port'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label27: TLabel
            Left = 8
            Top = 176
            Width = 92
            Height = 13
            Caption = 'Default Location'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label29: TLabel
            Left = 18
            Top = 248
            Width = 89
            Height = 13
            Caption = 'Vip Credit Name'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Button1: TButton
            Left = 8
            Top = 320
            Width = 137
            Height = 33
            Caption = 'Next Stock Code'
            TabOrder = 0
            OnClick = Button1Click
          end
          object edGSTNumber: TEdit
            Left = 8
            Top = 24
            Width = 145
            Height = 21
            TabOrder = 1
            Text = 'xx-xxx-xxx'
          end
          object chbHideStocktakeOnHand: TCheckBox
            Left = 168
            Top = 88
            Width = 161
            Height = 17
            Caption = 'Hide stocktake on hand'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 10
          end
          object Button2: TButton
            Left = 8
            Top = 280
            Width = 137
            Height = 33
            Caption = 'Locations'
            TabOrder = 3
            OnClick = Button2Click
          end
          object cbBarcodeSerialPort: TComboBox
            Left = 168
            Top = 24
            Width = 137
            Height = 21
            Style = csDropDownList
            ItemHeight = 13
            TabOrder = 4
            Items.Strings = (
              'Keyboard Wedge'
              'COM1'
              'COM2'
              'COM3'
              'COM4'
              'COM5'
              'COM6'
              'COM7'
              'COM8'
              'COM9'
              'COM10'
              'COM11'
              'COM12'
              'COM13'
              'COM14'
              'COM15'
              'COM16')
          end
          object chbSingleLocation: TCheckBox
            Left = 168
            Top = 62
            Width = 165
            Height = 17
            Caption = 'Use Single Stock Location'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
          end
          object cbDefaultLocation: TComboBox
            Left = 8
            Top = 192
            Width = 297
            Height = 21
            ItemHeight = 13
            TabOrder = 6
            OnDropDown = cbDefaultLocationDropDown
          end
          object edVIPCreditName: TEdit
            Left = 122
            Top = 246
            Width = 180
            Height = 21
            TabOrder = 7
          end
          object SyncStockLocation: TButton
            Left = 160
            Top = 280
            Width = 137
            Height = 33
            Caption = 'Sync Stock Locations'
            TabOrder = 8
            OnClick = SyncStockLocationClick
          end
          object RBRoundingPoint: TRadioGroup
            Left = 8
            Top = 48
            Width = 113
            Height = 57
            Caption = 'Rounding Point'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            Items.Strings = (
              'Two Decimal'
              'Four Decimal')
            ParentFont = False
            TabOrder = 9
            OnClick = RBRoundingPointClick
          end
          object chbAutoPrintReceiveTransferAudit: TCheckBox
            Left = 10
            Top = 146
            Width = 231
            Height = 17
            Caption = 'Auto Print Receive Invoice Audit'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 11
          end
          object chbAutoPrintStockTransferAudit: TCheckBox
            Left = 10
            Top = 122
            Width = 231
            Height = 17
            Caption = 'Auto Print Stock Transfer Audit'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object chbSuppliersFromDefaultLocationsOnly: TCheckBox
            Left = 10
            Top = 220
            Width = 271
            Height = 17
            Caption = 'Auto Supplier orders are from Default locations only'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 12
          end
        end
      end
      object tsPayrollSettings: TTabSheet
        Caption = 'Export/Import'
        ImageIndex = 4
        object Panel7: TPanel
          Left = 8
          Top = 8
          Width = 321
          Height = 361
          BevelOuter = bvNone
          BorderStyle = bsSingle
          Ctl3D = True
          ParentColor = True
          ParentCtl3D = False
          TabOrder = 0
          object GroupBox1: TGroupBox
            Left = 16
            Top = 8
            Width = 289
            Height = 129
            Caption = 'Payroll'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
            object Label18: TLabel
              Left = 16
              Top = 24
              Width = 70
              Height = 13
              Caption = 'Payroll System'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object Label19: TLabel
              Left = 16
              Top = 72
              Width = 92
              Height = 13
              Caption = 'Payroll Export Path'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object cbPayrollSystem: TComboBox
              Left = 16
              Top = 40
              Width = 257
              Height = 21
              Style = csDropDownList
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ItemHeight = 13
              ParentFont = False
              TabOrder = 0
              Items.Strings = (
                'Ace Payroll System'
                'MYOB Payroll'
                'Microsoft Excel Export')
            end
            object edExportPath: TEdit
              Left = 16
              Top = 88
              Width = 257
              Height = 21
              Ctl3D = True
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentCtl3D = False
              ParentFont = False
              TabOrder = 1
            end
            object btnSelectSysDestPath: TBitBtn
              Left = 251
              Top = 90
              Width = 20
              Height = 17
              Caption = '...'
              TabOrder = 2
              OnClick = btnSelectSysDestPathClick
              NumGlyphs = 2
            end
          end
          object GroupBox4: TGroupBox
            Left = 16
            Top = 144
            Width = 289
            Height = 153
            Caption = 'Accounts'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
            object Label13: TLabel
              Left = 16
              Top = 104
              Width = 76
              Height = 13
              Caption = 'MYOB Job Code'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object Label20: TLabel
              Left = 16
              Top = 64
              Width = 93
              Height = 13
              Caption = 'Export/Import Path'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object Label28: TLabel
              Left = 16
              Top = 24
              Width = 91
              Height = 13
              Caption = 'Accounting System'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object edMYOBJobCode: TEdit
              Left = 16
              Top = 120
              Width = 257
              Height = 21
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
            end
            object edMYOBPath: TEdit
              Left = 16
              Top = 80
              Width = 257
              Height = 21
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
            end
            object btnSelectMYOBPath: TBitBtn
              Left = 251
              Top = 82
              Width = 20
              Height = 17
              Caption = '...'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              OnClick = btnSelectMYOBPathClick
              NumGlyphs = 2
            end
            object cbAccountSystem: TComboBox
              Left = 16
              Top = 40
              Width = 257
              Height = 21
              Style = csDropDownList
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ItemHeight = 13
              ParentFont = False
              TabOrder = 3
              Items.Strings = (
                'CBA'
                'MYOB AU'
                'MYOB NZ Premier'
                'MYOB NZ Standard'
                'Xero'
                'Greentree')
            end
          end
        end
      end
      object tsStocktakeSettings: TTabSheet
        Caption = 'Stocktake Settings'
        ImageIndex = 5
        object Panel3: TPanel
          Left = 8
          Top = 8
          Width = 321
          Height = 361
          BevelOuter = bvNone
          BorderStyle = bsSingle
          Ctl3D = True
          ParentColor = True
          ParentCtl3D = False
          TabOrder = 0
          object Label23: TLabel
            Left = 16
            Top = 88
            Width = 94
            Height = 13
            Caption = 'Barcode Position'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label25: TLabel
            Left = 16
            Top = 48
            Width = 94
            Height = 13
            Caption = 'Export File Name'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label22: TLabel
            Left = 16
            Top = 8
            Width = 173
            Height = 13
            Caption = 'Stocktake Export/Import Path'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label26: TLabel
            Left = 168
            Top = 48
            Width = 97
            Height = 13
            Caption = 'Import File Name'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label24: TLabel
            Left = 168
            Top = 88
            Width = 97
            Height = 13
            Caption = 'Quantity Position'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object cbStocktakeBarcodePos: TComboBox
            Left = 16
            Top = 104
            Width = 137
            Height = 21
            Style = csDropDownList
            ItemHeight = 13
            TabOrder = 0
            Items.Strings = (
              '1'
              '2'
              '3'
              '4'
              '5'
              '6'
              '7'
              '8'
              '')
          end
          object edStocktakeExportFile: TEdit
            Left = 16
            Top = 64
            Width = 137
            Height = 21
            TabOrder = 1
          end
          object edStocktakePath: TEdit
            Left = 16
            Top = 24
            Width = 289
            Height = 21
            TabOrder = 2
          end
          object btnSelectStocktakePath: TBitBtn
            Left = 283
            Top = 26
            Width = 20
            Height = 17
            Caption = '...'
            TabOrder = 3
            OnClick = btnSelectStocktakePathClick
            NumGlyphs = 2
          end
          object edStocktakeImportFile: TEdit
            Left = 168
            Top = 64
            Width = 137
            Height = 21
            TabOrder = 4
          end
          object cbStocktakeQtyPos: TComboBox
            Left = 168
            Top = 104
            Width = 137
            Height = 21
            Style = csDropDownList
            ItemHeight = 13
            TabOrder = 5
            Items.Strings = (
              '1'
              '2'
              '3'
              '4'
              '5'
              '6'
              '7'
              '8'
              '')
          end
        end
      end
    end
    object btnConnectCompany: TButton
      Left = 368
      Top = 152
      Width = 129
      Height = 33
      Caption = 'Connect'
      TabOrder = 1
      OnClick = btnConnectCompanyClick
    end
    object btnDeleteCompany: TBitBtn
      Left = 368
      Top = 112
      Width = 129
      Height = 33
      Caption = 'Delete Company'
      TabOrder = 2
      OnClick = btnDeleteCompanyClick
    end
    object btnEditCompany: TBitBtn
      Left = 368
      Top = 72
      Width = 129
      Height = 33
      Caption = 'Edit Name'
      TabOrder = 3
      OnClick = btnEditCompanyClick
    end
    object btnAddCompany: TBitBtn
      Left = 368
      Top = 32
      Width = 129
      Height = 33
      Caption = 'Add Company'
      TabOrder = 4
      OnClick = btnAddCompanyClick
    end
    object btnAddRAS: TButton
      Left = 368
      Top = 408
      Width = 129
      Height = 33
      Caption = 'New Phonebook Entry'
      TabOrder = 5
      Visible = False
      OnClick = btnAddRASClick
    end
    object Button3: TButton
      Left = 368
      Top = 208
      Width = 129
      Height = 33
      Caption = 'Users'
      TabOrder = 6
      OnClick = Button3Click
    end
    object btnSecurityRoles: TButton
      Left = 368
      Top = 248
      Width = 129
      Height = 33
      Caption = 'Roles'
      TabOrder = 7
      OnClick = btnSecurityRolesClick
    end
    object btnTaxProfiles: TButton
      Left = 368
      Top = 288
      Width = 129
      Height = 33
      Caption = 'Tax Profiles'
      TabOrder = 8
      OnClick = btnTaxProfilesClick
    end
  end
  object btnClose: TBitBtn
    Left = 328
    Top = 528
    Width = 89
    Height = 33
    Caption = 'OK'
    Default = True
    TabOrder = 1
    OnClick = btnCloseClick
    Glyph.Data = {
      DE010000424DDE01000000000000760000002800000024000000120000000100
      0400000000006801000000000000000000001000000000000000000000000000
      80000080000000808000800000008000800080800000C0C0C000808080000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
      3333333333333333333333330000333333333333333333333333F33333333333
      00003333344333333333333333388F3333333333000033334224333333333333
      338338F3333333330000333422224333333333333833338F3333333300003342
      222224333333333383333338F3333333000034222A22224333333338F338F333
      8F33333300003222A3A2224333333338F3838F338F33333300003A2A333A2224
      33333338F83338F338F33333000033A33333A222433333338333338F338F3333
      0000333333333A222433333333333338F338F33300003333333333A222433333
      333333338F338F33000033333333333A222433333333333338F338F300003333
      33333333A222433333333333338F338F00003333333333333A22433333333333
      3338F38F000033333333333333A223333333333333338F830000333333333333
      333A333333333333333338330000333333333333333333333333333333333333
      0000}
    NumGlyphs = 2
  end
  object BitBtn2: TBitBtn
    Left = 424
    Top = 528
    Width = 89
    Height = 33
    TabOrder = 2
    Kind = bkCancel
  end
  object sqlSetStockCode: TIBSQL
    Database = dmStockData.dbStock
    ParamCheck = False
    SQL.Strings = (
      'Set Generator Gen_StockCode to :StockCode')
    Transaction = StockTransaction
    Left = 312
    Top = 456
  end
  object StockTransaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 184
    Top = 320
  end
  object MMTransaction: TIBTransaction
    DefaultDatabase = dmMMData.dbMenuMate
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 392
    Top = 304
  end
  object sqlLocation: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'Select'
      '   Name'
      'From'
      '   Locations'
      'Where'
      '   Location_Type = 1 or'
      '   Location_Type = 2'
      'Order By'
      '   Name')
    Transaction = MMTransaction
    Left = 408
    Top = 336
  end
  object qrLocation: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      'SELECT distinct STOCKLOCATION.LOCATION'
      'FROM STOCKLOCATION '
      'where'
      'STOCKLOCATION.LOCATION !='#39#39)
    Left = 416
    Top = 384
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Left = 448
    Top = 384
  end
  object CheckStockLocationExist: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'Select'
      '   Name'
      'From'
      '   Locations'
      'Where'
      '   Name = :Name'
      '')
    Transaction = MMTransaction
    Left = 448
    Top = 336
  end
  object qrAddLocation: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'Insert Into Locations ('
      '   Location_Key,'
      '   Location_Type,'
      '   Name,'
      'CATEGORY_TYPE)'
      'Values ('
      '   :Location_Key,'
      '   :Location_Type,'
      '   :Name,'
      ':CATEGORY_TYPE)'
      '')
    Transaction = MMTransaction
    Left = 200
    Top = 456
  end
  object sqlAddLK: TIBSQL
    Database = dmMMData.dbMenuMate
    ParamCheck = False
    SQL.Strings = (
      'SELECT GEN_ID(GEN_LOCATION, 1) FROM RDB$DATABASE')
    Transaction = MMTransaction
    Left = 232
    Top = 456
  end
  object qrFindLocation: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'Select'
      '   Location_Key,'
      '   Location_Type,'
      '   Name'
      'From'
      '   Locations'
      'Where'
      '   Upper(Name) = :Name')
    Transaction = MMTransaction
    Left = 272
    Top = 456
  end
  object sqlGetNoOfPriceLevels: TIBSQL
    Database = dmMMData.dbMenuMate
    ParamCheck = False
    SQL.Strings = (
      #9'SELECT '
      #9#9'INTEGER_VAL '
      #9'FROM '
      #9#9'VARIABLES'
      #9'WHERE '
      #9#9'VARIABLES_KEY = 5014;')
    Transaction = MMTransaction
    Left = 144
    Top = 456
  end
  object sqlSetNoOfPriceLevels: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      #9'UPDATE  VARIABLES'
      #9'SET'#9'INTEGER_VAL  = :INTEGER_VAL '
      #9
      #9'WHERE '
      #9#9'VARIABLES_KEY = 5014;')
    Transaction = MMTransaction
    Left = 96
    Top = 456
  end
  object qrInsertInToPriceLevelItem: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      ' INSERT INTO PRICELEVELS ('
      '                PRICELEVEL_KEY,'
      '                PRICELEVEL_NAME,'
      '                ISENABLED)'
      '            Values ('
      '                :PRICELEVEL_KEY,'
      '                :PRICELEVEL_NAME,'
      '                :ISENABLED );')
    Transaction = MMTransaction
    Left = 56
    Top = 456
  end
  object qrMaxPriceLevels: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'select max(a.PRICELEVEL_KEY)'
      'from PRICELEVELS a'
      'where a.ISENABLED = :ISENABLED'
      '')
    Transaction = MMTransaction
    Left = 288
    Top = 424
  end
  object qrUpdatePriceLevels: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'UPDATE pricelevels a  '
      'set a.ISENABLED = :ISENABLED '
      'where a.ISENABLED = :ISTRUE  and '
      'a.PRICELEVEL_KEY > :PRICELEVEL_KEY '
      'and a.PRICELEVEL_KEY <= :MAXPRICELEVEL_KEY')
    Transaction = MMTransaction
    Left = 320
    Top = 424
  end
  object qrInsertInToPriceLevelItemSize: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      ' INSERT INTO PRICELEVELITEMSIZE ('
      '                PRICELEVELITEMSIZE_KEY,'
      '                ITEMSIZE_KEY,'
      '                PRICELEVEL_KEY,'
      '                PRICE,'
      '                ISENABLED)'
      '            Values ('
      '                :PRICELEVELITEMSIZE_KEY,'
      '                :ITEMSIZE_KEY,'
      '                :PRICELEVEL_KEY,'
      '                :PRICE,'
      '                :ISENABLED);'
      ''
      '')
    Transaction = MMTransaction
    Left = 248
    Top = 424
  end
  object qrSelectPriceLevelItemSize: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      
        'SELECT a.ITEMSIZE_KEY,  subquery1.PRICE, a.ISENABLED,max(a.PRICE' +
        'LEVEL_KEY)  '
      'FROM PRICELEVELITEMSIZE a '
      'left join (SELECT a.ITEMSIZE_KEY,  a.PRICE, a.ISENABLED'
      'FROM PRICELEVELITEMSIZE a '
      
        'where a.PRICELEVEL_KEY = 1)subquery1 on subquery1.ITEMSIZE_KEY =' +
        ' a.ITEMSIZE_KEY    '
      'group by a.ITEMSIZE_KEY,subquery1.PRICE, a.ISENABLED'
      '')
    Transaction = MMTransaction
    Left = 208
    Top = 424
  end
  object qrGenPriceLevelItemSizeKey: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'SELECT GEN_ID(GEN_PriceLevelItemSize_KEY, 1) FROM RDB$DATABASE;'
      '')
    Transaction = MMTransaction
    Left = 168
    Top = 424
  end
  object qrMaxItemSizePriceLevels: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      '')
    Transaction = MMTransaction
    Left = 128
    Top = 424
  end
  object qrUpdateItemSizePriceLevels: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'UPDATE PRICELEVELITEMSIZE a  '
      'set a.ISENABLED = :ISENABLED '
      'where a.ISENABLED = :ISTRUE  and '
      'a.PRICELEVEL_KEY > :PRICELEVEL_KEY '
      'and a.PRICELEVEL_KEY <= :MAXPRICELEVEL_KEY')
    Transaction = MMTransaction
    Left = 48
    Top = 432
  end
  object qrGetExistingPriceNames: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'SELECT a.PRICELEVEL_KEY, a.PRICELEVEL_NAME'
      'FROM PRICELEVELS a'
      'order by a.PRICELEVEL_KEY')
    Transaction = MMTransaction
    Left = 88
    Top = 432
  end
end
