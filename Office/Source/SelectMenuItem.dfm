object frmSelectMenuItem: TfrmSelectMenuItem
  Left = 300
  Top = 163
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Select Side'
  ClientHeight = 458
  ClientWidth = 355
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 355
    Height = 458
    Align = alClient
    BorderWidth = 4
    TabOrder = 0
    object Panel10: TPanel
      Left = 5
      Top = 408
      Width = 345
      Height = 45
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 0
      DesignSize = (
        345
        45)
      object btnOk: TBitBtn
        Left = 152
        Top = 8
        Width = 89
        Height = 33
        Anchors = [akRight, akBottom]
        TabOrder = 0
        Kind = bkOK
      end
      object btnFind: TBitBtn
        Left = 8
        Top = 8
        Width = 89
        Height = 33
        Caption = '&Find'
        TabOrder = 1
        Visible = False
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
      object btnCancel: TBitBtn
        Left = 248
        Top = 8
        Width = 89
        Height = 33
        Anchors = [akRight, akBottom]
        TabOrder = 2
        Kind = bkCancel
      end
    end
    object Panel9: TPanel
      Left = 5
      Top = 5
      Width = 345
      Height = 48
      Align = alTop
      BevelOuter = bvNone
      Color = clAppWorkSpace
      TabOrder = 1
      object lbeTitle: TLabel
        Left = 16
        Top = 0
        Width = 321
        Height = 49
        AutoSize = False
        Caption = 'Select new items to be forced sides.'
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
    object vtvItems: TVirtualStringTree
      Left = 5
      Top = 56
      Width = 345
      Height = 352
      Align = alBottom
      CheckImageKind = ckDarkTick
      Color = clInfoBk
      Header.AutoSizeIndex = -1
      Header.Font.Charset = DEFAULT_CHARSET
      Header.Font.Color = clWindowText
      Header.Font.Height = -11
      Header.Font.Name = 'MS Sans Serif'
      Header.Font.Style = []
      Header.MainColumn = -1
      Header.Options = [hoAutoResize]
      Images = dmUtilities.imlTreeImages
      StateImages = dmUtilities.imlTreeStateImages
      TabOrder = 2
      TreeOptions.MiscOptions = [toCheckSupport, toEditable, toToggleOnDblClick, toWheelPanning]
      TreeOptions.PaintOptions = [toHideSelection, toShowButtons, toShowRoot, toShowTreeLines, toShowVertGridLines]
      TreeOptions.SelectionOptions = [toDisableDrawSelection]
      TreeOptions.StringOptions = [toAutoAcceptEditChange]
      OnGetText = vtvItemsGetText
      OnGetImageIndex = vtvItemsGetImageIndex
      Columns = <>
      WideDefaultText = ''
    end
  end
end
