{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvToolEvents;

interface

uses
  Windows, Messages,
  Classes, SysUtils, TypInfo,
  Types, Graphics, Controls, StdCtrls, ActnList, ExtCtrls, Forms, Dialogs, ImgList, Menus,
  RvTool, RvDefine, RvUtil, RvClass, RpDefine, RvProj, RvData;

type

(*****************************************************************************}
( Event handler interfaces - These interfaces define standard Rave event
( handler protocols.  See RANT.TXT for information on how to implement a
( project editor to handle a specific event.
(*****************************************************************************)

  IRaveProjectEventHandler = interface
    [{Trans-}'{65FD28BE-034A-4976-AE09-8B270129E4A4}']
    procedure AfterProjectOpen;
    function BeforeProjectClose: boolean;
    function BeforeNewProject: boolean;
    procedure AfterNewProject;
    function BeforeProjectSave: boolean;
    procedure AfterProjectSave;
    procedure PrepareViewChange(NewProject: boolean);
    procedure AfterViewChange(NewProject: boolean);
  end; { IRaveProjectEventHandler }

  IRaveProjectItemEventHandler = interface
    [{Trans-}'{98C4F713-83CB-4B7E-B1F9-748CAD155E09}']
    function BeforeNewReport: boolean;
    procedure AfterNewReport(Report: TRaveReport);
    function BeforeNewReportPage: boolean;
    procedure AfterNewReportPage(Page: TRavePage);
    function BeforeNewGlobalPage: boolean;
    procedure AfterNewGlobalPage(Page: TRavePage);
    function BeforeNewDataObject: boolean;
    procedure AfterNewDataObject(DataObject: TRaveDataObject);
  end; { IRaveProjectItemEventHandler }

  IRaveReportActionEventHandler = interface
    [{Trans-}'{BFD438DD-D5DE-40E2-AF67-9F471287A4FC}']
    function BeforeReportPrint: boolean;
    procedure AfterReportPrint;
    function BeforeReportActivate: boolean;
    procedure AfterReportActivate;
  end; { IRaveReportActionEventHandler }

  IRavePageDesignerEventHandler = interface
    [{Trans-}'{DD37AB08-4AC1-4039-8D1F-555FE00F8252}']
    function BeforePageChange: boolean;
    procedure AfterPageChange;
    procedure ShowPage(Component: TRaveComponent);
    procedure AfterZoomChange;
    procedure AfterZoomToolChange;
    procedure ShowControlPopup(Point: PPoint);
    procedure AfterNewComponent(Component: TRaveComponent);
  end; { IRavePageDesignerEventHandler }

  IRavePropertyPanelEventHandler = interface
    [{Trans-}'{397F4708-2959-40AD-888A-A146D591544D}']
    procedure AfterSelectionChange;
    procedure AfterPropertiesModified;
    procedure SelectComponent(Component: TRaveComponent);
  end; { IRavePropertyPanelEventHandler }

  IRaveApplicationEventHandler = interface
    [{Trans-}'{CBC14CBA-9466-4C13-879D-608F2677AA5C}']
    procedure AfterAppOpen;
    function BeforeAppClose: boolean;
    function ShowAboutDialog: boolean;
    procedure GetAppTitle;
    procedure AddShortCuts;
    procedure LoadState;
    procedure SaveState;
    procedure Initialize;
    procedure MergeMenu(MainMenu: TMainMenu);
  end; { IRaveApplicationEventHandler }

  IRaveToolMenuEventHandler = interface
    [{Trans-}'{4AA74506-8308-4FB2-92BE-739B8976BBC2}']
    procedure AddToMenu(AddMenuItem: TAddMenuItemProc);
    procedure RunFromMenu(ID: integer);
  end; { IRaveToolMenuEventHandler }

  IRaveModuleEventHandler = interface
    [{Trans-}'{0240A610-A421-485A-9B17-E4BA4CAE8193}']
    procedure CompileStatus(Module: TRaveModule);
    procedure CompileError(CompileStatus: TRaveCompileStatus);
    procedure CompileSuccess;
  end; { IRaveModuleEventHandler }

(*****************************************************************************}
( TRaveMasterProjectEditor is a pre-made project editor implementation of the
( project based events.  These events are called from infrequent or user-based
( actions so it is reasonable to have many separate project editors in the
( event chain.  To handle one of these events, simply descend from
( TRaveMasterProjectEditor and override the desired method.
(*****************************************************************************)

  TRaveMasterProjectEditor = class(TRaveProjectEditor,
                                   IRaveProjectEventHandler,
                                   IRaveProjectItemEventHandler,
                                   IRaveReportActionEventHandler,
                                   IRavePageDesignerEventHandler,
                                   IRavePropertyPanelEventHandler,
                                   IRaveApplicationEventHandler)
  public
  // Project events
    procedure AfterProjectOpen; virtual;
    function BeforeProjectClose: boolean; virtual;
    function BeforeNewProject: boolean; virtual;
    procedure AfterNewProject; virtual;
    function BeforeProjectSave: boolean; virtual;
    procedure AfterProjectSave; virtual;
    procedure PrepareViewChange(NewProject: boolean = false); virtual;
    procedure AfterViewChange(NewProject: boolean = false); virtual;
  // ProjectItem events
    function BeforeNewReport: boolean; virtual;
    procedure AfterNewReport(Report: TRaveReport); virtual;
    function BeforeNewReportPage: boolean; virtual;
    procedure AfterNewReportPage(Page: TRavePage); virtual;
    function BeforeNewGlobalPage: boolean; virtual;
    procedure AfterNewGlobalPage(Page: TRavePage); virtual;
    function BeforeNewDataObject: boolean; virtual;
    procedure AfterNewDataObject(DataObject: TRaveDataObject); virtual;
  // Report Action events
    function BeforeReportPrint: boolean; virtual;
    procedure AfterReportPrint; virtual;
    function BeforeReportActivate: boolean; virtual;
    procedure AfterReportActivate; virtual;
  // Designer events
    function BeforePageChange: boolean; virtual;
    procedure AfterPageChange; virtual;
    procedure ShowPage(Component: TRaveComponent); virtual;
    procedure AfterZoomChange; virtual;
    procedure AfterZoomToolChange; virtual;
    procedure ShowControlPopup(Point: PPoint); virtual;
    procedure AfterNewComponent(Component: TRaveComponent); virtual;
  // Property Panel events
    procedure AfterSelectionChange; virtual;
    procedure AfterPropertiesModified; virtual;
    procedure SelectComponent(Component: TRaveComponent); virtual;
  // Application events
    procedure AfterAppOpen; virtual;
    function BeforeAppClose: boolean; virtual;
    function ShowAboutDialog: boolean; virtual;
    procedure GetAppTitle; virtual;
    procedure AddShortCuts; virtual;
    procedure LoadState; virtual;
    procedure SaveState; virtual;
    procedure Initialize; virtual;
    procedure MergeMenu(MainMenu: TMainMenu); virtual;
  end; { TRaveMasterProjectEditor }

(*****************************************************************************}
( Event handler classes
(*****************************************************************************)

  TRaveProjectEventHandler = class(TRaveEventHandler, IRaveProjectEventHandler)
  protected
    FAllow: boolean;
    procedure Process(Index: integer); override;
  public
    procedure AfterProjectOpen;
    function BeforeProjectClose: boolean;
    function BeforeNewProject: boolean;
    procedure AfterNewProject;
    function BeforeProjectSave: boolean;
    procedure AfterProjectSave;
    procedure PrepareViewChange(NewProject: boolean = false);
    procedure AfterViewChange(NewProject: boolean = false);
  end; { TRaveProjectEventHandler }

  TRaveProjectItemEventHandler = class(TRaveEventHandler, IRaveProjectItemEventHandler)
  protected
    FAllow: boolean;
    procedure Process(Index: integer); override;
  public
    function BeforeNewReport: boolean;
    procedure AfterNewReport(Report: TRaveReport);
    function BeforeNewReportPage: boolean;
    procedure AfterNewReportPage(Page: TRavePage);
    function BeforeNewGlobalPage: boolean;
    procedure AfterNewGlobalPage(Page: TRavePage);
    function BeforeNewDataObject: boolean;
    procedure AfterNewDataObject(DataObject: TRaveDataObject);
  end; { TRaveProjectItemEventHandler }

  TRaveReportActionEventHandler = class(TRaveEventHandler, IRaveReportActionEventHandler)
  protected
    FAllow: boolean;
    procedure Process(Index: integer); override;
  public
    function BeforeReportPrint: boolean;
    procedure AfterReportPrint;
    function BeforeReportActivate: boolean;
    procedure AfterReportActivate;
  end; { TRaveReportActionEventHandler }

  TRavePageDesignerEventHandler = class(TRaveEventHandler, IRavePageDesignerEventHandler)
  protected
    FAllow: boolean;
    procedure Process(Index: integer); override;
  public
    function BeforePageChange: boolean;
    procedure AfterPageChange;
    procedure ShowPage(Component: TRaveComponent);
    procedure AfterZoomChange;
    procedure AfterZoomToolChange;
    procedure ShowControlPopup(Point: PPoint);
    procedure AfterNewComponent(Component: TRaveComponent);
  end; { TRavePageDesignerEventHandler }

  TRavePropertyPanelEventHandler = class(TRaveEventHandler, IRavePropertyPanelEventHandler)
  protected
    FAllow: boolean;
    procedure Process(Index: integer); override;
  public
    procedure AfterSelectionChange;
    procedure AfterPropertiesModified;
    procedure SelectComponent(Component: TRaveComponent);
  end; { TRavePropertyPanelEventHandler }

  TRaveApplicationEventHandler = class(TRaveEventHandler, IRaveApplicationEventHandler)
  protected
    FAllow: boolean;
    procedure Process(Index: integer); override;
  public
    procedure AfterAppOpen;
    function BeforeAppClose: boolean;
    function ShowAboutDialog: boolean;
    procedure GetAppTitle;
    procedure AddShortCuts;
    procedure LoadState;
    procedure SaveState;
    procedure Initialize;
    procedure MergeMenu(MainMenu: TMainMenu);
  end; { TRaveApplicationEventHandler }

  TRaveModuleEventHandler = class(TRaveEventHandler, IRaveModuleEventHandler)
  protected
    FAllow: boolean;
    procedure Process(Index: integer); override;
  public
    procedure CompileStatus(Module: TRaveModule);
    procedure CompileError(CompileStatus: TRaveCompileStatus);
    procedure CompileSuccess;
  end; { TRaveModuleEventHandler }

var
  ProjectEventHandler: TRaveProjectEventHandler;
  ProjectItemEventHandler: TRaveProjectItemEventHandler;
  ReportActionEventHandler: TRaveReportActionEventHandler;
  PageDesignerEventHandler: TRavePageDesignerEventHandler;
  PropertyPanelEventHandler: TRavePropertyPanelEventHandler;
  ApplicationEventHandler: TRaveApplicationEventHandler;
  ModuleEventHandler: TRaveModuleEventHandler;

  procedure RaveRegister;

implementation

procedure RaveRegister;

begin { RaveRegister }
  ProjectEventHandler := RegisterRaveEventHandler(TRaveProjectEventHandler) as TRaveProjectEventHandler;
  ProjectItemEventHandler := RegisterRaveEventHandler(TRaveProjectItemEventHandler) as TRaveProjectItemEventHandler;
  ReportActionEventHandler := RegisterRaveEventHandler(TRaveReportActionEventHandler) as TRaveReportActionEventHandler;
  PageDesignerEventHandler := RegisterRaveEventHandler(TRavePageDesignerEventHandler) as TRavePageDesignerEventHandler;
  PropertyPanelEventHandler := RegisterRaveEventHandler(TRavePropertyPanelEventHandler) as TRavePropertyPanelEventHandler;
  ApplicationEventHandler := RegisterRaveEventHandler(TRaveApplicationEventHandler) as TRaveApplicationEventHandler;
  ModuleEventHandler := RegisterRaveEventHandler(TRaveModuleEventHandler) as TRaveModuleEventHandler;
end;  { RaveRegister }

(*****************************************************************************}
( class TRaveMasterProjectEditor
(*****************************************************************************)

// Project events
procedure TRaveMasterProjectEditor.AfterProjectOpen; begin end;
function TRaveMasterProjectEditor.BeforeProjectClose: boolean; begin Result := true; end;
function TRaveMasterProjectEditor.BeforeNewProject: boolean; begin Result := true; end;
procedure TRaveMasterProjectEditor.AfterNewProject; begin end;
function TRaveMasterProjectEditor.BeforeProjectSave: boolean; begin Result := true; end;
procedure TRaveMasterProjectEditor.AfterProjectSave; begin end;
procedure TRaveMasterProjectEditor.PrepareViewChange(NewProject: boolean); begin end;
procedure TRaveMasterProjectEditor.AfterViewChange(NewProject: boolean); begin end;
// ProjectItem events
function TRaveMasterProjectEditor.BeforeNewReport: boolean; begin Result := true; end;
procedure TRaveMasterProjectEditor.AfterNewReport(Report: TRaveReport); begin end;
function TRaveMasterProjectEditor.BeforeNewReportPage: boolean; begin Result := true; end;
procedure TRaveMasterProjectEditor.AfterNewReportPage(Page: TRavePage); begin end;
function TRaveMasterProjectEditor.BeforeNewGlobalPage: boolean; begin Result := true; end;
procedure TRaveMasterProjectEditor.AfterNewGlobalPage(Page: TRavePage); begin end;
function TRaveMasterProjectEditor.BeforeNewDataObject: boolean; begin Result := true; end;
procedure TRaveMasterProjectEditor.AfterNewDataObject(DataObject: TRaveDataObject); begin end;
// Report Action events
function TRaveMasterProjectEditor.BeforeReportPrint: boolean; begin Result := true; end;
procedure TRaveMasterProjectEditor.AfterReportPrint; begin end;
function TRaveMasterProjectEditor.BeforeReportActivate: boolean; begin  Result := true; end;
procedure TRaveMasterProjectEditor.AfterReportActivate; begin end;
// Page Designer events
function TRaveMasterProjectEditor.BeforePageChange: boolean; begin Result := true; end;
procedure TRaveMasterProjectEditor.AfterPageChange; begin end;
procedure TRaveMasterProjectEditor.ShowPage(Component: TRaveComponent); begin end;
procedure TRaveMasterProjectEditor.AfterZoomChange; begin end;
procedure TRaveMasterProjectEditor.AfterZoomToolChange; begin end;
procedure TRaveMasterProjectEditor.ShowControlPopup(Point: PPoint); begin end;
procedure TRaveMasterProjectEditor.AfterNewComponent(Component: TRaveComponent); begin end;
// Property Panel events
procedure TRaveMasterProjectEditor.AfterSelectionChange; begin end;
procedure TRaveMasterProjectEditor.AfterPropertiesModified; begin end;
procedure TRaveMasterProjectEditor.SelectComponent(Component: TRaveComponent); begin end;
// Application events
procedure TRaveMasterProjectEditor.AfterAppOpen; begin end;
function TRaveMasterProjectEditor.BeforeAppClose: boolean; begin Result := true; end;
function TRaveMasterProjectEditor.ShowAboutDialog: boolean; begin Result := true; end;
procedure TRaveMasterProjectEditor.GetAppTitle; begin end;
procedure TRaveMasterProjectEditor.AddShortCuts; begin end;
procedure TRaveMasterProjectEditor.LoadState; begin end;
procedure TRaveMasterProjectEditor.SaveState; begin end;
procedure TRaveMasterProjectEditor.Initialize; begin end;
procedure TRaveMasterProjectEditor.MergeMenu(MainMenu: TMainMenu); begin end;

(*****************************************************************************}
( class TRaveProjectEventHandler
(*****************************************************************************)

procedure TRaveProjectEventHandler.Process(Index: integer);

var
  IItem: IRaveProjectEventHandler;

begin { Process }
  IItem := ProjectEditor as IRaveProjectEventHandler;
  case Index of
    1: IItem.AfterProjectOpen;
    2: FBoolRes := IItem.BeforeProjectClose;
    3: FBoolRes := IItem.BeforeNewProject;
    4: IItem.AfterNewProject;
    5: FBoolRes := IItem.BeforeProjectSave;
    6: IItem.AfterProjectSave;
    7: IItem.PrepareViewChange(boolean(FObj));
    8: IItem.AfterViewChange(boolean(FObj));
  end; { case }
end;  { Process }

procedure TRaveProjectEventHandler.AfterProjectOpen; begin Broadcast(1); end;
function TRaveProjectEventHandler.BeforeProjectClose: boolean; begin Result := BroadcastQuery(2); end;
function TRaveProjectEventHandler.BeforeNewProject; begin Result := BroadcastQuery(3); end;
procedure TRaveProjectEventHandler.AfterNewProject; begin Broadcast(4); end;
function TRaveProjectEventHandler.BeforeProjectSave; begin Result := BroadcastQuery(5); end;
procedure TRaveProjectEventHandler.AfterProjectSave; begin Broadcast(6); end;
procedure TRaveProjectEventHandler.PrepareViewChange(NewProject: boolean); begin BroadcastObj(7,TObject(NewProject)); end;
procedure TRaveProjectEventHandler.AfterViewChange(NewProject: boolean); begin BroadcastObj(8,TObject(NewProject)); end;

(*****************************************************************************}
( class TRaveProjectItemEventHandler
(*****************************************************************************)

procedure TRaveProjectItemEventHandler.Process(Index: integer);

var
  IItem: IRaveProjectItemEventHandler;

begin { Process }
  IItem := ProjectEditor as IRaveProjectItemEventHandler;
  case Index of
    1: FBoolRes := IItem.BeforeNewReport;
    2: IItem.AfterNewReport(TRaveReport(FObj));
    3: FBoolRes := IItem.BeforeNewReportPage;
    4: IItem.AfterNewReportPage(TRavePage(FObj));
    5: FBoolRes := IItem.BeforeNewGlobalPage;
    6: IItem.AfterNewGlobalPage(TRavePage(FObj));
    7: FBoolRes := IItem.BeforeNewDataObject;
    8: IItem.AfterNewDataObject(TRaveDataObject(FObj));
  end; { case }
end;  { Process }

function TRaveProjectItemEventHandler.BeforeNewReport: boolean; begin Result := BroadcastQuery(1); end;
procedure TRaveProjectItemEventHandler.AfterNewReport(Report: TRaveReport); begin BroadcastObj(2,Report); end;
function TRaveProjectItemEventHandler.BeforeNewReportPage: boolean; begin Result := BroadcastQuery(3); end;
procedure TRaveProjectItemEventHandler.AfterNewReportPage(Page: TRavePage); begin BroadcastObj(4,Page); end;
function TRaveProjectItemEventHandler.BeforeNewGlobalPage: boolean; begin Result := BroadcastQuery(5); end;
procedure TRaveProjectItemEventHandler.AfterNewGlobalPage(Page: TRavePage); begin BroadcastObj(6,Page); end;
function TRaveProjectItemEventHandler.BeforeNewDataObject: boolean; begin Result := BroadcastQuery(7); end;
procedure TRaveProjectItemEventHandler.AfterNewDataObject(DataObject: TRaveDataObject); begin BroadcastObj(8,DataObject); end;

(*****************************************************************************}
( class TRaveReportActionEventHandler
(*****************************************************************************)

procedure TRaveReportActionEventHandler.Process(Index: integer);

var
  IItem: IRaveReportActionEventHandler;

begin { Process }
  IItem := ProjectEditor as IRaveReportActionEventHandler;
  case Index of
    1: FBoolRes := IItem.BeforeReportPrint;
    2: IItem.AfterReportPrint;
    3: FBoolRes := IItem.BeforeReportActivate;
    4: IItem.AfterReportActivate;
  end; { case }
end;  { Process }

function TRaveReportActionEventHandler.BeforeReportPrint: boolean; begin Result := BroadcastQuery(1); end;
procedure TRaveReportActionEventHandler.AfterReportPrint; begin Broadcast(2); end;
function TRaveReportActionEventHandler.BeforeReportActivate: boolean; begin Result := BroadcastQuery(3); end;
procedure TRaveReportActionEventHandler.AfterReportActivate; begin Broadcast(4); end;

(*****************************************************************************}
( class TRavePageDesignerEventHandler
(*****************************************************************************)

procedure TRavePageDesignerEventHandler.Process(Index: integer);

var
  IItem: IRavePageDesignerEventHandler;

begin { Process }
  IItem := ProjectEditor as IRavePageDesignerEventHandler;
  case Index of
    1: FBoolRes := IItem.BeforePageChange;
    2: IItem.AfterPageChange;
    3: IItem.ShowPage(TRaveComponent(FObj));
    4: IItem.AfterZoomChange;
    5: IItem.AfterZoomToolChange;
    6: IItem.ShowControlPopup(PPoint(FPtr));
    7: IItem.AfterNewComponent(TRaveComponent(FObj));
  end; { case }
end;  { Process }

function TRavePageDesignerEventHandler.BeforePageChange: boolean; begin Result := BroadcastQuery(1); end;
procedure TRavePageDesignerEventHandler.AfterPageChange; begin Broadcast(2); end;
procedure TRavePageDesignerEventHandler.ShowPage(Component: TRaveComponent); begin BroadcastObj(3,Component); end;
procedure TRavePageDesignerEventHandler.AfterZoomChange; begin Broadcast(4); end;
procedure TRavePageDesignerEventHandler.AfterZoomToolChange; begin Broadcast(5); end;
procedure TRavePageDesignerEventHandler.ShowControlPopup(Point: PPoint); begin BroadcastPtr(6,Point); end;
procedure TRavePageDesignerEventHandler.AfterNewComponent(Component: TRaveComponent); begin BroadcastObj(7,Component); end;

(*****************************************************************************}
( class TRavePropertyPanelEventHandler
(*****************************************************************************)

procedure TRavePropertyPanelEventHandler.Process(Index: integer);

var
  IItem: IRavePropertyPanelEventHandler;

begin { Process }
  IItem := ProjectEditor as IRavePropertyPanelEventHandler;
  case Index of
    1: IItem.AfterSelectionChange;
    2: IItem.AfterPropertiesModified;
    3: IItem.SelectComponent(TRaveComponent(FObj));
  end; { case }
end;  { Process }

procedure TRavePropertyPanelEventHandler.AfterSelectionChange; begin Broadcast(1); end;
procedure TRavePropertyPanelEventHandler.AfterPropertiesModified; begin Broadcast(2); end;
procedure TRavePropertyPanelEventHandler.SelectComponent(Component: TRaveComponent); begin BroadcastObj(3,Component); end;

(*****************************************************************************}
( class TRaveApplicationEventHandler
(*****************************************************************************)

procedure TRaveApplicationEventHandler.Process(Index: integer);

var
  IItem: IRaveApplicationEventHandler;

begin { Process }
  IItem := ProjectEditor as IRaveApplicationEventHandler;
  case Index of
    1: IItem.AfterAppOpen;
    2: FBoolRes := IItem.BeforeAppClose;
    3: FBoolRes := IItem.ShowAboutDialog;
    4: IItem.GetAppTitle;
    5: IItem.AddShortCuts;
    6: IItem.LoadState;
    7: IItem.SaveState;
    8: IItem.Initialize;
    9: IItem.MergeMenu(TMainMenu(FObj));
  end; { case }
end;  { Process }

procedure TRaveApplicationEventHandler.AfterAppOpen; begin Broadcast(1); end;
function TRaveApplicationEventHandler.BeforeAppClose: boolean; begin Result := BroadcastQuery(2); end;
function TRaveApplicationEventHandler.ShowAboutDialog: boolean; begin Result := BroadcastQuery(3); end;
procedure TRaveApplicationEventHandler.GetAppTitle; begin Broadcast(4); end;
procedure TRaveApplicationEventHandler.AddShortCuts; begin Broadcast(5); end;
procedure TRaveApplicationEventHandler.LoadState; begin Broadcast(6); end;
procedure TRaveApplicationEventHandler.SaveState; begin Broadcast(7); end;
procedure TRaveApplicationEventHandler.Initialize; begin Broadcast(8); end;
procedure TRaveApplicationEventHandler.MergeMenu(MainMenu: TMainMenu); begin BroadcastObj(9,MainMenu) end;

(*****************************************************************************}
( class TRaveApplicationEventHandler
(*****************************************************************************)

procedure TRaveModuleEventHandler.Process(Index: integer);
var
  IItem: IRaveModuleEventHandler;
begin { Process }
  IItem := ProjectEditor as IRaveModuleEventHandler;
  case Index of
    1: IItem.CompileStatus(TRaveModule(FObj));
    2: IItem.CompileError(TRaveCompileStatus(FObj));
    3: IItem.CompileSuccess;
  end; { case }
end;  { Process }

procedure TRaveModuleEventHandler.CompileStatus(Module: TRaveModule); begin BroadcastObj(1,Module); end;
procedure TRaveModuleEventHandler.CompileError(CompileStatus: TRaveCompileStatus); begin BroadcastObj(2,CompileStatus); end;
procedure TRaveModuleEventHandler.CompileSuccess; begin Broadcast(3); end;

initialization
  RegisterProc({Trans-}'RVCL',RaveRegister);
end.