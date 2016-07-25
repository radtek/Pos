library DataLinkDBX;

{ Implements DBX Data Link Driver }

uses
  RvDLBase,
  RvDLDBX,
  RvDLDBXCfg;

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