object BarcodeSyncForm: TBarcodeSyncForm
  Left = 437
  Top = 163
  AutoScroll = False
  Caption = 'BarcodeSyncForm'
  ClientHeight = 416
  ClientWidth = 393
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnShow = FormShow
  DesignSize = (
    393
    416)
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl: TPageControl
    Left = 8
    Top = 8
    Width = 377
    Height = 399
    ActivePage = BarcodesTabSheet
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 0
    object MenusTabSheet: TTabSheet
      Caption = 'MenusTabSheet'
      TabVisible = False
      DesignSize = (
        369
        389)
      object Label1: TLabel
        Left = 8
        Top = 8
        Width = 153
        Height = 13
        Caption = 'Select the menus to synchronize'
      end
      object Menus: TCheckListBox
        Left = 8
        Top = 24
        Width = 353
        Height = 311
        Anchors = [akLeft, akTop, akRight, akBottom]
        ItemHeight = 13
        TabOrder = 0
      end
      object CancelButton2: TButton
        Left = 280
        Top = 346
        Width = 81
        Height = 33
        Anchors = [akRight, akBottom]
        Caption = 'Cancel'
        TabOrder = 1
        OnClick = CancelButtonClicked
      end
      object NextButton: TButton
        Left = 192
        Top = 346
        Width = 81
        Height = 33
        Anchors = [akRight, akBottom]
        Caption = 'Next ->'
        TabOrder = 2
        OnClick = NextButtonClick
      end
    end
    object WaitingTabSheet: TTabSheet
      Caption = 'WaitingTabSheet'
      ImageIndex = 1
      TabVisible = False
      object PleaseWaitLabel: TLabel
        Left = 0
        Top = 0
        Width = 369
        Height = 389
        Align = alClient
        Alignment = taCenter
        AutoSize = False
        Caption = 'Please Wait...'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Layout = tlCenter
      end
    end
    object BarcodesTabSheet: TTabSheet
      Caption = 'BarcodesTabSheet'
      ImageIndex = 2
      TabVisible = False
      DesignSize = (
        369
        389)
      object Label2: TLabel
        Left = 8
        Top = 8
        Width = 121
        Height = 13
        Caption = 'Barcodess to synchronize'
      end
      object CancelButton1: TButton
        Left = 280
        Top = 346
        Width = 81
        Height = 33
        Anchors = [akRight, akBottom]
        Caption = 'Cancel'
        TabOrder = 0
        OnClick = CancelButtonClicked
      end
      object SyncItems: TListView
        Left = 8
        Top = 24
        Width = 353
        Height = 313
        Anchors = [akLeft, akTop, akRight, akBottom]
        Columns = <
          item
            Caption = 'Menu Item'
            Width = 120
          end
          item
            Caption = 'Stock Item'
            Width = 120
          end
          item
            Caption = 'Barcode'
            Width = 100
          end>
        ReadOnly = True
        RowSelect = True
        TabOrder = 1
        ViewStyle = vsReport
      end
      object DoneButton: TButton
        Left = 192
        Top = 346
        Width = 81
        Height = 33
        Anchors = [akRight, akBottom]
        Caption = 'Ok'
        TabOrder = 2
        OnClick = DoneButtonClick
      end
    end
  end
end
