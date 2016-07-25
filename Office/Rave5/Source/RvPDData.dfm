object DataViewerForm: TDataViewerForm
  Left = 337
  Top = 149
  Width = 750
  Height = 496
  Caption = 'DataViewerForm'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object DataViewerPanel: TPanel
    Left = 0
    Top = 0
    Width = 742
    Height = 469
    Align = alClient
    BevelOuter = bvNone
    Caption = 'Please select a DataView...'
    TabOrder = 0
    object DataViewerGrid: TStringGrid
      Left = 0
      Top = 32
      Width = 742
      Height = 437
      Align = alClient
      DefaultRowHeight = 16
      RowCount = 2
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect, goThumbTracking]
      TabOrder = 0
    end
    object TitlePanel: TPanel
      Left = 0
      Top = 0
      Width = 742
      Height = 32
      Align = alTop
      BevelOuter = bvNone
      Color = clBackground
      TabOrder = 1
      object DataViewNameLabel: TLabel
        Left = 32
        Top = 1
        Width = 224
        Height = 29
        Caption = 'RvDirectDataView1'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -24
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Image1: TImage
        Left = 8
        Top = 8
        Width = 16
        Height = 16
        Picture.Data = {
          07544269746D6170F6000000424DF60000000000000076000000280000001000
          0000100000000100040000000000800000000000000000000000100000000000
          0000000000000000800000800000008080008000000080008000808000008080
          8000C0C0C0000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFF
          FF00777777777777777777770000000077777700055555500077705555555555
          5507705555555555550770555555555555077055555555555507705555555555
          550770555555555555077055555555555507705500000000550770000DDDDDD0
          000770DDDDDDDDDDDD0777000DDDDDD000777777000000007777777777777777
          7777}
        Transparent = True
      end
    end
  end
end
