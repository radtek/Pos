object frmRMSRoom: TfrmRMSRoom
  Tag = 1024
  Left = 0
  Top = 59
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 4
  Caption = 'Select Member'
  ClientHeight = 632
  ClientWidth = 886
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -17
  Font.Name = 'Comic Sans MS'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 24
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 746
    Height = 632
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 5
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    OnResize = FormResize
    object pnlKeyboard: TPanel
      Left = 5
      Top = 333
      Width = 736
      Height = 294
      Align = alBottom
      BevelOuter = bvNone
      BorderWidth = 2
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = True
      ParentFont = False
      TabOrder = 0
      OnResize = FormResize
      object TouchKeyboard1: TMMTouchKeyboard
        Left = 2
        Top = 2
        Width = 732
        Height = 290
        Align = alClient
        ParentColor = True
        BevelOuter = bvNone
        GridBorderWidth = 8
        ButtonGapWidth = 2
        ButtonGapHeight = 2
        ButtonColor = 6052441
        OnClick = TouchKeyboard1Click
      end
      object TouchNumpad1: TTouchNumpad
        Left = 343
        Top = 2
        Width = 242
        Height = 290
        ParentColor = True
        BorderStyle = bsNone
        GridBorderWidth = 8
        ButtonGapWidth = 4
        ButtonGapHeight = 4
        ButtonColor = 6052441
        CustomKeyCaption = '00'
        OnClick = TouchNumpad2Click
      end
    end
    object Panel4: TPanel
      Left = 248
      Top = 5
      Width = 493
      Height = 328
      Align = alRight
      BevelOuter = bvNone
      BorderWidth = 2
      ParentColor = True
      TabOrder = 1
      OnResize = FormResize
      object Panel3: TPanel
        Left = 2
        Top = 266
        Width = 489
        Height = 60
        Align = alBottom
        BevelOuter = bvNone
        BorderWidth = 3
        ParentColor = True
        TabOrder = 0
        object btnSearch: TTouchBtn
          Left = 298
          Top = 3
          Width = 92
          Height = 54
          Align = alRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Color = 14342874
          ParentFont = False
          Caption = 'Search'
          ButtonColor = 18944
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnSearchMouseClick
        end
        object btnClear: TTouchBtn
          Left = 400
          Top = 3
          Width = 86
          Height = 54
          Align = alRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Color = 14342874
          ParentFont = False
          Caption = 'Clear'
          ButtonColor = clSkyBlue
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          OnMouseClick = btnClearMouseClick
        end
        object edSearch: TMemo
          Left = 3
          Top = 3
          Width = 295
          Height = 54
          Align = alClient
          TabOrder = 2
          WantReturns = False
          WordWrap = False
          OnChange = edSearchChange
          OnKeyPress = edSearchKeyPress
        end
        object Panel7: TPanel
          Left = 390
          Top = 3
          Width = 10
          Height = 54
          Align = alRight
          BevelOuter = bvNone
          TabOrder = 3
        end
      end
      object tpDetails: TTouchPages
        Left = 2
        Top = 2
        Width = 489
        Height = 264
        Align = alClient
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -17
        Font.Name = 'Comic Sans MS'
        Font.Style = []
        ParentFont = False
        ActivePage = tsDetails
        TabHeight = 30
        TabColor = 6052441
        BevelOuter = bvNone
        object tsDetails: TTouchSheet
          Left = 2
          Top = 32
          Width = 485
          Height = 230
          BorderWidth = 2
          Caption = 'Details'
          Color = 6052441
          ParentColor = True
          object lbMemberText: TListBox
            Left = 0
            Top = 0
            Width = 481
            Height = 226
            Align = alClient
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -17
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ItemHeight = 19
            ParentFont = False
            TabOrder = 0
          end
        end
      end
    end
    object pnlList: TPanel
      Left = 5
      Top = 5
      Width = 243
      Height = 328
      Align = alClient
      ParentBackground = False
      ParentColor = True
      TabOrder = 2
      object Panel8: TPanel
        Left = 1
        Top = 271
        Width = 241
        Height = 56
        Align = alBottom
        ParentBackground = False
        ParentColor = True
        TabOrder = 0
        OnResize = Panel8Resize
        object TouchBtn1: TTouchBtn
          Left = 140
          Top = 1
          Width = 100
          Height = 54
          Align = alRight
          ParentColor = True
          ButtonColor = 9933970
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          ArrowType = atDown
          OnAutoRepeat = TouchBtn1AutoRepeat
          InitialDelay = 50
        end
        object TouchBtn2: TTouchBtn
          Left = 1
          Top = 1
          Width = 129
          Height = 54
          Align = alClient
          ParentColor = True
          ButtonColor = 9933970
          LatchedColor = clBtnFace
          DisabledButtonColor = clBtnFace
          LatchingProperties = [lkLatchColor, lkStayDown]
          ArrowType = atUp
          OnAutoRepeat = TouchBtn2AutoRepeat
          InitialDelay = 50
        end
        object Panel6: TPanel
          Left = 130
          Top = 1
          Width = 10
          Height = 54
          Align = alRight
          BevelOuter = bvNone
          TabOrder = 2
        end
      end
      object tlistRooms: TListBox
        Left = 1
        Top = 1
        Width = 241
        Height = 270
        Align = alClient
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Comic Sans MS'
        Font.Style = []
        ItemHeight = 29
        ParentColor = True
        ParentFont = False
        TabOrder = 1
        OnClick = tlistRoomsClick
      end
    end
  end
  object Panel5: TPanel
    Left = 882
    Top = 0
    Width = 4
    Height = 632
    Align = alRight
    BevelOuter = bvNone
    Color = clBlack
    TabOrder = 1
  end
  object Panel16: TPanel
    Left = 751
    Top = 0
    Width = 131
    Height = 632
    Align = alRight
    BevelOuter = bvNone
    BorderWidth = 6
    Color = 14342874
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -17
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentFont = False
    TabOrder = 2
    OnResize = FormResize
    object tbtnCancel: TTouchBtn
      Left = 6
      Top = 566
      Width = 119
      Height = 60
      Align = alBottom
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 2200566
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnCancelClick
    end
    object tbtnOk: TTouchBtn
      Left = 6
      Top = 6
      Width = 119
      Height = 60
      Align = alTop
      ParentColor = True
      Caption = 'Select'
      ButtonColor = 18944
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = btnOkClick
    end
    object tbtnSearchByName: TTouchBtn
      Left = 6
      Top = 306
      Width = 121
      Height = 60
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Name'
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnSearchByNameClick
    end
    object tbtnSearchByNumber: TTouchBtn
      Left = 6
      Top = 370
      Width = 121
      Height = 60
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Number'
      ButtonColor = 9933970
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnSearchByNumberClick
    end
    object tbtnInterface: TTouchBtn
      Left = 6
      Top = 500
      Width = 121
      Height = 60
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Keyboard'
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = tbtnInterfaceMouseClick
    end
  end
  object Panel2: TPanel
    Left = 746
    Top = 0
    Width = 5
    Height = 632
    Align = alRight
    BevelOuter = bvNone
    Color = clBlack
    TabOrder = 3
  end
end
