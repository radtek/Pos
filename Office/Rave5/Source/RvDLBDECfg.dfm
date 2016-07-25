object DlBDEConfigureForm: TDlBDEConfigureForm
  Left = 690
  Top = 184
  Width = 377
  Height = 316
  BorderIcons = [biSystemMenu]
  Caption = 'Database Connection Parameters'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 369
    Height = 248
    ActivePage = TabSheet3
    Align = alClient
    TabOrder = 0
    object TabSheet2: TTabSheet
      Caption = 'Connection'
      ImageIndex = 1
      object gboxAlias: TGroupBox
        Left = 0
        Top = 0
        Width = 361
        Height = 57
        Align = alTop
        TabOrder = 0
        object Label1: TLabel
          Left = 8
          Top = 24
          Width = 25
          Height = 13
          Caption = 'Alias:'
          Transparent = False
        end
        object cmboAlias: TComboBox
          Left = 48
          Top = 24
          Width = 169
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 0
        end
      end
      object gboxParams: TGroupBox
        Left = 0
        Top = 57
        Width = 361
        Height = 160
        Align = alTop
        TabOrder = 1
        object Label2: TLabel
          Left = 8
          Top = 24
          Width = 31
          Height = 13
          Caption = 'Driver:'
          Transparent = False
        end
        object Label5: TLabel
          Left = 8
          Top = 56
          Width = 56
          Height = 13
          Caption = 'Parameters:'
          Transparent = False
        end
        object cmboDriver: TComboBox
          Left = 48
          Top = 24
          Width = 169
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 0
        end
        object memoParams: TMemo
          Left = 2
          Top = 69
          Width = 357
          Height = 89
          Align = alBottom
          ScrollBars = ssVertical
          TabOrder = 1
        end
      end
      object radoAlias: TRadioButton
        Left = 8
        Top = 0
        Width = 113
        Height = 17
        Caption = 'Specify Alias'
        Checked = True
        TabOrder = 2
        TabStop = True
        OnClick = radoParamsClick
      end
      object radoParams: TRadioButton
        Left = 8
        Top = 56
        Width = 113
        Height = 17
        Caption = 'Specify Parameters'
        TabOrder = 3
        OnClick = radoParamsClick
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
        Transparent = False
      end
      object Label4: TLabel
        Left = 17
        Top = 49
        Width = 49
        Height = 13
        Caption = '&Password:'
        FocusControl = editPassword
        Transparent = False
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
    end
    object TabSheet3: TTabSheet
      Caption = 'Test'
      ImageIndex = 2
      object Label6: TLabel
        Left = 8
        Top = 40
        Width = 46
        Height = 13
        Caption = 'Table List'
      end
      object Button1: TButton
        Left = 104
        Top = 8
        Width = 137
        Height = 25
        Caption = 'Test Connection'
        TabOrder = 0
        OnClick = Button1Click
      end
      object lboxTables: TListBox
        Left = 8
        Top = 56
        Width = 345
        Height = 153
        ItemHeight = 13
        TabOrder = 1
      end
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 248
    Width = 369
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    DesignSize = (
      369
      41)
    object butnOk: TButton
      Left = 205
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
      Left = 285
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
    Top = 232
    object actnOk: TAction
      Caption = '&Ok'
      OnUpdate = actnOkUpdate
    end
  end
end
