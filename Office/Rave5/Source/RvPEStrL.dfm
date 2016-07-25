object StringsDialog: TStringsDialog
  Left = 311
  Top = 210
  ClientWidth = 300
  ClientHeight = 240
  VertScrollBar.Range = 41
  ActiveControl = Memo1
  BorderStyle = bsSizeToolWin
  Caption = 'Strings Editor'
  Color = clBtnFace
  Constraints.MinHeight = 240
  Constraints.MinWidth = 300
  Font.Color = clBtnText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Pitch = fpVariable
  Font.Style = []
  FormStyle = fsStayOnTop
  KeyPreview = True
  ParentFont = False
  Position = poMainFormCenter
  Scaled = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  PixelsPerInch = 75
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 300
    Height = 199
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 5
    TabOrder = 0
    object Panel3: TPanel
      Left = 5
      Top = 5
      Width = 290
      Height = 189
      Align = alClient
      BevelOuter = bvLowered
      TabOrder = 0
      object Panel4: TPanel
        Left = 1
        Top = 1
        Width = 288
        Height = 187
        Align = alClient
        BorderWidth = 2
        TabOrder = 0
        object StatusLine: TPanel
          Left = 3
          Top = 3
          Width = 282
          Height = 24
          Align = alTop
          Alignment = taLeftJustify
          BevelOuter = bvNone
          Caption = '@1 line'
          TabOrder = 0
        end
        object Panel6: TPanel
          Left = 3
          Top = 27
          Width = 282
          Height = 157
          Align = alClient
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 1
          object Memo1: TMemo
            Left = 4
            Top = 4
            Width = 274
            Height = 149
            Align = alClient
            ScrollBars = ssBoth
            TabOrder = 0
            OnChange = Memo1Change
          end
        end
      end
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 199
    Width = 300
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    object BitBtn1: TBitBtn
      Left = 220
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Cancel = True
      Caption = '&Cancel'
      ModalResult = 2
      TabOrder = 0
      NumGlyphs = 2
    end
    object BitBtn2: TBitBtn
      Left = 132
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = '&OK'
      Default = True
      ModalResult = 1
      TabOrder = 1
      NumGlyphs = 2
    end
  end
end