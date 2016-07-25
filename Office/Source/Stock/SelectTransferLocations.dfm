object frmSelectTransferLocations: TfrmSelectTransferLocations
  Left = 46
  Top = 86
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Select Transfer Locations'
  ClientHeight = 371
  ClientWidth = 499
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
  object Panel2: TPanel
    Left = 8
    Top = 8
    Width = 481
    Height = 353
    BorderWidth = 3
    TabOrder = 0
    object Panel3: TPanel
      Left = 4
      Top = 4
      Width = 473
      Height = 300
      Align = alClient
      BevelInner = bvLowered
      BevelOuter = bvNone
      TabOrder = 0
      object Label5: TLabel
        Left = 8
        Top = 64
        Width = 51
        Height = 13
        Caption = 'Location:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label19: TLabel
        Left = 8
        Top = 80
        Width = 29
        Height = 13
        Caption = 'From'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label3: TLabel
        Left = 248
        Top = 64
        Width = 51
        Height = 13
        Caption = 'Location:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label20: TLabel
        Left = 248
        Top = 80
        Width = 14
        Height = 13
        Caption = 'To'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Panel4: TPanel
        Left = 1
        Top = 1
        Width = 471
        Height = 56
        Align = alTop
        BevelOuter = bvNone
        Color = clAppWorkSpace
        TabOrder = 0
        DesignSize = (
          471
          56)
        object Label6: TLabel
          Left = 8
          Top = 0
          Width = 449
          Height = 57
          Alignment = taCenter
          Anchors = [akLeft, akTop, akRight, akBottom]
          AutoSize = False
          Caption = 'Select the locations you wish to transfer between'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clCaptionText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          Layout = tlCenter
          WordWrap = True
        end
      end
      object lbSource: TListBox
        Left = 72
        Top = 64
        Width = 161
        Height = 225
        Color = clInfoBk
        ItemHeight = 13
        Sorted = True
        TabOrder = 1
      end
      object lbDestination: TListBox
        Left = 304
        Top = 64
        Width = 161
        Height = 225
        Color = clInfoBk
        ItemHeight = 13
        Sorted = True
        TabOrder = 2
      end
    end
    object Panel5: TPanel
      Left = 4
      Top = 304
      Width = 473
      Height = 45
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 1
      DesignSize = (
        473
        45)
      object btnOk: TBitBtn
        Left = 296
        Top = 8
        Width = 81
        Height = 33
        Anchors = [akTop, akRight]
        Caption = 'OK'
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
        Left = 384
        Top = 8
        Width = 81
        Height = 33
        Anchors = [akTop, akRight]
        TabOrder = 1
        Kind = bkCancel
      end
    end
  end
end
