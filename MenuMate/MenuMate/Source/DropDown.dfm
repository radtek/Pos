object frmDropDownFunc: TfrmDropDownFunc
  Tag = 1024
  Left = 618
  Top = 70
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Select'
  ClientHeight = 592
  ClientWidth = 192
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnlCourses: TPanel
    Left = 8
    Top = 8
    Width = 177
    Height = 577
    BevelOuter = bvNone
    Color = 14342874
    ParentBackground = False
    TabOrder = 0
    OnResize = pnlCoursesResize
    object sbMessages: TScrollBox
      Left = 0
      Top = 0
      Width = 177
      Height = 497
      HorzScrollBar.Visible = False
      VertScrollBar.ButtonSize = 1
      VertScrollBar.Color = clSilver
      VertScrollBar.ParentColor = False
      VertScrollBar.Smooth = True
      VertScrollBar.Size = 1
      VertScrollBar.Style = ssFlat
      Align = alTop
      BorderStyle = bsNone
      Ctl3D = False
      ParentCtl3D = False
      TabOrder = 0
    end
    object btnMsgUp: TTouchBtn
      Left = 8
      Top = 512
      Width = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      ArrowType = atUp
      OnMouseDown = btnMsgUpMouseDown
    end
    object btnMsgDown: TTouchBtn
      Left = 104
      Top = 512
      Width = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ParentColor = True
      ButtonColor = 6052441
      LatchedColor = clBtnFace
      DisabledButtonColor = clBtnFace
      LatchingProperties = [lkLatchColor, lkStayDown]
      ArrowType = atDown
      OnMouseDown = btnMsgDownMouseDown
    end
  end
end
