{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RpDBUtil;

interface

uses
  Windows, Messages, Graphics, Controls, Forms, Dialogs, StdCtrls, ExtCtrls,
  SysUtils, Classes, DB, RpDefine, RpMemo;

type
  TDBMemoBuf = class(TMemoBuf)
  protected
    procedure SetMemoField(Value: TMemoField);
    procedure SetRTFMemoField(Value: TMemoField);
  public
    property Field: TMemoField write SetMemoField;
    property RTFField: TMemoField write SetRTFMemoField;
  end; { TDBMemoBuf }

  procedure GraphicFieldToBitmap(GraphicField: TBlobField; Bitmap: TBitmap);

implementation

  procedure TDBMemoBuf.SetMemoField(Value: TMemoField);
  var
    Stream: TMemoryStream;
  begin { SetMemoField }
    Stream := TMemoryStream.Create; { Create temporary stream }
    try
      Value.SaveToStream(Stream); { Save memo to stream }
      SetData(Stream.Memory^,Stream.Size); { Set memo buffer data to stream }
    finally
      Stream.Free; { Free up temporary stream }
    end; { try }
  end;  { SetMemoField }

  procedure TDBMemoBuf.SetRTFMemoField(Value: TMemoField);
  begin { SetRTFMemoField }
    ReadRTF := true;
    SetMemoField(Value);
  end;  { SetRTFMemoField }

  procedure GraphicFieldToBitmap(GraphicField: TBlobField; Bitmap: TBitmap);
  begin { GraphicFieldToBitmap }
    Bitmap.Assign(GraphicField);
  end;  { GraphicFieldToBitmap }

end.
