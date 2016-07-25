program Main;

{%ToDo 'Main.todo'}

uses
  Windows,
  SysUtils,
  Classes,
  Controls,
  Forms,
  Dialogs,
  MemberMenu in 'MemberMenu.pas' {frmMemberMenu},
  stdForm in 'stdForm.pas' {SMStdForm},
  dmMembers in 'dmMembers.pas' {dm: TDataModule},
  Global in 'Global.pas',
  MainMenu in 'MainMenu.pas' {frmMainMenu},
  Multi in 'Multi.pas' {frmPasswordDlg},
  MembersMAINT in 'MembersMAINT.pas' {frmMembersMAINT},
  MembersREPORT in 'MembersREPORT.pas' {frmMembersREPORT},
  MembersPAY in 'MembersPAY.pas' {frmMembersPAY},
  MembersSYS in 'MembersSYS.pas' {frmMembersSYS},
  MembersROLL in 'MembersROLL.pas' {frmMembersROLL},
  rCurrentPoints in 'rCurrentPoints.pas' {frmCurrentPoints},
  rNewMembers in 'rNewMembers.pas' {frmNewMember},
  rVotingList in 'rVotingList.pas' {frmVotingList},
  rEventsReport in 'rEventsReport.pas' {frmEventsReport},
  rPayReprint in 'rPayReprint.pas' {frmPayReprint},
  Tenders in 'Tenders.pas' {frmTenders},
  rAgeAnalysis in 'rAgeAnalysis.pas' {frmAgeAnalysis},
  rAnalysis in 'rAnalysis.pas' {frmAnalysis},
  rInformation in 'rInformation.pas' {frmInformation},
  rLuckyHistory in 'rLuckyHistory.pas' {frmLuckyHistory},
  rSubAnalysis in 'rSubAnalysis.pas' {frmSubAnalysis},
  rSectionInform in 'rSectionInform.pas' {frmSectionInform},
  rMailingLabels in 'rMailingLabels.pas' {frmMailingLabels},
  MembersLOOK in 'MembersLOOK.pas' {frmMembersLook},
  MembersLetter in 'MembersLetter.pas' {frmMembersLetter},
  rDueLetter in 'rDueLetter.pas' {frmDueLetter},
  MembersLOOKUP in 'MembersLOOKUP.pas' {frmMembersLookup},
  MembersEMAIL in 'MembersEMAIL.pas' {frmMembersEmail},
  rPayTotals in 'rPayTotals.pas' {frmPayTotals},
  ShowHelp in 'ShowHelp.pas' {frmShowHelp},
  Splash in 'Splash.pas' {frmSplash},
  MembersQUERY in 'MembersQUERY.pas' {frmMembersQUERY},
  MembersCLASSES in 'MembersCLASSES.pas',
  MembersCARDS in 'MembersCARDS.pas' {frmMembersCARDS},
  rReceiptReport in 'rReceiptReport.pas' {frmReceiptReport},
  rInvoiceLandscape in 'rInvoiceLandscape.pas' {frmInvoiceLandscape},
  rIndividualMember in 'rIndividualMember.pas' {frmIndividualMember},
  rRollover in 'rRollover.pas' {frmRollover},
  MembersSEARCH in 'MembersSEARCH.pas' {frmMembersSEARCH},
  MembersPHOTO in 'MembersPHOTO.pas' {frmMembersPhoto},
  rSubsByGroups in 'rSubsByGroups.pas' {frmSubsByGroups},
  rDirectDebit in 'rDirectDebit.pas' {frmDirectDebit},
  MembersEDIT_BRIEF in 'MembersEDIT_BRIEF.pas' {frmMembersEditBrief},
  Password in 'Password.pas' {frmPassword},
  rPointsLog in 'rPointsLog.pas' {frmPointsLog},
  rMemberList in 'rMemberList.pas' {frmMemberList},
  rParentCode in 'rParentCode.pas' {frmParentCode},
  rPointsAudit in 'rPointsAudit.pas' {frmPointsAudit},
  rMemberDraw in 'rMemberDraw.pas' {frmMemberDraw},
  rPointsStockmaster in 'rPointsStockmaster.pas' {frmPointsStockmaster},
  MembersVisits in 'MembersVisits.pas' {frmMemberVisits},
  rKioskReport in 'rKioskReport.pas' {frmKioskReport},
  rProposerSeconder in 'rProposerSeconder.pas' {frmProposerSeconder},
  rPointsSplit in 'rPointsSplit.pas' {frmPointsSplit},
  rPHMPointsLog in 'rPHMPointsLog.pas' {frmPHMPointsLog},
  rKioskHistory in 'rKioskHistory.pas' {frmKioskHistory},
  dmMenuMate in 'dmMenuMate.pas' {dmMM: TDataModule},
  rMMPointsAudit in 'rMMPointsAudit.pas' {frmPointsHistory},
  rSponsorshipBySection in 'rSponsorshipBySection.pas' {frmSponsorshipBySection},
  Setup in 'Setup.pas' {frmSetup},
  rGenericReceipt in 'rGenericReceipt.pas' {frmGenericReceipt},
  rMMDiscounts in 'rMMDiscounts.pas' {frmMMDiscounts},
  KioskPas in 'KioskPas.pas' {frmKiosk};

{$R *.RES}

{ **************************************************************************** }
procedure ErrorDlg( const ErrorMsg : String );
var
   Capt : String;
begin
   Capt := Global.BusinessLogo;
   Application.MessageBox( @ErrorMsg[1], @Capt[1], mb_Ok );
end;
{ **************************************************************************** }
var
     MemberSystem : Boolean = True;
begin
     Global.EnsureSingleInstance( 'MAIN.EXE' ); 
     Global.MemberSystem := MemberSystem;
     Application.Title := 'MemberShip';
     if not CheckSystemDate(now) then begin
        ErrorDlg(Format('The Date ( %s ) needs correcting',[DateTimeToStr(now)]));
     end;
     Application.CreateForm(Tdm, dm);
  if not Global.ReadReg then begin
        if Global.regServerName = '' then begin
           dm.free;             
           exit;
        end;   
     end;
     if GetKeyState(vk_F10) < 0 then begin
        // prompt for the Setup form
        frmSetup := TfrmSetup.Create(application);
        frmSetup.ShowModal;
        frmSetup.Free;
        // Registry not setup
        if Global.regServerName = '' then begin
           dm.free;             
           exit;
        end;   
     end;
     // Infinity user  - make them logon - dont check for # of users
     //if GetKeyState(vk_F8) < 0 then global.regLogOn := True;
     if dm.IBDatabase.Connected then begin
        dm.IBDatabase.Connected := False;
        //ErrorDlg('IBDatabase Already Connected');
        //dm.free;
        //exit;
     end;
     frmSplash := TfrmSplash.Create(application);
     { **************************************************************************** }
     //global.regLogon := True;
     if global.regLogon then begin // if password required then wait for it
        if not frmSplash.LoadUserNames then begin
           frmSplash.free; dm.free; exit;
        end;
        frmSplash.pnlPassword.Visible  := true;
        if not (frmSplash.ShowModal = mrOK) then begin
           frmSplash.free; dm.free; exit;
        end;
     end;
     try
        with frmSplash do begin
           pnlPassword.Visible  := False;
           //lblTableName.Visible := True;
           show;
           update; // repaint it
           if not global.regLogon then begin //  connected if logon = true
              if not ConnectToDB   then begin frmSplash.Free;dm.Free; Exit; end;
           end;
           if not UpdateVersion then begin frmSplash.Free;dm.Free; Exit; end;
           if not SetupSiteDetails then begin frmSplash.Free;dm.Free; Exit; end;
           // Removed 05/06/04 to much hassle with Menumate
           //if not CheckLogInCount  then begin dm.Free; Exit; end;
           //if not OpenTables       then begin dm.Free; Exit; end;
        end;
     finally
         //
     end;   
     // keep the user/process log tidy > 90 days old
     dm.DeleteLog(PrcssLogTableName,'',90); 
     dm.DeleteLog(ExportArevTableName, 'EXPORT_DATE',30);
     // dont delete temp tables if they are in use 
     if dm.LoggedOnUsers = 1 then  // allow for self
        dm.DeleteReportTables;
     // Exporting to Stockmaster
     if ControllerPCName = ReadComputerName then begin
        dm.ExportTimer.Interval := ExportSeconds;
        dm.ExportTimer.Enabled := True;
     end;
     // Exporting to PHM 
     if (PHMControllerPCName = ReadComputerName) and (PHMInstalled) then begin
        dm.PHMExportTimer.Interval := ExportSeconds;
        dm.PHMExportTimer.Enabled := True;
     end;   
     // MenuMate Database Connection
     Application.CreateForm(TdmMM, dmMM);
     // if in testing Mode
     if dmMM.MMDatabase.Connected then 
        dmMM.MMDatabase.Connected  := False;
     frmSplash.ConnectToMenuMateDB;   
     if MemberSystem  then begin
        if not dm.PreCheckLogOn(TfrmMemberMenu.ScreenCode,TfrmMemberMenu.ScreenDesc) then begin 
           dm.Free; 
           dmMM.Free;
           frmSplash.Free;
           Exit;     
        end else begin
           Application.CreateForm(TfrmMemberMenu,frmMemberMenu); // Membership only system
           frmSplash.Free;
        end;   
     end;
     InTestMode := False; // Connected to another DataBase for Testing
     Application.Run;
end.                            

