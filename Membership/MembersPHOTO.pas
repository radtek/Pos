unit MembersPHOTO;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, Grids, StdCtrls, RXClock, JPEG, db, Mask, wwdbedit, ComCtrls;
  //lmdcctrl, lmdctrl, lmdextcS;

type
  TfrmMembersPhoto = class(TForm)
    MemberTimer: TTimer;
    Panel1: TPanel;
    labelTimer: TTimer;
    lblFinished: TLabel;
    pnlCountdown: TPanel;
    pnlStart: TPanel;
    GroupBox1: TGroupBox;
    lblTimer: TLabel;
    GroupBox2: TGroupBox;
    ListBox: TListBox;
    Panel2: TPanel;
    GroupBox3: TGroupBox;
    RxClock: TRxClock;
    GroupBox4: TGroupBox;
    Panel3: TPanel;
    pnlStartTime: TPanel;
    Panel4: TPanel;
    gbxPhoto: TGroupBox;
    Image2: TImage;
    Image1: TImage;
    Panel5: TPanel;
    GroupBox6: TGroupBox;
    pnlEndTime: TPanel;
    pnlDrawn: TPanel;
    edtLuckyMember: TwwDBEdit;
    Label1: TLabel;
    btnOK: TButton;
    btnCancel: TButton;
    Label2: TLabel;
    lblLuckyMember: TLabel;
    lblLuckyMemberBottom: TLabel;
    procedure FormKeyUp(Sender: TObject; var Key: Word;      Shift: TShiftState);
    procedure FormShow(Sender: TObject);
    procedure MemberTimerTimer(Sender: TObject);
    procedure labelTimerTimer(Sender: TObject);
    procedure RxClockAlarm(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure SwapImage;
    procedure PickNextLuckyMember;
    function LoadPhotoIntoImage (thePhoto : TBlobfield ) : Boolean;
    procedure AddMemberToHistory;
    procedure btnCancelClick(Sender: TObject);
    procedure btnOKClick(Sender: TObject);
    procedure FormResize(Sender: TObject);
    
  private
    { Private declarations }
    FStartTime,FEndTime  : TDAteTime;
    FMemberTime,FgbxPhotoHalfWidth,FgbxPhotoHalfHeight : SmallInt;
    FMaxID,FMember,FMaxMember,FLuckyMember,FSaveLuckyMember : LongInt;
    FMemberDuration,FMemberDurationCtr,FListBoxTopItem  : SmallInt;
    FIncreasePhotoSize : Extended;
    FIncreaseDirection : Boolean;
  public
    { Public declarations }
    property MaxMember : LongInt Write FMaxMember;
    property MemberTime : Smallint Write FMemberTime;
    property EndTime  : TDAteTime  Write FEndTime;
    property StartTime  : TDAteTime  Write FStartTime;
    property IncreasePhotoSize  : Extended  Write FIncreasePhotoSize;
  end;
var
  frmMembersPhoto: TfrmMembersPhoto;

implementation

{$R *.DFM}

uses
   dmMembers,
   Global;

{ **************************************************************************** }
procedure TfrmMembersPhoto.FormCreate(Sender: TObject);
begin
     FListBoxTopItem := 0;
     FIncreaseDirection := True;
end; 
{ **************************************************************************** }
procedure TfrmMembersPhoto.FormResize(Sender: TObject);
begin
     FgbxPhotoHalfWidth  := gbxPhoto.Width div 2;
     FgbxPhotoHalfHeight := gbxPhoto.Height div 2;
end;
{ **************************************************************************** }
procedure TfrmMembersPhoto.FormShow(Sender: TObject);
var Hour,Min,Sec,MSec : Word;
begin
     // convert minutes to seconds
     FMemberDuration := FMemberTime * 60;
     // Start Time
     DecodeTime(FStartTime,Hour,Min,Sec,MSec);
     RxClock.AlarmHour := Hour;
     RxClock.AlarmMinute := Min;
     RxClock.AlarmSecond := 0;
     RxClock.AlarmEnabled := True;
     MSec := 0; Sec := 0;
     FStartTime := EncodeTime(Hour,Min,Sec,MSec);
     // End Time
     DecodeTime(FEndTime,Hour,Min,Sec,MSec);
     MSec := 0; Sec := 0;
     FEndTime := EncodeTime(Hour,Min,Sec,MSec);

     PnlStartTime.Caption := formatDateTime('hh: mm :ss ampm', FStartTime);
     PnlEndTime.Caption   := formatDateTime('hh: mm :ss ampm', FEndTime );
     Self.WindowState := wsMaximized;
     PickNextLuckyMember;
     screen.cursor := crNone;
     // Start if passed the time
     if FStartTime < Time then
        RxClockAlarm(Sender);
end;
{ **************************************************************************** }
procedure TfrmMembersPhoto.FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
begin
     if (key = vk_Return) and pnlDrawn.Visible then 
        btnOKClick(Sender);
     if key = vk_Escape then begin
        screen.cursor := crDefault;
        close;
     end;   
     // start the process right away
     if key = vk_F5 then begin
        // stop it from re firing
        if RxClock.Enabled then begin
           RxClock.Enabled := False;
           RxClockAlarm(Sender);
        end;   
     end;   
     // Lucky Member Drawn - end the process right away and show the last Member
     if key = vk_F10 then begin
        pnlDrawn.Visible := True;
        Screen.Cursor := crDefault;
        edtLuckyMember.Setfocus;
        edtLuckyMember.Text := IntToStr(FSaveLuckyMember);
        edtLuckyMember.SelectAll;
     end;   
end;
{ **************************************************************************** }
procedure TfrmMembersPhoto.MemberTimerTimer(Sender: TObject);
begin
     // only do this at a Member Break
     if FEndTime <= Time then begin
        AddMemberToHistory;
        Image1.Picture.Assign(nil);
        Image2.Picture.Assign(nil);
        MemberTimer.Enabled := False;
        labelTimer.Enabled := False;
        lblTimer.Caption   := '';
        // see Lucky Member thru Image
        lblLuckyMember.Align := alClient;
        lblLuckyMember.Visible := True;
        lblLuckyMember.Caption := 'No Lucky Member Drawn';
        exit;
     end;
     // reset the timer
     lblTimer.Caption    := IntToStr(FMemberDuration -1);
     FMemberDurationCtr  := FMemberDuration -1;
     SwapImage;
     AddMemberToHistory;
     // Save the Lucky Member - because of 2 images
     FSaveLuckyMember := FLuckyMember;
     PickNextLuckyMember;
end;
{ **************************************************************************** }
procedure TfrmMembersPhoto.labelTimerTimer(Sender: TObject);
begin
     lblTimer.Caption := IntToStr(FMemberDurationCtr);
     dec(FMemberDurationCtr); 
end;
{ **************************************************************************** }
procedure TfrmMembersPhoto.RxClockAlarm(Sender: TObject);
begin
     // change panels
     pnlStart.Align       := alClient;
     pnlStart.Visible     := True;
     pnlCountdown.Visible := False;                      
     // Start Timers
     //MemberTimer.Interval := FMemberDuration * 200;
     // Seconds to Msecs
     MemberTimer.Interval := FMemberDuration * 1000;
     lblTimer.Caption     := IntToStr(FMemberDuration -1);
     FMemberDurationCtr   := FMemberDuration -1;
     // reset the timer
     SwapImage;
     MemberTimer.Enabled := True;     
     LabelTimer.Enabled  := True;
     // Save the Lucky Member - because of 2 images
     FSaveLuckyMember    := FLuckyMember;
     PickNextLuckyMember;
     // dont need it after here
     RxClock.Enabled := False;
end;
{ **************************************************************************** }
procedure TfrmMembersPhoto.SwapImage;
begin
     if image1.Visible then begin
        image1.Visible := False;
        image2.Visible := True;
     end else begin   
        image1.Visible := True;
        image2.Visible := False;
     end;   
     //lblLuckyMember.Caption := IntToStr(FLuckyMember);
end;
{ **************************************************************************** }
procedure TfrmMembersPhoto.PickNextLuckyMember;
label Reselect;
var   LuckyID : LongInt;
const SQL1 = 'Select MEMBER from LUCKY_MEMBER where LUCKY_ID = %d';
      SQL2 = 'Select PHOTO from M_PHOTOS where MEMBER = %d';
begin
     Randomize;
     ReSelect:;
     LuckyID := Random(FMaxMember);
     // Member will always be Financial 
     if dm.sqlExecQueryCheckEOF(dm.qryGeneral,Format(SQL1,[LuckyID])) then begin 
        FLuckyMember := dm.qryGeneral.FieldByName('Member').AsInteger;
        // get the photo
        if dm.qryOpenCheckEOF(dm.qryReport,Format(SQL2,[FLuckyMember])) then begin
           // checks for blank photo etc
           if not LoadPhotoIntoImage(TBlobField(dm.qryReport.Fields[0]) ) then goto ReSelect;
           //ListBox.Items.Add('FLuckyMember  ' + IntToStr(FLuckyMember));
        end else
          goto ReSelect;                  
     end else
        goto ReSelect;
end;
{ **************************************************************************** }
procedure TfrmMembersPhoto.AddMemberToHistory;
begin
     ListBox.Items.Add('  ' + IntToStr(FSaveLuckyMember) + '  ' + FormatDateTime('hh:mm:ss ampm',Time));
     if ListBox.Items.Count >= 10 then begin
        inc(FListBoxTopItem);
        ListBox.TopIndex := FListBoxTopItem;
     end;   
end;
{ **************************************************************************** }
function TfrmMembersPhoto.LoadPhotoIntoImage (thePhoto : TBlobfield ) : Boolean;
var photo : TBlobField;
    AStream : TMemoryStream;
    jpg : TJPegImage;
    TempImage : TImage;
    jpgWidth,jpgHeight : smallint;
begin
     Result := False;
     // load photo into background image
     if image1.Visible then
        TempImage := Image2
     else    
        TempImage := Image1;
    if thePhoto.BlobSize < 1 then begin
       TempImage.Picture.Assign(nil);
       exit; 
    end;   
    jpg := TJPEGImage.Create;
    AStream := TMemoryStream.Create;
    try
       TempImage.Picture.Assign(nil);
       photo := thePhoto as TBlobField;
       photo.SaveToStream(AStream);
       AStream.Seek(0, soFromBeginning);
       with jpg do begin
          Pixelformat := jf24bit;
          performance := jpBestQuality;
          ProgressiveEncoding := True;
          LoadFromStream(AStream);
       end;
       TempImage.Picture.Assign(jpg);
       // Got to keep the jpg increased Proportionately
       // Blow to full size
       if FIncreasePhotoSize = 0 then begin
          // only add onto the max width
          jpgWidth  := (gbxPhoto.Width - 2)- jpg.Width;
          TempImage.Height := jpg.Height + jpgWidth;
          TempImage.Width  := jpg.Width  + jpgWidth;
        end else begin  
          // Blow up by a percentage
          jpgWidth   := jpg.Width;
          jpgHeight  := jpg.Height;
          TempImage.Width  := Trunc(jpgWidth  * (1 + FIncreasePhotoSize));
          TempImage.Height := Trunc(jpgHeight * (1 + FIncreasePhotoSize));
        end;
       // centre on the control
       TempImage.Left := FgbxPhotoHalfWidth  - (TempImage.Width div 2);
       TempImage.Top  := FgbxPhotoHalfHeight - (TempImage.Height div 2);
    finally
        photo := nil;
        AStream.Free;
        jpg.Free;
    end;
    Result := True;
    
end;
{ **************************************************************************** }
procedure TfrmMembersPhoto.btnCancelClick(Sender: TObject);
begin
     pnlDrawn.Visible := False;
     Screen.Cursor    := crNone;
end;
{ **************************************************************************** }
procedure TfrmMembersPhoto.btnOKClick(Sender: TObject);
const  SQL2 = 'Select PHOTO from M_PHOTOS where MEMBER = %s';
begin
     labelTimer.Enabled  := False;
     MemberTimer.Enabled := False;
     lblTimer.Caption    := '';
     pnlDrawn.Visible := False;
     // Get Members Photo Again
     dm.qryOpen(dm.qryReport,Format(SQL2,[edtLuckyMember.Text]));
     // Loads into the background image
     LoadPhotoIntoImage(TBlobField(dm.qryReport.Fields[0]) );
     // swap to the front
     SwapImage;
     // see Lucky Member thru Image
     lblLuckyMember.Align := alTop;
     lblLuckyMember.Visible := True;
     lblLuckyMemberBottom.Align   := alBottom;
     lblLuckyMemberBottom.Visible := True;
     Image1.Transparent := True;
     Image2.Transparent := True;
     Screen.Cursor := crDefault;
end; 
{ **************************************************************************** }

end.
