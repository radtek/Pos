object RemoteBrowseForm: TRemoteBrowseForm
  Left = 170
  Top = 191
  AutoScroll = False
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'RemoteBrowseForm'
  ClientHeight = 385
  ClientWidth = 291
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    291
    385)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 273
    Height = 33
    AutoSize = False
    WordWrap = True
  end
  object TreeView1: TTreeView
    Left = 8
    Top = 48
    Width = 273
    Height = 297
    Anchors = [akLeft, akTop, akRight, akBottom]
    Images = imlTreeImages
    Indent = 19
    ReadOnly = True
    StateImages = imlTreeStateImages
    TabOrder = 0
    OnDblClick = TreeView1DblClick
    OnDeletion = TreeView1Deletion
  end
  object BitBtn1: TBitBtn
    Left = 128
    Top = 352
    Width = 73
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'OK'
    Default = True
    TabOrder = 1
    OnClick = BitBtn1Click
    NumGlyphs = 2
  end
  object BitBtn2: TBitBtn
    Left = 208
    Top = 352
    Width = 73
    Height = 25
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
    NumGlyphs = 2
  end
  object imlTreeImages: TImageList
    BkColor = clWhite
    DrawingStyle = dsTransparent
    Left = 72
    Top = 48
  end
  object imlTreeStateImages: TImageList
    BkColor = clWhite
    Left = 72
    Top = 104
  end
end
