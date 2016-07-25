object frmHelp: TfrmHelp
  Left = 74
  Top = 63
  Width = 473
  Height = 609
  Caption = 'frmHelp'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 465
    Height = 49
    Align = alTop
    BevelInner = bvLowered
    TabOrder = 0
    object btnClose: TButton
      Left = 368
      Top = 8
      Width = 83
      Height = 33
      Caption = 'Close'
      TabOrder = 0
      OnClick = btnCloseClick
    end
  end
  object Memo1: TMemo
    Left = 0
    Top = 49
    Width = 465
    Height = 533
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Lines.Strings = (
      'JoinMate Help Summary'
      ''
      
        '1. The two Database files you wish to merge MUST be the same ver' +
        'sion. '
      
        '    Open each database file using IB Console and look in the Ver' +
        'sion Table.'
      '    Ensure that the latest version numbers match.'
      ''
      '    This Program must be run on an Interbase Server.'
      ''
      '2. Close Down All MenuMate Tills.'
      ''
      '3. Close Down PalmMate, ChefMate, LogsMate and ImptMate.'
      ''
      '4. Close down the Parser and Office if they are running.'
      ''
      
        '5. If any other 3rd Party software (IBConsole)  is connected to ' +
        'the MenuMate '
      '    Database then ensure that it is closed down as well. '
      ''
      
        '6. Double Click on the MenuMate.gdb file  you wish to Merge the ' +
        'data into '
      
        '    (the Destination) and back it up using IBTool.exe. Ensure yo' +
        'u have a '
      '    back up of your Source Database as well.'
      ''
      '7. The Source file will be copied into the Destination File.'
      ''
      
        '8. Select the source MenuMate.gdb and the Destination MenuMate.g' +
        'db.'
      ''
      '9. Hit the Join button and wait for the merge to complete.'
      ''
      
        '10. It is possible (and likely) that after the merge you will ha' +
        've duplicate '
      '    invoice numbers referring to two separate invoices. '
      '    This wont cause MenuMate any problems, just be aware that '
      
        '    when you search for an invoice number the system might find ' +
        'two.'
      
        '    Their dates will be different so use the date of the sale to' +
        ' locate the '
      '    one you need.'
      '')
    ParentFont = False
    TabOrder = 1
  end
end
