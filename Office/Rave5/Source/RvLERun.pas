{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvLERun;

interface

uses
  Windows, Dialogs,
  Classes, SysUtils, RvClass, RvLEDefine, RvLEUtil, RvLEModule, RvUtil, RpDefine;

type
  TRCPURunner = class
  protected
    SP: longint; { Stack Pointer register }
    BP: longint; { Base Pointer register }
    MI: TRCPUModule; { Module Index register }
    CO: longint; { Code Offset register }
    SS: pointer; { Stack Segment }
    DS: pointer; { Current Data Segment }
    CS: pointer; { Current Code Segment }
    StackSize: integer; { Stack Segment size }
    CompareFlag: shortint; { -1=Less / 0=Equal / 1=Greater }

    procedure Push(Len: integer);
    procedure PushData(var Data;
                           Len: integer);
    procedure PushInteger(var Data: integer);
    procedure PushNull(Len: integer);
    procedure Pop(Len: integer);
    procedure PopData(var Data;
                          Len: integer);
    function PopInteger: integer;
    procedure ReadStack(var Data;
                            Offset: integer;
                            Len: integer);
    procedure WriteStack(var Data;
                             Offset: integer;
                             Len: integer);
    procedure ReadData(var Data;
                           Offset: integer;
                           Len: integer);
    procedure WriteData(var Data;
                            Offset: integer;
                            Len: integer);
    function GetCodeByte: byte;
    function GetCodeIndex: longint;
    function GetCodeAddress: longint;
    function PushCodeAddress: pointer;
    function GetMICOPointer: pointer;
    procedure NewModule(AModule: TRCPUModule);
  public
    constructor Create;
    destructor Destroy; override;
    procedure Execute(RCPUModule: TRCPUModule; Params: TList; var SuspendState: TObject);
  end; { TRCPURunner }

var
  RCPURunner: TRCPURunner;
  ModuleRegList: TStringList = nil;

implementation

type
  TRCPUSuspendState = class
    SP: longint; { Stack Pointer register }
    BP: longint; { Base Pointer register }
    MI: TRCPUModule; { Module Index register }
    CO: longint; { Code Offset register }
    SS: pointer; { Stack Segment }
    DS: pointer; { Current Data Segment }
    CS: pointer; { Current Code Segment }
    StackSize: integer; { Stack Segment size }
    CompareFlag: shortint; { -1=Less / 0=Equal / 1=Greater }
  end; { TRCPUSuspendState }

{ class TRCPURunner }

  constructor TRCPURunner.Create;
  begin { Create }
    inherited Create;
  end;  { Create }

  destructor TRCPURunner.Destroy;
  begin { Destroy }
    inherited Destroy;
  end;  { Destroy }

  procedure TRCPURunner.Push(Len: integer);
  begin { Push }
    Dec(SP,Len);
    if SP < 0 then begin
      RaiseError(Trans('Stack Overflow'));
    end; { if }
  end;  { Push }

  procedure TRCPURunner.PushData(var Data;
                                     Len: integer);
  begin { PushData }
    Push(Len);
    Move(Data,pointer(longint(SS) + SP)^,Len);
  end;  { PushData }

  procedure TRCPURunner.PushInteger(var Data: integer);
  begin { PushInteger }
    Push(4);
    longint(pointer(longint(SS) + SP)^) := Data;
  end;  { PushInteger }

  procedure TRCPURunner.PushNull(Len: integer);
  begin { PushNull }
    Push(Len);
    FillChar(pointer(longint(SS) + SP)^,Len,0);
  end;  { PushNull }

  procedure TRCPURunner.Pop(Len: integer);
  begin { Pop }
    Inc(SP,Len);
    if SP > StackSize then begin
      RaiseError(Trans('Stack Underflow'));
    end; { if }
  end;  { Pop }

  procedure TRCPURunner.PopData(var Data;
                                    Len: integer);
  begin { PopData }
    Move(pointer(longint(SS) + SP)^,Data,Len);
    Pop(Len);
  end;  { Pop }

  function TRCPURunner.PopInteger: integer;
  begin { PopInteger }
    Result := integer(pointer(longint(SS) + SP)^);
    Pop(4);
  end;  { PopInteger }

  procedure TRCPURunner.ReadStack(var Data;
                                      Offset: integer;
                                      Len: integer);
  begin { ReadStack }
    Move(pointer(longint(SS) + Offset)^,Data,Len);
  end;  { ReadStack }

  procedure TRCPURunner.WriteStack(var Data;
                                       Offset: integer;
                                       Len: integer);
  begin { WriteStack }
    Move(Data,pointer(longint(SS) + Offset)^,Len);
  end;  { WriteStack }

  procedure TRCPURunner.ReadData(var Data;
                                     Offset: integer;
                                     Len: integer);
  begin { ReadData }
    Move(pointer(longint(DS) + Offset)^,Data,Len);
  end;  { ReadData }

  procedure TRCPURunner.WriteData(var Data;
                                      Offset: integer;
                                      Len: integer);
  begin { WriteData }
    Move(Data,pointer(longint(DS) + Offset)^,Len);
  end;  { WriteData }

  function TRCPURunner.GetCodeByte: byte;
  begin { GetCodeByte }
  { Grab byte from current MI:CO and increment CO }
    Result := byte(pointer(longint(CS) + CO)^);
    Inc(CO);
  end;  { GetCodeByte }

  function TRCPURunner.GetCodeIndex: longint;
  {$IFNDEF NOINDEX}
  var
    B1: byte;
    B2: byte;
    B3: byte;
  {$ENDIF}
  begin { GetCodeIndex }
  { Grab index from current MI:CO }
  {$IFDEF NOINDEX}
    Result := longint(pointer(longint(CS) + CO)^);
    Inc(CO,4);
  {$ELSE}
    B1 := GetCodeByte;
    if (B1 < $80) then begin
      Result := B1;
    end else if (B1 < $C0) then begin
      Result := ((longint(B1) * $0100) + GetCodeByte) - longint($00008000);
    end else if (B1 < $E0) then begin
      B2 := GetCodeByte;
      Result := ((((longint(B1) * $0100) + B2) * $0100) + GetCodeByte) -
       longint($00C00000);
    end else begin
      B2 := GetCodeByte;
      B3 := GetCodeByte;
      Result := ((((((longint(B1) * $0100) + B2) * $0100) + B3) * $0100) +
       GetCodeByte) - longint($E0000000);
    end; { else }
  {$ENDIF}
  end;  { GetCodeIndex }

  function TRCPURunner.GetCodeAddress: longint;
  var
    AddrMode: byte;
    AddrExtra: longint;
    Offset: longint;
  begin { GetCodeAddress }
  { Figure out address mode and calculate address }
    AddrMode := GetCodeByte;
    AddrExtra := AddrMode and $3F;
    if AddrExtra = $3F then begin
      AddrExtra := GetCodeIndex;
    end; { if }

    case (AddrMode shr 6) of
      0: begin { BP + Offset }
        AddrExtra := IndexToOffset(AddrExtra);
        Result := longint(SS) + BP + AddrExtra;
      end;
      1: begin { SP + Offset }
        AddrExtra := IndexToOffset(AddrExtra);
        Result := longint(SS) + SP + AddrExtra;
      end;
      2: begin { Local DS }
        Result := longint(DS) + AddrExtra;
      end;
      3: begin { Global DS }
        Offset := GetCodeIndex;
        Result := longint(MI.IndexToModule(AddrExtra).DataSeg) + Offset;
      end;
      else begin
        RaiseError(Trans('Invalid address mode'));
        Result := 0;
      end;
    end; { case }
  end;  { GetCodeAddress }

  function TRCPURunner.PushCodeAddress: pointer;
  begin { PushCodeAddress }
    Result := pointer(GetCodeAddress);
    Push(4);
    pointer(pointer(longint(SS) + SP)^) := Result;
  end;  { PushCodeAddress }

  function TRCPURunner.GetMICOPointer: pointer;
  begin { GetMICOPointer }
  { Return pointer to current code MI:CO location }
    Result := pointer(longint(CS) + CO);
  end;  { GetMICOPointer }

  procedure TRCPURunner.NewModule(AModule: TRCPUModule);
  begin { NewModule }
    MI := AModule;
    DS := MI.DataSeg;
    CS := MI.CodeSeg;
  end;  { NewModule }

  procedure TRCPURunner.Execute(RCPUModule: TRCPUModule; Params: TList; var SuspendState: TObject);
  var
    ExtStackData: array[0..255] of longint;
    ExtStackItems: integer;
    RaveCommand: byte;
    Finished: boolean;
    Addr: longint;
    Offset: longint;
    Len: longint;
    P1: pointer;
    P2: pointer;
    P3: pointer;
    SaveCO: longint;
    SaveMI: longint;
    I1: integer;
    E1: extended;
    B1: byte;
    B2: byte;
    ExecProc: procedure;
    ModuleIndex: longint;
    TypeIndex: longint;
    S1: string;
    State: TRCPUSuspendState;
  begin { Execute }
  { Init registers and other values }
    ExecProc := FuncCaller;
    ExtStackItems := 0;
    if Assigned(SuspendState) then begin
      State := SuspendState as TRCPUSuspendState;
      SP := State.SP;
      BP := State.BP;
      MI := State.MI;
      CO := State.CO;
      SS := State.SS;
      DS := State.DS;
      CS := State.CS;
      StackSize := State.StackSize;
      CompareFlag := State.CompareFlag;
      FreeAndNil(SuspendState);
    end else begin
      CompareFlag := 0;
      MI := RCPUModule;
      CO := RCPUModule.StartCO;
      DS := RCPUModule.DataSeg;
      CS := RCPUModule.CodeSeg;
      SP := RCPUModule.StackSize;
      BP := SP;
      StackSize := SP;
      GetMem(SS,StackSize);
      if Assigned(Params) then begin
        for I1 := 0 to Params.Count - 1 do begin
          Addr := integer(Params[I1]);
          PushInteger(Addr);
        end; { for }
        Addr := -1;
        PushInteger(Addr); // CO
        PushInteger(Addr); // MI
      end; { if }
    end; { else }

    try
      Finished := false;
      repeat
        repeat
          RaveCommand := byte(pointer(longint(CS) + CO)^);
          Inc(CO);
        until RaveCommand <> RAVENOP;
        case RaveCommand of
        { Stack }
          RAVEPUSHADDR: begin { Push address }
            PushCodeAddress;
          end;
          RAVEPUSHMETHODADDR: begin { Pop class, Push address then Push class }
            I1 := PopInteger;
            PushCodeAddress;
            PushInteger(I1);
          end;
          RAVEPUSHVAR: begin { Push Variable }
            P1 := pointer(pointer(GetCodeAddress)^);
            PushInteger(longint(P1));
          end;
          RAVEPUSHDATA: begin { Push data at address }
            Len := GetCodeIndex;
            Addr := GetCodeAddress;
            PushData(pointer(Addr)^,Len);
          end;
          RAVEPUSHCONST: begin { Push address to constant }
            Len := GetCodeIndex;
            Addr := longint(CS) + CO;
            PushInteger(Addr);
            Inc(CO,Len);
          end;
          RAVEPUSHSTR: begin
            Len := GetCodeIndex;
            P1 := pointer(GetCodeAddress);
            Initialize(string(longint(P1^)));
            string(longint(P1^)) := PChar(longint(CS) + CO);
            PushInteger(longint(P1));
            Inc(CO,Len);
          end;
          RAVEPUSHMETHODSTR: begin { Pop class, Push string then Push class }
            I1 := PopInteger;
            Len := GetCodeIndex;
            P1 := pointer(GetCodeAddress);
            Initialize(string(longint(P1^)));
            string(longint(P1^)) := PChar(longint(CS) + CO);
            PushInteger(longint(P1));
            Inc(CO,Len);
            PushInteger(I1);
          end;
          RAVEPUSHDEFAULT: begin { Push a nil address }
            Addr := 0;
            PushInteger(Addr);
          end;
          RAVEPOP: begin
            Len := GetCodeIndex;
            Pop(Len);
          end;

        { Call }
          RAVEENTER: begin
            Len := GetCodeIndex; { Local data size }
            PushInteger(BP);
            BP := SP;
            PushNull(Len);
          end;
          RAVECOPYPARAMDATA: begin
            Len := GetCodeIndex;
            P1 := pointer(pointer(GetCodeAddress)^);
            P2 := pointer(GetCodeAddress);
            Move(P1^,P2^,Len);
          end;
          RAVECOPYPARAMSTR: begin
            P1 := pointer(pointer(GetCodeAddress)^);
            P2 := pointer(GetCodeAddress);
            string(longint(P2^)) := string(longint(P1^));
          end;
          RAVECOPYPARAMDATADEF: begin
            Len := GetCodeIndex;
            P1 := pointer(pointer(GetCodeAddress)^);
            P2 := pointer(GetCodeAddress);
            Offset := GetCodeIndex;
            if P1 = nil then begin { Copy default data }
              Move(pointer(longint(CS) + Offset)^,P2^,Len);
            end else begin
              Move(P1^,P2^,Len);
            end; { else }
          end;
          RAVECOPYPARAMSTRDEF: begin
            P1 := pointer(pointer(GetCodeAddress)^);
            P2 := pointer(GetCodeAddress);
            Offset := GetCodeAddress;
            Push(4);
            if pointer(Addr) = nil then begin { Copy default data }
              string(longint(P2^)) := PChar(longint(CS) + Offset);
            end else begin
              string(longint(P2^)) := string(longint(P1^));
            end; { else }
          end;
          RAVEFREESTR: begin
            P1 := pointer(GetCodeAddress);
            Finalize(string(longint(P1^)));
          end;
          RAVEFREEREC: begin
            P1 := pointer(GetCodeAddress);
            ModuleIndex := GetCodeIndex;
            TypeIndex := GetCodeIndex;
            RCPUModule.IndexToModule(ModuleIndex).IndexToType(TypeIndex).FreeData(P1);
          end;
          RAVELOADVAR: begin
            Addr := GetCodeAddress;
            ExtStackData[ExtStackItems] := longint(pointer(Addr)^);
            Inc(ExtStackItems);
          end;
          RAVELOADDATA: begin
            P1 := pointer(pointer(GetCodeAddress)^);
            Len := GetCodeIndex;
            P1 := pointer(longint(P1) + ((Len div 4) * 4));

          { Push last chunk if it is 1 2 or 3 bytes }
            case Len mod 4 of
              1: begin
                ExtStackData[ExtStackItems] := byte(P1^);
                Inc(ExtStackItems);
              end;
              2: begin
                ExtStackData[ExtStackItems] := word(P1^);
                Inc(ExtStackItems);
              end;
              3: begin
                ExtStackData[ExtStackItems] := 0;
                Move(P1^,ExtStackData[ExtStackItems],3);
                Inc(ExtStackItems);
              end;
            end; { case }

            for I1 := 1 to (Len div 4) do begin
              P1 := pointer(longint(P1) - 4);
              ExtStackData[ExtStackItems] := longint(P1^);
              Inc(ExtStackItems);
            end; { for }
          end;
          RAVERETURN: begin
            Len := GetCodeIndex; { Parameter size }
            SP := BP;
            BP := PopInteger;
            if SP >= StackSize then begin { Called from event }
              Finished := true;
            end else begin { Called from other function }
              CO := PopInteger;
              if CO < 0 then begin
                Finished := true; { Called from event }
              end else begin
                MI := TRCPUModule(PopInteger);
                Pop(Len);
                NewModule(MI);
              end; { else }
            end; { else }
          end;
          RAVERETURNRES: begin
            Len := GetCodeIndex; { Parameter size }
            SP := BP;
            BP := PopInteger;
            CO := PopInteger;
            MI := TRCPUModule(PopInteger);
            Pop(Len);
            NewModule(MI);
          end;
          RAVECALL: begin
            SaveCO := GetCodeIndex;
            PushInteger(longint(MI));
            PushInteger(CO);
            CO := SaveCO;
          end;
          RAVECALLEXTERNALFUNC: begin
          { Get result type }
            B1 := byte(pointer(longint(CS) + CO)^);
            Inc(CO);
          { Get temporary address }
            P1 := pointer(pointer(GetCodeAddress)^);
          { Get function address }
            Addr := longint(pointer(longint(CS) + CO)^);
            Inc(CO,4);

          { Call function and get result }
            case TTypeKind(B1) of
              tkBoolean: boolean(P1^) := TBolCaller(ExecProc)(Addr,ExtStackData,ExtStackItems);
              tkShortint: shortint(P1^) := TShtCaller(ExecProc)(Addr,ExtStackData,ExtStackItems);
              tkByte: byte(P1^) := TBytCaller(ExecProc)(Addr,ExtStackData,ExtStackItems);
              tkSmallint: smallint(P1^) := TSmlCaller(ExecProc)(Addr,ExtStackData,ExtStackItems);
              tkWord: word(P1^) := TWrdCaller(ExecProc)(Addr,ExtStackData,ExtStackItems);
              tkInteger: integer(P1^) := TIntCaller(ExecProc)(Addr,ExtStackData,ExtStackItems);
              tkSingle: single(P1^) := TSngCaller(ExecProc)(Addr,ExtStackData,ExtStackItems);
              tkDouble: double(P1^) := TDblCaller(ExecProc)(Addr,ExtStackData,ExtStackItems);
              tkExtended: extended(P1^) := TExtCaller(ExecProc)(Addr,ExtStackData,ExtStackItems);
              tkCurrency: currency(P1^) := TCurCaller(ExecProc)(Addr,ExtStackData,ExtStackItems);
              tkChar: char(P1^) := TChrCaller(ExecProc)(Addr,ExtStackData,ExtStackItems);
              tkString: begin
                Initialize(string(longint(P1^)));
                RecCaller(Addr,ExtStackData,ExtStackItems,P1^);
              end;
              tkPointer: pointer(P1^) := TPtrCaller(ExecProc)(Addr,ExtStackData,ExtStackItems);
              tkComplex: pointer(P1^) := TPtrCaller(ExecProc)(Addr,ExtStackData,ExtStackItems);
            end; { case }
            ExtStackItems := 0;
          end;
          RAVECALLEXTERNALPROC: begin
            Addr := longint(pointer(longint(CS) + CO)^);
            Inc(CO,4);
            TIntCaller(ExecProc)(Addr,ExtStackData,ExtStackItems);
            ExtStackItems := 0;
          end;
          RAVELONGCALL: begin
            SaveMI := GetCodeIndex;
            SaveCO := GetCodeIndex;
            PushInteger(longint(MI));
            PushInteger(CO);
            NewModule(MI.IndexToModule(SaveMI));
            CO := SaveCO;
          end;

        { Jump }
          RAVEJUMPEQ: begin
            SaveCO := CO;
            Offset := GetCodeIndex;
            if CompareFlag = 0 then begin
              CO := SaveCO + IndexToOffset(Offset);
            end; { if }
          end;
          RAVEJUMPNE: begin
            SaveCO := CO;
            Offset := GetCodeIndex;
            if CompareFlag <> 0 then begin
              CO := SaveCO + IndexToOffset(Offset);
            end; { if }
          end;
          RAVEJUMPLT: begin
            SaveCO := CO;
            Offset := GetCodeIndex;
            if CompareFlag < 0 then begin
              CO := SaveCO + IndexToOffset(Offset);
            end; { if }
          end;
          RAVEJUMPLE: begin
            SaveCO := CO;
            Offset := GetCodeIndex;
            if CompareFlag <= 0 then begin
              CO := SaveCO + IndexToOffset(Offset);
            end; { if }
          end;
          RAVEJUMPGT: begin
            SaveCO := CO;
            Offset := GetCodeIndex;
            if CompareFlag > 0 then begin
              CO := SaveCO + IndexToOffset(Offset);
            end; { if }
          end;
          RAVEJUMPGE: begin
            SaveCO := CO;
            Offset := GetCodeIndex;
            if CompareFlag >= 0 then begin
              CO := SaveCO + IndexToOffset(Offset);
            end; { if }
          end;
          RAVEJUMP: begin
            SaveCO := CO;
            Offset := GetCodeIndex;
            CO := SaveCO + IndexToOffset(Offset);
          end;
          RAVELONGJUMP: begin
            NewModule(MI.IndexToModule(GetCodeIndex));
            CO := GetCodeIndex;
          end;

        { Compare }
          RAVEEQBOL..RAVEGESTR: begin
            P2 := pointer(PopInteger);
            P1 := pointer(PopInteger);
            P3 := PushCodeAddress;
            case RaveCommand of
              RAVEEQBOL: boolean(P3^) := boolean(P1^) = boolean(P2^);
              RAVENEBOL: boolean(P3^) := boolean(P1^) <> boolean(P2^);
              RAVELTBOL: boolean(P3^) := boolean(P1^) < boolean(P2^);
              RAVEGTBOL: boolean(P3^) := boolean(P1^) > boolean(P2^);
              RAVELEBOL: boolean(P3^) := boolean(P1^) <= boolean(P2^);
              RAVEGEBOL: boolean(P3^) := boolean(P1^) >= boolean(P2^);
              RAVEEQINT: boolean(P3^) := integer(P1^) = integer(P2^);
              RAVENEINT: boolean(P3^) := integer(P1^) <> integer(P2^);
              RAVELTINT: boolean(P3^) := integer(P1^) < integer(P2^);
              RAVEGTINT: boolean(P3^) := integer(P1^) > integer(P2^);
              RAVELEINT: boolean(P3^) := integer(P1^) <= integer(P2^);
              RAVEGEINT: boolean(P3^) := integer(P1^) >= integer(P2^);
              RAVEEQEXT: boolean(P3^) := extended(P1^) = extended(P2^);
              RAVENEEXT: boolean(P3^) := extended(P1^) <> extended(P2^);
              RAVELTEXT: boolean(P3^) := extended(P1^) < extended(P2^);
              RAVEGTEXT: boolean(P3^) := extended(P1^) > extended(P2^);
              RAVELEEXT: boolean(P3^) := extended(P1^) <= extended(P2^);
              RAVEGEEXT: boolean(P3^) := extended(P1^) >= extended(P2^);
              RAVEEQCUR: boolean(P3^) := currency(P1^) = currency(P2^);
              RAVENECUR: boolean(P3^) := currency(P1^) <> currency(P2^);
              RAVELTCUR: boolean(P3^) := currency(P1^) < currency(P2^);
              RAVEGTCUR: boolean(P3^) := currency(P1^) > currency(P2^);
              RAVELECUR: boolean(P3^) := currency(P1^) <= currency(P2^);
              RAVEGECUR: boolean(P3^) := currency(P1^) >= currency(P2^);
              RAVEEQCHR: boolean(P3^) := char(P1^) = char(P2^);
              RAVENECHR: boolean(P3^) := char(P1^) <> char(P2^);
              RAVELTCHR: boolean(P3^) := char(P1^) < char(P2^);
              RAVEGTCHR: boolean(P3^) := char(P1^) > char(P2^);
              RAVELECHR: boolean(P3^) := char(P1^) <= char(P2^);
              RAVEGECHR: boolean(P3^) := char(P1^) >= char(P2^);
              RAVEEQSTR: boolean(P3^) := string(longint(P1^)) = string(longint(P2^));
              RAVENESTR: boolean(P3^) := string(longint(P1^)) <> string(longint(P2^));
              RAVELTSTR: boolean(P3^) := string(longint(P1^)) < string(longint(P2^));
              RAVEGTSTR: boolean(P3^) := string(longint(P1^)) > string(longint(P2^));
              RAVELESTR: boolean(P3^) := string(longint(P1^)) <= string(longint(P2^));
              RAVEGESTR: boolean(P3^) := string(longint(P1^)) >= string(longint(P2^));
            end; { case }
          end;

        { Binary Operators }
          RAVEORBOL..RAVEDIVCUR: begin
            P2 := pointer(PopInteger);
            P1 := pointer(PopInteger);
            P3 := PushCodeAddress;
            case RaveCommand of
              RAVEORBOL: boolean(P3^) := boolean(P1^) or boolean(P2^);
              RAVEXORBOL: boolean(P3^) := boolean(P1^) xor boolean(P2^);
              RAVEANDBOL: boolean(P3^) := boolean(P1^) and boolean(P2^);
              RAVEADDINT: longint(P3^) := longint(P1^) + longint(P2^);
              RAVESUBINT: longint(P3^) := longint(P1^) - longint(P2^);
              RAVEMULINT: longint(P3^) := longint(P1^) * longint(P2^);
              RAVEDIVINT: longint(P3^) := longint(P1^) div longint(P2^);
              RAVEMODINT: longint(P3^) := longint(P1^) mod longint(P2^);
              RAVEORINT: longint(P3^) := longint(P1^) or longint(P2^);
              RAVEXORINT: longint(P3^) := longint(P1^) xor longint(P2^);
              RAVEANDINT: longint(P3^) := longint(P1^) and longint(P2^);
              RAVESHLINT: longint(P3^) := longint(P1^) shl longint(P2^);
              RAVESHRINT: longint(P3^) := longint(P1^) shr longint(P2^);
              RAVEADDEXT: extended(P3^) := extended(P1^) + extended(P2^);
              RAVESUBEXT: extended(P3^) := extended(P1^) - extended(P2^);
              RAVEMULEXT: extended(P3^) := extended(P1^) * extended(P2^);
              RAVEDIVEXT: extended(P3^) := extended(P1^) / extended(P2^);
              RAVEADDCUR: currency(P3^) := currency(P1^) + currency(P2^);
              RAVESUBCUR: currency(P3^) := currency(P1^) - currency(P2^);
              RAVEMULCUR: currency(P3^) := currency(P1^) * currency(P2^);
              RAVEDIVCUR: currency(P3^) := currency(P1^) / currency(P2^);
            end; { case }
          end;

        { Unary operators }
          RAVENOTBOL..RAVENEGCUR: begin
            P1 := pointer(PopInteger);
            P3 := PushCodeAddress;
            case RaveCommand of
              RAVENOTBOL: boolean(P3^) := not boolean(P3^);
              RAVENOTINT: longint(P3^) := not longint(P1^);
              RAVENEGINT: longint(P3^) := -longint(P1^);
              RAVENEGEXT: extended(P3^) := -extended(P1^);
              RAVENEGCUR: currency(P3^) := -currency(P1^);
            end; { case }
          end;

        { Conv }
          RAVESHT2INT..RAVEEXT2CUR: begin
            P1 := pointer(PopInteger); { From address }
            P2 := pointer(GetCodeAddress); { To address }
            PushInteger(longint(P2));
            case RaveCommand of
              RAVESHT2INT: integer(P2^) := shortint(P1^);
              RAVEBYT2INT: integer(P2^) := byte(P1^);
              RAVESML2INT: integer(P2^) := smallint(P1^);
              RAVEWRD2INT: integer(P2^) := word(P1^);
              RAVEINT2SHT: shortint(P2^) := integer(P1^);
              RAVEINT2BYT: byte(P2^) := integer(P1^);
              RAVEINT2SML: smallint(P2^) := integer(P1^);
              RAVEINT2WRD: word(P2^) := integer(P1^);
              RAVEINT2SNG: single(P2^) := integer(P1^);
              RAVEINT2DBL: double(P2^) := integer(P1^);
              RAVEINT2EXT: extended(P2^) := integer(P1^);
              RAVEINT2CUR: currency(P2^) := integer(P1^);
              RAVESHT2EXT: extended(P2^) := shortint(P1^);
              RAVEBYT2EXT: extended(P2^) := byte(P1^);
              RAVESML2EXT: extended(P2^) := smallint(P1^);
              RAVEWRD2EXT: extended(P2^) := word(P1^);
              RAVESNG2EXT: extended(P2^) := single(P1^);
              RAVEDBL2EXT: extended(P2^) := double(P1^);
              RAVECUR2EXT: extended(P2^) := currency(P1^);
              RAVEEXT2SNG: single(P2^) := extended(P1^);
              RAVEEXT2DBL: double(P2^) := extended(P1^);
              RAVEEXT2CUR: currency(P2^) := extended(P1^);
            end; { case }
          end;
          RAVECHR2STR: begin
            P1 := pointer(GetCodeAddress);
            P2 := pointer(GetCodeAddress);
            Initialize(string(longint(P2^)));
            SetString(string(longint(P2^)),PChar(P1^),1);
          end;
          RAVEDOUBLECONV: begin { Two conversions right in a row }
            P1 := pointer(PopInteger);
            B1 := GetCodeByte;
            B2 := GetCodeByte;
            P2 := pointer(GetCodeAddress);
            PushInteger(longint(P2));
            I1 := 0;
            E1 := 0;
            case B1 of
              RAVESHT2INT: I1 := shortint(P1^);
              RAVEBYT2INT: I1 := byte(P1^);
              RAVESML2INT: I1 := smallint(P1^);
              RAVEWRD2INT: I1 := word(P1^);
              RAVESHT2EXT: E1 := shortint(P1^);
              RAVEBYT2EXT: E1 := byte(P1^);
              RAVESML2EXT: E1 := smallint(P1^);
              RAVEWRD2EXT: E1 := word(P1^);
              RAVESNG2EXT: E1 := single(P1^);
              RAVEDBL2EXT: E1 := double(P1^);
              RAVECUR2EXT: E1 := currency(P1^);
              else begin
                I1 := 0;
                E1 := 0;
              end;
            end; { case }
            case B2 of
              RAVEINT2SHT: shortint(P2^) := I1;
              RAVEINT2BYT: byte(P2^) := I1;
              RAVEINT2SML: smallint(P2^) := I1;
              RAVEINT2WRD: word(P2^) := I1;
              RAVEINT2SNG: single(P2^) := I1;
              RAVEINT2DBL: double(P2^) := I1;
              RAVEINT2CUR: currency(P2^) := I1;
              RAVEEXT2SNG: single(P2^) := E1;
              RAVEEXT2DBL: double(P2^) := E1;
              RAVEEXT2CUR: currency(P2^) := E1;
            end; { case }
          end;

        { Assign }
          RAVEASSIGN..RAVERIGHTASSIGNSTR: begin
            if RaveCommand in [RAVERIGHTASSIGN,RAVERIGHTASSIGNSTR] then begin
              P1 := pointer(PopInteger);
              P2 := pointer(PopInteger);
            end else begin
              P2 := pointer(PopInteger);
              P1 := pointer(PopInteger);
            end; { else }
            if RaveCommand <= RAVERIGHTASSIGN then begin { Data assign }
              Len := GetCodeIndex;
              Move(P2^,P1^,Len);
            end else begin { String assign }
              S1 := string(longint(P2^));
              string(longint(P1^)) := S1;
            end; { else }
            if not (RaveCommand in [RAVEASSIGN,RAVEASSIGNSTR]) then begin
              PushInteger(longint(P1));
            end; { if }
          end;
          RAVEADDINTASSIGN..RAVEDIVCURASSIGN: begin
            P2 := pointer(PopInteger);
            P1 := pointer(PopInteger);
            case RaveCommand of
              RAVEADDINTASSIGN: integer(P1^) := integer(P1^) + integer(P2^);
              RAVESUBINTASSIGN: integer(P1^) := integer(P1^) - integer(P2^);
              RAVEMULINTASSIGN: integer(P1^) := integer(P1^) * integer(P2^);
              RAVEADDEXTASSIGN: extended(P1^) := extended(P1^) + extended(P2^);
              RAVESUBEXTASSIGN: extended(P1^) := extended(P1^) - extended(P2^);
              RAVEMULEXTASSIGN: extended(P1^) := extended(P1^) * extended(P2^);
              RAVEDIVEXTASSIGN: extended(P1^) := extended(P1^) / extended(P2^);
              RAVEADDCURASSIGN: currency(P1^) := currency(P1^) + currency(P2^);
              RAVESUBCURASSIGN: currency(P1^) := currency(P1^) - currency(P2^);
              RAVEMULCURASSIGN: currency(P1^) := currency(P1^) * currency(P2^);
              RAVEDIVCURASSIGN: currency(P1^) := currency(P1^) / currency(P2^);
            end; { case }
          end;

        { Misc }
          RAVENOP: begin
          { Don't do anything }
          end;
          RAVETERMINATE: begin
            Finished := true;
          end;
          RAVEADDSTR: begin
            P2 := pointer(PopInteger);
            P1 := pointer(PopInteger);
            P3 := PushCodeAddress;
            if (P3 <> P1) and (P3 <> P2) then begin { Init string }
              Initialize(string(longint(P3^)));
            end; { if }
            string(longint(P3^)) := string(longint(P1^)) + string(longint(P2^));
          end;
          RAVETEST: begin
            P1 := pointer(PopInteger);
            if boolean(P1^) then begin
              CompareFlag := 0;
            end else begin
              CompareFlag := 1;
            end; { else }
          end;
          RAVESUSPEND: begin
            State := TRCPUSuspendState.Create;
            State.SP := SP;
            State.BP := BP;
            State.MI := MI;
            State.CO := CO;
            State.SS := SS;
            State.DS := DS;
            State.CS := CS;
            State.StackSize := StackSize;
            State.CompareFlag := CompareFlag;
            SuspendState := State;
            Finished := true;
          end;

        { Debug }
          RAVEBREAKPOINT: begin {!!!}
          end;
          else begin
            RaiseError(Trans(Format({Trans+}'Cannot execute command (%s)',
             [IntToHex(RaveCommand,2)])));
          end;
        end; { case }
      until Finished;
    finally
      if not Assigned(SuspendState) then begin
        FreeMem(SS,StackSize);
      end; { if }  
    end; { tryf }
  end;  { Execute }

  procedure RCPURegisterFunc(const Proc;
                             const FuncName: string;
                             const ModuleName: string);
  var
    FuncList: TStringList;
    Index: integer;
  begin { RCPURegisterFunc }
    if not Assigned(ModuleRegList) then Exit;

  { Add function registration to module registration list }
    if ModuleRegList.Find(UpperCase(ModuleName),Index) then begin
      FuncList := ModuleRegList.Objects[Index] as TStringList;
    end else begin
      FuncList := TStringList.Create;
      FuncList.Sorted := true;
      ModuleRegList.AddObject(UpperCase(ModuleName),FuncList);
    end; { else }
    FuncList.AddObject(UpperCase(FuncName),@Proc);
  end;  { RCPURegisterFunc }

  procedure RCPUUnregisterFunc(FuncName: string;
                               ModuleName: string);
  var
    FuncList: TStringList;
    Index: integer;
    Index2: integer;
  begin { RCPUUnregisterFunc }
    if not Assigned(ModuleRegList) then Exit;

  { Remove function registration from module registration list }
    if ModuleRegList.Find(UpperCase(ModuleName),Index) then begin
      FuncList := ModuleRegList.Objects[Index] as TStringList;
      if FuncList.Find(UpperCase(FuncName),Index2) then begin
        FuncList.Delete(Index2);
        if FuncList.Count = 0 then begin { Remove module registration }
          FreeAndNil(FuncList);
          ModuleRegList.Delete(Index);
        end; { if }
      end else begin
      {!!! Error, function registration not found }
      end; { else }
    end else begin
    {!!! Error, function registration not found }
    end; { else }
  {!!! Reload module in ModuleManager }
  end;  { RCPUUnregisterFunc }

  procedure RCPURegisterModule(Module: TRaveModule);
  var
    FuncList: TStringList;
    Index: integer;
    I1: integer;
    P1: pointer;
    ExternalFunc: PExternalFunc;
  begin { RCPURegisterModule }
    if not Assigned(ModuleRegList) then Exit;

    if ModuleRegList.Find(UpperCase(Module.ModuleName),Index) then begin
      FuncList := ModuleRegList.Objects[Index] as TStringList;
      for I1 := 0 to (FuncList.Count - 1) do begin
        ExternalFunc := TRCPUModule(Module).FindExternal(FuncList[I1]);
        if Assigned(ExternalFunc) then begin
          P1 := pointer(longint(TRCPUModule(Module).CodeSeg) + ExternalFunc.Offset);
          pointer(P1^) := pointer(FuncList.Objects[I1]);
        end; { if }
      end; { for }
    end; { if }
  end;  { RCPURegisterModule }

initialization
  ModuleRegList := TStringList.Create;
  ModuleRegList.Sorted := true;
  RCPURunner := nil;
  RegisterFuncProc := RCPURegisterFunc;
  if not Assigned(RegisterRaveFuncProc) then begin
    RegisterRaveFuncProc := RCPURegisterFunc;
  end; { if }
  UnregisterFuncProc := RCPUUnregisterFunc;
  RegisterRaveModuleProc := RCPURegisterModule;
finalization
  ClearStringList(ModuleRegList);
  FreeAndNil(ModuleRegList);
  FreeAndNil(RCPURunner);
end.