object frmSelectDebtor: TfrmSelectDebtor
  Left = 122
  Top = 244
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Select Debtor'
  ClientHeight = 455
  ClientWidth = 479
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object btnFind: TBitBtn
    Left = 392
    Top = 8
    Width = 81
    Height = 33
    Caption = '&Find/Add'
    TabOrder = 0
    OnClick = btnFindClick
  end
  object dbgResult: TDBGrid
    Left = 8
    Top = 120
    Width = 465
    Height = 281
    Color = 15269887
    DataSource = dsDebtor
    Options = [dgTitles, dgColLines, dgRowLines, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
    TabOrder = 1
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'Tahoma'
    TitleFont.Style = []
    OnDblClick = dbgResultDblClick
    OnKeyPress = dbgResultKeyPress
    OnKeyUp = dbgResultKeyUp
    Columns = <
      item
        Expanded = False
        FieldName = 'NAME'
        Title.Caption = 'Debtor'#39's Name'
        Width = 460
        Visible = True
      end>
  end
  object btnCancel: TBitBtn
    Left = 392
    Top = 88
    Width = 81
    Height = 33
    TabOrder = 2
    Kind = bkCancel
  end
  object btnOk: TBitBtn
    Left = 392
    Top = 416
    Width = 81
    Height = 33
    Caption = '&Select'
    TabOrder = 3
    OnClick = dbgResultDblClick
  end
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 377
    Height = 113
    BevelOuter = bvNone
    BorderStyle = bsSingle
    Color = 14221311
    Ctl3D = False
    ParentCtl3D = False
    TabOrder = 4
    object Label1: TLabel
      Left = 8
      Top = 38
      Width = 361
      Height = 27
      Alignment = taCenter
      AutoSize = False
      Caption = 
        'Enter a few letters/words contained within the debtor'#39's name you' +
        ' want.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      WordWrap = True
    end
    object Label2: TLabel
      Left = 8
      Top = 8
      Width = 361
      Height = 24
      Alignment = taCenter
      AutoSize = False
      Caption = 'Select Debtor'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -20
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object btnGo: TBitBtn
      Left = 288
      Top = 64
      Width = 73
      Height = 33
      Caption = '&Go'
      TabOrder = 0
      OnClick = btnGoClick
    end
    object edSearch: TEdit
      Left = 16
      Top = 72
      Width = 257
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
      TabOrder = 1
      OnChange = edSearchChange
      OnKeyPress = edSearchKeyPress
    end
  end
  object qrDebtor: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = Transaction
    SQL.Strings = (
      'Select '
      '   Contacts_Key,'
      '   Name     '#9#9#9#9
      ' From'
      '   Contacts'
      'Where'
      '   Upper(Name) Like :Name and'
      '   Contact_Type = :ContactType'
      'Order By'
      '   Name')
    Left = 24
    Top = 176
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Name'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'ContactType'
        ParamType = ptUnknown
      end>
  end
  object dsDebtor: TDataSource
    AutoEdit = False
    DataSet = qrDebtor
    Left = 80
    Top = 176
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmMMData.dbMenuMate
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 136
    Top = 176
  end
end
