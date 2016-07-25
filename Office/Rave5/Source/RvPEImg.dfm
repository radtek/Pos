object ImageEditor: TImageEditor
  Left = 310
  Top = 132
  ClientWidth = 400
  ClientHeight = 330
  HorzScrollBar.Range = 85
  ActiveControl = LoadBtn
  BorderStyle = bsSizeToolWin
  Caption = 'Image Selector'
  Color = clBtnFace
  Constraints.MinHeight = 330
  Constraints.MinWidth = 400
  Font.Color = clBtnText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Pitch = fpVariable
  Font.Style = []
  ParentFont = False
  Scaled = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 315
    Height = 330
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 10
    TabOrder = 0
    object Panel3: TPanel
      Left = 10
      Top = 10
      Width = 295
      Height = 310
      Align = alClient
      BevelInner = bvRaised
      BevelOuter = bvLowered
      Caption = '.'
      TabOrder = 0
      object Panel4: TPanel
        Left = 2
        Top = 2
        Width = 291
        Height = 306
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 10
        Caption = '.'
        TabOrder = 0
        object Panel5: TPanel
          Left = 10
          Top = 263
          Width = 271
          Height = 33
          Align = alBottom
          BevelOuter = bvNone
          TabOrder = 0
          object LoadBtn: TBitBtn
            Left = 8
            Top = 8
            Width = 75
            Height = 25
            Caption = '&Open...'
            TabOrder = 0
            OnClick = LoadBtnClick
          end
          object ClearBtn: TBitBtn
            Left = 96
            Top = 8
            Width = 75
            Height = 25
            Caption = '&Clear'
            TabOrder = 1
            OnClick = ClearBtnClick
          end
          object bbtnSaveAs: TBitBtn
            Left = 184
            Top = 8
            Width = 75
            Height = 25
            Caption = '&Save As...'
            TabOrder = 2
            OnClick = bbtnSaveAsClick
          end
        end
        object BgPanel: TPanel
          Left = 10
          Top = 10
          Width = 271
          Height = 253
          Align = alClient
          BevelInner = bvLowered
          BevelOuter = bvLowered
          Caption = 'None'
          Color = clWindow
          ParentColor = False
          TabOrder = 1
          object DispImage: TImage
            Left = 2
            Top = 2
            Width = 267
            Height = 249
            Align = alClient
            Center = True
          end
        end
      end
    end
  end
  object RtBtnPanel: TPanel
    Left = 315
    Top = 0
    Width = 77
    Height = 330
    Align = alRight
    BevelOuter = bvNone
    TabOrder = 1
    object OkBtn: TBitBtn
      Left = 3
      Top = 11
      Width = 70
      Height = 25
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 0
      NumGlyphs = 2
    end
    object CancelBtn: TBitBtn
      Left = 3
      Top = 44
      Width = 70
      Height = 25
      Cancel = True
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 1
      NumGlyphs = 2
    end
  end
  object Panel2: TPanel
    Left = 392
    Top = 0
    Width = 8
    Height = 330
    Align = alRight
    BevelOuter = bvNone
    TabOrder = 2
  end
  object sdlgSaveAs: TSaveDialog
    FilterIndex = 0
    Title = 'Save'
    Left = 349
    Top = 264
  end
  object OpenDlg: TOpenDialog
    FilterIndex = 0
    Left = 352
    Top = 208
  end
end