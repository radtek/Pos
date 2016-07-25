object frmLogin: TfrmLogin
  Tag = 1024
  Left = 327
  Top = 245
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Login'
  ClientHeight = 217
  ClientWidth = 329
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 8
    Top = 8
    Width = 313
    Height = 201
    BorderWidth = 3
    TabOrder = 0
    object Panel3: TPanel
      Left = 4
      Top = 4
      Width = 305
      Height = 148
      Align = alClient
      BevelInner = bvLowered
      BevelOuter = bvNone
      TabOrder = 0
      object Panel4: TPanel
        Left = 1
        Top = 1
        Width = 303
        Height = 32
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        object lbeInstructions: TLabel
          Left = 8
          Top = 0
          Width = 289
          Height = 33
          AutoSize = False
          Caption = 'Please enter your user name and PIN to enter the stock system.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clCaptionText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          WordWrap = True
        end
      end
      object pnlOldLogin: TPanel
        Left = 8
        Top = 40
        Width = 289
        Height = 97
        BevelOuter = bvNone
        BorderStyle = bsSingle
        Color = clInfoBk
        TabOrder = 1
        object Label1: TLabel
          Left = 16
          Top = 8
          Width = 52
          Height = 13
          Caption = 'User Name'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label2: TLabel
          Left = 16
          Top = 48
          Width = 17
          Height = 13
          Caption = 'PIN'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object edPIN: TEdit
          Left = 16
          Top = 64
          Width = 65
          Height = 21
          Color = 15269887
          Ctl3D = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentCtl3D = False
          ParentFont = False
          PasswordChar = '*'
          TabOrder = 1
          OnKeyPress = edPINKeyPress
        end
        object cbUserName: TComboBox
          Left = 16
          Top = 24
          Width = 257
          Height = 21
          Style = csDropDownList
          Color = 15269887
          ItemHeight = 13
          TabOrder = 0
          OnCloseUp = cbUserNameCloseUp
        end
      end
      object pnlNewLogin: TPanel
        Left = 8
        Top = 40
        Width = 289
        Height = 97
        BevelOuter = bvNone
        BorderStyle = bsSingle
        Color = clInfoBk
        TabOrder = 2
        object Label3: TLabel
          Left = 16
          Top = 8
          Width = 52
          Height = 13
          Caption = 'User Name'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label4: TLabel
          Left = 16
          Top = 48
          Width = 46
          Height = 13
          Caption = 'Password'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object edPassword: TEdit
          Left = 16
          Top = 64
          Width = 257
          Height = 21
          Color = 15269887
          Ctl3D = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          MaxLength = 32
          ParentCtl3D = False
          ParentFont = False
          PasswordChar = '*'
          TabOrder = 1
        end
        object edUserName: TEdit
          Left = 16
          Top = 24
          Width = 257
          Height = 21
          Color = 15269887
          Ctl3D = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          MaxLength = 32
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 0
        end
      end
    end
    object Panel5: TPanel
      Left = 4
      Top = 152
      Width = 305
      Height = 45
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 1
      object btnOk: TBitBtn
        Left = 128
        Top = 8
        Width = 81
        Height = 33
        Caption = 'OK'
        Default = True
        TabOrder = 0
        OnClick = btnOkClick
        Glyph.Data = {
          DE010000424DDE01000000000000760000002800000024000000120000000100
          0400000000006801000000000000000000001000000010000000000000000000
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
      object btnCancel: TBitBtn
        Left = 216
        Top = 8
        Width = 81
        Height = 33
        TabOrder = 1
        Kind = bkCancel
      end
    end
  end
  object qrLogin: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      'SELECT *'
      'FROM'
      '   USERS'
      'WHERE'
      '   UPPER(USER_ID) = UPPER(:UserID) AND PIN = :PIN')
    Left = 432
    Top = 40
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'UserID'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'PIN'
        ParamType = ptUnknown
      end>
  end
  object qrUsers: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      'Select'
      '   User_Name,'
      '   User_ID'
      'From'
      '   Users'
      'Order By'
      '   User_Name')
    Left = 432
    Top = 96
  end
  object qrInfo: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      'SELECT *'
      'FROM'
      '   USERS'
      'WHERE'
      '   UPPER(USER_ID) = UPPER(:UserID)')
    Left = 360
    Top = 56
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'UserID'
        ParamType = ptUnknown
      end>
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 100
    OnTimer = Timer1Timer
    Left = 208
    Top = 104
  end
end
