{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit QRvLDCompiler;

interface

{.$DEFINE DOMAP}
{TransOff}

uses
  Classes, SysUtils,
  QDialogs, QStdCtrls, QForms,
  QRvClass, QRvLEDefine, QRvLEUtil, QRvLEType, QRvLEID,
  QRvLEModule, QRvLECode, QRvLEExpr, QRvUtil, QRpDefine;

type
  TCompileMode = (cmType,cmConst,cmVar,cmFunc);
  TCompileLoc = (clStart,clDefinition,clImplementation,clCreation,clDestruction);

  TRaveDelphiCompiler = class(TRaveCompiler)
  protected
    DefineMode: boolean; { Are we in the definition or implementation section? }
    CompileLoc: TCompileLoc;
    GlobalFreeList: TList;
    BreakStack: TList;
    ContinueStack: TList;
    ExitLabel: TCodeLabel;

    Parser: TRaveParser;
    CM: TCodeManager;
    Expr: TExpression;
    Module: TRCPUModule;
    TempStart: longint; { Start of temporary stack area }
    TempMax: longint; { Max temp space needed for statements in a func}
    FStatusLabel: TLabel;
    {$IFDEF DOMAP}xMapFile: TStringList;{$ENDIF}

    procedure StartLoopStatement;
    procedure FinishLoopStatement;
    function ContinueObject: TCodeLabelObject;
    function BreakObject: TCodeLabelObject;

    procedure InitCompiler;
    procedure CompileConst;
    procedure GetOrdinalConst(var Value: longint; var BaseType: TBaseType; Terminators: string);
    procedure GetRange(var Range: TRange);
    function CreateEnumeratedType: TEnumeratedType;
    function CreateArrayType: TArrayType;
    function CreateRecordType(AID: TIdentifier): TRecordType;
    function CreateFuncType: TFuncType;
    function CreateClassType(AID: TIdentifier): TClassType;
    function CreateTypeDef(Simple: boolean; AID: TIdentifier): TBaseType;
    procedure CompileType;
    procedure CompileExpression;
    procedure CompileIf;
    procedure CompileFor;
    procedure CompileWhile;
    procedure CompileRepeat;
    procedure CompileBreak;
    procedure CompileContinue;
    procedure CompileExit;
    procedure CompileSuspend;
    procedure CompileWith;
    function CompileStatement(Terminators: string): boolean;
    procedure CompileVar(FreeList: TList);
    procedure CompileFunc;
    procedure CompileImport;
    procedure CompileModule;
    procedure CompileSection(Definition: boolean);
  public
    constructor Create; override;
    destructor Destroy; override;

    class function SyntaxName: string; override;
    procedure CompileSystemModule;
    procedure CompileStream(Stream: TStream; FileName: string; AModule: TRCPUModule; Definition: boolean); override;
    procedure CompileFile(FileName: string; AModule: TRCPUModule);
    procedure CompileExpr(Source: string; AModule: TRCPUModule);
    property StatusLabel: TLabel read FStatusLabel write FStatusLabel;
  end; { TRaveDelphiCompiler }

  procedure RaveRegister;

implementation

{$IFDEF DOMAP}
uses
  QRvLEDebug;
{$ENDIF}

procedure RaveRegister;
begin { RaveRegister }
{ Register compiler with Language Engine }
  AddRaveCompiler(TRaveDelphiCompiler);
end;  { RaveRegister }

{ class TRaveDelphiCompiler }

constructor TRaveDelphiCompiler.Create;
begin { Create }
  inherited Create;
  BreakStack := TList.Create;
  ContinueStack := TList.Create;
end;  { Create }

destructor TRaveDelphiCompiler.Destroy;
begin { Destroy }
  inherited Destroy;
  FreeAndNil(BreakStack);
  FreeAndNil(ContinueStack);
end;  { Destroy }

procedure TRaveDelphiCompiler.CompileConst;
var
  ID: string;
begin { CompileConst }
  with Parser,Expr do begin
    ID := GetID;
    MatchSymbol('=');
    CreateCodeList('`;`',true);
    MatchSymbol(';');

  { Create new ConstID }
    if Assigned(CodeList) and (CodeList.Count = 1) then begin
      with PCodeListItem(CodeList.Peek(0))^ do begin
        if (Kind = idkConst) and ((ToType is TSimpleType) or
         (ToType is TEnumeratedType)) then begin
          if ToType.TypeKind = tkString then begin
            Module.AddConst(ID,StringType,StringData);
          end else begin
            Module.AddConst(ID,ToType,Data^);
          end; { else }

          {$IFDEF DOMAP}MapLine;{$ENDIF}
          {$IFDEF DOMAP}Map('Constant ' + ID + ' = ');{$ENDIF}
          {$IFDEF DOMAP}MapConstant(PCodeListItem(CodeList.Peek(0)));{$ENDIF}
        end else begin
          AddError(9,''); { Handle constant vars? }
        end; { else }
      end; { with }
    end else begin
      AddError(10,Parser.TokenName);
    end; { else }
    FreeCodeList;
  end; { with }
end;  { CompileConst }

procedure TRaveDelphiCompiler.GetOrdinalConst(var Value: longint; var BaseType: TBaseType; Terminators: string);
begin { GetOrdinalConst }
  with Expr do begin
    BaseType := nil;
    CreateCodeList(Terminators,false);
    if Assigned(CodeList) and (CodeList.Count = 1) then begin
      with PCodeListItem(CodeList.Peek(0))^ do begin
        if (Kind = idkConst) and ((ToType is TSimpleType) or
         (ToType is TEnumeratedType)) then begin
          BaseType := ToType;
          if (ToType is TSimpleType) then begin
            case ToType.TypeKind of
              tkBoolean: Value := Ord(boolean(Data^));
              tkInteger: Value := integer(Data^);
              tkChar: Value := Ord(char(Data^));
              else BaseType := nil;
            end; { case }
          end else begin { Enumerated }
            Value := byte(Data^);
          end; { else }
        end else begin
          AddError(19,Parser.TokenName);
        end; { else }
      end; { with }
    end else begin
      AddError(19,Parser.TokenName);
    end; { else }
    FreeCodeList;
  end; { with }
end;  { GetOrdinalConst }

procedure TRaveDelphiCompiler.GetRange(var Range: TRange);
var
  Low: integer;
  LowType: TBaseType;
  High: integer;
  HighType: TBaseType;
  BaseID: TBaseID;
  GetLowHigh: boolean;
begin { GetRange }
  with Parser do begin
    Range.BaseType := nil;
    GetToken;
    GetLowHigh := true;
    if Parser.TokenType = ttID then begin
      BaseID := Module.FindID(Parser.TokenUpper);
      if Assigned(BaseID) and (BaseID is TTypeID) then begin
        if (BaseID.BaseType is TSimpleType) and
         (BaseID.BaseType.TypeKind in OrdinalTypes) then begin
          Range.Low := (BaseID.BaseType as TSimpleType).Low;
          Range.High := (BaseID.BaseType as TSimpleType).High;
          Range.BaseType := BaseID.BaseType;
          GetLowHigh := false;
        end else if BaseID.BaseType is TEnumeratedType then begin
          Range.Low := 0;
          Range.High := (BaseID.BaseType as TEnumeratedType).High;
          Range.BaseType := BaseID.BaseType;
          GetLowHigh := false;
        end; { else }
      end; { if }
    end; { if }

    if GetLowHigh then begin
      Parser.KeepToken := true;
      GetOrdinalConst(Low,LowType,'`..`');
      if Assigned(LowType) then begin
        MatchSymbol('..');
        GetOrdinalConst(High,HighType,'`]`,`');
        if Assigned(HighType) then begin
          if HighType = LowType then begin
            Range.Low := Low;
            Range.High := High;
            if Range.Low > Range.High then begin
              AddError(20,'');
            end else begin
              Range.BaseType := LowType;
            end; { else }
          end else begin
            AddError(16,'');
          end; { else }
        end else begin
          AddError(19,'');
        end; { else }
      end else begin
        AddError(19,'');
      end; { else }
    end; { if }
  end; { with }
end;  { GetRange }

function TRaveDelphiCompiler.CreateEnumeratedType: TEnumeratedType;
var
  B1: byte;
begin { CreateEnumeratedType }
{ Create type based on name and then create constants of all values in ()s }
  with Parser do begin
    Result := Module.CreateType(TEnumeratedType.Create) as TEnumeratedType;
    if Assigned(Result) then begin
    { Read in identifiers and create constants }
      B1 := 0;
      repeat
        GetID;
        if TokenType = ttID then begin
          if not Assigned(Module.AddConst(TokenName,Result,B1)) then begin
            AddError(15,TokenName);
          end; { if }
        end; { if }
        GetToken;
        if (TokenName <> ',') and (TokenName <> ')') then begin
          AddError(2,')');
        end; { if }
        if (TokenName = ',') then begin
          if B1 = 255 then begin
            AddError(17,'');
          end else begin
            Inc(B1);
          end; { else }
        end; { if }
      until TokenName = ')';
      MatchSymbol(';');
      (Result as TEnumeratedType).High := B1;
    end; { if }
  end; { with }
end;  { CreateEnumeratedType }

function TRaveDelphiCompiler.CreateArrayType: TArrayType;
var
  Range: TRange;
  ArrayType: TArrayType;
  Finished: boolean;
begin { CreateArrayType }
  with Parser do begin
    ArrayType := TArrayType.Create;
    MatchSymbol('[');
    Finished := false;
    repeat
    { Create new index level }
      GetRange(Range);
      if Assigned(Range.BaseType) then begin
        ArrayType.AddDim(Range.Low,Range.High,Range.BaseType);
      end; { if }
      GetToken; { Look for next dimension or end of dim definition }
      if TokenType = ttOperator then begin
        if TokenName = ']' then begin
          Finished := true;
        end else if TokenName <> ',' then begin
          AddError(2,']');
        end; { else }
      end else begin
        AddError(2,']');
      end; { else }
    until Finished;
    MatchID('OF');
    ArrayType.ElementType := CreateTypeDef(true,'');

    {$IFDEF DOMAP}MapArray(ArrayType);{$ENDIF}
    if ArrayType.DimSize[-1] >= MaxInt then begin
      AddError(21,'');
    end; { if }

  { Create array type }
    Result := Module.CreateType(ArrayType) as TArrayType;
    if not Assigned(Result) then begin
      ArrayType.Free;
      AddError(15,TokenName);
    end; { if }
  end; { with }
end;  { CreateArrayType }

function TRaveDelphiCompiler.CreateRecordType(AID: TIdentifier): TRecordType;
var
  ID: TIdentifier;
  Finished: boolean;
  BaseType: TBaseType;
  RecordType: TRecordType;
begin { CreateRecordType }
  RecordType := TRecordType.Create;
  Finished := false;
  repeat
    ID := Parser.GetID;
    if Parser.TokenUpper <> 'END' then begin { Read in field }
      Parser.MatchSymbol(':');
      BaseType := CreateTypeDef(true,'');
      RecordType.AddElement(ID,BaseType);
    end else begin
      Finished := true;
    end; { else }
  until Finished;
  Parser.MatchTerm(AID,';');

{ Create record type }
  Result := Module.CreateType(RecordType) as TRecordType;
  if not Assigned(Result) then begin
    RecordType.Free;
    AddError(15,Parser.TokenName);
  end; { if }
end;  { CreateRecordType }

function TRaveDelphiCompiler.CreateFuncType: TFuncType;
var
  Finished: boolean;
  FuncType: TFuncType;

  procedure GetParam;
  var
    BaseID: TBaseID;
    I1: integer;
    FirstParam: word;
    LastParam: word;
    Finished: boolean;
    ParamType: TParamType;
    S1: string;
  begin { GetParam }
    with Parser,Expr do begin
      FirstParam := FuncType.Params;
      LastParam := FirstParam;

      GetID;
      if TokenUpper = 'VAR' then begin
        ParamType := ptVar;
        GetID;
      end else begin
        ParamType := ptValue;
      end; { else }

    { Get ID's for all parameters on the line }
      Finished := false;
      repeat
        if TokenType = ttID then begin
          FuncType.AddParam(TokenName,ParamType);
        end; { if }
        GetToken;
        if TokenType = ttOperator then begin
          if TokenName = ',' then begin
            GetID;
            Inc(LastParam);
          end else if TokenName = ':' then begin
          { Get type for previous parameter names and assign to parameters }
            GetID; { Type name }
            BaseID := Module.FindID(TokenUpper);
            if Assigned(BaseID) and (BaseID is TTypeID) then begin
              for I1 := FirstParam to LastParam do begin
                FuncType.Param[I1].BaseType := BaseID.BaseType;
              end; { for }
            end else begin
              AddError(23,'');
            end; { else }
            Finished := true;
          end else if TokenName = ';' then begin { untyped parameter }
            if ParamType <> ptVar then begin
              AddError(31,'');
            end; { if }
          {!!! Handle untyped parameters }
            Finished := true;
            KeepToken := true;
          end else begin
            AddError(2,':');
          end; { else }
        end else begin
          AddError(2,':');
        end; { if }
      until Finished;

      GetToken;
      if TokenType = ttID then begin
        if TokenUpper = 'DEFAULT' then begin { Get constant expression }
          CreateCodeList('`;`)`',false);
          if Assigned(CodeList) and (CodeList.Count = 1) then begin
            with PCodeListItem(CodeList.Peek(0))^ do begin
              if (Kind = idkConst) and ((ToType is TSimpleType) or
               (ToType is TEnumeratedType)) then begin
              { Init Param.DefaultValue to code seg address }
                for I1 := FirstParam to LastParam do begin
                  FuncType.Param[I1].DefaultValue := CM.CodeSize;
                end; { for }
              { Store constant data in code segment }
                if ToType.TypeKind = tkString then begin
                  if StringData = '' then begin
                    S1 := #0;
                    CM.Append(S1[1],1);
                  end else begin
                    CM.Append(StringData[1],Length(StringData) + 1);
                  end; { else }
                end else begin
                  CM.Append(Data^,ToType.Size);
                end; { else }
              end else begin
                AddError(9,'');
              end; { else }
            end; { with }
          end else begin
            AddError(10,TokenName);
          end; { else }
          if ParamType <> ptValue then begin
            AddError(30,'');
          end; { if }
          FreeCodeList;
        end else begin
          KeepToken := true;
        end; { if }
      end else begin
        KeepToken := true;
      end; { else }
    end; { with }
  end;  { GetParam }

begin { CreateFuncType }
  with Parser do begin
    FuncType := TFuncType.Create;

  { Read in parameters }
    GetToken;
    if TokenType = ttOperator then begin
      if (TokenName <> '(') and (TokenName <> ':') and (TokenName <> ';') then begin
        AddError(2,';');
      end; { if }
      if TokenName = '(' then begin { Read in parameters }
      { Build parameters for function }
        Finished := false;
        repeat
          GetParam;
        { Look for ; or ) }
          GetToken;
          if TokenType = ttOperator then begin
            if TokenName = ')' then begin
              Finished := true;
            end else if TokenName <> ';' then begin
              AddError(2,')');
            end; { if }
          end else begin
            AddError(2,')');
          end; { else }
        until Finished;
        GetToken;
      end; { if }

    { Read in result type }
      if TokenType = ttOperator then begin
        if TokenName = ':' then begin
          GetID;
          FuncType.ResultType := Module.FindType(TokenUpper);
          MatchSymbol(';');
        end else if TokenName = ';' then begin
          FuncType.ResultType := nil;
        end else begin
          AddError(2,';');
        end; { else }
      end; { if }
    end else begin
      AddError(2,';');
    end; { else }
  end; { with }

  Result := FuncType;
end;  { CreateFuncType }

function TRaveDelphiCompiler.CreateClassType(AID: TIdentifier): TClassType;
var
  ClassType: TClassType;
  ParentClassName: string;
  ParentClassType: TBaseType;
  CurrScope: TScope;
  FuncType: TFuncType;
  ID: TIdentifier;
  ItemID: TIdentifier;
  Finished: boolean;
  BaseType: TBaseType;
  FuncID: TFuncID;
  ReadIndex: integer;
  WriteIndex: integer;
  Member: TStructureMember;
  Offset: integer;
begin { CreateClassType }
  Parser.GetToken;
  if Parser.TokenType = ttOperator then begin
    if Parser.TokenName = '(' then begin
      ParentClassName := Parser.GetID;
      Parser.MatchSymbol(')');
    end else begin
      AddError(2,'(');
    end; { else }
  end else begin
    ParentClassName := 'TClass';
    Parser.KeepToken := true;
  end; { else }
  ParentClassType := Module.FindType(ParentClassName);
  if (ParentClassType is TClassType) then begin
    ClassType := TClassType.Create(TClassType(ParentClassType),AID);
  end else begin
    Result := nil;
    AddError(43,'');
    Exit;
  end; { else }

  CurrScope := scPublic;
  Finished := false;
  repeat
    ID := Parser.GetID;
    case FindString(Parser.TokenUpper,'`PRIVATE`PROTECTED`PUBLIC`FUNCTION`PROPERTY`END`') of
      0: begin { Variable }
        Parser.MatchSymbol(':');
        BaseType := CreateTypeDef(true,'');
        ClassType.AddField(ID,BaseType,CurrScope);
      end;
      1: begin { Private }
        CurrScope := scPrivate;
      end;
      2: begin { Protected }
        CurrScope := scProtected;
      end;
      3: begin { Public }
        CurrScope := scPublic;
      end;
      4: begin { Function }
        ID := Parser.GetID; { Function Name }
        FuncType := CreateFuncType;
        Module.CreateType(FuncType);

      (* Beginning of code for Virtual/Override }
        With ClassType.AddMember(ID,FuncType,CurrScope) do begin
          Repeat
            GetToken;
            Case FindString(Parser.TokenUpper,'`VIRTUAL`OVERRIDE`') of
              0: Parser.KeepToken := true;
              1: begin
                Options := Options + [soVirtual];
                ClassType.AddVirtual();

              { Make sure this method is not in parent class }


                If TClassType(ParentClassType).FindID(ID,[CurrScope],Offset,
                 BaseType) then begin
                  AddError(
                end else begin
                end; { else }
              end;
              2: begin
                Options := Options + [soOverride];
              end;
            end; { case }
          until Parser.KeepToken;
        end; { with }
      *)

        ClassType.AddMethod(ID,FuncType,CurrScope,[]);
        FuncID := Module.AddFunc(AID + '.' + ID,FuncType);
        if Assigned(FuncID) then begin
          FuncID.DefineLevel := 1; { Defined only }
        end else begin
          AddError(29,AID + '.' + ID);
        end; { else }
      end;
      5: begin { Property }
        ID := Parser.GetID; { Property Name }
        Parser.MatchSymbol(':');
        ItemID := Parser.GetID;
        BaseType := Module.FindType(ItemID);

        ReadIndex := 0;
        WriteIndex := 0;
        repeat
          Parser.GetToken;
          case FindString(Parser.TokenUpper,'`READ`WRITE`') of
            1: begin { Read from item }
              ItemID := Parser.GetID;
              if ReadIndex > 0 then begin
                AddError(46,'');
              end else if not ClassType.FindMember(ItemID,AllScopes,Offset,
               Member,ReadIndex) then begin
                AddError(22,ItemID);
              end else begin { Read Member found }
                if (Member.BaseType is TFuncType) then begin
                  FuncType := TFuncType(Member.BaseType);
                  if FuncType.ResultType <> BaseType then begin
                    AddError(48,'');
                  end; { if }
                  if FuncType.Params <> 0 then begin
                    AddError(49,'');
                  end; { if }
                end; { if }
              end; { else }
            end;
            2: begin { Write to item }
              ItemID := Parser.GetID;
              if WriteIndex > 0 then begin
                AddError(47,'');
              end else if not ClassType.FindMember(ItemID,AllScopes,Offset,
               Member,WriteIndex) then begin
                AddError(22,ItemID);
              end else begin { Write Member found }
                if (Member.BaseType is TFuncType) then begin
                  FuncType := TFuncType(Member.BaseType);
                  if FuncType.Params = 1 then begin
                    if Assigned(FuncType.ResultType) then begin
                      AddError(52,'');
                    end else if FuncType.Param[0].BaseType <> BaseType then begin
                      AddError(50,'');
                    end; { else }
                  end else begin
                    AddError(51,'');
                  end; { else }
                end; { if }
              end; { else }
            end;
          end; { case }
        until Parser.TokenName = ';';
        Parser.KeepToken := true;
        Parser.MatchSymbol(';');
        ClassType.AddProperty(ID,BaseType,CurrScope,ReadIndex,WriteIndex);
      end;
      6: begin { End }
        Finished := true;
      end;
    end; { case }
  until Finished;
  Parser.MatchTerm(AID,';');

{ Create record type }
  Result := Module.CreateType(ClassType) as TClassType;
  if not Assigned(Result) then begin
    ClassType.Free;
    AddError(15,Parser.TokenName);
  end; { if }
end;  { CreateClassType }

function TRaveDelphiCompiler.CreateTypeDef(Simple: boolean; AID: TIdentifier): TBaseType;
var
  BaseID: TBaseID;
  SearchString: string;
begin { CreateTypeDef }
  with Parser do begin
  { Find or create type definition }
    GetToken;
    Result := nil;
    if (TokenType = ttOperator) and (TokenName = '(') then begin { Enumerated }
      Result := CreateEnumeratedType;
    end else if TokenType = ttID then begin
      if Simple then begin
        SearchString := '`ARRAY`';
      end else begin
        SearchString := '`ARRAY`RECORD`CLASS`';
      end; { else }
      case FindString(TokenUpper,SearchString) of
        0: begin
          BaseID := Module.FindID(TokenUpper);
          if Assigned(BaseID) and (BaseID is TTypeID) then begin
            Result := BaseID.BaseType;
          end; { if }
          MatchSymbol(';');
        end;
        1: Result := CreateArrayType;
        2: Result := CreateRecordType(AID);
        3: Result := CreateClassType(AID);
      end; { case }
    end else begin { Invalid type }
      AddError(14,'');
      ReadUntil(';');
    end; { else }
  end; { with }
end;  { CreateTypeDef }

procedure TRaveDelphiCompiler.CompileType;
var
  ID: string;
  BaseType: TBaseType;
begin { CompileType }
  with Parser do begin
    ID := GetID;
    MatchSymbol('=');
    BaseType := CreateTypeDef(false,ID);
    if Assigned(BaseType) then begin
      Module.AddType(ID,BaseType);
    end; { if }
  end; { with }
end;  { CompileType }

procedure TRaveDelphiCompiler.CompileVar(FreeList: TList);

type
  PVarItem = ^TVarItem;
  TVarItem = record
    Line: integer;
    Col: integer;
    Name: string;
  end; { TVarItem }

var
  NameList: TList;
  Finished: boolean;
  VarItem: PVarItem;
  BaseType: TBaseType;
  VarID: TVarID;
  I1: integer;
  FreeVar: PFreeVar;

begin { CompileVar }
  with Parser do begin
    NameList := TList.Create;
    Finished := false;
    BaseType := nil;
    repeat
      New(VarItem);
      VarItem.Name := GetID;
      VarItem.Line := TokenLine;
      VarItem.Col := TokenCol;
      NameList.Add(VarItem);
      GetToken;
      if TokenType = ttOperator then begin
        if TokenName = ':' then begin
          BaseType := CreateTypeDef(true,'');
          Finished := true;
        end else if TokenName <> ',' then begin
          AddError(2,':');
        end; { else }
      end else begin
        AddError(2,':');
      end; { else }
    until Finished;

    for I1 := 0 to NameList.Count - 1 do begin
      VarItem := PVarItem(NameList[I1]);
      if Assigned(BaseType) then begin
        VarID := Module.AddVar(VarItem.Name,BaseType);
        if Assigned(VarID) then begin
          if VarID.BaseType.NeedFree and
           Assigned(FreeList) then begin
            New(FreeVar);
            FreeVar.Address := VarID.Address;
            FreeVar.BaseType := VarID.BaseType;
            FreeList.Add(FreeVar);
          end; { if }
        end else begin
          TokenLine := VarItem.Line;
          TokenCol := VarItem.Col;
          AddError(15,VarItem.Name);
        end; { else }
      end; { if }
      Dispose(VarItem);
    end; { for }
    NameList.Free;
  end; { with }
end;  { CompileVar }

procedure TRaveDelphiCompiler.CompileExpression;
begin { CompileExpression }
  with Parser,Expr do begin
    CreateCodeList('`;`',false);
    MatchSymbol(';');
    {$IFDEF DOMAP}MapCodeList('Code List',CodeList);{$ENDIF}
    CreateCode(TempMax,TempStart);
    FreeCodeList;
  end; { with }
end;  { CompileExpression }

procedure TRaveDelphiCompiler.CompileIf;
var
  CL1: TCodeLabel;
  CL2: TCodeLabel;
  Finished: boolean;
  ElseFound: boolean;
  ReadCond: boolean;
  SimpleIf: boolean;
begin { CompileIf }
(*
  if ConditionA then
    Statement;
  elseif ConditionB then
    Statement;
  elseif ConditionC then
    Statement;
  else
    Statement;
  end if;
*)

  with Expr,Parser do begin
    ElseFound := false;
    SimpleIf := true;
    MatchID('IF');
    ReadCond := true;
    Finished := false;
    repeat
      if ReadCond then begin
        ReadCond := false;
        CreateCodeList('`THEN`',true);
        if ResultItem.ToType.TypeKind <> tkBoolean then begin
          AddError(40,'');
        end; { else }
        CreateCode(TempMax,TempStart);
        MatchID('THEN');
        CL1 := CM.CreateLabel;
        CM.Test;
        CM.JumpNE(CL1);
        {$IFDEF DOMAP}MapCodeList('Code List',CodeList);{$ENDIF}
        FreeCodeList;
      end; { if }

      while CompileStatement('`ELSEIF`ELSE`END`') do begin end;
      GetToken;
      if TokenUpper = 'ELSEIF' then begin
        if SimpleIf then begin
          CL2 := CM.CreateLabel;
          SimpleIf := false;
        end; { if }
        CM.Jump(CL2);
        CM.FixLabel(CL1);
        if ElseFound then begin
          AddError(41,'');
        end; { if }
        ReadCond := true;
      end else if TokenUpper = 'ELSE' then begin
        if SimpleIf then begin
          CL2 := CM.CreateLabel;
          SimpleIf := false;
        end; { if }
        CM.Jump(CL2);
        CM.FixLabel(CL1);
        if ElseFound then begin
          AddError(42,'');
        end; { if }
        ElseFound := true;
      end else begin { END }
        Finished := true;
      end; { else }
    until Finished;
    MatchTerm('IF',';');
    if SimpleIf then begin
      CM.FixLabel(CL1);
    end else begin
      CM.FixLabel(CL2);
    end; { else }
  end; { with }
end;  { CompileIf }

procedure TRaveDelphiCompiler.CompileFor;
var
  BaseID: TBaseID;
  ToLoop: boolean;
  I1: integer;
  Addr: TAddress;
  TopLabel: TCodeLabel;
begin { CompileFor }
(*
  for I1 := I2 to I3 do   OR    for I1 := I2 downto I3 do
    Statement;                    Statement;
  end for;                      end for;
*)

  with Expr,Parser do begin
    MatchID('FOR');
    GetID;
    BaseID := Module.FindID(TokenUpper);
    ToLoop := true;
    if Assigned(BaseID) then begin
      if not (BaseID is TVarID) or
       not (BaseID.BaseType.TypeKind = tkInteger) then begin
        AddError(38,'');
      end else begin
      // Push Address to index var
        CM.PushAddr((BaseID as TVarID).Address);

        MatchSymbol(':=');

      // Create code for init value and leave on stack
        CreateCodeList('`DOWNTO`TO`',true);
        with ResultItem^ do begin
          if ToType.TypeKind in IntegerTypes then begin
            ToType := IntegerType;
          end else begin
            AddError(39,'');
          end; { else }
        end; { with }
        CreateCode(TempMax,TempStart);
        {$IFDEF DOMAP}MapCodeList('Code List',CodeList);{$ENDIF}
        FreeCodeList;

      // Assign the init value to the counter variable
        CM.Assign(RAVEASSIGN,4);

      // Get TO or DOWNTO
        GetToken;
        if TokenType = ttID then begin
          case FindString(TokenUpper,'`TO`DOWNTO`') of
            1: ToLoop := true;
            2: ToLoop := false;
            else begin
              AddError(2,'TO or DOWNTO');
            end;
          end; { case }
        end else begin
          AddError(2,'TO or DOWNTO');
        end; { else }

        TopLabel := CM.GetLabel;
        ContinueStack.Add(TCodeLabelObject.Create);
        ContinueObject.CodeLabel := CM.CreateLabel;
        BreakStack.Add(TCodeLabelObject.Create);
        BreakObject.CodeLabel := CM.CreateLabel;

      // Create code for terminal value
        CreateCodeList('`DO`',true);
        with ResultItem^ do begin
          if ToType.TypeKind in IntegerTypes then begin
            ToType := IntegerType;
          end else begin
            AddError(39,'');
          end; { else }
        end; { with }
        CreateCode(TempMax,TempStart);
        {$IFDEF DOMAP}MapCodeList('Code List',CodeList);{$ENDIF}
        FreeCodeList;
        MatchID('DO');

      // Create code for top of for loop
        Addr.Loc := alBP;
        Addr.Module := 0;
        Addr.Offset := -(TempStart + 4);
        if TempMax < 4 then begin
          TempMax := 4;
        end; { if }
        CM.PushAddr((BaseID as TVarID).Address); // Push counter
        if ToLoop then begin
          CM.RaveMath(RAVELTINT,Addr); // terminal value <= counter
        end else begin
          CM.RaveMath(RAVEGTINT,Addr); // terminal value >= counter
        end; { else }
        CM.Test;
        CM.JumpEQ(BreakObject.CodeLabel);
      end; { else }
    end else begin
      AddError(22,TokenName);
      ReadUntil('DO');
    end; { if }

    while CompileStatement('`END`') do begin end;
    MatchID('END');
    MatchTerm('FOR',';');

    CM.FixLabel(ContinueObject.CodeLabel);

  // Create code for end of for loop
    CM.PushAddr((BaseID as TVarID).Address); // Push counter
    I1 := 1;
    CM.PushConst(I1,SizeOf(I1));
    if ToLoop then begin
      CM.MathAssign(RAVEADDINTASSIGN); // Counter++
    end else begin
      CM.MathAssign(RAVESUBINTASSIGN); // Counter--
    end; { else }
    CM.Jump(TopLabel);
    FinishLoopStatement;
  end; { with }
end;  { CompileFor }

procedure TRaveDelphiCompiler.CompileWhile;
begin { CompileWhile }
  with Expr,Parser do begin
    MatchID('WHILE');
    CreateCodeList('`DO`',true);
    if CodeList.Count = 0 then begin
      AddError(40,'');
    end else begin
      with PCodeListItem(CodeList[CodeList.Count - 1])^ do begin
        if (Kind = idkOp) and (OpKind in OpStatementAssign) then begin
          AddError(28,'');
        end; { if }
        if ToType.TypeKind <> tkBoolean then begin
          AddError(40,'');
        end else begin
          ToType := BooleanType; { Make sure final result is a boolean }
        end; { else }
      end; { with }

      StartLoopStatement;
      CreateCode(TempMax,TempStart);
      CM.Test;
      CM.JumpNE(BreakObject.CodeLabel);
    end; { else }
    FreeCodeList;

    MatchID('DO');
    while CompileStatement('`END`') do begin end;
    MatchID('END');
    MatchTerm('WHILE',';');
    CM.Jump(ContinueObject.CodeLabel);
    FinishLoopStatement;
  end; { with }
end;  { CompileWhile }

procedure TRaveDelphiCompiler.CompileRepeat;
begin { CompileRepeat }
  with Expr,Parser do begin
    MatchID('REPEAT');
    StartLoopStatement;
    while CompileStatement('`UNTIL`') do begin end;
    CreateCodeList('`;`',true);
    if CodeList.Count = 0 then begin
      AddError(40,'');
    end else begin
      with PCodeListItem(CodeList[CodeList.Count - 1])^ do begin
        if (Kind = idkOp) and (OpKind in OpStatementAssign) then begin
          AddError(28,'');
        end; { if }
        if ToType.TypeKind <> tkBoolean then begin
          AddError(40,'');
        end else begin
          ToType := BooleanType; { Make sure final result is a boolean }
        end; { else }
      end; { with }
      CreateCode(TempMax,TempStart);
      CM.Test;
      CM.JumpNE(ContinueObject.CodeLabel);
      FinishLoopStatement;
    end; { else }
    FreeCodeList;
  end; { with }
end;  { CompileRepeat }

function TRaveDelphiCompiler.CompileStatement(Terminators: string): boolean;
begin { CompileStatement }
  with Parser do begin
    Result := true;
    GetToken;
    if Assigned(StatusLabel) then begin
      StatusLabel.Caption := 'Line: ' + IntToStr(TokenLine);
      Application.ProcessMessages;
    end; { if }
    {$IFDEF DOMAP}MapLine;{$ENDIF}
    {$IFDEF DOMAP}Map('Line ' + IntToStr(TokenLine));{$ENDIF}
    KeepToken := true;
    if TokenType = ttID then begin
      case FindString(TokenUpper,
       '`IF`FOR`WHILE`REPEAT`BREAK`CONTINUE`EXIT`SUSPEND`WITH' + Terminators) of
        0: CompileExpression;
        1: CompileIf;
        2: CompileFor;
        3: CompileWhile;
        4: CompileRepeat;
        5: CompileBreak;
        6: CompileContinue;
        7: CompileExit;
        8: CompileSuspend;
        9: CompileWith;
        else Result := false;
      end; { case }
    end else begin
      CompileExpression;
    end; { else }
  end; { with }
end;  { CompileStatement }

procedure TRaveDelphiCompiler.CompileFunc;
var
  ID: TIdentifier;
  FuncType: TFuncType;
  FuncID: TFuncID;
  BaseID: TBaseID;
  CompileMode: TCompileMode;
  Finished: boolean;
  LocalSize: integer;
  CL1: TCodeLabel;
  EnterPos: longint;
  I1: integer;
  VarID: TVarID;
  Addr: TAddress;
  LocalFreeList: TList;
  FreeVar: PFreeVar;
  IsMethod: boolean;
  OwnerClassName: string;
  OwnerClass: TClassType;
  SelfID: TVarID;
begin { CompileFunc }
  with Parser do begin
  { Create function type }
    ID := GetID; { Function Name }
    GetToken;
    if TokenName = '.' then begin
      IsMethod := true;
      OwnerClassName := ID;
      OwnerClass := (Module.FindType(OwnerClassName) as TClassType);
      ID := ID + '.' + GetID;
    end else begin
      IsMethod := false;
      OwnerClassName := '';
      OwnerClass := nil;
      KeepToken := true;
    end; { else }
    {$IFDEF DOMAP}if not DefineMode then begin
      MapFunc(ID);
    end; { if }{$ENDIF}
    FuncType := CreateFuncType;

  { Create function definition and quit if DefineMode }
    if DefineMode then begin
      Module.CreateType(FuncType);
      FuncID := Module.AddFunc(ID,FuncType);
      if Assigned(FuncID) then begin
        FuncID.DefineLevel := 1; { Defined }
      end else begin
        AddError(29,ID);
      end; { else }
      Exit;
    end; { if }

  { See if function was defined globally in this module }
    FuncID := nil;
    BaseID := Module.FindID(ID);
    if Assigned(BaseID) and (BaseID.Module <> 0) then begin
      BaseID := nil;
    end; { if }
    if Assigned(BaseID) then begin
      if BaseID is TFuncID then begin
        if FuncType.EqualTo(BaseID.BaseType as TFuncType) then begin
        { Use original definition }
          FuncType.Free;
          FuncType := BaseID.BaseType as TFuncType;
          FuncID := BaseID as TFuncID;
          FuncID.DefineLevel := 2;
        end else begin
          AddError(44,'');
        //!!! Abort compile
        end; { else }
      end else begin
        AddError(15,ID);
      end; { else }
    end else begin
      Module.CreateType(FuncType);
      FuncID := Module.AddFunc(ID,FuncType);
      if Assigned(FuncID) then begin
        FuncID.DefineLevel := 2;
      end else begin
        AddError(29,ID);
      end; { else }
    end; { else }

  { Check for an externally defined function and quit }
    GetToken;
    KeepToken := true;
    if TokenType = ttID then begin
      if TokenUpper = 'EXTERNAL' then begin
        KeepToken := false;
        MatchSymbol(';');

      { Write code for loading params and init ExecAddr }
        CL1 := CM.GetLabel;
        CM.UpdateEnter(0,CM.Enter);
        if IsMethod then begin
          Addr.Loc := alBP;
          Addr.Module := 0;
          Addr.Offset := FuncType.Params * 4 + 12;
          CM.LoadData(Addr,4);
        end; { if }
        Addr.Loc := alBP;
        Addr.Module := 0;
        Addr.Offset := FuncType.Params * 4 + 8;
        for I1 := 0 to FuncType.Params - 1 do begin
          with FuncType.Param[I1]^ do begin
            if ParamType = ptVar then begin
              CM.LoadVar(Addr);
            end else begin { Value }
              if Assigned(BaseType) then begin
                CM.LoadData(Addr,BaseType.Size);
              end; { if }
            end; { else }
          end; { with }
          Dec(Addr.Offset,4);
        end; { for }

      { Create function }
        FuncID.CodeLabel := CL1;
        if Assigned(FuncType.ResultType) then begin
          Addr.Loc := alBP;
          Addr.Module := 0;
          Addr.Offset := FuncType.Params * 4 + 12;
          if IsMethod then begin
            Inc(Addr.Offset,4);
          end; { if }
          FuncID.ExecAddr := CM.CallExternalFunc(FuncType.ResultType.TypeKind,
           Addr);
        end else begin
          FuncID.ExecAddr := CM.CallExternalProc;
        end; { else }
        if IsMethod then begin
          CM.Return((FuncType.Params + 1) * 4,Assigned(FuncType.ResultType));
        end else begin
          CM.Return(FuncType.Params * 4,Assigned(FuncType.ResultType));
        end; { else }
        Module.AddExternal(ID,FuncID.ExecAddr);
        Exit;
      end; { if }
    end; { if }

  { Create Result variable }
    Module.CreateScopeLevel;
    if Assigned(FuncType.ResultType) then begin
      VarID := Module.AddVar('RESULT',FuncType.ResultType);
      if Assigned(VarID) then begin
        Addr := FuncType.ParamAddr[-1];
        if IsMethod then begin
          Addr.Offset := Addr.Offset + 4;
        end; { if }
        VarID.Address := Addr;
      end else begin
        AddError(15,'Self');
      end; { else }
    end; { if }

  { Create Self variable }
    if IsMethod then begin
      SelfID := Module.AddVar('SELF',OwnerClass);
      if Assigned(SelfID) then begin
        SelfID.Address := FuncType.ParamAddr[-1];
      end else begin
        AddError(15,ID);
      end; { else }
    end else begin
      SelfID := nil;
    end; { else }

  { Create vars for function parameters }
    LocalFreeList := TList.Create;
    Addr.Loc := alBP;
    Addr.Module := 0;
    Addr.Offset := 0;
    for I1 := 0 to FuncType.Params - 1 do begin
      with FuncType.Param[I1]^ do begin
      { Create var for parameters }
        VarID := Module.AddVar(ID,BaseType);
        if Assigned(VarID) then begin
          if ParamType = ptVar then begin
            VarID.Address := FuncType.ParamAddr[I1];
          end else begin
            Dec(Addr.Offset,BaseType.Size); { allocate space for local var }
            VarID.Address := Addr;
            if VarID.BaseType.NeedFree then begin
              New(FreeVar);
              FreeVar.BaseType := VarID.BaseType;
              FreeVar.Address := Addr;
              LocalFreeList.Add(FreeVar);
            end; { if }
          end; { else }
        end else begin
          AddError(15,ID);
        end; { else }
      end; { with }
    end; { for }
    Module.Scope.DataSize := Abs(Addr.Offset);

  { Function definition section }
    Finished := false;
    CompileMode := cmFunc;
    repeat
      GetToken;
      if TokenType = ttID then begin
        case FindString(TokenUpper,'`CONST`TYPE`VAR`FUNCTION`BEGIN`') of
          0: begin { none }
            KeepToken := true;
            case CompileMode of
              cmConst: CompileConst;
              cmType: CompileType;
              cmVar: CompileVar(LocalFreeList);
              cmFunc: begin
                AddError(2,'BEGIN');
                Finished := true;
              end;
            end; { case }
          end;
          1: CompileMode := cmConst;
          2: CompileMode := cmType;
          3: CompileMode := cmVar;
          4: begin
            CompileMode := cmFunc;
            CompileFunc;
          end;
          5: Finished := true;
        end; { case }
      end else begin
        AddError(2,'BEGIN');
      end; { else }
    until Finished;
    LocalSize := Module.Scope.DataSize; { Size of local vars on stack }

  { Create function header }
    CL1 := CM.GetLabel;
    if Assigned(FuncID) then begin
      FuncID.CodeLabel := CL1;
    end; { if }
    if UpperCase(ID) = 'MAIN' then begin { Define starting position in code }
      Module.StartCO := CL1.Offset;
    end; { if }
    EnterPos := CM.Enter;

  { Create code for value parameters }
    TempStart := LocalSize;
    TempMax := 0;
    Addr.Loc := alBP;
    Addr.Module := 0;
    Addr.Offset := 0;
    for I1 := 0 to FuncType.Params - 1 do begin
      with FuncType.Param[I1]^ do begin
      { Update LocalSize and write code to copy value parameters onto stack }
        if ParamType = ptValue then begin
          Dec(Addr.Offset,BaseType.Size);
          if DefaultValue > 0 then begin
            if BaseType.TypeKind = tkString then begin
              CM.CopyParamStrDef(FuncType.ParamAddr[I1],Addr,DefaultValue);
            end else begin
              CM.CopyParamDataDef(FuncType.ParamAddr[I1],Addr,BaseType.Size,
               DefaultValue);
            end; { else }
          end else begin
            if BaseType.TypeKind = tkString then begin
              CM.CopyParamStr(FuncType.ParamAddr[I1],Addr);
            end else begin
              CM.CopyParamData(FuncType.ParamAddr[I1],Addr,BaseType.Size);
            end; { else }
          end; { else }
        end; { if }
      end; { with }
    end; { for }

  { Code to init local strings - Don't need since data is set to 0's }

  { Create Exit label }
    ExitLabel := CM.CreateLabel;

  { Function body section }
    if IsMethod then begin
      Module.Scope.PushObjectScope(SelfID);
    end; { if }
    while CompileStatement('`END`') do begin end;
    if IsMethod then begin
      Module.Scope.PopObjectScope;
    end; { if }
    MatchID('END');
    MatchTerm(ID,';');

  { Resolve any references to ExitLabel }
    CM.FixLabel(ExitLabel);

  { Code to finalize local strings and value parameter strings }
    for I1 := 0 to LocalFreeList.Count - 1 do begin
      FreeVar := PFreeVar(LocalFreeList[I1]);
      Expr.FreeType(FreeVar.Address,FreeVar.BaseType);
      Dispose(FreeVar);
    end; { for }
    LocalFreeList.Free;

  { Clean up }
    LocalSize := LocalSize + TempMax;
    CM.UpdateEnter(LocalSize,EnterPos);
    if UpperCase(ID) = 'MAIN' then begin { Terminate app }
      CM.Terminate;
    end else begin
      I1 := FuncType.Params * 4;
      if IsMethod then begin
        Inc(I1,4); { Add extra 4 bytes for Self pointer }
      end; { if }
      CM.Return(I1,Assigned(FuncType.ResultType));
    end; { else }
    Module.FreeScopeLevel;
  end; { with }
end;  { CompileFunc }

procedure TRaveDelphiCompiler.CompileImport;
var
  Finished: boolean;
begin { CompileImport }
{ Read in import list }
  Finished := false;
  repeat
    Module.AddModule(Parser.GetID);
    Parser.GetToken;
    if Parser.TokenType = ttOperator then begin
      if Parser.TokenName = ';' then begin
        Finished := true;
      end else if Parser.TokenName <> ',' then begin
        AddError(2,';');
      end; { else }
    end else begin
      AddError(2,';');
    end; { else }
  until Finished;
end;  { CompileImport }

procedure TRaveDelphiCompiler.CompileModule;
begin { CompileModule }
  CompileSection(true);
  CompileSection(false);
end;  { CompileModule }

procedure TRaveDelphiCompiler.CompileSection(Definition: boolean);
var
  CompileMode: TCompileMode;
  Finished: boolean;
  FreeVar: PFreeVar;
  I1: integer;
begin { CompileSection }
  with Parser do begin
    CompileMode := cmFunc;
    if Definition then begin
      CompileLoc := clStart;

    { Read module name }
      MatchID('MODULE');
      Module.ModuleName := GetID;
      MatchSymbol(';');
      {$IFDEF DOMAP}Map('Compiling Module ' + Module.ModuleName);
      MapLine;
      MapLine;
      MapLine;{$ENDIF}

    { Read Definition section }
      DefineMode := true;
      MatchID('DEFINITION');
      CompileLoc := clDefinition;
      if Assigned(GlobalFreeList) then begin
        GlobalFreeList.Clear;
      end; { if }
      GlobalFreeList := TList.Create;
      {$IFDEF DOMAP}MapCM := CM;{$ENDIF}
    end; { if }

    Finished := false;
    repeat
      GetToken;
      case TokenType of
        ttID: begin
          case FindString(TokenUpper,'`IMPORT`CONST`TYPE`VAR`FUNCTION' +
           '`IMPLEMENTATION`END`CREATION`DESTRUCTION`') of
            0: begin { None }
              KeepToken := true;
              case CompileMode of
                cmConst: CompileConst;
                cmType: CompileType;
                cmVar: CompileVar(GlobalFreeList);
                cmFunc: begin
                  AddError(35,'');
                  Finished := true;
                end;
              end; { case }
            end;
            1: begin { import }
              CompileImport;
              {$IFDEF DOMAP}MapCM := CM;{$ENDIF}
            end;
            2: begin { const }
              CompileMode := cmConst;
            end;
            3: begin { type }
              CompileMode := cmType;
            end;
            4: begin { var }
              CompileMode := cmVar;
            end;
            5: begin { function }
              CompileMode := cmFunc;
              CompileFunc;
            end;
            6: begin { implementation }
              if CompileLoc <> clDefinition then begin
                AddError(34,'');
                Exit;
              end; { if }
              CompileLoc := clImplementation;
              DefineMode := false;
              CompileMode := cmFunc;
              Finished := true;
            end;
            7: begin { end }
              if CompileLoc <> clImplementation then begin
                AddError(3,'');
                Exit;
              end; { if }
              MatchTerm(UpperCase(Module.ModuleName),';');
              GetToken;
              if TokenType <> ttEOF then begin
                AddError(4,TokenName);
              end; { if }
              Finished := true;
            end;
            8: begin { creation }
              if CompileLoc <> clImplementation then begin
                AddError(32,'');
                Exit;
              end; { if }
              CompileLoc := clCreation;
              Module.CreateCO := CM.CodeSize;
            {!!! Read in creation section until DESTRUCTION or END found }
            end;
            9: begin { destruction }
              if not (CompileLoc in [clImplementation,clCreation]) then begin
                AddError(33,'');
                Exit;
              end; { if }
              CompileLoc := clDestruction;
              Module.DestroyCO := CM.CodeSize;
            {!!! Read in destruction section until END found }
            end;
          end; { case }
        end;
        ttEOF: begin
          AddError(35,'');
          Finished := true;
        end;
        else begin
          AddError(4,TokenName);
        end;
      end; { case }
    until Finished;

    if not Definition then begin
    { If there was no OnDestroy section, then create one }
      if CompileLoc <> clDestruction then begin
        Module.DestroyCO := CM.CodeSize;
        CompileLoc := clDestruction;
      end; { if }

    { Free any global strings defined in this module }
      for I1 := 0 to GlobalFreeList.Count - 1 do begin
        FreeVar := PFreeVar(GlobalFreeList[I1]);
        Expr.FreeType(FreeVar.Address,FreeVar.BaseType);
        Dispose(FreeVar);
      end; { for }
      CM.Terminate;
      FreeAndNil(GlobalFreeList);
    end; { if }
  end; { with }
end;  { CompileSection }

class function TRaveDelphiCompiler.SyntaxName: string;
begin { SyntaxName }
  Result := 'Delphi';
end;  { SyntaxName }

procedure TRaveDelphiCompiler.CompileSystemModule;
var
  RM: TRCPUModule;
  B1: boolean;
  P1: pointer;
begin { CompileSystemModule }
  RM := TRCPUModule.Create;
//    {$IFDEF DOMAP}AssignFile(MapFile,'SYSTEM.MAP');{$ENDIF}
//    {$IFDEF DOMAP}Rewrite(MapFile);{$ENDIF}
  with RM do try
    ModuleName := 'SYSTEM';

  { Add types }
    AddType('BOOLEAN',CreateType(TSimpleType.Create(tkBoolean,0,1)));
    AddType('SHORTINT',CreateType(TSimpleType.Create(tkShortInt,-128,127)));
    AddType('BYTE',CreateType(TSimpleType.Create(tkByte,0,255)));
    AddType('SMALLINT',CreateType(TSimpleType.Create(tkSmallInt,-32768,32767)));
    AddType('WORD',CreateType(TSimpleType.Create(tkWord,0,65535)));
    AddType('INTEGER',CreateType(TSimpleType.Create(tkInteger,-MaxInt - 1,MaxInt)));
    AddType('SINGLE',CreateType(TSimpleType.Create(tkSingle,0,0)));
    AddType('DOUBLE',CreateType(TSimpleType.Create(tkDouble,0,0)));
    AddType('EXTENDED',CreateType(TSimpleType.Create(tkExtended,0,0)));
    AddType('CURRENCY',CreateType(TSimpleType.Create(tkCurrency,0,0)));
    AddType('CHAR',CreateType(TSimpleType.Create(tkChar,0,255)));
    AddType('STRING',CreateType(TSimpleType.Create(tkString,0,0)));
    AddType('POINTER',CreateType(TSimpleType.Create(tkPointer,0,0)));
    AddType('TCLASS',CreateType(TClassType.Create(nil,'TClass')));

  { Add operators }
    AddOp(':=',okAssign);
    AddOp('+=',okAddAssign);
    AddOp('-=',okSubAssign);
    AddOp('*=',okMulAssign);
    AddOp('/=',okDivAssign);
    AddOp('=',okEQ);
    AddOp('==',okEQ);
    AddOp('<>',okNE);
    AddOp('!=',okNE);
    AddOp('<',okLT);
    AddOp('>',okGT);
    AddOp('<=',okLE);
    AddOp('>=',okGE);
    AddOp('IS',okIs);
    AddOp('+',okAdd);
    AddOp('-',okSub);
    AddOp('OR',okOR);
    AddOp('|',okOR);
    AddOp('XOR',okXOR);
    AddOp('*',okMul);
    AddOp('/',okDiv);
    AddOp('DIV',okIDiv);
    AddOp('MOD',okMod);
    AddOp('%',okMod);
    AddOp('AND',okAND);
    AddOp('&',okAND);
    AddOp('SHL',okSHL);
    AddOp('<<',okSHL);
    AddOp('SHR',okSHR);
    AddOp('>>',okSHR);
    AddOp('AS',okAs);
    AddOp('U+',okPos);
    AddOp('U-',okNeg);
    AddOp('NOT',okNOT);
    AddOp('!',okNOT);
    AddOp('<-',okLeftAssign);
    AddOp('->',okRightAssign);

  { Add functions }
    with AddFunc('ORD',CreateType(TFuncType.Create) as TFuncType).BaseType as TFuncType do begin
      with AddParam('Value',ptValue)^ do begin
        BaseType := FindID('INTEGER').BaseType;
        DefaultValue := 0;
        Offset := 0;
      end; { with }
      ResultType := FindID('INTEGER').BaseType;
    end; { with }

  { Add constants }
    B1 := true;
    AddConst('TRUE',FindID('boolean').BaseType,B1);
    B1 := false;
    AddConst('FALSE',FindID('boolean').BaseType,B1);
    P1 := nil;
    AddConst('NIL',FindID('pointer').BaseType,P1);

    SaveToFile('System');
  finally
//      {$IFDEF DOMAP}CloseFile(MapFile);{$ENDIF}
    Free;
  end; { with }
end;  { CompileSystemModule }

procedure TRaveDelphiCompiler.InitCompiler;
begin { InitCompiler }
  if not Assigned(BooleanType) then begin
    BooleanType := Module.FindID('BOOLEAN').BaseType as TSimpleType;
  end; { if }
  if not Assigned(IntegerType) then begin
    IntegerType := Module.FindID('INTEGER').BaseType as TSimpleType;
  end; { if }
  if not Assigned(ExtendedType) then begin
    ExtendedType := Module.FindID('EXTENDED').BaseType as TSimpleType;
  end; { if }
  if not Assigned(CurrencyType) then begin
    CurrencyType := Module.FindID('CURRENCY').BaseType as TSimpleType;
  end; { if }
  if not Assigned(CharType) then begin
    CharType := Module.FindID('CHAR').BaseType as TSimpleType;
  end; { if }
  if not Assigned(StringType) then begin
    StringType := Module.FindID('STRING').BaseType as TSimpleType;
  end; { if }
  if not Assigned(OrdFunc) then begin
    OrdFunc := Module.FindID('ORD').BaseType as TFuncType;
  end; { if }
end;  { InitCompiler }

procedure TRaveDelphiCompiler.CompileStream(Stream: TStream; FileName: string; AModule: TRCPUModule; Definition: boolean);
begin { CompileStream }
{$IFDEF DOMAP}
  if Definition then begin
    xMapFile := TStringList.Create;
  end; { if }
  MapFile := xMapFile;
{$ENDIF}

  if Definition then begin
    if Assigned(StatusLabel) then begin
      StatusLabel.Caption := '';
      Application.ProcessMessages;
    end; { if }
    Module := AModule;
    Module.AddModule('SYSTEM'); { Always add the system module }
    InitCompiler;

  { Load file up }
    Parser := TRaveParser.Create;
    Parser.LoadFromStream(Stream);

  { Create code manager }
    CM := TCodeManager.Create(1024,Module);
    Expr := TExpression.Create(Module,Parser,CM);
  end; { if }

  {$IFDEF DOMAP}MapFile := xMapFile;{$ENDIF}
  try
    CompileSection(Definition);

    if not Definition then begin
      CM.FinishBuf;
      Module.Init;
//      {$IFDEF DOMAP}xMapFile.SaveToFile(FileName + '.MAP');{$ENDIF}
//      {$IFDEF DOMAP}FreeAndNil(xMapFile);{$ENDIF}
      FreeAndNil(Parser);
      FreeAndNil(Expr);
      FreeAndNil(CM);
      if Assigned(StatusLabel) then begin
        StatusLabel.Caption := Trans('Compile completed.');
        Application.ProcessMessages;
      end; { if }
    end; { with }
  finally
    if not Definition then begin
      {$IFDEF DOMAP}xMapFile.SaveToFile(FileName + '.MAP');{$ENDIF}
      {$IFDEF DOMAP}FreeAndNil(xMapFile);{$ENDIF}
    end; { if }
  end; { tryf }

  {$IFDEF DOMAP}MapFile := nil;{$ENDIF}
end;  { CompileStream }

procedure TRaveDelphiCompiler.CompileFile(FileName: string; AModule: TRCPUModule);
var
  Stream: TStream;
begin { CompileFile }
  Stream := TFileStream.Create(FileName + '.rvs',fmOpenRead);
  try
    CompileStream(Stream,FileName,AModule,true);
    CompileStream(Stream,FileName,AModule,false);
  finally
    Stream.Free;
  end; { tryf }
end;  { CompileFile }

procedure TRaveDelphiCompiler.CompileExpr(Source: string; AModule: TRCPUModule);
var
  VarID: TVarID;
  Address: TAddress;
  EnterPos: longint;
begin { CompileExpr }
  if Assigned(StatusLabel) then begin
    StatusLabel.Caption := '';
    Application.ProcessMessages;
  end; { if }
  Module := AModule;
  Module.AddModule('SYSTEM'); { Always add the system module }
  Module.AddModule('SYSFUNC'); { Always add the system module }
  InitCompiler;

{ Load file up }
  Parser := TRaveParser.Create;
  Parser.Text := Source;
{$IFDEF DOMAP}
  //AssignFile(MapFile,'EXPR.MAP');
  //Rewrite(MapFile);
{$ENDIF}

{ Create code manager }
  CM := TCodeManager.Create(1024,Module);
  {$IFDEF DOMAP}MapCM := CM;{$ENDIF}
  Expr := TExpression.Create(Module,Parser,CM);
  try
  { Add Result variable }
    VarID := Module.AddVar('RESULT',StringType); {!!! Handle other types }
    if Assigned(VarID) then begin
      Address.Loc := alDS;
      Address.Module := 0;
      Address.Offset := 0;
      VarID.Address := Address;
    end; { if }
    EnterPos := CM.Enter;
    TempStart := 0;
    TempMax := 0;
    repeat
      Parser.GetToken;
      if Parser.TokenType <> ttEOF then begin
        Parser.KeepToken := true;
        CompileStatement(';');
      end; { if }
    until Parser.TokenType = ttEOF;
    CM.UpdateEnter(TempMax,EnterPos);
    CM.Terminate;
    CM.FinishBuf;
  finally
  {$IFDEF DOMAP}{CloseFile(MapFile);}{$ENDIF}
    FreeAndNil(Parser);
    FreeAndNil(Expr);
    FreeAndNil(CM);
  end; { with }
  if Assigned(StatusLabel) then begin
    StatusLabel.Caption := Trans('Compile completed.');
    Application.ProcessMessages;
  end; { if }
end;  { CompileExpr }

procedure TRaveDelphiCompiler.CompileBreak;
begin
// Emit a jump to current Break Label
  Parser.MatchID('BREAK');
  Parser.MatchSymbol(';');
//!!! Add support for multiple Break(#) levels
  CM.Jump(BreakObject.CodeLabel);
end;

procedure TRaveDelphiCompiler.CompileContinue;
begin
// Emit a jump to current Continue Label
  Parser.MatchID('CONTINUE');
  Parser.MatchSymbol(';');
  CM.Jump(ContinueObject.CodeLabel);
end;

procedure TRaveDelphiCompiler.CompileExit;
begin
// Emit a jump to current Exit label
  Parser.MatchID('EXIT');
  Parser.MatchSymbol(';');
  CM.Jump(ExitLabel);
end;

function TRaveDelphiCompiler.BreakObject: TCodeLabelObject;
begin
  if BreakStack.Count = 0 then begin
    AddError(53,'');
    Result := nil;
  end else begin
    Result := TCodeLabelObject(BreakStack.Items[BreakStack.Count - 1]);
  end; { else }
end;

function TRaveDelphiCompiler.ContinueObject: TCodeLabelObject;
begin
  if ContinueStack.Count = 0 then begin
    AddError(53,'');
    Result := nil;
  end else begin
    Result := TCodeLabelObject(ContinueStack.Items[ContinueStack.Count - 1]);
  end; { else }
end;

procedure TRaveDelphiCompiler.StartLoopStatement;
begin
  ContinueStack.Add(TCodeLabelObject.Create);
  ContinueObject.CodeLabel := CM.GetLabel;
  BreakStack.Add(TCodeLabelObject.Create);
  BreakObject.CodeLabel := CM.CreateLabel;
end;

procedure TRaveDelphiCompiler.FinishLoopStatement;
begin
  CM.FixLabel(BreakObject.CodeLabel);
  ContinueObject.Free;
  ContinueStack.Delete(ContinueStack.Count - 1);
  BreakObject.Free;
  BreakStack.Delete(BreakStack.Count - 1);
end;

procedure TRaveDelphiCompiler.CompileSuspend;
begin
  Parser.MatchID('SUSPEND');
  Parser.MatchSymbol(';');
  CM.AppendCode(RAVESUSPEND);
end;

procedure TRaveDelphiCompiler.CompileWith;
var
  ClassID: TVarID;
begin
  with Expr,Parser do begin
    MatchID('WITH');
    CreateCodeList('`DO`',true);
    if CodeList.Count = 0 then begin
      AddError(40,'');
    end else begin
    //!!! Make sure code item is an object reference

      with PCodeListItem(CodeList[CodeList.Count - 1])^ do begin
        if (Kind = idkVar) and (ToType is TClassType) then begin
          ClassID := VarID as TVarID;
        end else begin
          AddError(54,'');
          ClassID := nil;
        end; { else }
      end; { with }

    // Add a layer of scoping for the refernce
      Module.Scope.PushObjectScope(ClassID);
    end; { else }
    FreeCodeList;

    MatchID('DO');
    while CompileStatement('`END`') do begin end;
    MatchID('END');
    MatchTerm('WITH',';');
  // Remove one layer of scoping
    Module.Scope.PopObjectScope;
  end; { with }
end;

initialization
  RegisterProc({Trans-}'RVCL',RaveRegister);
end.
