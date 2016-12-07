object frmListManager: TfrmListManager
  Tag = 1024
  Left = 446
  Top = 291
  BorderIcons = []
  BorderStyle = bsSingle
  BorderWidth = 5
  ClientHeight = 488
  ClientWidth = 534
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 534
    Height = 488
    Align = alClient
    Color = clWhite
    ParentBackground = False
    TabOrder = 0
    ExplicitWidth = 488
    object Panel2: TPanel
      Left = 1
      Top = 1
      Width = 532
      Height = 486
      Align = alClient
      Color = clGradientInactiveCaption
      ParentBackground = False
      TabOrder = 0
      ExplicitWidth = 486
      object Panel3: TPanel
        Left = 1
        Top = 1
        Width = 530
        Height = 484
        Align = alClient
        Caption = 'Panel3'
        Color = clDefault
        ParentBackground = False
        TabOrder = 0
        ExplicitWidth = 484
        object pnlMajor: TPanel
          Left = 1
          Top = 1
          Width = 382
          Height = 482
          Align = alLeft
          BorderWidth = 6
          Color = 14342874
          ParentBackground = False
          TabOrder = 0
          object pnlLabel: TPanel
            Left = 7
            Top = 7
            Width = 368
            Height = 34
            Align = alTop
            BevelOuter = bvLowered
            Color = 2200566
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -19
            Font.Name = 'Arial'
            Font.Style = []
            ParentBackground = False
            ParentFont = False
            TabOrder = 0
            ExplicitWidth = 324
          end
          object sgDisplay: TStringGrid
            Left = 7
            Top = 41
            Width = 368
            Height = 434
            Align = alClient
            ColCount = 2
            DefaultColWidth = 150
            DefaultRowHeight = 40
            FixedCols = 0
            RowCount = 17
            FixedRows = 0
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            ScrollBars = ssVertical
            TabOrder = 1
            OnDrawCell = sgDisplayDrawCell
            OnSelectCell = sgDisplaySelectCell
            ExplicitWidth = 382
            ColWidths = (
              151
              150)
          end
        end
        object pnlMinor: TPanel
          Left = 390
          Top = 1
          Width = 139
          Height = 482
          Align = alRight
          BevelInner = bvRaised
          BevelOuter = bvNone
          Color = 14342874
          ParentBackground = False
          TabOrder = 1
          ExplicitLeft = 344
          object btnClose: TTouchBtn
            Left = 7
            Top = 8
            Width = 121
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -19
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Ok'
            ButtonColor = 18944
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseUp = btnCloseMouseUp
          end
          object tbtnAdd: TTouchBtn
            Left = 7
            Top = 128
            Width = 124
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -19
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Add'
            ButtonColor = 2200566
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseUp = tbtnAddMouseUp
          end
          object tbtnDelete: TTouchBtn
            Left = 7
            Top = 304
            Width = 124
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -19
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Delete'
            ButtonColor = clMaroon
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseUp = tbtnDeleteMouseUp
          end
          object tbtnEdit: TTouchBtn
            Left = 7
            Top = 216
            Width = 124
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -19
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Caption = 'Edit'
            ButtonColor = clNavy
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseUp = tbtnEditMouseUp
          end
          object btnActive: TTouchBtn
            Left = 7
            Top = 392
            Width = 121
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -19
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            ParentColor = True
            Visible = False
            Caption = 'Active'
            ButtonColor = clTeal
            LatchedColor = clBtnFace
            DisabledButtonColor = clBtnFace
            LatchingProperties = [lkLatchColor, lkStayDown]
            OnMouseUp = btnActiveMouseUp
          end
        end
      end
    end
  end
end
