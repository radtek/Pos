object DLInterbaseConfigureForm: TDLInterbaseConfigureForm
  Left = 311
  Top = 149
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Database Connection Parameters'
  ClientHeight = 266
  ClientWidth = 303
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
    Width = 303
    Height = 225
    ActivePage = TabSheet2
    Align = alClient
    TabOrder = 0
    object TabSheet2: TTabSheet
      Caption = 'Connection'
      ImageIndex = 1
      object Label1: TLabel
        Left = 8
        Top = 16
        Width = 42
        Height = 13
        Caption = 'Protocol:'
      end
      object lablServer: TLabel
        Left = 8
        Top = 40
        Width = 34
        Height = 13
        Caption = 'Server:'
      end
      object Label5: TLabel
        Left = 8
        Top = 64
        Width = 68
        Height = 13
        Caption = 'Database File:'
      end
      object Label2: TLabel
        Left = 8
        Top = 88
        Width = 60
        Height = 13
        Caption = 'SQL Dialect:'
      end
      object cmboProtocol: TComboBox
        Left = 80
        Top = 8
        Width = 203
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        Sorted = True
        TabOrder = 0
        OnChange = cmboProtocolChange
      end
      object editServer: TEdit
        Left = 80
        Top = 32
        Width = 202
        Height = 21
        TabOrder = 1
      end
      object editFile: TEdit
        Left = 80
        Top = 56
        Width = 185
        Height = 21
        TabOrder = 2
      end
      object butnBrowseFile: TButton
        Left = 264
        Top = 57
        Width = 19
        Height = 19
        Caption = '...'
        TabOrder = 3
        OnClick = butnBrowseFileClick
      end
      object cboxDialect: TComboBox
        Left = 80
        Top = 80
        Width = 204
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        ItemIndex = 1
        TabOrder = 4
        Text = 'Dialect 3'
        Items.Strings = (
          'Dialect 1'
          'Dialect 3')
      end
    end
    object TabSheet1: TTabSheet
      Caption = 'Authentication'
      object Label3: TLabel
        Left = 15
        Top = 25
        Width = 51
        Height = 13
        Caption = '&Username:'
        FocusControl = editUsername
      end
      object Label4: TLabel
        Left = 15
        Top = 49
        Width = 49
        Height = 13
        Caption = '&Password:'
        FocusControl = editPassword
      end
      object Label6: TLabel
        Left = 15
        Top = 73
        Width = 46
        Height = 13
        Caption = '&SQL Role'
        FocusControl = editSQLRole
      end
      object editUsername: TEdit
        Left = 72
        Top = 20
        Width = 185
        Height = 21
        TabOrder = 0
      end
      object editPassword: TEdit
        Left = 72
        Top = 44
        Width = 185
        Height = 21
        PasswordChar = '*'
        TabOrder = 1
      end
      object editSQLRole: TEdit
        Left = 72
        Top = 68
        Width = 185
        Height = 21
        TabOrder = 2
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'Test'
      ImageIndex = 2
      object Label7: TLabel
        Left = 8
        Top = 32
        Width = 46
        Height = 13
        Caption = 'Table List'
      end
      object Button1: TButton
        Left = 96
        Top = 8
        Width = 97
        Height = 25
        Caption = 'Test Connection'
        TabOrder = 0
        OnClick = Button1Click
      end
      object lboxTables: TListBox
        Left = 8
        Top = 48
        Width = 276
        Height = 137
        ItemHeight = 13
        TabOrder = 1
      end
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 225
    Width = 303
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    DesignSize = (
      303
      41)
    object butnOk: TButton
      Left = 139
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
      Left = 219
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
    Left = 72
    Top = 228
    object actnOk: TAction
      Caption = '&Ok'
      OnUpdate = actnOkUpdate
    end
  end
  object odlgDatabase: TOpenDialog
    DefaultExt = '.gdb'
    Filter = 'Interbase Databases|*.gdb|All Files|*.*'
    Options = [ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Title = 'Select Database File'
    Left = 8
    Top = 224
  end
end
