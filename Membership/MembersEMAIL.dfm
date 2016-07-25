inherited frmMembersEmail: TfrmMembersEmail
  Tag = 99
  Left = 234
  Top = 122
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'frmMembersEmail'
  ClientHeight = 378
  ClientWidth = 548
  OldCreateOrder = True
  Position = poScreenCenter
  Scaled = True
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel [0]
    Left = 0
    Top = 0
    Width = 548
    Height = 3
    Align = alTop
    Shape = bsTopLine
  end
  inherited stbStatus: TStatusBar
    Top = 358
    Width = 548
  end
  inherited ControlBar1: TControlBar
    Top = 3
    Width = 548
    BevelEdges = [beBottom]
    TabOrder = 0
    inherited ToolBar: TToolBar
      Width = 531
      inherited pnlFill: TPanel
        TabOrder = 1
      end
      object btnEmail: TToolButton
        Left = 3
        Top = 0
        Caption = 'btnEmail'
        ImageIndex = 35
        OnClick = menuSendEmailClick
      end
      object btnInsert: TToolButton
        Left = 46
        Top = 0
        Hint = 'Insert an Attachment'
        Caption = 'btnInsert'
        ImageIndex = 33
        OnClick = btnInsertClick
      end
      object btnDelete: TToolButton
        Left = 89
        Top = 0
        Hint = 'Delete an Attachment'
        Caption = 'btnDelete'
        ImageIndex = 21
        OnClick = btnDeleteClick
      end
      object btnRichText: TToolButton
        Left = 132
        Top = 0
        Hint = 'USe Rich Edit in the Message'
        Caption = 'btnRichText'
        ImageIndex = 32
        OnClick = btnRichTextClick
      end
      object btnClipboard: TToolButton
        Left = 175
        Top = 0
        Hint = 'Copy Bcc address'#39's to the Clipboard'
        Caption = 'btnClipboard'
        ImageIndex = 26
        OnClick = btnClipboardClick
      end
      object btnFile: TToolButton
        Left = 218
        Top = 0
        Hint = 'Copy Bcc address'#39's to a File'
        Caption = 'btnFile'
        ImageIndex = 15
        OnClick = btnFileClick
      end
      object pnlSpace: TPanel
        Left = 261
        Top = 0
        Width = 219
        Height = 42
        BevelOuter = bvNone
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
      object ToolButton1: TToolButton
        Left = 480
        Top = 0
        Caption = 'ToolButton1'
        ImageIndex = 0
        OnClick = menuExitClick
      end
    end
  end
  object Panel1: TPanel [3]
    Left = 0
    Top = 160
    Width = 548
    Height = 39
    Align = alTop
    BevelOuter = bvNone
    BorderWidth = 5
    Caption = 'Panel1'
    TabOrder = 2
    object Label5: TLabel
      Left = 4
      Top = 8
      Width = 62
      Height = 13
      Alignment = taCenter
      AutoSize = False
      Caption = 'Attachment'
    end
    object lbxAttachments: TListBox
      Left = 69
      Top = 3
      Width = 470
      Height = 31
      Hint = 'Press Insert to add more files or Delete to remove them'
      Color = clWhite
      ItemHeight = 13
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      OnKeyDown = lbxAttachmentsKeyDown
    end
  end
  object Panel2: TPanel [4]
    Left = 0
    Top = 199
    Width = 548
    Height = 159
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 5
    Caption = 'Panel2'
    TabOrder = 3
    object memoBody: TwwDBRichEdit
      Left = 5
      Top = 5
      Width = 538
      Height = 149
      Align = alClient
      AutoURLDetect = False
      PrintJobName = 'Delphi 5'
      TabOrder = 0
      EditorCaption = 'Edit Rich Text'
      EditorPosition.Left = 0
      EditorPosition.Top = 0
      EditorPosition.Width = 0
      EditorPosition.Height = 0
      MeasurementUnits = muInches
      PrintMargins.Top = 1
      PrintMargins.Bottom = 1
      PrintMargins.Left = 1
      PrintMargins.Right = 1
      RichEditVersion = 2
      Data = {
        730000007B5C727466315C616E73695C616E7369637067313235325C64656666
        305C6465666C616E67313033337B5C666F6E7474626C7B5C66305C666E696C20
        4D532053616E732053657269663B7D7D0D0A5C766965776B696E64345C756331
        5C706172645C66305C667331365C7061720D0A7D0D0A00}
    end
  end
  object pnlToCc: TPanel [5]
    Left = 0
    Top = 63
    Width = 548
    Height = 31
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 4
    object Label2: TLabel
      Left = 16
      Top = 10
      Width = 13
      Height = 13
      Caption = 'To'
    end
    object Label3: TLabel
      Left = 300
      Top = 10
      Width = 13
      Height = 13
      Caption = 'Cc'
    end
    object cbxTo: TLMDComboBox
      Left = 70
      Top = 6
      Width = 201
      Height = 21
      Hint = 'Dropdown the list and type in a name, press clear to remove '
      Color = clWhite
      DropDownCount = 12
      ItemHeight = 13
      TabOrder = 0
      OnKeyDown = cbxToKeyDown
    end
    object cbxCC: TLMDComboBox
      Left = 336
      Top = 6
      Width = 203
      Height = 21
      Hint = 'Dropdown the list and type in a name, press clear to remove '
      Color = clWhite
      ItemHeight = 13
      TabOrder = 1
      OnKeyDown = cbxToKeyDown
    end
  end
  object Panel4: TPanel [6]
    Left = 0
    Top = 134
    Width = 548
    Height = 26
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 5
    object Label1: TLabel
      Left = 17
      Top = 3
      Width = 36
      Height = 13
      Caption = 'Subject'
    end
    object edtSubject: TEdit
      Left = 69
      Top = 0
      Width = 470
      Height = 21
      Color = clWhite
      TabOrder = 0
      OnKeyDown = cbxToKeyDown
    end
  end
  object pnlBcc: TPanel [7]
    Left = 0
    Top = 94
    Width = 548
    Height = 40
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 6
    object Label4: TLabel
      Left = 17
      Top = 9
      Width = 19
      Height = 13
      Caption = 'Bcc'
    end
    object lbxBcc: TListBox
      Left = 69
      Top = 2
      Width = 470
      Height = 31
      Hint = 'Press Insert to add more files or Delete to remove them'
      Color = clWhite
      ItemHeight = 13
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
    end
  end
  inherited MainMenu: TMainMenu
    Left = 362
    Top = 2
    inherited menuFile: TMenuItem
      object menuExit: TMenuItem
        Caption = 'E&xit'
        OnClick = menuExitClick
      end
    end
    object menuOptions: TMenuItem [1]
      Caption = '&Options'
      object menuSendEmail: TMenuItem
        Caption = '&Send Email'
        OnClick = menuSendEmailClick
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object menuInsertAtt: TMenuItem
        Caption = 'Insert Attachment'
        OnClick = btnInsertClick
      end
      object menuDeleteAtt: TMenuItem
        Caption = 'Delete Attachment'
        OnClick = btnDeleteClick
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object menuRichText: TMenuItem
        Caption = 'Rich Text in Message'
        OnClick = btnRichTextClick
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object menuBccClipboard: TMenuItem
        Caption = 'Copy Bcc to Clipboard'
        OnClick = btnClipboardClick
      end
      object menuBccNotepad: TMenuItem
        Caption = 'Copy Bcc to Notepad'
        OnClick = btnFileClick
      end
    end
  end
  object OpOutlook: TOpOutlook
    Version = '1.64'
    NewSession = False
    ShowLoginDialog = False
    Left = 408
    Top = 8
  end
end
