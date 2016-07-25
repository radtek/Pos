object frmDealEditorForm: TfrmDealEditorForm
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 6
  Caption = 'Editing Deal'
  ClientHeight = 262
  ClientWidth = 409
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object BackgroundPanel: TPanel
    Left = 0
    Top = 0
    Width = 266
    Height = 262
    BevelOuter = bvNone
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    object btnName: TTouchBtn
      Left = 8
      Top = 8
      Width = 250
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Deal Name'
      ButtonColor = 6315356
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnNameMouseClick
    end
    object btnPriority: TTouchBtn
      Left = 8
      Top = 71
      Width = 250
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Priority'
      ButtonColor = 6315356
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnPriorityMouseClick
    end
    object btnCategory: TTouchBtn
      Left = 8
      Top = 134
      Width = 250
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Category'
      ButtonColor = 6315356
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCategoryMouseClick
    end
    object btnPriceLevels: TTouchBtn
      Left = 8
      Top = 197
      Width = 250
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Price Levels'
      ButtonColor = 6315356
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnPriceLevelsMouseClick
    end
  end
  object OkCancelPanel: TPanel
    Left = 272
    Top = 0
    Width = 137
    Height = 262
    BevelOuter = bvNone
    Color = 14342874
    ParentBackground = False
    TabOrder = 1
    object btnOk: TTouchBtn
      Left = 8
      Top = 8
      Width = 121
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Ok'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnOkMouseClick
    end
    object btnCancel: TTouchBtn
      Left = 8
      Top = 197
      Width = 121
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 39157
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnCancelMouseClick
    end
  end
end
