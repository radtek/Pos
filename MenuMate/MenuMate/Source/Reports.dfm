object frmReports: TfrmReports
  Tag = 1024
  Left = 351
  Top = 281
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 6
  Caption = 'Reports'
  ClientHeight = 330
  ClientWidth = 762
  Color = clBlack
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -20
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  PixelsPerInch = 96
  TextHeight = 24
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 627
    Height = 330
    Align = alLeft
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object btnStock: TTouchBtn
      Left = 17
      Top = 150
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Ariel'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Stock On Hand'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnStockMouseClick
    end
    object btnTransaction: TTouchBtn
      Left = 168
      Top = 8
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Ariel'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Points Transaction'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnTransactionMouseClick
    end
    object btnTransferred: TTouchBtn
      Left = 168
      Top = 79
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Ariel'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Points Transferred'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnTransferredMouseClick
    end
    object btnConsumption: TTouchBtn
      Left = 17
      Top = 8
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Ariel'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Consumption'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnConsumptionMouseClick
    end
    object btnConsumpByMenu: TTouchBtn
      Left = 17
      Top = 79
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Ariel'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Consumption By Menu'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnConsumpByMenuMouseClick
    end
    object btnMemberPoints: TTouchBtn
      Left = 168
      Top = 150
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Ariel'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Points Member Audit (Day)'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnMemberPointsMouseClick
    end
    object btnTabsTables: TTouchBtn
      Left = 319
      Top = 79
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Ariel'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Summary Tabs & Tables'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnTabsTablesMouseClick
    end
    object BtnFloatAdjustments: TTouchBtn
      Left = 470
      Top = 79
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Ariel'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Float Adjustments'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = BtnFloatAdjustmentsMouseClick
    end
    object btnSiteSummary: TTouchBtn
      Left = 319
      Top = 8
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Ariel'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Summary Site'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnSiteSummaryMouseClick
    end
    object btnXReport: TTouchBtn
      Left = 470
      Top = 8
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Ariel'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'X Report'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnXReportMouseClick
    end
    object btnEJournal: TTouchBtn
      Left = 319
      Top = 150
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'E-Journal'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnEJournalMouseClick
    end
    object btnConsolidatedZed: TTouchBtn
      Left = 470
      Top = 150
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Consolidated Zed'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnConsolidatedZedMouseClick
    end
    object btnHavanaReport: TTouchBtn
      Left = 17
      Top = 221
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Ariel'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Havana Report'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnHavanaReportMouseClick
    end
    object btnRoomPaymentReport: TTouchBtn
      Left = 168
      Top = 221
      Width = 145
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Ariel'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'PMS Room Payment'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnbtnRoomPaymentReportMouseClick
    end
  end
  object Panel1: TPanel
    Left = 633
    Top = 0
    Width = 129
    Height = 330
    Align = alRight
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object btnCancel: TTouchBtn
      Left = 9
      Top = 8
      Width = 113
      Height = 65
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Ariel'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCancelMouseClick
    end
  end
end
