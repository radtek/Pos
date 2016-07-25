library DataLinkInterbase;

{ Implements Interbase Data Link Driver }

uses
  RvDLBase,
  RvDLInterbase,
  RvDLInterbaseCfg;

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