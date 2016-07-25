object IBTools: TIBTools
  Left = -2
  Top = 103
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  BorderWidth = 4
  Caption = 'IBTools'
  ClientHeight = 346
  ClientWidth = 616
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object RepairPanel: TPanel
    Left = 0
    Top = 0
    Width = 616
    Height = 346
    Align = alClient
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    Visible = False
    ExplicitHeight = 329
    DesignSize = (
      616
      346)
    object Label5: TLabel
      Left = 40
      Top = 304
      Width = 32
      Height = 13
      Caption = 'Label5'
    end
    object TouchBtn2: TTouchBtn
      Left = 456
      Top = 264
      Width = 153
      Height = 56
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -24
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Close'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = CancelButtonMouseClick
    end
    object RepairCaption: TPanel
      Left = 8
      Top = 8
      Width = 601
      Height = 48
      Anchors = [akLeft, akTop, akRight]
      BevelOuter = bvNone
      BorderStyle = bsSingle
      Caption = 'POS1:C:\Program Files\MenuMate\MenuMate.gdb'
      Color = clSilver
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
    end
    object CheckButton: TTouchBtn
      Left = 456
      Top = 64
      Width = 153
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -24
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Check'
      ButtonColor = 1087568
      LatchedColor = clBtnFace
      DisabledButtonColor = 1087568
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = CheckButtonMouseClick
    end
    object RepairMemo: TMemo
      Left = 8
      Top = 64
      Width = 433
      Height = 257
      Lines.Strings = (
        
          'Unable to Repair DB from this tool with Firebird, You can check ' +
          'it though.'
        
          'To Check and Repair the DB, Cut and paste the following into a c' +
          'ommand line.'
        ''
        'Check for errors.'
        ''
        
          '"C:\Program Files\Firebird\Firebird_2_1\bin\gfix.exe" -v -user S' +
          'YSDBA -password '
        'masterkey "C:\Program Files (x86)\MenuMate\MenuMate.fdb"'
        ''
        'Mend the DB'
        ''
        
          '"C:\Program Files\Firebird\Firebird_2_1\bin\gfix.exe" -mend -use' +
          'r SYSDBA -password '
        'masterkey "C:\Program Files (x86)\MenuMate\MenuMate.fdb"'
        ''
        'Back it up.'
        ''
        
          '"C:\Program Files\Firebird\Firebird_2_1\bin\gbak.exe" -b -g -use' +
          'r SYSDBA -password '
        
          'masterkey "C:\Program Files (x86)\MenuMate\MenuMate.fdb" "C:\Pro' +
          'gram Files '
        '(x86)\MenuMate\MenuMate.fbk"'
        ''
        'Rename the Broken DB'
        ''
        
          'rename "C:\Program Files (x86)\MenuMate\MenuMate.fdb" MenuMate.b' +
          'roken.fdb"'
        ''
        'Restore the Fixed DB'
        ''
        
          '"C:\Program Files\Firebird\Firebird_2_1\bin\gbak.exe" -c -g -use' +
          'r SYSDBA -password '
        
          'masterkey "C:\Program Files (x86)\MenuMate\MenuMate.fbk" "C:\Pro' +
          'gram Files '
        '(x86)\MenuMate\MenuMate.fdb"')
      ParentColor = True
      ReadOnly = True
      ScrollBars = ssVertical
      TabOrder = 3
    end
  end
  object RestorePanel: TPanel
    Left = 0
    Top = 0
    Width = 616
    Height = 346
    Align = alClient
    Color = 14342874
    ParentBackground = False
    TabOrder = 2
    Visible = False
    ExplicitHeight = 329
    DesignSize = (
      616
      346)
    object TouchBtn1: TTouchBtn
      Left = 456
      Top = 256
      Width = 153
      Height = 64
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -24
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Close'
      ButtonColor = 1073320
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = CancelButtonMouseClick
    end
    object RestoreCaption: TPanel
      Left = 8
      Top = 8
      Width = 601
      Height = 48
      Anchors = [akLeft, akTop, akRight]
      BevelOuter = bvNone
      BorderStyle = bsSingle
      Caption = 'POS1:C:\Program Files\MenuMate\MenuMate.gdb'
      Color = clSilver
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
    end
    object RestoreSettingsPanel: TPanel
      Left = 8
      Top = 64
      Width = 433
      Height = 257
      BevelOuter = bvNone
      BorderStyle = bsSingle
      TabOrder = 2
      DesignSize = (
        429
        253)
      object Label7: TLabel
        Left = 16
        Top = 16
        Width = 183
        Height = 23
        Caption = 'Deactivate Indexes'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label8: TLabel
        Left = 16
        Top = 112
        Width = 169
        Height = 23
        Caption = 'No Validity Check'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label12: TLabel
        Left = 16
        Top = 160
        Width = 223
        Height = 23
        Caption = 'One Relation At A Time'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label9: TLabel
        Left = 16
        Top = 208
        Width = 128
        Height = 23
        Caption = 'Use All Space'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label10: TLabel
        Left = 16
        Top = 64
        Width = 109
        Height = 23
        Caption = 'No Shadow'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object chkDeactivateIndexes: TTouchBtn
        Left = 352
        Top = 8
        Width = 49
        Height = 41
        Anchors = [akTop, akRight]
        ParentColor = True
        ButtonColor = clBtnShadow
        LatchedColor = clActiveBorder
        DisabledButtonColor = clBtnFace
        LatchingType = ltCheckbox
        LatchingProperties = [lkLatchColor, lkStayDown]
        ExplicitLeft = 356
      end
      object chkNoValidityCheck: TTouchBtn
        Left = 352
        Top = 104
        Width = 49
        Height = 41
        Anchors = [akTop, akRight]
        ParentColor = True
        ButtonColor = clBtnShadow
        LatchedColor = clActiveBorder
        DisabledButtonColor = clBtnFace
        Latched = True
        LatchingType = ltCheckbox
        LatchingProperties = [lkLatchColor, lkStayDown]
        ExplicitLeft = 356
      end
      object chkOneRelationAtATime: TTouchBtn
        Left = 352
        Top = 152
        Width = 49
        Height = 41
        Anchors = [akTop, akRight]
        ParentColor = True
        ButtonColor = clBtnShadow
        LatchedColor = clActiveBorder
        DisabledButtonColor = clBtnFace
        LatchingType = ltCheckbox
        LatchingProperties = [lkLatchColor, lkStayDown]
        ExplicitLeft = 356
      end
      object chkUseAllSpace: TTouchBtn
        Left = 352
        Top = 200
        Width = 49
        Height = 41
        Anchors = [akTop, akRight]
        ParentColor = True
        ButtonColor = clBtnShadow
        LatchedColor = clActiveBorder
        DisabledButtonColor = clBtnFace
        LatchingType = ltCheckbox
        LatchingProperties = [lkLatchColor, lkStayDown]
        ExplicitLeft = 356
      end
      object chkNoShadow: TTouchBtn
        Left = 352
        Top = 56
        Width = 49
        Height = 41
        Anchors = [akTop, akRight]
        ParentColor = True
        ButtonColor = clBtnShadow
        LatchedColor = clActiveBorder
        DisabledButtonColor = clBtnFace
        LatchingType = ltCheckbox
        LatchingProperties = [lkLatchColor, lkStayDown]
        ExplicitLeft = 356
      end
    end
    object RestoreIBButton: TTouchBtn
      Left = 455
      Top = 64
      Width = 153
      Height = 81
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -24
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Restore to .fdb'
      ButtonColor = 1087568
      LatchedColor = clBtnFace
      DisabledButtonColor = 1087568
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = RestoreIBButtonMouseClick
    end
  end
  object BackupPanel: TPanel
    Left = 0
    Top = 0
    Width = 616
    Height = 346
    Align = alClient
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    Visible = False
    ExplicitHeight = 329
    DesignSize = (
      616
      346)
    object BackupCaption: TPanel
      Left = 8
      Top = 8
      Width = 601
      Height = 48
      Anchors = [akLeft, akTop, akRight]
      BevelOuter = bvNone
      BorderStyle = bsSingle
      Caption = 'POS1:C:\Program Files\MenuMate\MenuMate.fdb'
      Color = clSilver
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
    end
    object BackupSettingsPanel: TPanel
      Left = 8
      Top = 64
      Width = 433
      Height = 273
      BevelOuter = bvNone
      BorderStyle = bsSingle
      TabOrder = 1
      DesignSize = (
        429
        269)
      object Label1: TLabel
        Left = 16
        Top = 16
        Width = 177
        Height = 23
        Caption = 'Ignore Checksums'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label2: TLabel
        Left = 16
        Top = 64
        Width = 129
        Height = 23
        Caption = 'Ignore Limbo'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label3: TLabel
        Left = 16
        Top = 160
        Width = 178
        Height = 23
        Caption = 'Non Transportable'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label4: TLabel
        Left = 16
        Top = 208
        Width = 144
        Height = 23
        Caption = 'Meta-data only'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label6: TLabel
        Left = 16
        Top = 112
        Width = 214
        Height = 23
        Caption = 'No Garbage Collection'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object lbeVersion: TLabel
        Left = 16
        Top = 245
        Width = 62
        Height = 19
        Caption = 'Version '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object chkIgnoreChecksums: TTouchBtn
        Left = 352
        Top = 8
        Width = 49
        Height = 41
        Anchors = [akTop, akRight]
        ParentColor = True
        ButtonColor = clBtnShadow
        LatchedColor = clActiveBorder
        DisabledButtonColor = clBtnFace
        LatchingType = ltCheckbox
        LatchingProperties = [lkLatchColor, lkStayDown]
        ExplicitLeft = 356
      end
      object chkIgnoreLimbo: TTouchBtn
        Left = 352
        Top = 56
        Width = 49
        Height = 41
        Anchors = [akTop, akRight]
        ParentColor = True
        ButtonColor = clBtnShadow
        LatchedColor = clActiveBorder
        DisabledButtonColor = clBtnFace
        Latched = True
        LatchingType = ltCheckbox
        LatchingProperties = [lkLatchColor, lkStayDown]
        ExplicitLeft = 356
      end
      object chkNonTransportable: TTouchBtn
        Left = 352
        Top = 152
        Width = 49
        Height = 41
        Anchors = [akTop, akRight]
        ParentColor = True
        ButtonColor = clBtnShadow
        LatchedColor = clActiveBorder
        DisabledButtonColor = clBtnFace
        LatchingType = ltCheckbox
        LatchingProperties = [lkLatchColor, lkStayDown]
        ExplicitLeft = 356
      end
      object chkMetadataOnly: TTouchBtn
        Left = 352
        Top = 200
        Width = 49
        Height = 41
        Anchors = [akTop, akRight]
        ParentColor = True
        ButtonColor = clBtnShadow
        LatchedColor = clActiveBorder
        DisabledButtonColor = clBtnFace
        LatchingType = ltCheckbox
        LatchingProperties = [lkLatchColor, lkStayDown]
        ExplicitLeft = 356
      end
      object chkNoGarbageCollection: TTouchBtn
        Left = 352
        Top = 104
        Width = 49
        Height = 41
        Anchors = [akTop, akRight]
        ParentColor = True
        ButtonColor = clBtnShadow
        LatchedColor = clActiveBorder
        DisabledButtonColor = clBtnFace
        LatchingType = ltCheckbox
        LatchingProperties = [lkLatchColor, lkStayDown]
        ExplicitLeft = 356
      end
    end
    object SweepButton: TTouchBtn
      Left = 456
      Top = 128
      Width = 153
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -24
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Sweep'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = 10870243
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = SweepButtonMouseClick
    end
    object RepairButton: TTouchBtn
      Left = 456
      Top = 192
      Width = 153
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -24
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Repair...'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = 10870243
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = RepairButtonMouseClick
    end
    object BackupButton: TTouchBtn
      Left = 456
      Top = 64
      Width = 153
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -24
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Backup'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = 1087568
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = BackupButtonMouseClick
    end
    object TouchBtn3: TTouchBtn
      Left = 456
      Top = 266
      Width = 152
      Height = 52
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Quit'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = CancelButtonMouseClick
    end
  end
end
