{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvLEExpr;

interface

{.$DEFINE DOMAP}
{TransOff}

uses
  Classes, SysUtils, RvLEDefine, RvLEUtil, RvLEType, RvLEID, RvLEModule,
  RvLECode, RvUtil, RvClass;

type
  PCodeListItem = ^TCodeListItem;
  TCodeListItem = packed record
    FromType: TBaseType;
    ToType: TBaseType;
    TempSize: word; { Used during phase 3 processing }
    TempLoc: integer; { Used during phase 3 processing }
    Line: integer; { Line that this item is at }
    Col: word; { Column that this item is at }
    Kind: TIDKind; { idkVar, idkConst, idkType, idkFunc, idkMethod, idkOp }
  { idkVar settings }
    Address: TAddress;
    DefaultParam: boolean; { Is this a default parameter? }
    FuncResult: boolean; { Is this a function result? }
    VarID: TBaseID; { VarID if this is a variable reference }
  { idkFunc, idkMethod and idkOp settings }
    OpList: TRaveStackList; { Only used if Kind=idkOp, idkFunc or idkMethod }
  { idkFunc, idkMethod settings }
    FuncType: TFuncType;
    FuncModule: longint;
    FuncCodeLabel: PCodeLabel;
    FuncMethod: boolean; { Is this function a method call? }
    NumParams: word;
  { idkOp settings }
    OpKind: TOpKind;
  { idkConst settings }
    StringData: string; { Constant data storage for string types }
    Data: pointer; { Constant data storage for normal types }
    RefCount: byte; { Reference Counter }
  end; { TCodeListItem }

  POpStackItem = ^TOpStackItem;
  TOpStackItem = packed record
    Level: word; { What level is this operator at eg: 0(1(2(3(4))2)) }
    Order: byte;
    Kind: TOpKind;
    OSFuncType: TFuncType;
    OSFuncModule: word; { Module operator contained in }
    OSFuncCodeLabel: PCodeLabel; { Location of code }
    OSNumParams: word; { How many parameters have been processed }
    OpLine: integer; { Line that this item is at }
    OpCol: word; { Column that this item is at }
    OpToken: string; {!!! Temp for debugging }
    RefCount: byte; { Reference Counter }
  end; { TOpStackItem }

  PFreeVar = ^TFreeVar;
  TFreeVar = record
    BaseType: TBaseType;
    Address: TAddress;
  end; { TFreeVar }

  TExpression = class
  protected
    FCodeList: TRaveStackList;
    FOpStack: TRaveStackList;
    FResultItem: PCodeListItem;
    FNeedResult: boolean;
    CalcStack: TRaveStackList;
    CurrLevel: word;
    Parser: TRaveParser;
    CM: TCodeManager;
    Module: TRCPUModule;

    function CalcOpResult(var TK1,TK2,TK3,TK4: TTypeKind;
                              OpKind: TOpKind): boolean;
    function GetBaseType(TypeKind: TTypeKind): TBaseType;
    procedure AddCodeList(CodeListItem: PCodeListItem);
    procedure ProcessOpStack(NewOpKind: TOpKind);
    function FoldConstant(OpStackItem: POpStackItem;
                          OpKind: TOpKind): boolean;
    procedure FreeCodeListItem(CLI: PCodeListItem);
    procedure PushCLIStack(Stack: TRaveStackList;
                           CLI: PCodeListItem);
    procedure PopCLIStack(Stack: TRaveStackList);
    procedure PushOSIStack(Stack: TRaveStackList;
                           OSI: POpStackItem);
    procedure PopOSIStack(Stack: TRaveStackList);
  public
    constructor Create(AModule: TRCPUModule;
                       AParser: TRaveParser;
                       ACM: TCodeManager);
    destructor Destroy; override;
    procedure CreateCodeList(Terminators: string;
                             LeaveResultOnStack: boolean);
    procedure FreeCodeList;
    procedure FreeType(Addr: TAddress;
                       BaseType: TBaseType);
    procedure CreateCode(var TempMax: longint;
                             TempStart: longint);

    property OpStack: TRaveStackList read FOpStack write FOpStack;
    property CodeList: TRaveStackList read FCodeList write FCodeList;
    property ResultItem: PCodeListItem read FResultItem;
    property NeedResult: boolean read FNeedResult write FNeedResult;
  end; { TExpression }

implementation

uses
{$IFDEF DOMAP}
  RvLEDebug,
{$ENDIF}
  RpDefine;

{ class TExpression }

  constructor TExpression.Create(AModule: TRCPUModule;
                                 AParser: TRaveParser;
                                 ACM: TCodeManager);
  begin { Create }
    inherited Create;
    Module := AModule;
    Parser := AParser;
    CM := ACM;
  end;  { Create }

  destructor TExpression.Destroy;
  begin { Destroy }
    if Assigned(ResultItem) then begin
      FreeCodeListItem(ResultItem);
    end; { if }
    inherited Destroy;
  end;  { Destroy }

  function TExpression.CalcOpResult(var TK1,TK2,TK3,TK4: TTypeKind;
                                        OpKind: TOpKind): boolean;
  begin { CalcOpResult }
    TK1 := UpgradeType[TK1];
    TK2 := UpgradeType[TK2];
    if not (TK1 in OpTypes[OpKind]) or not (TK2 in OpTypes[OpKind]) then begin
      Result := false;
      Exit;
    end else begin
      Result := true;
    end; { else }

  { Find TK3 }
    case TK1 of
      tkBoolean: begin
        if TK2 = tkBoolean then begin
          TK3 := TK1;
        end else begin
          Result := false;
        end; { else }
      end;
      tkInteger: begin
        if TK2 in [tkInteger,tkExtended,tkCurrency] then begin
          TK3 := TK2;
        end else begin
          Result := false;
        end; { else }
      end;
      tkExtended: begin
        if TK2 in [tkInteger,tkExtended,tkCurrency] then begin
          TK3 := tkExtended;
        end else begin
          Result := false;
        end; { if }
      end;
      tkCurrency: begin
        case TK2 of
          tkInteger: TK3 := tkCurrency;
          tkExtended: TK3 := tkExtended;
          tkCurrency: TK3 := tkCurrency;
          else Result := false;
        end; { case }
      end;
      tkChar: begin
        if TK2 in [tkChar,tkString] then begin
          TK3 := TK2;
        end else begin
          Result := false;
        end; { else }
      end;
      tkString: begin
        if TK2 in [tkChar,tkString] then begin
          TK3 := tkString;
        end else begin
          Result := false;
        end; { else }
      end;
      tkPointer: begin
        if TK2 = tkPointer then begin
          TK3 := TK1;
        end else begin
          Result := false;
        end; { else }
      end;
      else Result := false;
    end; { case }
    TK4 := TK3;

    if Result then begin { Figure out result types of TK1, TK2 and TK3 }
      case OpKind of
        okEQ,okNE,okLT,okGT,okLE,okGE,okIS: begin
          TK1 := TK3;
          TK2 := TK3;
          TK4 := tkBoolean;
        end;
        okAdd,okSub,okOR,okXOR,okMul,okDiv,okIDiv,okMod,okAND,okSHL,okSHR: begin
          if (OpKind = okDiv) and (TK3 <> tkCurrency) then begin
            TK3 := tkExtended;
          end; { if }
          TK1 := TK3;
          TK2 := TK3;
        end;
        else Result := false;
      end; { case }
    end; { if }
  end;  { CalcOpResult }

  function TExpression.GetBaseType(TypeKind: TTypeKind): TBaseType;

  begin { TypeKind }
    case TypeKind of
      tkBoolean: Result := BooleanType;
      tkInteger: Result := IntegerType;
      tkExtended: Result := ExtendedType;
      tkCurrency: Result := CurrencyType;
      tkChar: Result := CharType;
      tkString: Result := StringType;
      else Result := nil;
    end; { case }
  end;  { TypeKind }

  procedure TExpression.AddCodeList(CodeListItem: PCodeListItem);

  var
    P1: PCodeListItem;
    P2: PCodeListItem;
    TK1: TTypeKind;
    TK2: TTypeKind;
    TK3: TTypeKind;
    TK4: TTypeKind;
    I1: integer;

  begin { AddCodeList }
    with CodeListItem^ do begin
      if Kind in [idkFunc,idkMethod] then begin
      { Build op list }
        for I1 := (NumParams - 1) downto 0 do begin
          P1 := CalcStack.Peek(I1);
          PushCLIStack(OpList,P1);
        end; { for }

        for I1 := 1 to NumParams do begin
          PopCLIStack(CalcStack);
        end; { for }
        if FuncMethod then begin { Remove self pointer }
          PopCLIStack(CalcStack);
        end; { if }
        if Assigned(FuncType.ResultType) then begin { Pop off result var }
          PopCLIStack(CalcStack);
        end; { if }
      end else if Kind = idkOp then begin
      { Init OpList and FromType and assign ToType for operand(s) }
        if OpKind in OpUnary then begin
          P1 := CalcStack.Pop;
          OpList.Push(P1); // Same reference count, pop then push
          TK1 := UpgradeType[P1.FromType.TypeKind];
          FromType := nil;
          case OpKind of
            okPos,okNeg: begin
              if TK1 in MathTypes then begin
                FromType := GetBaseType(TK1);
              end; { if }
            end;
            okNOT: begin
              if TK1 in [tkBoolean,tkInteger] then begin
                FromType := GetBaseType(TK1);
              end; { if }
            end;
          end; { case }
          if not Assigned(FromType) then begin
            ErrorLine := P1.Line;
            ErrorCol := P1.Col;
            AddError(25,'');
            FromType := IntegerType;
          end; { if }
          ToType := FromType;
          P1.ToType := FromType;
        end else begin // Binary op
          P2 := CalcStack.Pop;
          P1 := CalcStack.Pop;
          OpList.Push(P1);
          OpList.Push(P2); // Same ref counts, pop then push
          if OpKind in OpSimpleAssign then begin
            if OpKind = okRightAssign then begin
              FromType := P2.FromType;
              if P2.Kind <> idkVar then begin
                ErrorLine := P2.Line;
                ErrorCol := P2.Col;
                AddError(26,'');
              end; { if }
            end else begin
              FromType := P1.FromType;
              if P1.Kind <> idkVar then begin
                ErrorLine := P1.Line;
                ErrorCol := P1.Col;
                AddError(26,'');
              end; { if }
            end; { else }
            ToType := FromType;
            P1.ToType := FromType;
            P2.ToType := FromType;
          end else if OpKind in OpMathAssign then begin
            TK1 := UpgradeType[P1.FromType.TypeKind];
            TK2 := UpgradeType[P2.FromType.TypeKind];
            if not (TK1 in MathTypes) then begin
              ErrorLine := P1.Line;
              ErrorCol := P1.Col;
              AddError(25,'');
            end; { if }
            if not (TK2 in MathTypes) then begin
              ErrorLine := P2.Line;
              ErrorCol := P2.Col;
              AddError(25,'');
            end; { if }
            if (TK1 in IntegerTypes) and (TK2 in FloatTypes) then begin
              ErrorLine := P1.Line;
              ErrorCol := P1.Col;
              AddError(24,'');
            end; { if }
            case TK1 of
              tkInteger: begin
                FromType := GetBaseType(TK2);
              end;
              tkExtended: begin
                FromType := ExtendedType;
              end;
              tkCurrency: begin
                if TK2 = tkExtended then begin
                  FromType := ExtendedType;
                end else begin
                  FromType := CurrencyType;
                end; { else }
              end;
            end; { case }
            P1.ToType := P1.FromType;
            P2.ToType := FromType;
            ToType := P1.FromType;
          end else begin { Non-assignment operator }
          //  TK1 := P1.FromType.TypeKind;
          //  TK2 := P2.FromType.TypeKind;
            TK1 := P1.ToType.TypeKind;
            TK2 := P2.ToType.TypeKind;
            CalcOpResult(TK1,TK2,TK3,TK4,OpKind);
            P1.ToType := GetBaseType(TK1);
            P2.ToType := GetBaseType(TK2);
            FromType := GetBaseType(TK3);
            ToType := GetBaseType(TK4);
          end; { else }
        end; { else }
      end; { else }

      PushCLIStack(CodeList,CodeListItem);
      if (Kind <> idkOp) or not (OpKind in OpStatementAssign) then begin
        PushCLIStack(CalcStack,CodeListItem);
      end; { if }
    end; { with }
  end;  { AddCodeList }

  procedure TExpression.ProcessOpStack(NewOpKind: TOpKind);

  var
    CodeListItem: PCodeListItem;
    OpStackItem: POpStackItem;

  begin { ProcessOpStack }
  { Process all operators from operator stack that are at this level
    and have an order greater than or equal to the new operator }
    while true do begin
      if OpStack.Empty then Break;
      OpStackItem := POpStackItem(OpStack.Peek(0));
      with OpStackItem^ do begin
        if (Level >= CurrLevel) and (Order >= OpOrder[NewOpKind]) then begin
        { When adding an operator to the code list, check to see it if has
          constant parameters that can be evaluated at compile time }
          if not FoldConstant(OpStackItem,Kind) then begin
          { Create code list item for this operator }
            New(CodeListItem);
            with CodeListItem^ do begin
              if OpStackItem.Kind = okFunction then begin
                Kind := idkFunc;
                FuncType := OpStackItem.OSFuncType;
                FuncModule := OpStackItem.OSFuncModule;
                FuncCodeLabel := OpStackItem.OSFuncCodeLabel;
                FuncMethod := false;
                NumParams := OpStackItem.OSNumParams;
                FromType := FuncType.ResultType;
              end else begin { Operator }
                Kind := idkOp;
                FromType := nil;
              end; { else }
              TempSize := 99;
              ToType := FromType;
              OpKind := OpStackItem.Kind;
              OpList := TRaveStackList.Create;
              Line := OpLine;
              Col := OpCol;
              RefCount := 0;
            end; { with }
            AddCodeList(CodeListItem);
          end; { if }
          PopOSIStack(OpStack); { Remove operator from OpStack }
        end else begin
          Break;
        end; { else }
      end; { with }
    end; { while }
  end;  { ProcessOpStack }

  procedure TExpression.CreateCodeList(Terminators: string;
                                       LeaveResultOnStack: boolean);

  var
    Finished: boolean;
    LastToken: string;
    ThisOper: boolean; { Is this token an operator? }
    LastOper: boolean; { Was last token an operator? }
    BaseID: TBaseID;
    CodeListItem: PCodeListItem;
    OpStackItem: POpStackItem;
    Offset: longint;
    ID: TIdentifier;
    Member: TStructureMember;
    TempMember: TStructureMember;
    Index: integer;
    IgnoreAssign: boolean;
    MemberID: string;
    SimulateClassScope: boolean;
    ClassScopeID: TIdentifier;

    procedure AddFunc(FuncID: TFuncID;
                      IsMethod: boolean);

    var
      OpStackItem: POpStackItem;
      CodeListItem: PCodeListItem;

    begin { AddFunc }
    { Add function to the operator stack }
      New(OpStackItem);
      with OpStackItem^,FuncID do begin
        Level := CurrLevel;
        Kind := okFunction;
        Order := OpOrder[Kind];
        OSFuncType := BaseType as TFuncType;
        OSFuncModule := Module;
        OSFuncCodeLabel := @CodeLabel;
        OSNumParams := 0;
        OpLine := Parser.TokenLine;
        OpCol := Parser.TokenCol;
        OpToken := FuncID.ID;
        RefCount := 0;
      end; { with }
      PushOSIStack(OpStack,OpStackItem);
//      ThisOper := true; //!!!

    { Add result to code list if this function returns a value }
      with FuncID.BaseType as TFuncType do begin
        if Assigned(ResultType) then begin
          New(CodeListItem);
          with CodeListItem^ do begin
            Kind := idkVar;
            OpList := nil;
            DefaultParam := false;
            FuncResult := true;
            FuncMethod := IsMethod;
            FromType := ResultType;
            ToType := FromType;
            Line := Parser.TokenLine;
            Col := Parser.TokenCol;
            VarID := nil;
            TempSize := 99;
            RefCount := 0;
          end; { with }
          AddCodeList(CodeListItem);
        end; { if }
      end; { with }
    end;  { AddFunc }

    procedure OpenParen;
    begin { OpenParen }
      Inc(CurrLevel);
    end;  { OpenParen }

    procedure CloseParen;
    begin { CloseParen }
      if CurrLevel = 0 then begin
        AddError(2,';');
      end else begin
        ProcessOpStack(okTerm); { Process all operators inside ()s }

      { Look to see if we need to fill the stack with default parameters }
        if not OpStack.Empty then begin
          with POpStackItem(OpStack.Peek(0))^ do begin
            if (Kind = okFunction) and (Level = (CurrLevel - 1)) then begin //!!! idkMethod
            { Check for any default parameters needed }
              if LastToken <> ',' then begin
                Inc(OSNumParams);
              end; { else }
              while OSNumParams < OSFuncType.Params do begin
                New(CodeListItem);
                with CodeListItem^ do begin
                  Kind := idkVar;
                  OpList := nil;
                  DefaultParam := true;
                  FuncResult := false;
                  FromType := OSFuncType.Param[OSNumParams].BaseType;
                  ToType := FromType;
                  Line := 0;
                  Col := 0;
                  TempSize := 99;
                  RefCount := 0;
                end; { with }
                AddCodeList(CodeListItem);
                Inc(OSNumParams);
              end; { while }

// The following code gives a range error since TypeConv1 doesn't support types
// like string or boolean !!! Renable it
//
//                    If TypeConv1[PCodeListItem(CodeList.Peek(0)).ToType.TypeKind,
//                     OSFuncType.Param[OSNumParams - 1].BaseType.TypeKind] =
//                     RAVEINVALID then begin
//                      AddError(6,'');
//                    end else begin

              { Change ToType of last parameter }
                PCodeListItem(CodeList.Peek(0)).ToType :=
                 OSFuncType.Param[OSNumParams - 1].BaseType;
//                    end; { else }
            end; { if }
          end; { with }
        end; { if }

      { Decrement level }
        ThisOper := false;
        Dec(CurrLevel);

      { Process function on stack }
        if not OpStack.Empty then begin
          with POpStackItem(OpStack.Peek(0))^ do begin
            if (Kind in [okFunction,okMethod]) and (Level = CurrLevel) then begin
              ProcessOpStack(Kind);
            end; { if }
          end; { with }
        end; { if }
      end; { else }
    end;  { CloseParen }

  begin { CreateCodeList }
    // MBCS-SAFE
    Finished := false;
    CurrLevel := 0;
    LastToken := '';
    CodeList := TRaveStackList.Create;
    OpStack := TRaveStackList.Create;
    CalcStack := TRaveStackList.Create;
    LastOper := true;
    IgnoreAssign := false;
    SimulateClassScope := false;
    NeedResult := LeaveResultOnStack;
    repeat
      if SimulateClassScope then begin
        ClassScopeID := Parser.TokenName;
        Parser.TokenType := ttOperator;
        Parser.TokenName := '.';
      end else begin
        Parser.GetToken;
      end; { else }

(*
      {$IFDEF DOMAP}MapCodeList('Code List',CodeList);
      MapCodeList('Calc Stack',CalcStack);
      MapOpStack(OpStack);
      MapLine;
      Map('^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^');
      MapLine;
      Map('Processing token: ' + Parser.TokenName);{$ENDIF}
      MapFile.SaveToFile('temp.map');
*)

      ThisOper := false;
      case Parser.TokenType of
        ttID: begin
          if Pos('`' + Parser.TokenUpper + '`',Terminators) > 0 then begin
            ProcessOpStack(okTerm);
            if IgnoreAssign then begin
              Dec(CurrLevel); { If a method property was being written to }
            end; { if }
            if CurrLevel <> 0 then begin
              AddError(2,')');
            end; { if }
            Parser.KeepToken := true;
            Finished := true;
          end else begin
          { Find ID }
            BaseID := Module.Scope.FindObjectScope(Parser.TokenName);
            if Assigned(BaseID) then begin
              SimulateClassScope := true; // Simulate Class. in front of parser member
            end else begin
              BaseID := Module.FindID(Parser.TokenName);
            end; { else }

            if Assigned(BaseID) then begin
              case BaseID.IDKind of
                idkVar: begin
                { Add variable to code list }
                  New(CodeListItem);
                  with CodeListItem^ do begin
                    Kind := idkVar;
                    OpList := nil;
                    DefaultParam := false;
                    FuncResult := false;
                    FromType := BaseID.BaseType;
                    ToType := FromType;
                    Address := (BaseID as TVarID).Address;
                    Line := Parser.TokenLine;
                    Col := Parser.TokenCol;
                    VarID := BaseID;
                    FuncModule := BaseID.Module; // Module reference for classes
                    TempSize := 99;
                    RefCount := 0;
                  end; { with }
                  AddCodeList(CodeListItem);

                { Handle record/class members such as Test.Member }
                  {!!!}
                end;
                idkType: begin
                { Add type override function to operator stack }
                  {!!!}
                end;
                idkConst: begin
                { Add constant to code list }
                  New(CodeListItem);
                  with CodeListItem^ do begin
                    Kind := idkConst;
                    OpList := nil;
                    FromType := BaseID.BaseType;
                    ToType := FromType;
                    if (FromType.TypeKind = tkString) then begin
                      (BaseID as TConstID).GetData(StringData);
                    end else begin
                      GetMem(Data,FromType.Size);
                      (BaseID as TConstID).GetData(Data^);
                    end; { else }
                    Line := Parser.TokenLine;
                    Col := Parser.TokenCol;
                    TempSize := 99;
                    RefCount := 0;
                  end; { with }
                  AddCodeList(CodeListItem);
                end;
                idkFunc: begin
                  AddFunc(BaseID as TFuncID,false);
                end;
                idkMethod: begin
//!!!
                end;
                idkOp: begin
                { Add to operator stack }
                  ProcessOpStack((BaseID as TOpID).OpKind);
                  New(OpStackItem);
                  with OpStackItem^,BaseID as TOpID do begin
                    Level := CurrLevel;
                    Kind := OpKind;
                    Order := OpOrder[Kind];
                    OpLine := Parser.TokenLine;
                    OpCol := Parser.TokenCol;
                    OpToken := Parser.TokenName;
                    RefCount := 0;
                  end; { with }
                  PushOSIStack(OpStack,OpStackItem);
                  ThisOper := true;
                end;
              end; { case }
            end else begin
              AddError(22,Parser.TokenName);
            end; { else }
          end; { else }

        end;
        ttString: begin
        { Add constant string to code list }
          New(CodeListItem);
          with CodeListItem^ do begin
            Kind := idkConst;
            OpList := nil;
            if Length(Parser.TokenName) = 1 then begin { Char }
              FromType := CharType;
              GetMem(Data,FromType.Size);
              char(Data^) := Parser.TokenName[1];
            end else begin
              FromType := StringType;
              StringData := Parser.TokenName;
            end; { else }
            ToType := FromType;
            Line := Parser.TokenLine;
            Col := Parser.TokenCol;
            TempSize := 99;
            RefCount := 0;
          end; { with }
          AddCodeList(CodeListItem);
        end;
        ttInteger: begin
        { Add constant integer to code list }
          New(CodeListItem);
          with CodeListItem^ do begin
            Kind := idkConst;
            OpList := nil;
            FromType := IntegerType;
            ToType := FromType;
            GetMem(Data,FromType.Size);
            integer(Data^) := Parser.TokenInt;
            Line := Parser.TokenLine;
            Col := Parser.TokenCol;
            TempSize := 99;
            RefCount := 0;
          end; { with }
          AddCodeList(CodeListItem);
        end;
        ttFloat: begin
        { Add constant extended to code list }
          New(CodeListItem);
          with CodeListItem^ do begin
            Kind := idkConst;
            OpList := nil;
            FromType := ExtendedType;
            ToType := FromType;
            GetMem(Data,FromType.Size);
            extended(Data^) := Parser.TokenExt;
            Line := Parser.TokenLine;
            Col := Parser.TokenCol;
            TempSize := 99;
            RefCount := 0;
          end; { with }
          AddCodeList(CodeListItem);
        end;
        ttCurrency: begin
        { Add constant currency to code list }
          New(CodeListItem);
          with CodeListItem^ do begin
            Kind := idkConst;
            OpList := nil;
            FromType := CurrencyType;
            ToType := FromType;
            GetMem(Data,FromType.Size);
            currency(Data^) := Parser.TokenCurr;
            Line := Parser.TokenLine;
            Col := Parser.TokenCol;
            TempSize := 99;
            RefCount := 0;
          end; { with }
          AddCodeList(CodeListItem);
        end;
        ttOperator: begin
          ThisOper := true;
          if Pos('`' + Parser.TokenName + '`',Terminators) > 0 then begin
            if IgnoreAssign then begin
            // Simulate a ')'
              CloseParen;
//              PopCLIStack(CalcStack);
//              Dec(CurrLevel); { If a method property was being written to }
            end; { if }
            ProcessOpStack(okTerm);
            if CurrLevel <> 0 then begin
              AddError(2,')');
            end; { if }
            Parser.KeepToken := true;
            Finished := true;
          end else if Parser.TokenName = '[' then begin
          {!!! Check to see if previous item is an array type }
          {!!! Increment array level }
          end else if Parser.TokenName = ']' then begin
          {!!! Decrement array level }
          end else if Parser.TokenName = '(' then begin
            OpenParen;
          end else if Parser.TokenName = ')' then begin
            CloseParen;
          end else if Parser.TokenName = ',' then begin
            ProcessOpStack(okTerm);

          {!!! Check to see if we are on an array or function }

          { Increment parameter count for top function on previous level }
            if not OpStack.Empty then begin
              with POpStackItem(OpStack.Peek(0))^ do begin
                if (Kind in [okFunction,okMethod]) and (Level = (CurrLevel - 1)) and
                 (LastToken = ',') then begin { Default Parameter }
                  New(CodeListItem);
                  with CodeListItem^ do begin
                    Kind := idkVar;
                    OpList := nil;
                    DefaultParam := true;
                    FuncResult := false;
                    FromType := OSFuncType.Param[OSNumParams].BaseType;
                    ToType := FromType;
                    Line := Parser.TokenLine;
                    Col := Parser.TokenCol;
                    TempSize := 99;
                    RefCount := 0;
                  end; { with }
                  AddCodeList(CodeListItem);
                end; { if }

              {!!! Check for valid type - doesn't work with char to word for example }
                if TypeConv1[PCodeListItem(CodeList.Peek(0)).ToType.TypeKind,
                 OSFuncType.Param[OSNumParams].BaseType.TypeKind] = RAVEINVALID then begin
                  AddError(6,'');
                end else begin
                { Change ToType of parameter }
                  PCodeListItem(CodeList.Peek(0)).ToType :=
                   OSFuncType.Param[OSNumParams].BaseType;
                  Inc(OSNumParams);
                end; { else }
              end; { with }
            end; { if }
          end else if Parser.TokenName = '.' then begin { Member operator }
          { Replace current item on code list with its member }
          { Set scope to current record/class/module item on code list }
            ThisOper := false;
            ProcessOpStack(okFunction); { Process in case of Func1.Method1 }
            with PCodeListItem(CodeList.Peek(0))^ do begin
              if FromType is TStructureType then begin
                if SimulateClassScope then begin
                  ID := ClassScopeID;
                  SimulateClassScope := false;
                end else begin
                  ID := Parser.GetID;
                end; { else }
                if (FromType as TStructureType).FindMember(ID,[scPublic],Offset,Member,Index) then begin
                  case Member.MemberType of
                    mtElement: begin { Replace current item with element }
                      FromType := Member.BaseType;
                      ToType := FromType;
                      Inc(Address.Offset,Offset);
                    { Do the same thing on calc stack }
                      with PCodeListItem(CalcStack.Peek(0))^ do begin
                        FromType := Member.BaseType;
                        ToType := FromType;
                        Inc(Address.Offset,Offset);
                      end; { with }
                    end;
                    mtField: begin { Replace current item with element }
                    {!!! Need to actually calculate address from class storage, not class pointer }
                      FromType := Member.BaseType;
                      ToType := FromType;
                      Inc(Address.Offset,Offset);
                    { Do the same thing on calc stack }
                      with PCodeListItem(CalcStack.Peek(0))^ do begin
                        FromType := Member.BaseType;
                        ToType := FromType;
                        Inc(Address.Offset,Offset);
                      end; { with }
                    end;
                    mtMethod: begin
                      BaseID := Module.FindID(TClassType(Member.StructureType).RaveClassName + '.' + ID);
                      if Assigned(BaseID) then begin
                        if BaseID is TFuncID then begin
                          AddFunc(TFuncID(BaseID),true);
                        end else begin
                          AddError(45,'101');
                        end; { else }
                      end else begin
                        AddError(22,TClassType(FromType).RaveClassName + '.' + ID);
                      end; { else }
                      PopCLIStack(CalcStack); // Remove class reference from calc stack
                    end;
                    mtProperty: begin
                    { If accessing a field then begin
                        FromType := Member.BaseType;
                        ToType := FromType;
                        Inc(Address.Offset,Offset);
                      end else begin }
                      if NeedResult or (CurrLevel > 0) then begin { Reading from property }
                        TempMember := (Member.StructureType as TClassType).IndexToMember(
                         (Member as TPropertyMember).ReadIndex,MemberID);
                        if TempMember.MemberType = mtField then begin
                        {!!! Need to actually calculate address from class storage, not class pointer }
                          FromType := Member.BaseType;
                          ToType := FromType;
                          Inc(Address.Offset,Offset);
                        end else begin { Should be a method }
                          BaseID := Module.FindID(TClassType(Member.StructureType).RaveClassName +
                           '.' + MemberID);
                          if Assigned(BaseID) then begin
                            if BaseID is TFuncID then begin
                              AddFunc(TFuncID(BaseID),true);
                            end else begin
                              AddError(45,'101');
                            end; { else }
                          end else begin
                            AddError(22,TClassType(FromType).RaveClassName +
                             '.' + MemberID);
                          end; { else }
                          PopCLIStack(CalcStack); // Remove class reference from calc stack
                        end; { else }
                      end else begin { Writing to property }
                        TempMember := (Member.StructureType as TClassType).IndexToMember(
                         (Member as TPropertyMember).WriteIndex,MemberID);
                        if TempMember.MemberType = mtField then begin
                        {!!! Need to actually calculate address from class storage, not class pointer }
                          FromType := Member.BaseType;
                          ToType := FromType;
                          Inc(Address.Offset,Offset);
                        end else begin { Should be a method }
                          IgnoreAssign := true;
                          BaseID := Module.FindID(TClassType(Member.StructureType).RaveClassName +
                           '.' + MemberID);
                          if Assigned(BaseID) then begin
                            if BaseID is TFuncID then begin
                              AddFunc(TFuncID(BaseID),true);
                            end else begin
                              AddError(45,'101');
                            end; { else }
                          end else begin
                            AddError(22,TClassType(FromType).RaveClassName +
                             '.' + MemberID);
                          end; { else }
                          PopCLIStack(CalcStack); // Remove class reference from calc stack
                        end; { else }
                      end; { else }
                    end;
                  end; { case }
                end else begin
                  AddError(22,ID);
                end; { else }
              end else begin
                AddError(37,'');
              end; { else }
            end; { with }
          end else begin
          { Look for unary + and - }
            if LastOper then begin
              if (Parser.TokenName = '-') or (Parser.TokenName = '+') then begin
                Parser.TokenName := 'U' + Parser.TokenName;
              end; { if }
            end; { if }

            if Parser.TokenName = ':=' then begin
              NeedResult := true;
            end; { if }
            if IgnoreAssign and (Parser.TokenName = ':=') then begin
              OpenParen; // Simulate '(' for property assignment
            end else begin
            { Find current operator defintion }
              BaseID := Module.FindID(Parser.TokenName);
              if not Assigned(BaseID) then begin
                AddError(11,'');
              end else begin
                if ((BaseID as TOpID).OpKind in OpStatementAssign) and
                 (OpStack.Count > 0) then begin
                  AddError(26,'');
                end; { if }
                ProcessOpStack((BaseID as TOpID).OpKind);
              { Add operator to operator stack }
                New(OpStackItem);
                with OpStackItem^,BaseID as TOpID do begin
                  Level := CurrLevel;
                  Kind := OpKind;
                  Order := OpOrder[Kind];
                  OpLine := Parser.TokenLine;
                  OpCol := Parser.TokenCol;
                  OpToken := Parser.TokenName;
                  RefCount := 0;
                end; { with }
                PushOSIStack(OpStack,OpStackItem);
              end; { else }
            end; { if }
          end; { else }
        end;
        else begin
          AddError(1,'');
          Finished := true;
        end;
      end; { case }
      LastToken := Parser.TokenName;
      LastOper := ThisOper;
    until Finished;

//MapOpStack(OpStack);

    if not OpStack.Empty then begin
      AddError(36,'OpStack not empty.');
    end; { if }
    FreeAndNil(FOpStack);

//MapCodeList('CalcStack',CalcStack);

    if CalcStack.Count > 1 then begin
      AddError(36,'CalcStack has ' + IntToStr(CalcStack.Count) + ' items.');
    end; { if }
    FResultItem := CalcStack.Pop; // Reference count transfers to ResultItem
    FreeAndNil(CalcStack);
  end;  { CreateCodeList }

  procedure TExpression.FreeCodeListItem(CLI: PCodeListItem);

  begin { FreeCodeListItem }
    Dec(CLI.RefCount);
    if CLI.RefCount = 0 then begin
      if CLI = ResultItem then begin
        FResultItem := nil;
      end; { if }
      if (CLI.Kind = idkConst) and (CLI.FromType.TypeKind <> tkString) then begin
        FreeMem(CLI.Data,CLI.FromType.Size);
      end; { if }
      if Assigned(CLI.OpList) then begin
        while not CLI.OpList.Empty do begin
          PopCLIStack(CLI.OpList);
        end; { while }
        FreeAndNil(CLI.OpList);
      end; { if }
      Dispose(CLI);
    end; { if }
  end;  { FreeCodeListItem }

  procedure TExpression.PushCLIStack(Stack: TRaveStackList;
                                     CLI: PCodeListItem);

  begin { PushCLIStack }
    Stack.Push(CLI);
    Inc(CLI.RefCount);
  end;  { PushCLIStack }

  procedure TExpression.PopCLIStack(Stack: TRaveStackList);

  begin { PopCLIStack }
    FreeCodeListItem(PCodeListItem(Stack.Pop));
  end;  { PopCLIStack }

  procedure TExpression.PushOSIStack(Stack: TRaveStackList;
                                     OSI: POpStackItem);

  begin { PushOSIStack }
    Stack.Push(OSI);
    Inc(OSI.RefCount);
  end;  { PushOSIStack }

  procedure TExpression.PopOSIStack(Stack: TRaveStackList);

  var
    OSI: POpStackItem;

  begin { PopOSIStack }
    OSI := Stack.Pop;
    Dec(OSI.RefCount);
    if OSI.RefCount = 0 then begin
      Dispose(OSI);
    end; { if }
  end;  { PopOSIStack }

  procedure TExpression.FreeCodeList;

  begin { FreeCodeList }
    while not CodeList.Empty do begin
      PopCLIStack(CodeList);
    end; { while }
    FreeAndNil(FCodeList);
    if Assigned(ResultItem) then begin
      FreeCodeListItem(ResultItem);
      FResultItem := nil;
    end; { if }
  end;  { FreeCodeList }

{$WARNINGS OFF}{$HINTS OFF}
  function TExpression.FoldConstant(OpStackItem: POpStackItem;
                                    OpKind: TOpKind): boolean;

  var
    P1,P2,P3: PCodeListItem;
    TK1,TK2,TK3: TTypeKind;
    B1,B2,B3: boolean;
    I1,I2,I3: integer;
    E1,E2,E3: extended;
    C1,C2,C3: currency;
    H1,H2,H3: char;
    S1,S2,S3: string;
    Compare: shortint;

  begin { FoldConstant }
    Result := false;
    if OpKind in OpUnary then begin
      if CodeList.Count >= 1 then begin
        P1 := CodeList.Peek(0);
        if P1.Kind = idkConst then begin
          if not (P1.FromType is TSimpleType) then begin
            AddError(18,'');
            Exit;
          end; { if }
          TK1 := P1.FromType.TypeKind;
          Result := true;
          case OpKind of
            okPos: begin { Don't do anything for this op }
              Result := false;
            end;
            okNeg: begin
              case TK1 of
                tkInteger: I3 := -integer(P1.Data^);
                tkExtended: E3 := -extended(P1.Data^);
                tkCurrency: C3 := -currency(P1.Data^);
                else Result := false;
              end; { case }
              TK3 := TK1;
            end;
            okNOT: begin
              case TK1 of
                tkBoolean: B3 := not boolean(P1.Data^);
                tkInteger: I3 := not integer(P1.Data^);
                else Result := false;
              end; { case }
              TK3 := TK1;
            end;
            else Result := false;
          end; { case }
          if Result then begin
            PopCLIStack(CodeList);
          end; { if }
        end; { if }
      end; { if }
    end else if OpKind = okFunction then begin { Process constant functions }
      if OpStackItem.OSFuncType = OrdFunc then begin { Ord function }
        P1 := CodeList.Peek(0);
        if P1.Kind = idkConst then begin
          Result := true;
          if P1.FromType is TEnumeratedType then begin
            I3 := byte(P1.Data^);
          end else begin
            TK1 := P1.FromType.TypeKind;
            case TK1 of
              tkBoolean: I3 := Ord(boolean(P1.Data^));
              tkInteger: I3 := integer(P1.Data^);
              tkChar: I3 := Ord(char(P1.Data^));
              else Result := false;
            end; { case }
          end; { else }
          TK3 := tkInteger;
          if Result then begin
            PopCLIStack(CodeList);
            PopCLIStack(CalcStack);
          end; { if }
        end; { if }
      end; { if }
    end else begin
      if CodeList.Count >= 2 then begin
        P1 := CodeList.Peek(1);
        P2 := CodeList.Peek(0);
        if (P1.Kind = idkConst) and (P2.Kind = idkConst) then begin
          if not (P1.FromType is TSimpleType) or not
           (P2.FromType is TSimpleType) then begin
            AddError(18,'');
            Exit;
          end; { if }
          TK1 := P1.FromType.TypeKind;
          case TK1 of
            tkBoolean: B1 := boolean(P1.Data^);
            tkInteger: I1 := integer(P1.Data^);
            tkExtended: E1 := extended(P1.Data^);
            tkCurrency: C1 := currency(P1.Data^);
            tkChar: H1 := char(P1.Data^);
            tkString: S1 := P1.StringData;
          end; { case }

          TK2 := P2.FromType.TypeKind;
          case TK2 of
            tkBoolean: B2 := boolean(P2.Data^);
            tkInteger: I2 := integer(P2.Data^);
            tkExtended: E2 := extended(P2.Data^);
            tkCurrency: C2 := currency(P2.Data^);
            tkChar: H2 := char(P2.Data^);
            tkString: S2 := P2.StringData;
          end; { case }

          Result := true;
          case OpKind of
            okEQ,okNE,okLT,okGT,okLE,okGE: begin { Relational }
              Compare := 0;
              TK3 := tkBoolean;
              case TK1 of
                tkBoolean: begin
                  case TK2 of
                    tkBoolean: if B1 < B2 then Compare := -1
                          else if B1 > B2 then Compare := 1;
                    else Result := false;
                  end; { case }
                end;
                tkInteger: begin
                  case TK2 of
                    tkInteger: if I1 < I2 then Compare := -1
                          else if I1 > I2 then Compare := 1;
                    tkExtended: if I1 < E2 then Compare := -1
                           else if I1 > E2 then Compare := 1;
                    tkCurrency: if I1 < C2 then Compare := -1
                           else if I1 > C2 then Compare := 1;
                    else Result := false;
                  end; { case }
                end;
                tkExtended: begin
                  case TK2 of
                    tkInteger: if E1 < I2 then Compare := -1
                          else if E1 > I2 then Compare := 1;
                    tkExtended: if E1 < E2 then Compare := -1
                           else if E1 > E2 then Compare := 1;
                    tkCurrency: if E1 < C2 then Compare := -1
                           else if E1 > C2 then Compare := 1;
                    else Result := false;
                  end; { case }
                end;
                tkCurrency: begin
                  case TK2 of
                    tkInteger: if C1 < I2 then Compare := -1
                          else if C1 > I2 then Compare := 1;
                    tkExtended: if C1 < E2 then Compare := -1
                           else if C1 > E2 then Compare := 1;
                    tkCurrency: if C1 < C2 then Compare := -1
                           else if C1 > C2 then Compare := 1;
                    else Result := false;
                  end; { case }
                end;
                tkChar: begin
                  case TK2 of
                    tkChar: if H1 < H2 then Compare := -1
                       else if H1 > H2 then Compare := 1;
                    tkString: if H1 < S2 then Compare := -1
                         else if H1 > S2 then Compare := 1;
                    else Result := false;
                  end; { case }
                end;
                tkString: begin
                  case TK2 of
                    tkChar: if S1 < H2 then Compare := -1
                       else if S1 > H2 then Compare := 1;
                    tkString: if S1 < S2 then Compare := -1
                         else if S1 > S2 then Compare := 1;
                    else Result := false;
                  end; { case }
                end;
                else Result := false;
              end; { case }
              case OpKind of
                okEQ: B3 := (Compare = 0);
                okNE: B3 := (Compare <> 0);
                okLT: B3 := (Compare < 0);
                okGT: B3 := (Compare > 0);
                okLE: B3 := (Compare <= 0);
                okGE: B3 := (Compare >= 0);
              end; { case }
            end;
            okAdd: begin
              case TK1 of
                tkInteger: begin
                  case TK2 of
                    tkInteger: I3 := I1 + I2;
                    tkExtended: E3 := I1 + E2;
                    tkCurrency: C3 := I1 + C2;
                    else Result := false;
                  end; { case }
                  TK3 := TK2;
                end;
                tkExtended: begin
                  case TK2 of
                    tkInteger: E3 := E1 + I2;
                    tkExtended: E3 := E1 + E2;
                    tkCurrency: E3 := E1 + C2;
                    else Result := false;
                  end; { case }
                  TK3 := tkExtended;
                end;
                tkCurrency: begin
                  case TK2 of
                    tkInteger: C3 := C1 + I2;
                    tkExtended: E3 := C1 + E2;
                    tkCurrency: C3 := C1 + C2;
                    else Result := false;
                  end; { case }
                  if TK2 = tkExtended then begin
                    TK3 := tkExtended;
                  end else begin
                    TK3 := tkCurrency;
                  end; { else }
                end;
                tkChar: begin
                  case TK2 of
                    tkChar: S3 := H1 + H2;
                    tkString: S3 := H1 + S2;
                    else Result := false;
                  end; { case }
                  TK3 := tkString;
                end;
                tkString: begin
                  case TK2 of
                    tkChar: S3 := S1 + H2;
                    tkString: S3 := S1 + S2;
                    else Result := false;
                  end; { case }
                  TK3 := tkString;
                end;
                else Result := false;
              end; { case }
            end;
            okSub: begin
              case TK1 of
                tkInteger: begin
                  case TK2 of
                    tkInteger: I3 := I1 - I2;
                    tkExtended: E3 := I1 - E2;
                    tkCurrency: C3 := I1 - C2;
                    else Result := false;
                  end; { case }
                  TK3 := TK2;
                end;
                tkExtended: begin
                  case TK2 of
                    tkInteger: E3 := E1 - I2;
                    tkExtended: E3 := E1 - E2;
                    tkCurrency: E3 := E1 - C2;
                    else Result := false;
                  end; { case }
                  TK3 := tkExtended;
                end;
                tkCurrency: begin
                  case TK2 of
                    tkInteger: C3 := C1 - I2;
                    tkExtended: E3 := C1 - E2;
                    tkCurrency: C3 := C1 - C2;
                    else Result := false;
                  end; { case }
                  if TK2 = tkExtended then begin
                    TK3 := tkExtended;
                  end else begin
                    TK3 := tkCurrency;
                  end; { else }
                end;
                else Result := false;
              end; { case }
            end;
            okOR: begin
              case TK1 of
                tkBoolean: begin
                  case TK2 of
                    tkBoolean: B3 := B1 or B2;
                    else Result := false;
                  end; { case }
                  TK3 := tkBoolean;
                end;
                tkInteger: begin
                  case TK2 of
                    tkInteger: I3 := I1 or I2;
                    else Result := false;
                  end; { case }
                  TK3 := tkInteger;
                end;
                else Result := false;
              end; { case }
            end;
            okXOR: begin
              case TK1 of
                tkBoolean: begin
                  case TK2 of
                    tkBoolean: B3 := B1 xor B2;
                    else Result := false;
                  end; { case }
                  TK3 := tkBoolean;
                end;
                tkInteger: begin
                  case TK2 of
                    tkInteger: I3 := I1 xor I2;
                    else Result := false;
                  end; { case }
                  TK3 := tkInteger;
                end;
                else Result := false;
              end; { case }
            end;
            okMul: begin
              case TK1 of
                tkInteger: begin
                  case TK2 of
                    tkInteger: I3 := I1 * I2;
                    tkExtended: E3 := I1 * E2;
                    tkCurrency: C3 := I1 * C2;
                    else Result := false;
                  end; { case }
                  TK3 := TK2;
                end;
                tkExtended: begin
                  case TK2 of
                    tkInteger: E3 := E1 * I2;
                    tkExtended: E3 := E1 * E2;
                    tkCurrency: E3 := E1 * C2;
                    else Result := false;
                  end; { case }
                  TK3 := tkExtended;
                end;
                tkCurrency: begin
                  case TK2 of
                    tkInteger: C3 := C1 * I2;
                    tkExtended: E3 := C1 * E2;
                    tkCurrency: C3 := C1 * C2;
                    else Result := false;
                  end; { case }
                  if TK2 = tkExtended then begin
                    TK3 := tkExtended;
                  end else begin
                    TK3 := tkCurrency;
                  end; { else }
                end;
                else Result := false;
              end; { case }
            end;
            okDiv: begin
              case TK1 of
                tkInteger: begin
                  case TK2 of
                    tkInteger: E3 := I1 / I2;
                    tkExtended: E3 := I1 / E2;
                    tkCurrency: C3 := I1 / C2;
                    else Result := false;
                  end; { case }
                  if TK2 = tkCurrency then begin
                    TK3 := tkCurrency;
                  end else begin
                    TK3 := tkExtended;
                  end; { else }
                end;
                tkExtended: begin
                  case TK2 of
                    tkInteger: E3 := E1 / I2;
                    tkExtended: E3 := E1 / E2;
                    tkCurrency: E3 := E1 / C2;
                    else Result := false;
                  end; { case }
                  TK3 := tkExtended;
                end;
                tkCurrency: begin
                  case TK2 of
                    tkInteger: C3 := C1 / I2;
                    tkExtended: E3 := C1 / E2;
                    tkCurrency: C3 := C1 / C2;
                    else Result := false;
                  end; { case }
                  if TK2 = tkExtended then begin
                    TK3 := tkExtended;
                  end else begin
                    TK3 := tkCurrency;
                  end; { else }
                end;
                else Result := false;
              end; { case }
            end;
            okIDiv: begin
              case TK1 of
                tkInteger: begin
                  case TK2 of
                    tkInteger: I3 := I1 div I2;
                    else Result := false;
                  end; { case }
                  TK3 := tkInteger;
                end;
                else Result := false;
              end; { case }
            end;
            okMod: begin
              case TK1 of
                tkInteger: begin
                  case TK2 of
                    tkInteger: I3 := I1 mod I2;
                    else Result := false;
                  end; { case }
                  TK3 := tkInteger;
                end;
                else Result := false;
              end; { case }
            end;
            okAND: begin
              case TK1 of
                tkBoolean: begin
                  case TK2 of
                    tkBoolean: B3 := B1 and B2;
                    else Result := false;
                  end; { case }
                  TK3 := tkBoolean;
                end;
                tkInteger: begin
                  case TK2 of
                    tkInteger: I3 := I1 and I2;
                    else Result := false;
                  end; { case }
                  TK3 := tkInteger;
                end;
                else Result := false;
              end; { case }
            end;
            okSHL: begin
              case TK1 of
                tkInteger: begin
                  case TK2 of
                    tkInteger: I3 := I1 shl I2;
                    else Result := false;
                  end; { case }
                  TK3 := tkInteger;
                end;
                else Result := false;
              end; { case }
            end;
            okSHR: begin
              case TK1 of
                tkInteger: begin
                  case TK2 of
                    tkInteger: I3 := I1 shr I2;
                    else Result := false;
                  end; { case }
                  TK3 := tkInteger;
                end;
                else Result := false;
              end; { case }
            end;
          end; { case }

          if Result then begin
          { Pop two parameters off of code list }
            PopCLIStack(CodeList);
            PopCLIStack(CodeList);
            PopCLIStack(CalcStack);
            PopCLIStack(CalcStack);
          end; { if }
        end; { if }
      end; { if }
    end; { else }

    if Result then begin
    { Replace with TK3 constant }
      New(P3);
      with P3^ do begin
        Kind := idkConst;
        FromType := GetBaseType(TK3);
        ToType := FromType;
        if TK3 <> tkString then begin
          GetMem(Data,FromType.Size);
        end; { if }
        case TK3 of
          tkBoolean: boolean(Data^) := B3;
          tkInteger: integer(Data^) := I3;
          tkExtended: extended(Data^) := E3;
          tkCurrency: currency(Data^) := C3;
          tkChar: char(Data^) := H3;
          tkString: StringData := S3;
        end; { case }
        OpList := nil;
        Line := Parser.TokenLine;
        Col := Parser.TokenCol;
        TempSize := 99;
        RefCount := 0;
      end; { with }
      PushCLIStack(CodeList,P3);
      PushCLIStack(CalcStack,P3);
    end; { if }
  end;  { FoldConstant }
{$WARNINGS ON}{$HINTS ON}

  procedure TExpression.FreeType(Addr: TAddress;
                                 BaseType: TBaseType);

  begin { FreeType }
    if BaseType.TypeStruc = tsClass then Exit;
    if BaseType.TypeKind = tkString then begin
      CM.FreeStr(Addr);
    end else begin
      CM.FreeRec(Addr,Module.ModuleToIndex(BaseType.Owner as TRCPUModule),
       Module.TypeToIndex(BaseType));
    end; { else }
  end;  { FreeType }

  procedure TExpression.CreateCode(var TempMax: longint;
                                       TempStart: longint);

  var
    Item: PCodeListItem;
    I1: integer;
    I2: integer;
    TempIdx: longint;
    Addr: TAddress;
    OpFunc: byte;
    ConvBuf: pointer;
    PerformPostConvert: boolean;

    procedure AddTemp(Size: word);

    begin { AddTemp }
      Item.TempSize := Size;
      TempIdx := TempIdx + Item.TempSize;
      Item.TempLoc := -(TempStart + TempIdx);
      if TempIdx > TempMax then begin
        TempMax := TempIdx;
      end; { if }
    end;  { AddTemp }

  begin { CreateCode }
  { Create code from CodeList }
    TempIdx := 0;
    for I1 := 0 to CodeList.Count - 1 do begin
      Item := PCodeListItem(CodeList[I1]);
      if Item.Kind in [idkVar,idkConst] then begin
      { Push data }
        Item.TempSize := 0;

        if Item.Kind = idkConst then begin
          if Item.ToType.TypeKind = tkString then begin
          { Create temporary space for string variable }
            AddTemp(Item.ToType.Size);

          { Calc temp space address based on TempIdx }
            Addr.Loc := alBP;
            Addr.Module := 0;
            Addr.Offset := -(TempStart + TempIdx);
            if Item.FromType.TypeKind = tkChar then begin
              CM.PushStr(char(Item.Data^),Addr);
            end else begin
              CM.PushStr(Item.StringData,Addr);
            end; { else }
          end else begin
            if Item.FromType.TypeKind <> Item.ToType.TypeKind then begin
              GetMem(ConvBuf,Item.ToType.Size);
              case Item.FromType.TypeKind of
                tkInteger: begin
                  case Item.ToType.TypeKind of
                    tkShortint: shortint(ConvBuf^) := integer(Item.Data^);
                    tkByte: byte(ConvBuf^) := integer(Item.Data^);
                    tkSmallint: smallint(ConvBuf^) := integer(Item.Data^);
                    tkWord: word(ConvBuf^) := integer(Item.Data^);
                    tkSingle: single(ConvBuf^) := integer(Item.Data^);
                    tkDouble: double(ConvBuf^) := integer(Item.Data^);
                    tkExtended: extended(ConvBuf^) := integer(Item.Data^);
                    tkCurrency: currency(ConvBuf^) := integer(Item.Data^);
                  end; { case }
                  CM.PushConst(ConvBuf^,Item.ToType.Size);
                end;
                tkExtended: begin
                  case Item.ToType.TypeKind of
                    tkSingle: single(ConvBuf^) := extended(Item.Data^);
                    tkDouble: double(ConvBuf^) := extended(Item.Data^);
                    tkCurrency: currency(ConvBuf^) := extended(Item.Data^);
                  end; { case }
                  CM.PushConst(ConvBuf^,Item.ToType.Size);
                end;
                tkCurrency: begin
                  case Item.ToType.TypeKind of
                    tkSingle: single(ConvBuf^) := currency(Item.Data^);
                    tkDouble: double(ConvBuf^) := currency(Item.Data^);
                    tkExtended: extended(ConvBuf^) := currency(Item.Data^);
                  end; { case }
                  CM.PushConst(ConvBuf^,Item.ToType.Size);
                end;
                tkChar: begin
                  if Item.ToType.TypeKind = tkString then begin { Convert to string }
                  { Create temporary space for string variable }
                    AddTemp(Item.ToType.Size);

                  { Calc temp space address based on TempIdx }
                    Addr.Loc := alBP;
                    Addr.Module := 0;
                    Addr.Offset := -(TempStart + TempIdx);
                  end; { if }
                end;
              end; { case }
              FreeMem(ConvBuf,Item.ToType.Size);
            end else begin
              CM.PushConst(Item.Data^,Item.ToType.Size);
            end; { else }
          end; { else }
        end else begin { Variable }
          if Item.DefaultParam then begin
            CM.PushDefault;
          end else if Item.FuncResult then begin
          { Allocate temporary space for result and push address }
            AddTemp(Item.FromType.Size);
            Addr.Loc := alBP;
            Addr.Module := 0;
            Addr.Offset := -(TempStart + TempIdx);
            if Item.FuncMethod then begin { Call special Push for instance var }
              if Item.FromType.TypeKind = tkString then begin
                CM.PushMethodStr('',Addr);
              end else begin
                CM.PushMethodAddr(Addr);
              end; { else }
            end else begin
              if Item.FromType.TypeKind = tkString then begin
                CM.PushStr('',Addr);
              end else begin
                CM.PushAddr(Addr);
              end; { else }
            end; { else }
          end else begin
            if (Item.Address.Loc = alBP) and (Item.Address.Offset > 0) then begin
            { Parameter address }
              CM.PushVar(Item.Address);
            end else begin
              CM.PushAddr(Item.Address);
            end; { else }
          end; { else }

        { Figure out temporary space required and call conversion func }
          if Item.FromType.TypeKind <> Item.ToType.TypeKind then begin
            AddTemp(Item.ToType.Size);

          { Calc temp space address based on TempIdx }
            Addr.Loc := alBP;
            Addr.Module := 0;
            Addr.Offset := -(TempStart + TempIdx);
            if not CM.ConvType(Item.FromType.TypeKind,Item.ToType.TypeKind,Addr) then begin
              ErrorLine := Item.Line;
              ErrorCol := Item.Col;
              AddError(24,'');
            end; { if }
          end; { if }
        end; { else }
      end else begin { Op or Func }
        PerformPostConvert := true;

      { Free up any temp space that won't be needed }
        for I2 := 0 to Item.OpList.Count - 1 do begin
          TempIdx := TempIdx - PCodeListItem(Item.OpList[I2]).TempSize;
        end; { for }

      { Generate code for operator or function call }
        if Item.Kind = idkFunc then begin //!!! idkMethod
          if Assigned(Item.FuncType.ResultType) then begin
            Item.TempSize := Item.FuncType.ResultType.Size;
            Item.TempLoc := -(TempStart + TempIdx);
          end else begin
            Item.TempSize := 0;
            Item.TempLoc := 0;
          end; { else }

        { Call function using Module/Offset }
          if Item.FuncModule = 0 then begin
            CM.Call(Item.FuncCodeLabel^);
          end else begin
            CM.LongCall(Item.FuncModule,Item.FuncCodeLabel.Offset);
          end; { else }

        { Check for any string parameters that need to be freed }
          for I2 := 0 to Item.OpList.Count - 1 do begin
            with PCodeListItem(Item.OpList.Peek(I2))^ do begin
              if ToType.NeedFree and (TempSize > 0) then begin
                Addr.Loc := alBP;
                Addr.Module := 0;
                Addr.Offset := TempLoc;
                FreeType(Addr,ToType);
              end; { if }
            end; { with }
          end; { for }

        { See if this is a function call that ignores the result }
          if Assigned(Item.FuncType.ResultType) and
           (I1 = (CodeList.Count - 1)) and not NeedResult then begin
            if Item.FuncType.ResultType.NeedFree then begin
              Addr.Loc := alBP;
              Addr.Module := 0;
              Addr.Offset := -(TempStart + TempIdx);
              FreeType(Addr,Item.FuncType.ResultType);
              Dec(TempIdx,4);
            end; { if }
            CM.Pop(4); { Remove result from stack }
          end; { if }
        end else if Item.Kind = idkOp then begin
        { This was Item.FromType.Size, changed to ToType for relation operators }
          OpFunc := RAVEINVALID;
          if Item.OpKind in OpSimpleAssign then begin
{!!! Need to perform special processing for assigning structures with strings!!!}
            Item.TempSize := 0;
            if Item.FromType.TypeKind = tkString then begin
              case Item.OpKind of
                okAssign: OpFunc := RAVEASSIGNSTR;
                okLeftAssign: OpFunc := RAVELEFTASSIGNSTR;
                okRightAssign: OpFunc := RAVERIGHTASSIGNSTR;
              end; { case }
              CM.StrAssign(OpFunc);

            { See if we need to free a string }
              with PCodeListItem(Item.OpList.Peek(0))^ do begin
                if TempSize > 0 then begin
                  Addr.Loc := alBP;
                  Addr.Module := 0;
                  Addr.Offset := -(TempStart + TempIdx + 4);
                  CM.FreeStr(Addr);
                end; { if }
              end; { with }
            end else begin
              case Item.OpKind of
                okAssign: OpFunc := RAVEASSIGN;
                okLeftAssign: OpFunc := RAVELEFTASSIGN;
                okRightAssign: OpFunc := RAVERIGHTASSIGN;
              end; { case }
              CM.Assign(OpFunc,Item.FromType.Size);
            end; { else }
          end else if Item.OpKind in opMathAssign then begin
            case Item.FromType.TypeKind of
              tkInteger: OpFunc := OpIntCommand[Item.OpKind];
              tkExtended: OpFunc := OpExtCommand[Item.OpKind];
              tkCurrency: OpFunc := OpCurCommand[Item.OpKind];
            end; { case }
            if OpFunc <> RAVEINVALID then begin
              CM.MathAssign(OpFunc);
            end; { if }
          end else begin
          //!!! Was Item.ToType.Size
            AddTemp(Item.FromType.Size); { Re-add temp space for operator result }
            case Item.FromType.TypeKind of
              tkBoolean: OpFunc := OpBolCommand[Item.OpKind];
              tkInteger: OpFunc := OpIntCommand[Item.OpKind];
              tkExtended: OpFunc := OpExtCommand[Item.OpKind];
              tkCurrency: OpFunc := OpCurCommand[Item.OpKind];
              tkChar: OpFunc := OpChrCommand[Item.OpKind];
              tkString: OpFunc := OpStrCommand[Item.OpKind];
            end; { case }
            if OpFunc <> RAVEINVALID then begin
              Addr.Loc := alBP;
              Addr.Module := 0;
              Addr.Offset := -(TempStart + TempIdx);
              CM.RaveMath(OpFunc,Addr);
              PerformPostConvert := OpPostConv[Item.OpKind];

            { See if we need to free a string }
              if Item.FromType.TypeKind = tkString then begin
                if (PCodeListItem(Item.OpList.Peek(0)).TempSize > 0) and
                 (PCodeListItem(Item.OpList.Peek(1)).TempSize > 0) then begin
                  Addr.Loc := alBP;
                  Addr.Module := 0;
                  Addr.Offset := -(TempStart + TempIdx + 4);
                  CM.FreeStr(Addr);
                end; { if }
              end; { if }
            end; { else }
          end; { else }
        end; { else }

        if PerformPostConvert and Assigned(Item.FromType) and
         (Item.FromType <> Item.ToType) then begin { Call conversion }
          TempIdx := TempIdx - Item.TempSize;
          AddTemp(Item.ToType.Size);
          Addr.Loc := alBP;
          Addr.Module := 0;
          Addr.Offset := -(TempStart + TempIdx);
          if not CM.ConvType(Item.FromType.TypeKind,Item.ToType.TypeKind,Addr) then begin
            ErrorLine := Item.Line;
            ErrorCol := Item.Col;
            AddError(24,'');
          end; { if }
        end; { if }
      end; { else }
    end; { for }
  end;  { CreateCode }

end.
