object frmLocationSelection: TfrmLocationSelection
  Left = 289
  Top = 110
  Width = 286
  Height = 480
  Caption = 'Select Location'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel3: TPanel
    Left = 0
    Top = 0
    Width = 278
    Height = 453
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 4
    TabOrder = 0
    object Panel1: TPanel
      Left = 4
      Top = 4
      Width = 270
      Height = 445
      Align = alClient
      BorderWidth = 4
      TabOrder = 0
      object Panel10: TPanel
        Left = 5
        Top = 395
        Width = 260
        Height = 45
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 0
        DesignSize = (
          260
          45)
        object btnClose: TBitBtn
          Left = 158
          Top = 8
          Width = 89
          Height = 33
          Anchors = [akRight, akBottom]
          Caption = '&Close'
          ModalResult = 1
          TabOrder = 0
          Visible = False
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
      end
      object Panel2: TPanel
        Left = 8
        Top = 8
        Width = 257
        Height = 393
        BorderWidth = 4
        TabOrder = 1
        DesignSize = (
          257
          393)
        object Panel8: TPanel
          Left = 5
          Top = 5
          Width = 247
          Height = 340
          Align = alTop
          Anchors = [akLeft, akTop, akRight, akBottom]
          BevelOuter = bvLowered
          TabOrder = 0
          object Panel9: TPanel
            Left = 1
            Top = 1
            Width = 245
            Height = 48
            Align = alTop
            BevelOuter = bvNone
            Color = clAppWorkSpace
            TabOrder = 0
            object lbeTitle: TLabel
              Left = 16
              Top = 0
              Width = 401
              Height = 49
              AutoSize = False
              Caption = 'Select A Location'
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
          object lstLocation: TListBox
            Left = 11
            Top = 56
            Width = 225
            Height = 273
            Color = 15269887
            ItemHeight = 13
            TabOrder = 1
          end
        end
        object btnOk: TBitBtn
          Left = 16
          Top = 352
          Width = 89
          Height = 33
          Anchors = [akRight, akBottom]
          Caption = '&OK'
          Default = True
          TabOrder = 1
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
          Left = 136
          Top = 352
          Width = 89
          Height = 33
          Anchors = [akRight, akBottom]
          Cancel = True
          Caption = '&Cancel'
          ModalResult = 2
          TabOrder = 2
          OnClick = btnCancelClick
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
    end
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 160
    Top = 16
  end
  object tiTreeScroll: TTimer
    Enabled = False
    Interval = 150
    Left = 128
    Top = 248
  end
  object FindDialog: TFindDialog
    Options = [frDown, frFindNext]
    Left = 40
    Top = 328
  end
  object qrStockCat: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select'
      '   '
      '  distinct StockCategory.Stock_Category '
      ''
      'From'
      '   StockCategory '
      
        '   Left Join StockGroup On StockCategory.Stock_Category_Key = St' +
        'ockGroup.Stock_Category_Key'
      
        '   Left Join Stock On StockGroup.Stock_Group_Key = Stock.Stock_G' +
        'roup_Key'
      
        '   Left Join StockLocation On Stock.Stock_Key = StockLocation.St' +
        'ock_Key'
      
        '    left join SUPPLIERSTOCK on SUPPLIERSTOCK.STOCK_KEY=STOCK.STO' +
        'CK_KEY'
      ''
      '   where StockCategory.Deleted='#39'F'#39' and'
      '   SUPPLIERSTOCK.SUPPLIER_KEY=:SUPPLIER_KEY'
      'Group By'
      '   StockCategory.Stock_Category_Key,'
      '   StockCategory.Stock_Category,'
      '   StockCategory.GL_Code,'
      '   StockCategory.Sort_Order,'
      '   StockCategory.Deleted,'
      ''
      '   StockGroup.Stock_Group_Key,'
      '   StockGroup.Stock_Group,'
      '   StockGroup.GL_Code,'
      '   StockGroup.Stock_Category_Key,'
      '   StockGroup.Sort_Order,'
      '   StockGroup.Deleted,'
      ''
      '   Stock.Stock_Key,'
      '   Stock.Description,'
      '   Stock.Code,'
      '   Stock.Stocktake_Unit,'
      '   Stock.Enabled,'
      '   Stock.Deleted'
      'Order by'
      '  STOCKCATEGORY.SORT_ORDER,'
      '   StockCategory.Stock_Category,'
      '   StockGroup.Stock_Group,'
      '   Stock.Description'
      '')
    Transaction = Transaction
    Left = 200
    Top = 160
  end
  object qrStockLocation: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'SELECT distinct STOCKLOCATION.LOCATION'
      'FROM STOCKLOCATION '
      'where'
      'STOCKLOCATION.LOCATION !='#39#39
      '   ')
    Transaction = Transaction
    Left = 64
    Top = 216
  end
  object Transaction1: TIBTransaction
    DefaultDatabase = dmMMData.dbMenuMate
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 64
    Top = 112
  end
  object sqlLocation: TIBSQL
    Database = dmMMData.dbMenuMate
    SQL.Strings = (
      'Select'
      ' distinct'
      '   Name '
      '  '
      'From'
      '   Locations'
      '   where Location_Type>0'
      'Order By'
      '   Name')
    Transaction = Transaction1
    Left = 112
    Top = 136
  end
end
