object frmMYOBPurchasesExport: TfrmMYOBPurchasesExport
  Left = 295
  Top = 184
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'MYOB Purchases Export'
  ClientHeight = 481
  ClientWidth = 465
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 8
    Top = 8
    Width = 449
    Height = 465
    BorderWidth = 3
    TabOrder = 0
    object Panel4: TPanel
      Left = 4
      Top = 4
      Width = 441
      Height = 412
      Align = alRight
      BevelOuter = bvLowered
      TabOrder = 0
      object Panel7: TPanel
        Left = 1
        Top = 1
        Width = 439
        Height = 48
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        object Label7: TLabel
          Left = 8
          Top = 0
          Width = 425
          Height = 49
          AutoSize = False
          Caption = 'lbeDateTitle'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clCaptionText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
          ShowAccelChar = False
          Layout = tlCenter
          WordWrap = True
        end
        object Label8: TLabel
          Left = 8
          Top = -1
          Width = 425
          Height = 49
          AutoSize = False
          Caption = 'lbeDateTitle'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clCaptionText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
          ShowAccelChar = False
          Layout = tlCenter
          WordWrap = True
        end
        object Label9: TLabel
          Left = 8
          Top = -1
          Width = 425
          Height = 49
          AutoSize = False
          Caption = 'Select the date range for the invoices you wish to export'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clCaptionText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
          ShowAccelChar = False
          Layout = tlCenter
          WordWrap = True
        end
      end
      object pcReportProperties: TPageControl
        Left = 1
        Top = 49
        Width = 439
        Height = 362
        ActivePage = tsInvoices
        Align = alClient
        Style = tsFlatButtons
        TabOrder = 1
        object tsStartTime: TTabSheet
          Caption = 'tsStartTime'
          ImageIndex = 1
          TabVisible = False
          object Label1: TLabel
            Left = 8
            Top = 8
            Width = 197
            Height = 13
            Alignment = taCenter
            AutoSize = False
            Caption = 'Start Date/Time:'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label2: TLabel
            Left = 224
            Top = 8
            Width = 197
            Height = 13
            Alignment = taCenter
            AutoSize = False
            Caption = 'End Date/Time:'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object lbeFrom: TLabel
            Left = 10
            Top = 240
            Width = 197
            Height = 25
            AutoSize = False
            Caption = 'lbeFrom'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            WordWrap = True
          end
          object lbeTo: TLabel
            Left = 226
            Top = 240
            Width = 197
            Height = 25
            AutoSize = False
            Caption = 'lbeTo'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            WordWrap = True
          end
          object Bevel1: TBevel
            Left = 8
            Top = 304
            Width = 417
            Height = 9
            Shape = bsTopLine
          end
          object mcStartDate: TMonthCalendar
            Left = 8
            Top = 24
            Width = 197
            Height = 153
            AutoSize = True
            CalColors.TitleBackColor = clNavy
            Date = 37581.6892304514
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            MaxSelectRange = 365
            ParentFont = False
            TabOrder = 0
            OnClick = mcStartDateClick
            OnExit = mcStartDateClick
          end
          object mcEndDate: TMonthCalendar
            Left = 224
            Top = 24
            Width = 197
            Height = 153
            AutoSize = True
            CalColors.TitleBackColor = clMaroon
            Date = 36795.6892304514
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnClick = mcStartDateClick
            OnExit = mcStartDateClick
          end
          object pnlEndTime: TPanel
            Left = 256
            Top = 184
            Width = 133
            Height = 35
            BevelOuter = bvNone
            BorderStyle = bsSingle
            Caption = '2:00 am'
            Color = clWhite
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object csEndHour: TCSpinButton
            Left = 224
            Top = 184
            Width = 25
            Height = 33
            DownGlyph.Data = {
              BA000000424DBA00000000000000420000002800000009000000060000000100
              1000030000007800000000000000000000000000000000000000007C0000E003
              00001F000000E03DE03DE03DE03DE03DE03DE03DE03DE03D0300E03DE03DE03D
              E03D0000E03DE03DE03DE03D7100E03DE03DE03D000000000000E03DE03DE03D
              7100E03DE03D00000000000000000000E03DE03DE500E03D0000000000000000
              000000000000E03DC381E03DE03DE03DE03DE03DE03DE03DE03DE03DCA03}
            TabOrder = 3
            UpGlyph.Data = {
              BA000000424DBA00000000000000420000002800000009000000060000000100
              1000030000007800000000000000000000000000000000000000007C0000E003
              00001F000000E03DE03DE03DE03DE03DE03DE03DE03DE03D0100E03D00000000
              00000000000000000000E03DBE02E03DE03D00000000000000000000E03DE03D
              0200E03DE03DE03D000000000000E03DE03DE03D0000E03DE03DE03DE03D0000
              E03DE03DE03DE03D5401E03DE03DE03DE03DE03DE03DE03DE03DE03D0200}
            OnDownClick = csEndHourDownClick
            OnUpClick = csEndHourUpClick
          end
          object csEndMinute: TCSpinButton
            Left = 396
            Top = 184
            Width = 25
            Height = 33
            DownGlyph.Data = {
              BA000000424DBA00000000000000420000002800000009000000060000000100
              1000030000007800000000000000000000000000000000000000007C0000E003
              00001F000000E03DE03DE03DE03DE03DE03DE03DE03DE03D0300E03DE03DE03D
              E03D0000E03DE03DE03DE03D7100E03DE03DE03D000000000000E03DE03DE03D
              7100E03DE03D00000000000000000000E03DE03DE500E03D0000000000000000
              000000000000E03DC381E03DE03DE03DE03DE03DE03DE03DE03DE03DCA03}
            TabOrder = 4
            UpGlyph.Data = {
              BA000000424DBA00000000000000420000002800000009000000060000000100
              1000030000007800000000000000000000000000000000000000007C0000E003
              00001F000000E03DE03DE03DE03DE03DE03DE03DE03DE03D0100E03D00000000
              00000000000000000000E03DBE02E03DE03D00000000000000000000E03DE03D
              0200E03DE03DE03D000000000000E03DE03DE03D0000E03DE03DE03DE03D0000
              E03DE03DE03DE03D5401E03DE03DE03DE03DE03DE03DE03DE03DE03D0200}
            OnDownClick = csEndMinuteDownClick
            OnUpClick = csEndMinuteUpClick
          end
          object csStartMinute: TCSpinButton
            Left = 180
            Top = 184
            Width = 25
            Height = 33
            DownGlyph.Data = {
              BA000000424DBA00000000000000420000002800000009000000060000000100
              1000030000007800000000000000000000000000000000000000007C0000E003
              00001F000000E03DE03DE03DE03DE03DE03DE03DE03DE03D0300E03DE03DE03D
              E03D0000E03DE03DE03DE03D7100E03DE03DE03D000000000000E03DE03DE03D
              7100E03DE03D00000000000000000000E03DE03DE500E03D0000000000000000
              000000000000E03DC381E03DE03DE03DE03DE03DE03DE03DE03DE03DCA03}
            TabOrder = 5
            UpGlyph.Data = {
              BA000000424DBA00000000000000420000002800000009000000060000000100
              1000030000007800000000000000000000000000000000000000007C0000E003
              00001F000000E03DE03DE03DE03DE03DE03DE03DE03DE03D0100E03D00000000
              00000000000000000000E03DBE02E03DE03D00000000000000000000E03DE03D
              0200E03DE03DE03D000000000000E03DE03DE03D0000E03DE03DE03DE03D0000
              E03DE03DE03DE03D5401E03DE03DE03DE03DE03DE03DE03DE03DE03D0200}
            OnDownClick = csStartMinuteDownClick
            OnUpClick = csStartMinuteUpClick
          end
          object pnlStartTime: TPanel
            Left = 40
            Top = 184
            Width = 133
            Height = 34
            BevelOuter = bvNone
            BorderStyle = bsSingle
            Caption = '2:00 am'
            Color = clWhite
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
          end
          object csStartHour: TCSpinButton
            Left = 8
            Top = 184
            Width = 25
            Height = 33
            DownGlyph.Data = {
              BA000000424DBA00000000000000420000002800000009000000060000000100
              1000030000007800000000000000000000000000000000000000007C0000E003
              00001F000000E03DE03DE03DE03DE03DE03DE03DE03DE03D0300E03DE03DE03D
              E03D0000E03DE03DE03DE03D7100E03DE03DE03D000000000000E03DE03DE03D
              7100E03DE03D00000000000000000000E03DE03DE500E03D0000000000000000
              000000000000E03DC381E03DE03DE03DE03DE03DE03DE03DE03DE03DCA03}
            TabOrder = 7
            UpGlyph.Data = {
              BA000000424DBA00000000000000420000002800000009000000060000000100
              1000030000007800000000000000000000000000000000000000007C0000E003
              00001F000000E03DE03DE03DE03DE03DE03DE03DE03DE03D0100E03D00000000
              00000000000000000000E03DBE02E03DE03D00000000000000000000E03DE03D
              0200E03DE03DE03D000000000000E03DE03DE03D0000E03DE03DE03DE03D0000
              E03DE03DE03DE03D5401E03DE03DE03DE03DE03DE03DE03DE03DE03D0200}
            OnDownClick = csStartHourDownClick
            OnUpClick = csStartHourUpClick
          end
          object btnDateNext: TBitBtn
            Left = 328
            Top = 312
            Width = 97
            Height = 33
            Caption = 'Next ->'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 8
            OnClick = btnDateNextClick
          end
          object btnThisMonth: TBitBtn
            Left = 216
            Top = 312
            Width = 105
            Height = 33
            Caption = 'Calendar Month'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 9
            OnClick = btnThisMonthClick
          end
          object btnWeek: TBitBtn
            Left = 112
            Top = 312
            Width = 97
            Height = 33
            Caption = 'Week'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 10
            OnClick = btnWeekClick
          end
          object btnThisDay: TBitBtn
            Left = 8
            Top = 312
            Width = 97
            Height = 33
            Caption = 'Day'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 11
            OnClick = btnThisDayClick
          end
        end
        object tsInvoices: TTabSheet
          ImageIndex = 1
          TabVisible = False
          OnShow = tsInvoicesShow
          DesignSize = (
            431
            352)
          object Bevel2: TBevel
            Left = 8
            Top = 304
            Width = 417
            Height = 9
            Shape = bsTopLine
          end
          object lbeFilterRange: TLabel
            Left = 8
            Top = 8
            Width = 417
            Height = 25
            AutoSize = False
            Caption = 'From 12/5/2002 to 13/5/2002'
            WordWrap = True
          end
          object vtvInvoices: TVirtualStringTree
            Left = 8
            Top = 32
            Width = 414
            Height = 260
            Anchors = [akLeft, akTop, akBottom]
            CheckImageKind = ckDarkTick
            Color = clInfoBk
            Header.AutoSizeIndex = 2
            Header.Font.Charset = DEFAULT_CHARSET
            Header.Font.Color = clWindowText
            Header.Font.Height = -11
            Header.Font.Name = 'MS Sans Serif'
            Header.Font.Style = []
            Header.Options = [hoAutoResize, hoColumnResize, hoVisible]
            Images = dmUtilities.imlTreeImages
            StateImages = dmUtilities.imlTreeStateImages
            TabOrder = 0
            TreeOptions.MiscOptions = [toCheckSupport, toEditable, toToggleOnDblClick, toWheelPanning]
            TreeOptions.PaintOptions = [toHideSelection, toShowButtons, toShowRoot, toShowTreeLines, toShowVertGridLines]
            TreeOptions.SelectionOptions = [toDisableDrawSelection]
            TreeOptions.StringOptions = [toAutoAcceptEditChange]
            OnGetText = vtvInvoicesGetText
            OnGetImageIndex = vtvInvoicesGetImageIndex
            Columns = <
              item
                Position = 0
                Width = 220
                WideText = 'Invoice Number'
              end
              item
                Position = 1
                Width = 110
                WideText = 'Date'
              end
              item
                Alignment = taRightJustify
                Position = 2
                Width = 80
                WideText = 'Total'
              end>
            WideDefaultText = ''
          end
          object BitBtn1: TBitBtn
            Left = 328
            Top = 312
            Width = 97
            Height = 33
            Caption = 'Next ->'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
            OnClick = St
          end
          object btnFilterBack: TBitBtn
            Left = 8
            Top = 312
            Width = 97
            Height = 33
            Caption = '<- Back'
            TabOrder = 2
            OnClick = btnFilterBackClick
          end
        end
      end
    end
    object Panel8: TPanel
      Left = 4
      Top = 416
      Width = 441
      Height = 45
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 1
      DesignSize = (
        441
        45)
      object BitBtn5: TBitBtn
        Left = 336
        Top = 8
        Width = 97
        Height = 33
        Anchors = [akTop, akRight]
        TabOrder = 0
        Kind = bkClose
      end
    end
  end
  object Transaction: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 49
    Top = 121
  end
  object SaveDialog: TSaveDialog
    FileName = 'SERVPUR.TXT'
    Left = 152
    Top = 416
  end
  object qrInvoices: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select'
      #9'TransactionBatch.Batch_Key,'
      #9'TransactionBatch.Supplier_Name,'
      #9'TransactionBatch.Supplier_ID,'
      #9'TransactionBatch.Export_1,'
      #9'TransactionBatch.Export_2,'
      #9'TransactionBatch.Export_3,'
      #9'TransactionBatch.Created,'
      #9'TransactionBatch.Reference,'
      #9'TransactionBatch.Total_Cost Invoice_Total_Cost,'
      #9'TransactionBatch.Total_GST Invoice_Total_GST,'
      #9'TransactionBatch.Exported,'
      ''
      #9'StockTrans.GL_CODE,'
      '                StockTrans.Stock_Category,'
      #9'StockTrans.GST_Percent,'
      ''
      #9'Sum(StockTrans.Total_Cost) GL_Total_Cost,'
      #9'Sum(StockTrans.Total_GST) GL_Total_GST'
      'From'
      #9'TransactionBatch Left Join StockTrans On'
      #9#9'TransactionBatch.Batch_Key = StockTrans.Batch_Key'
      'Where'
      #9'TransactionBatch.Transaction_Type = '#39'Receipt'#39' And'
      #9'TransactionBatch.Created >= :StartTime And'
      #9'TransactionBatch.Created < :EndTime'
      'Group By'
      #9'TransactionBatch.Batch_Key,'
      #9'TransactionBatch.Supplier_Name,'
      #9'TransactionBatch.Supplier_ID,'
      #9'TransactionBatch.Export_1,'
      #9'TransactionBatch.Export_2,'
      #9'TransactionBatch.Export_3,'
      #9'TransactionBatch.Created,'
      #9'TransactionBatch.Reference,'
      #9'TransactionBatch.Total_Cost,'
      #9'TransactionBatch.Total_GST,'
      #9'TransactionBatch.Exported,'
      ''
      #9'StockTrans.GL_CODE,'
      #9'StockTrans.Stock_Category,'
      ' '
      #9'StockTrans.GST_Percent'
      'Order By'
      #9'TransactionBatch.Supplier_Name ASC,'
      #9'TransactionBatch.Reference ASC,'
      '                TransactionBatch.Created '
      #9)
    Transaction = Transaction
    Left = 80
    Top = 288
  end
  object qrExported: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Update'
      #9'TransactionBatch'
      'Set'
      #9'Exported = '#39'T'#39
      'Where'
      #9'TransactionBatch.Batch_Key = :BatchKey')
    Transaction = Transaction
    Left = 160
    Top = 288
  end
  object qrInvoicesForXero: TIBSQL
    Database = dmStockData.dbStock
    SQL.Strings = (
      'Select'
      #9'TransactionBatch.Batch_Key,'
      #9'TransactionBatch.Supplier_Name,'
      #9'TransactionBatch.Supplier_ID,'
      #9'TransactionBatch.Export_1,'
      #9'TransactionBatch.Export_2,'
      #9'TransactionBatch.Export_3,'
      #9'TransactionBatch.Created,'
      #9'TransactionBatch.Reference,'
      #9'TransactionBatch.Total_Cost Invoice_Total_Cost,'
      #9'TransactionBatch.Total_GST Invoice_Total_GST,'
      #9'TransactionBatch.Exported,'
      ''
      #9'StockTrans.GL_Code,'
      #9'StockTrans.Stock_Category,'
      ''
      '                StockTrans.Description,'
      '                StockTrans.Order_Qty,'
      ''
      #9'StockTrans.GST_Percent,'
      ''
      #9'Sum(StockTrans.Total_Cost) GL_Total_Cost,'
      #9'Sum(StockTrans.Total_GST) GL_Total_GST'
      'From'
      #9'TransactionBatch Left Join StockTrans On'
      #9#9'TransactionBatch.Batch_Key = StockTrans.Batch_Key'
      'Where'
      #9'TransactionBatch.Transaction_Type = '#39'Receipt'#39' And'
      #9'TransactionBatch.Created >= :StartTime And'
      #9'TransactionBatch.Created < :EndTime'
      'Group By'
      #9'TransactionBatch.Batch_Key,'
      #9'TransactionBatch.Supplier_Name,'
      #9'TransactionBatch.Supplier_ID,'
      #9'TransactionBatch.Export_1,'
      #9'TransactionBatch.Export_2,'
      #9'TransactionBatch.Export_3,'
      #9'TransactionBatch.Created,'
      #9'TransactionBatch.Reference,'
      #9'TransactionBatch.Total_Cost,'
      #9'TransactionBatch.Total_GST,'
      #9'TransactionBatch.Exported,'
      ''
      #9'StockTrans.GL_Code,'
      #9'StockTrans.Stock_Category,'
      ''
      '                StockTrans.Description,'
      '                StockTrans.Order_Qty,'
      ' '
      #9'StockTrans.GST_Percent'
      'Order By'
      #9'TransactionBatch.Supplier_Name,'
      #9'TransactionBatch.Created Desc,'
      #9'TransactionBatch.Reference Desc')
    Transaction = Transaction
    Left = 120
    Top = 288
  end
end
