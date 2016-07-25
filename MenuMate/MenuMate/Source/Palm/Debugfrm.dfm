object frmDebug: TfrmDebug
  Left = 273
  Top = 181
  Width = 672
  Height = 577
  Caption = 'Debug'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 18
    Width = 57
    Height = 16
    Caption = 'Message'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 8
    Top = 50
    Width = 34
    Height = 16
    Caption = 'Count'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 8
    Top = 82
    Width = 81
    Height = 16
    Caption = 'Trigger Count'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 8
    Top = 114
    Width = 69
    Height = 16
    Caption = 'Error Action'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object lbBugs: TListBox
    Left = 0
    Top = 214
    Width = 664
    Height = 336
    Align = alBottom
    ItemHeight = 13
    TabOrder = 0
  end
  object Button1: TButton
    Left = 576
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Close'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 104
    Top = 144
    Width = 233
    Height = 25
    Caption = 'Add'
    TabOrder = 2
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 544
    Top = 184
    Width = 105
    Height = 25
    Caption = 'Delete'
    TabOrder = 3
    OnClick = Button3Click
  end
  object cbMessageType: TComboBox
    Left = 104
    Top = 16
    Width = 233
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ItemHeight = 16
    ParentFont = False
    TabOrder = 4
    Text = 'Message Type'
    Items.Strings = (
      'REGISTRATION = 0xE0'
      'INFO_SYNC 0xE1'
      'MENU_SYNC 0xE2'
      'DRINK_SYNC 0xE3'
      'LOGIN 0xE4'
      'KEEPALIVE 0xE5'
      'ORDERS 0xE6'
      'CALL_AWAY 0xE7'
      'REQUEST_RECEIPT 0xE8'
      'TRANSFER_TABLE_ORDERS 0xE9'
      'RETRIEVE_ORDERS 0xEA'
      'NA = 0x00')
  end
  object edTriggerCount: TEdit
    Left = 104
    Top = 80
    Width = 233
    Height = 21
    TabOrder = 5
    Text = '1'
  end
  object edCount: TEdit
    Left = 104
    Top = 48
    Width = 233
    Height = 21
    TabOrder = 6
    Text = '1'
  end
  object cbErrorType: TComboBox
    Left = 104
    Top = 112
    Width = 233
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ItemHeight = 16
    ParentFont = False
    TabOrder = 7
    Text = 'Error Type'
    Items.Strings = (
      'Err_ErrorMsg 1'
      'Err_Msg_CRC 2'
      'Err_Currupt_Message 3'
      'Err_Mismatched_Message_Data 4'
      'Err_InfoOutOfSync 5'
      'Err_MenuOutOfSync 6'
      'Err_DrinkOutOfSync 7'
      'Err_Database 8'
      'Err_Not_Logged_In 9'
      'Err_PIN_Incorrect 10'
      'Err_User_Not_Found 11'
      'Err_User_OverFlow 12'
      'Err_DeviceID_Not_Found 13'
      'Err_Not_In_Register_Mode 14'
      'Err_No_Printer_Found 15'
      'Err_Destructor 16'
      'Wrn_NonFatalWarnings 128'
      'Wrn_WarningMsg 129'
      'Wrn_No_Menu_Found 130'
      'Wrn_Printing_Redirected 131'
      'Wrn_No_Printer_For_Course 132'
      'Wrn_Duplicate_Order 133'
      'PalmToServerErrors = 200'
      'proA_Ok = 255'
      'Bail out and dont respond.'
      'Send proA_Ok But Mess up the CRC')
  end
  object Button4: TButton
    Left = 8
    Top = 184
    Width = 105
    Height = 25
    Caption = 'Refresh'
    TabOrder = 8
    OnClick = Button4Click
  end
end
