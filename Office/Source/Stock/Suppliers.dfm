object frmSuppliers: TfrmSuppliers
  Tag = 1024
  Left = 231
  Top = 169
  Width = 640
  Height = 483
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Edit Suppliers'
  Color = clBtnFace
  Constraints.MinHeight = 483
  Constraints.MinWidth = 356
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 632
    Height = 456
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 4
    TabOrder = 0
    object Panel4: TPanel
      Left = 4
      Top = 4
      Width = 624
      Height = 448
      Align = alClient
      BorderWidth = 4
      TabOrder = 0
      object Splitter1: TSplitter
        Left = 293
        Top = 5
        Width = 3
        Height = 393
        Cursor = crHSplit
        Align = alRight
        ResizeStyle = rsUpdate
        OnCanResize = Splitter1CanResize
      end
      object tvContacts: TTreeView
        Left = 5
        Top = 5
        Width = 288
        Height = 393
        Align = alClient
        BevelInner = bvNone
        BevelOuter = bvNone
        Color = clInfoBk
        DragCursor = crDefault
        DragMode = dmAutomatic
        HideSelection = False
        Images = dmUtilities.imlTreeImages
        Indent = 19
        RightClickSelect = True
        TabOrder = 0
        OnChange = tvContactsChange
        OnDblClick = tvContactsDblClick
        OnEdited = tvContactsEdited
        OnKeyDown = tvContactsKeyDown
        OnKeyPress = tvContactsKeyPress
        OnMouseDown = tvContactsMouseDown
      end
      object Panel8: TPanel
        Left = 296
        Top = 5
        Width = 323
        Height = 393
        Align = alRight
        Anchors = [akLeft, akTop, akBottom]
        BevelOuter = bvLowered
        Constraints.MinWidth = 323
        TabOrder = 1
        object Panel9: TPanel
          Left = 1
          Top = 1
          Width = 321
          Height = 48
          Align = alTop
          BevelOuter = bvNone
          Color = clAppWorkSpace
          TabOrder = 0
          object lbeTitle: TLabel
            Left = 16
            Top = 0
            Width = 297
            Height = 49
            AutoSize = False
            Caption = 'lbeTitle'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clCaptionText
            Font.Height = -19
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            ShowAccelChar = False
            Layout = tlCenter
            WordWrap = True
          end
        end
        object pcContactsDetails: TPageControl
          Left = 1
          Top = 49
          Width = 321
          Height = 343
          ActivePage = tsContact
          Align = alClient
          Style = tsFlatButtons
          TabOrder = 1
          object tsContacts: TTabSheet
            TabVisible = False
            object Panel7: TPanel
              Left = 201
              Top = 0
              Width = 112
              Height = 333
              Align = alRight
              BevelOuter = bvNone
              TabOrder = 1
              object btnEditStock: TButton
                Left = 8
                Top = 32
                Width = 97
                Height = 25
                Caption = '&Edit Supplier'
                TabOrder = 1
                OnClick = btnEditStockClick
              end
              object btnDeleteStock: TButton
                Left = 8
                Top = 96
                Width = 97
                Height = 25
                Caption = '&Delete Supplier'
                TabOrder = 3
                OnClick = btnDeleteStockClick
              end
              object btnAddStock: TButton
                Left = 8
                Top = 0
                Width = 97
                Height = 25
                Caption = '&Add Supplier'
                TabOrder = 0
                OnClick = btnAddStockClick
              end
              object btnRenameStock: TButton
                Left = 8
                Top = 64
                Width = 97
                Height = 25
                Caption = '&Rename Supplier'
                TabOrder = 2
                OnClick = btnRenameStockClick
              end
            end
            object lbContacts: TListBox
              Left = 0
              Top = 0
              Width = 201
              Height = 333
              Style = lbOwnerDrawFixed
              Align = alClient
              BevelInner = bvNone
              BevelOuter = bvNone
              Color = clInfoBk
              ExtendedSelect = False
              ItemHeight = 16
              TabOrder = 0
              OnDblClick = lbContactsDblClick
              OnDrawItem = lbContactsDrawItem
            end
          end
          object tsContact: TTabSheet
            ImageIndex = 1
            TabVisible = False
            DesignSize = (
              313
              333)
            object Label16: TLabel
              Left = 8
              Top = 16
              Width = 38
              Height = 13
              Caption = 'Phone:'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
            end
            object Label13: TLabel
              Left = 8
              Top = 32
              Width = 40
              Height = 13
              Caption = 'Mobile:'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
            end
            object Label14: TLabel
              Left = 8
              Top = 48
              Width = 23
              Height = 13
              Caption = 'Fax:'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
            end
            object Label15: TLabel
              Left = 8
              Top = 64
              Width = 38
              Height = 13
              Caption = 'E-mail:'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
            end
            object Label21: TLabel
              Left = 8
              Top = 0
              Width = 82
              Height = 13
              Caption = 'Contact Name:'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
            end
            object Label1: TLabel
              Left = 8
              Top = 80
              Width = 100
              Height = 13
              Caption = 'Location Address:'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
            end
            object Label2: TLabel
              Left = 8
              Top = 156
              Width = 87
              Height = 13
              Caption = 'Postal Address:'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
            end
            object Label3: TLabel
              Left = 8
              Top = 230
              Width = 29
              Height = 13
              Caption = 'Note:'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
            end
            object lbePhone: TLabel
              Left = 136
              Top = 16
              Width = 44
              Height = 13
              Caption = 'lbePhone'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object lbeMobile: TLabel
              Left = 136
              Top = 32
              Width = 44
              Height = 13
              Caption = 'lbeMobile'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object lbeFax: TLabel
              Left = 136
              Top = 48
              Width = 32
              Height = 13
              Caption = 'lbeFax'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object lbeEmail: TLabel
              Left = 136
              Top = 64
              Width = 38
              Height = 13
              Caption = 'lbeEmail'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object lbeContactName: TLabel
              Left = 136
              Top = 0
              Width = 79
              Height = 13
              Caption = 'lbeContactName'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
            end
            object memLocationAddress: TMemo
              Left = 8
              Top = 96
              Width = 297
              Height = 57
              Anchors = [akLeft, akTop, akRight]
              Color = clInfoBk
              ReadOnly = True
              ScrollBars = ssVertical
              TabOrder = 0
            end
            object memBillingAddress: TMemo
              Left = 8
              Top = 172
              Width = 297
              Height = 57
              Anchors = [akLeft, akTop, akRight]
              Color = clInfoBk
              ReadOnly = True
              ScrollBars = ssVertical
              TabOrder = 1
            end
            object memNote: TMemo
              Left = 8
              Top = 248
              Width = 201
              Height = 81
              Anchors = [akLeft, akTop, akRight]
              Color = clInfoBk
              ReadOnly = True
              ScrollBars = ssVertical
              TabOrder = 2
            end
            object btnAdd: TButton
              Left = 216
              Top = 240
              Width = 89
              Height = 25
              Anchors = [akTop, akRight]
              Caption = '&Add Supplier'
              TabOrder = 3
              OnClick = btnAddStockClick
            end
            object btnEdit: TBitBtn
              Left = 216
              Top = 272
              Width = 89
              Height = 25
              Anchors = [akTop, akRight]
              Caption = '&Edit Supplier'
              TabOrder = 4
              OnClick = btnEditStockClick
            end
            object btnDelete: TBitBtn
              Left = 216
              Top = 304
              Width = 89
              Height = 25
              Anchors = [akTop, akRight]
              Caption = '&Delete Supplier'
              TabOrder = 5
              OnClick = btnDeleteStockClick
            end
          end
        end
      end
      object Panel10: TPanel
        Left = 5
        Top = 398
        Width = 614
        Height = 45
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 2
        DesignSize = (
          614
          45)
        object btnClose: TBitBtn
          Left = 512
          Top = 8
          Width = 89
          Height = 33
          Anchors = [akRight, akBottom]
          Caption = '&Close'
          ModalResult = 1
          TabOrder = 2
          Glyph.Data = {
            DE010000424DDE01000000000000760000002800000024000000120000000100
            0400000000006801000000000000000000001000000000000000000000000000
            80000080000000808000800000008000800080800000C0C0C000808080000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00388888888877
            F7F787F8888888888333333F00004444400888FFF444448888888888F333FF8F
            000033334D5007FFF4333388888888883338888F0000333345D50FFFF4333333
            338F888F3338F33F000033334D5D0FFFF43333333388788F3338F33F00003333
            45D50FEFE4333333338F878F3338F33F000033334D5D0FFFF43333333388788F
            3338F33F0000333345D50FEFE4333333338F878F3338F33F000033334D5D0FFF
            F43333333388788F3338F33F0000333345D50FEFE4333333338F878F3338F33F
            000033334D5D0EFEF43333333388788F3338F33F0000333345D50FEFE4333333
            338F878F3338F33F000033334D5D0EFEF43333333388788F3338F33F00003333
            4444444444333333338F8F8FFFF8F33F00003333333333333333333333888888
            8888333F00003333330000003333333333333FFFFFF3333F00003333330AAAA0
            333333333333888888F3333F00003333330000003333333333338FFFF8F3333F
            0000}
          NumGlyphs = 2
        end
        object btnOk: TBitBtn
          Left = 416
          Top = 8
          Width = 89
          Height = 33
          Anchors = [akRight, akBottom]
          TabOrder = 0
          OnClick = btnOkClick
          Kind = bkOK
        end
        object btnCancel: TBitBtn
          Left = 512
          Top = 8
          Width = 89
          Height = 33
          Anchors = [akRight, akBottom]
          TabOrder = 1
          Kind = bkCancel
        end
        object btnFind: TBitBtn
          Left = 8
          Top = 8
          Width = 89
          Height = 33
          Caption = 'Find'
          TabOrder = 3
          OnClick = btnFindClick
          Glyph.Data = {
            42010000424D4201000000000000760000002800000011000000110000000100
            040000000000CC00000000000000000000001000000010000000000000000000
            BF0000BF000000BFBF00BF000000BF00BF00BFBF0000C0C0C000808080000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00777777777777
            77777000000077777777777777777000000070000077777000007000000070B0
            00777770F0007000000070F000777770B0007000000070000000700000007000
            0000700B000000B0000070000000700F000700F0000070000000700B000700B0
            0000700000007700000000000007700000007770B00070B00077700000007770
            0000700000777000000077770007770007777000000077770B07770B07777000
            0000777700077700077770000000777777777777777770000000777777777777
            777770000000}
        end
        object btnImport: TBitBtn
          Left = 276
          Top = 8
          Width = 131
          Height = 33
          Anchors = [akTop, akRight]
          Caption = 'Import From MYOB'
          TabOrder = 4
          OnClick = btnImportClick
        end
      end
    end
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 48
    Top = 72
  end
  object sqlAddLK: TIBSQL
    Database = dmStockData.dbStock
    ParamCheck = False
    SQL.Strings = (
      'select gen_id(gen_contact, 1) from rdb$database')
    Transaction = Transaction
    Left = 112
    Top = 408
  end
  object qrContact: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'select'
      '   CONTACT_LK,'
      '   COMPANY_NAME,'
      '   CONTACT_NAME,'
      '   CONTACT_PHONE,'
      '   EMAIL,'
      '   FAX,'
      '   LOCATION_ADDRESS,'
      '   BILL_ADDRESS,'
      '   MOBILE,'
      '   NOTE,'
      '   PHONE '
      'from'
      '   CONTACT'
      'order by'
      '   COMPANY_NAME')
    Left = 48
    Top = 168
  end
  object pmTreeView: TPopupMenu
    OnPopup = pmTreeViewPopup
    Left = 88
    Top = 16
    object miNew: TMenuItem
      Caption = 'New Contact'
      OnClick = miNewClick
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object miEdit: TMenuItem
      Caption = 'Edit'
      OnClick = miEditClick
    end
    object miRename: TMenuItem
      Caption = 'Rename'
      OnClick = miRenameClick
    end
    object miDelete: TMenuItem
      Caption = 'Delete'
      OnClick = miDeleteClick
    end
  end
  object FindDialog: TFindDialog
    OnFind = FindDialogFind
    Left = 152
    Top = 16
  end
  object qrUpdateContact: TIBSQL
    Database = dmStockData.dbStock
    Transaction = Transaction
    Left = 48
    Top = 216
  end
  object qrFindContact: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select'
      '   Contact_LK'
      'From'
      '   Contact'
      'Where'
      '   Upper(Company_Name) Like :Company_Name and'
      '   Contact_LK <> :Contact_LK')
    Transaction = Transaction
    Left = 48
    Top = 264
  end
  object odImport: TOpenDialog
    DefaultExt = 'txt'
    FileName = 'Suppliers.txt'
    Filter = 'Text Files|*.txt|All Files (*.*)|*.*'
    Options = [ofEnableSizing]
    Left = 232
    Top = 408
  end
  object qrAddContact: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Insert Into Contact ('
      #9'Contact_LK,'
      #9'Contact_Group_Key,'
      #9'Company_Name,'
      #9'Supplier_ID,'
      #9'Contact_Name,'
      #9'Contact_Phone,'
      #9'Phone,'
      #9'FAX,'
      #9'Mobile,'
      #9'EMail,'
      #9'Location_Address,'
      #9'Bill_Address,'
      #9'Note,'
      #9'MYOB1,'
      #9'MYOB2,'
      #9'MYOB3)'
      'Values ('
      #9':Contact_LK,'
      #9':Contact_Group_Key,'
      #9':Company_Name,'
      #9':Supplier_ID,'
      #9':Contact_Name,'
      #9':Contact_Phone,'
      #9':Phone,'
      #9':FAX,'
      #9':Mobile,'
      #9':EMail,'
      #9':Location_Address,'
      #9':Bill_Address,'
      #9':Note,'
      #9':MYOB1,'
      #9':MYOB2,'
      #9':MYOB3)')
    Transaction = Transaction
    Left = 168
    Top = 408
  end
  object qrContactGroup: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Contact_Group_Key,'
      '   Contact_Group'
      'From'
      '      ContactGroup'
      'Order By'
      '      Contact_Group')
    Left = 144
    Top = 168
  end
  object qrAddContactGroup: TIBQuery
    Database = dmStockData.dbStock
    Transaction = Transaction
    SQL.Strings = (
      'Insert Into ContactGroup ('
      '   Contact_Group_Key,'
      '   Contact_Group,'
      '   Sort_Order)'
      'Values ('
      '   :Contact_Group_Key,'
      '   :Contact_Group,'
      '   :Sort_Order)')
    Left = 144
    Top = 216
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Contact_Group_Key'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Contact_Group'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Sort_Order'
        ParamType = ptUnknown
      end>
  end
  object IBSQL1: TIBSQL
    Database = dmStockData.dbStock
    ParamCheck = False
    SQL.Strings = (
      'select gen_id(gen_contact_group_key, 1) from rdb$database')
    Transaction = Transaction
    Left = 144
    Top = 264
  end
end
