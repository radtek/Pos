object frmDebtors: TfrmDebtors
  Left = 201
  Top = 106
  AutoScroll = False
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Debtors'
  ClientHeight = 521
  ClientWidth = 630
  Color = clBtnFace
  Constraints.MinHeight = 548
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 630
    Height = 521
    Align = alClient
    BorderWidth = 4
    TabOrder = 0
    object Splitter1: TSplitter
      Left = 202
      Top = 5
      Width = 4
      Height = 466
      Cursor = crHSplit
      Align = alRight
      Color = clBtnFace
      ParentColor = False
      ResizeStyle = rsUpdate
    end
    object Panel8: TPanel
      Left = 206
      Top = 5
      Width = 419
      Height = 466
      Align = alRight
      BevelOuter = bvLowered
      Constraints.MinWidth = 330
      TabOrder = 0
      object Panel9: TPanel
        Left = 1
        Top = 1
        Width = 417
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
      object pcDebtors: TPageControl
        Left = 8
        Top = 56
        Width = 401
        Height = 401
        Hint = 'The sales unit refers to the measure deducted by sales.'
        ActivePage = tsDebtors
        MultiLine = True
        TabIndex = 1
        TabOrder = 1
        object tsCompanies: TTabSheet
          Caption = 'Company Details'
          DesignSize = (
            393
            373)
          object btnAddCompany: TButton
            Left = 8
            Top = 332
            Width = 121
            Height = 33
            Anchors = [akLeft, akBottom]
            Caption = 'Add Company'
            TabOrder = 0
            OnClick = btnAddCompanyClick
          end
          object btnEditCompany: TBitBtn
            Left = 136
            Top = 332
            Width = 121
            Height = 33
            Anchors = [akLeft, akBottom]
            Caption = '&Edit Company'
            TabOrder = 1
            OnClick = btnEditClick
          end
          object btnDeleteCompany: TBitBtn
            Left = 264
            Top = 332
            Width = 121
            Height = 33
            Anchors = [akLeft, akBottom]
            Caption = 'Delete Company'
            TabOrder = 2
            Visible = False
          end
          object pnlCompanyDetails: TPanel
            Left = 0
            Top = 0
            Width = 393
            Height = 329
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 3
            object Label19: TLabel
              Left = 8
              Top = 16
              Width = 91
              Height = 13
              Caption = 'Company Name:'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
            end
            object lbeCompanyName: TLabel
              Left = 128
              Top = 16
              Width = 257
              Height = 13
              AutoSize = False
              Caption = 'lbeCompanyName'
            end
            object lbeCompanyPhone: TLabel
              Left = 128
              Top = 48
              Width = 121
              Height = 13
              AutoSize = False
              Caption = 'lbeCompanyPhone'
            end
            object Label20: TLabel
              Left = 8
              Top = 48
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
            object Label22: TLabel
              Left = 8
              Top = 64
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
            object Label23: TLabel
              Left = 8
              Top = 80
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
            object Label21: TLabel
              Left = 8
              Top = 96
              Width = 37
              Height = 13
              Caption = 'E-Mail:'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
            end
            object Label24: TLabel
              Left = 8
              Top = 128
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
            object Label25: TLabel
              Left = 8
              Top = 200
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
            object Label26: TLabel
              Left = 8
              Top = 272
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
            object lbeCompanyEMail: TLabel
              Left = 128
              Top = 96
              Width = 257
              Height = 13
              AutoSize = False
              Caption = 'Label1'
            end
            object lbeCompanyMobile: TLabel
              Left = 128
              Top = 80
              Width = 121
              Height = 13
              AutoSize = False
              Caption = 'Label1'
            end
            object lbeCompanyFax: TLabel
              Left = 128
              Top = 64
              Width = 121
              Height = 13
              AutoSize = False
              Caption = 'lbeCompanyFax'
            end
            object memCompanyNote: TMemo
              Left = 128
              Top = 272
              Width = 257
              Height = 49
              Color = clInfoBk
              ScrollBars = ssVertical
              TabOrder = 0
              WordWrap = False
            end
            object memCompanyPostalAddress: TMemo
              Left = 128
              Top = 200
              Width = 257
              Height = 65
              Color = clInfoBk
              ScrollBars = ssVertical
              TabOrder = 1
              WordWrap = False
            end
            object memCompanyLocationAddress: TMemo
              Left = 128
              Top = 128
              Width = 257
              Height = 65
              Color = clInfoBk
              ScrollBars = ssVertical
              TabOrder = 2
              WordWrap = False
            end
          end
        end
        object tsDebtors: TTabSheet
          Caption = 'Debtor Details'
          ImageIndex = 2
          DesignSize = (
            393
            373)
          object btnDeleteDebtor: TBitBtn
            Left = 264
            Top = 332
            Width = 121
            Height = 33
            Anchors = [akLeft, akBottom]
            Caption = '&Delete Debtor'
            TabOrder = 0
            OnClick = btnDeleteDebtorClick
          end
          object btnEditDebtor: TBitBtn
            Left = 136
            Top = 332
            Width = 121
            Height = 33
            Anchors = [akLeft, akBottom]
            Caption = '&Edit Debtor'
            TabOrder = 1
            OnClick = btnEditClick
          end
          object btnAddDebtor: TButton
            Left = 8
            Top = 332
            Width = 121
            Height = 33
            Anchors = [akLeft, akBottom]
            Caption = '&Add Debtor'
            TabOrder = 2
            OnClick = btnAddDebtorClick
          end
          object pnlDebtorDetails: TPanel
            Left = 0
            Top = 0
            Width = 393
            Height = 329
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 3
            object lbeFirstName: TLabel
              Left = 128
              Top = 16
              Width = 257
              Height = 13
              AutoSize = False
              Caption = 'lbeFirstName'
            end
            object lbeMiddleName: TLabel
              Left = 128
              Top = 32
              Width = 257
              Height = 13
              AutoSize = False
              Caption = 'lbeMiddleName'
            end
            object lbeLastName: TLabel
              Left = 128
              Top = 48
              Width = 257
              Height = 13
              AutoSize = False
              Caption = 'lbeLastName'
            end
            object lbeDebtorPhone: TLabel
              Left = 128
              Top = 96
              Width = 121
              Height = 13
              AutoSize = False
              Caption = 'lbeDebtorPhone'
            end
            object lbeDebtorFax: TLabel
              Left = 128
              Top = 112
              Width = 121
              Height = 13
              AutoSize = False
              Caption = 'lbeDebtorFax'
            end
            object lbeDebtorMobile: TLabel
              Left = 128
              Top = 128
              Width = 121
              Height = 13
              AutoSize = False
              Caption = 'lbeDebtorMobile'
            end
            object lbeDebtorEMail: TLabel
              Left = 128
              Top = 144
              Width = 257
              Height = 13
              AutoSize = False
              Caption = 'lbeDebtorEMail'
            end
            object Label11: TLabel
              Left = 8
              Top = 144
              Width = 37
              Height = 13
              Caption = 'E-Mail:'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
            end
            object Label13: TLabel
              Left = 8
              Top = 128
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
            object Label12: TLabel
              Left = 8
              Top = 112
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
            object Label10: TLabel
              Left = 8
              Top = 96
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
            object Label18: TLabel
              Left = 8
              Top = 48
              Width = 62
              Height = 13
              Caption = 'Last Name:'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
            end
            object Label17: TLabel
              Left = 8
              Top = 32
              Width = 75
              Height = 13
              Caption = 'Middle Name:'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
            end
            object Label16: TLabel
              Left = 8
              Top = 16
              Width = 63
              Height = 13
              Caption = 'First Name:'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
            end
            object Label14: TLabel
              Left = 8
              Top = 176
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
            object Label15: TLabel
              Left = 8
              Top = 248
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
            object Label4: TLabel
              Left = 256
              Top = 96
              Width = 21
              Height = 13
              Caption = 'Ext:'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
            end
            object lbeDebtorPhoneExt: TLabel
              Left = 288
              Top = 96
              Width = 97
              Height = 13
              AutoSize = False
              Caption = 'Label1'
            end
            object Label1: TLabel
              Left = 8
              Top = 72
              Width = 70
              Height = 13
              Caption = 'Customer #:'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
            end
            object lbeCustomerNumber: TLabel
              Left = 128
              Top = 72
              Width = 61
              Height = 13
              Caption = 'Customer #:'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
              Visible = False
            end
            object memDebtorPostalAddress: TMemo
              Left = 128
              Top = 248
              Width = 257
              Height = 65
              Color = clInfoBk
              ScrollBars = ssVertical
              TabOrder = 0
              WordWrap = False
            end
            object memDebtorLocationAddress: TMemo
              Left = 128
              Top = 176
              Width = 257
              Height = 65
              Color = clInfoBk
              ScrollBars = ssVertical
              TabOrder = 1
              WordWrap = False
            end
          end
        end
      end
    end
    object Panel10: TPanel
      Left = 5
      Top = 471
      Width = 620
      Height = 45
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 1
      DesignSize = (
        620
        45)
      object btnClose: TBitBtn
        Left = 525
        Top = 8
        Width = 89
        Height = 33
        Anchors = [akRight, akBottom]
        Caption = '&Close'
        ModalResult = 1
        TabOrder = 0
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
      object btnFind: TBitBtn
        Left = 8
        Top = 8
        Width = 89
        Height = 33
        Caption = '&Find'
        TabOrder = 1
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
      object chbShowCompanies: TCheckBox
        Left = 120
        Top = 16
        Width = 121
        Height = 17
        Caption = 'Sort By Company'
        TabOrder = 2
        OnClick = chbShowCompaniesClick
      end
      object btnOk: TBitBtn
        Left = 429
        Top = 8
        Width = 89
        Height = 33
        Anchors = [akRight, akBottom]
        Caption = '&OK'
        Default = True
        TabOrder = 3
        OnClick = btnOkClick
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
      object btnCancel: TBitBtn
        Left = 525
        Top = 8
        Width = 89
        Height = 33
        Anchors = [akRight, akBottom]
        Cancel = True
        Caption = '&Cancel'
        ModalResult = 2
        TabOrder = 4
        Glyph.Data = {
          DE010000424DDE01000000000000760000002800000024000000120000000100
          0400000000006801000000000000000000001000000000000000000000000000
          80000080000000808000800000008000800080800000C0C0C000808080000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
          333333333333333333333333000033338833333333333333333F333333333333
          0000333911833333983333333388F333333F3333000033391118333911833333
          38F38F333F88F33300003339111183911118333338F338F3F8338F3300003333
          911118111118333338F3338F833338F3000033333911111111833333338F3338
          3333F8330000333333911111183333333338F333333F83330000333333311111
          8333333333338F3333383333000033333339111183333333333338F333833333
          00003333339111118333333333333833338F3333000033333911181118333333
          33338333338F333300003333911183911183333333383338F338F33300003333
          9118333911183333338F33838F338F33000033333913333391113333338FF833
          38F338F300003333333333333919333333388333338FFF830000333333333333
          3333333333333333333888330000333333333333333333333333333333333333
          0000}
        NumGlyphs = 2
      end
    end
    object tvDebtors: TTreeView
      Left = 5
      Top = 5
      Width = 197
      Height = 466
      Align = alClient
      BevelInner = bvNone
      BevelOuter = bvNone
      Color = clInfoBk
      Constraints.MinWidth = 32
      DragCursor = crDefault
      DragMode = dmAutomatic
      HideSelection = False
      Images = dmUtilities.imlTreeImages
      Indent = 19
      PopupMenu = pmTreeView
      ReadOnly = True
      RightClickSelect = True
      TabOrder = 2
      OnChange = tvDebtorsChange
      OnDblClick = tvDebtorsDblClick
      OnMouseDown = tvDebtorsMouseDown
    end
  end
  object qrDebtors: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      '   Contacts.Contacts_Key,'
      '   Contacts.Name,'
      '   Contacts.First_Name,'
      '   Contacts.Middle_Name,'
      '   Contacts.Last_Name,'
      '   Contacts.Phone,'
      '   Contacts.Phone_Extension,'
      '   Contacts.Mobile,'
      '   Contacts.Email,'
      '   Contacts.FAX,'
      '   Contacts.Location_Address,'
      '   Contacts.Mailing_Address,'
      ''
      '   Companies.Company_Key,'
      '   Companies.Company_Name,'
      '   Companies.Phone Company_Phone,'
      '   Companies.Fax Company_Fax,'
      '   Companies.Mobile Company_Mobile,'
      '   Companies.Email Company_Email,'
      '   Companies.Location_Address Company_Location_Address,'
      '   Companies.Mailing_Address Company_Mailing_Address,'
      '   Companies.Note Company_Note'
      'From'
      '   Contacts Left Join Companies On'
      '      Contacts.Company_Key = Companies.Company_Key'
      'Where'
      '   Contact_Type = 2'
      'Order By'
      '   Company_Name,'
      '   Name')
    Left = 40
    Top = 24
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmMMData.dbMenuMate
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 120
    Top = 24
  end
  object pmTreeView: TPopupMenu
    Left = 104
    Top = 232
    object miNewCompany: TMenuItem
      Caption = 'New Company'
      OnClick = miNewCompanyClick
    end
    object miNewDebtor: TMenuItem
      Caption = 'New Debtor'
      OnClick = miNewDebtorClick
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
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object miDelete: TMenuItem
      Caption = 'Delete'
      OnClick = btnDeleteDebtorClick
    end
  end
  object FindDialog: TFindDialog
    Options = [frDown, frFindNext]
    OnFind = FindDialogFind
    Left = 80
    Top = 480
  end
  object qrDeleteDebtor: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = Transaction
    SQL.Strings = (
      'Update'
      #9'Contacts'
      'Set'
      #9'Name = :Name,'
      #9'Contact_Type = Contact_Type + 1'
      'Where'
      #9'Contacts_Key = :Contacts_Key')
    Left = 40
    Top = 120
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Name'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Contacts_Key'
        ParamType = ptUnknown
      end>
  end
  object qrDebtorInvoices: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = Transaction
    SQL.Strings = (
      'Select'
      #9'Orders.Order_Key'
      'From'
      #9'Contacts Inner Join Invoices on'
      #9#9'Contacts.Contacts_Key = Invoices.Contacts_Key'
      #9'Inner Join Tab on'
      #9#9'Invoices.Invoice_Key = Tab.Invoice_Key'
      #9'Left Join Orders on'
      #9#9'Tab.Tab_Key = Orders.Tab_Key'
      'Where'
      #9'Invoices.Closed <> '#39'T'#39' and'
      #9'Contacts.Contacts_Key = :Contacts_Key')
    Left = 40
    Top = 72
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Contacts_Key'
        ParamType = ptUnknown
      end>
  end
  object sqlCheckDebtors: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'select count(CONTACT_TYPE) as CntContactType from CONTACTS where'
      ' CONTACT_TYPE = 4')
    Transaction = Transaction
    Left = 136
    Top = 96
  end
end
