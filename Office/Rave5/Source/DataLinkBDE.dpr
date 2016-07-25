library DataLinkBDE;

{ Implements BDE Data Link Driver }

uses
  RvDLBase,
  RvDLBDE,
  RvDLBDECfg;

{$E rvd}

exports
  DataInformation,
  DataConnectionConfigure,
  DataConnectionOpen,
  DataConnectionClose,
  DataGetTables,
  DataGetFields,
  DataResultSetOpen,
  DataResultSetClose,
  DataResultSetFirst,
  DataResultSetNext,
  DataResultSetEOF,
  DataResultSetGetRow,
  DataResultSetSetFilter,
  DataResultSetSetSort,
  DataResultSetConfigure,
  DataGetErrorText;

begin
end.