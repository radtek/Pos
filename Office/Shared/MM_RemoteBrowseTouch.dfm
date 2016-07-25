object RemoteBrowseTouchForm: TRemoteBrowseTouchForm
  Left = 210
  Top = 139
  Width = 824
  Height = 643
  BorderWidth = 8
  Caption = 'Browse For Files'
  Color = clBlack
  Constraints.MinHeight = 643
  Constraints.MinWidth = 824
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -24
  Font.Name = 'Tahoma'
  Font.Style = [fsBold]
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 29
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 800
    Height = 600
    Align = alClient
    Color = clMoneyGreen
    TabOrder = 0
    DesignSize = (
      800
      600)
    object HostLabel: TLabel
      Left = 72
      Top = 104
      Width = 193
      Height = 49
      AutoSize = False
      Caption = 'HostLabel'
      ShowAccelChar = False
      Layout = tlCenter
    end
    object FolderList: TListBox
      Left = 8
      Top = 160
      Width = 385
      Height = 433
      Style = lbOwnerDrawFixed
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'Tahoma'
      Font.Style = []
      ItemHeight = 40
      ParentFont = False
      TabOrder = 0
      OnClick = FolderListClick
      OnDrawItem = FolderListDrawItem
    end
    object FilesList: TListBox
      Left = 400
      Top = 160
      Width = 393
      Height = 361
      Style = lbOwnerDrawFixed
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'Tahoma'
      Font.Style = []
      ItemHeight = 40
      ParentFont = False
      TabOrder = 1
      OnClick = FilesListClick
      OnDrawItem = FolderListDrawItem
    end
    object TouchBtn1: TTouchBtn
      Left = 480
      Top = 536
      Width = 153
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -24
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'OK'
      ButtonColor = 1087568
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn1MouseClick
    end
    object TouchBtn2: TTouchBtn
      Left = 640
      Top = 536
      Width = 153
      Height = 56
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -24
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Cancel'
      ButtonColor = 1073320
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn2MouseClick
    end
    object TouchBtn4: TTouchBtn
      Left = 336
      Top = 104
      Width = 57
      Height = 49
      ParentColor = True
      ButtonColor = clInactiveBorder
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      ArrowType = atDown
      OnAutoRepeat = TouchBtn4MouseClick
      InitialDelay = 250
      RepeatDelay = 25
    end
    object TouchBtn5: TTouchBtn
      Left = 272
      Top = 104
      Width = 57
      Height = 49
      ParentColor = True
      ButtonColor = clInactiveBorder
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      ArrowType = atUp
      OnAutoRepeat = TouchBtn5MouseClick
      InitialDelay = 250
      RepeatDelay = 25
    end
    object TouchBtn6: TTouchBtn
      Left = 672
      Top = 104
      Width = 57
      Height = 49
      ParentColor = True
      ButtonColor = clInactiveBorder
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      ArrowType = atUp
      OnAutoRepeat = TouchBtn6MouseClick
      InitialDelay = 250
      RepeatDelay = 25
    end
    object TouchBtn7: TTouchBtn
      Left = 736
      Top = 104
      Width = 57
      Height = 49
      ParentColor = True
      ButtonColor = clInactiveBorder
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      ArrowType = atDown
      OnAutoRepeat = TouchBtn7MouseClick
      InitialDelay = 250
      RepeatDelay = 25
    end
    object TouchBtn3: TTouchBtn
      Left = 400
      Top = 104
      Width = 129
      Height = 49
      ParentColor = True
      Caption = 'Refresh'
      ButtonColor = 10870243
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn3MouseClick
      InitialDelay = 250
      RepeatDelay = 25
    end
    object TouchBtn8: TTouchBtn
      Left = 8
      Top = 104
      Width = 57
      Height = 49
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -24
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      Caption = 'Up'
      ButtonColor = 10870243
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      OnMouseClick = TouchBtn8MouseClick
      InitialDelay = 250
      RepeatDelay = 25
    end
    object CaptionPanel: TPanel
      Left = 8
      Top = 9
      Width = 785
      Height = 48
      Anchors = [akLeft, akTop, akRight]
      BevelOuter = bvNone
      BorderStyle = bsSingle
      Caption = 'Browse For Files'
      Color = clSilver
      TabOrder = 10
    end
    object Panel3: TPanel
      Left = 8
      Top = 64
      Width = 784
      Height = 33
      Anchors = [akLeft, akTop, akRight]
      BevelOuter = bvNone
      BorderStyle = bsSingle
      ParentColor = True
      TabOrder = 11
      object StatusCaption: TLabel
        Left = 0
        Top = 0
        Width = 780
        Height = 29
        Align = alClient
        AutoSize = False
        Caption = ' Loading...'
        Color = clWhite
        ParentColor = False
        ShowAccelChar = False
        Layout = tlCenter
      end
    end
  end
  object TreeImages: TImageList
    BkColor = clWhite
    Height = 32
    Masked = False
    ShareImages = True
    Width = 32
    Left = 584
    Top = 344
  end
end
