program RaveCC;

{$APPTYPE CONSOLE}

uses
  RvLEDefine, RvLEUtil, RvLEModule, RvLDCompiler, RvProj, Classes, SysUtils;

var
  Compiler: TRaveDelphiCompiler;
  Module: TRCPUModule;
  Project: TRaveProjectManager;
  Success: boolean;

begin
  Compiler := TRaveDelphiCompiler.Create;
  if not FileExists('System.rvc') then begin
    Compiler.CompileSystemModule;
  end; { if }
  Project := TRaveProjectManager.Create(nil);
  Module := TRCPUModule.Create;
  Module.ProjectManager := Project;
  Success := true;
  IncludeErrPos := true;
  try
    try
      Compiler.CompileFile(ParamStr(1),Module);
    except
      on E: Exception do begin
        Writeln('Error: ',E.Message);
        Success := false;
      end;
    end; { tryx }
  finally
    If Success then begin
      Writeln(ParamStr(1) + '.rvs compiled successful.');
      Module.SaveToFile(ParamStr(1));
    end; { if }
    FreeAndNil(Module);
    Compiler.Free;
  end; { tryf }
end.
