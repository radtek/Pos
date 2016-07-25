# ---------------------------------------------------------------------------
!if !$d(BCB)
BCB = $(MAKEDIR)\..
!endif

# ---------------------------------------------------------------------------
# IDE SECTION
# ---------------------------------------------------------------------------
# The following section of the project makefile is managed by the BCB IDE.
# It is recommended to use the IDE to change any of the values in this
# section.
# ---------------------------------------------------------------------------

VERSION = BCB.06.00
# ---------------------------------------------------------------------------
PROJECT = ..\..\..\Execs\PalmMateIR.exe
OBJFILES = ..\..\PalmIRObj\PalmMateIR.obj ..\..\PalmIRObj\palmmain.obj \
    ..\..\PalmIRObj\PrinterConfig.obj ..\..\PalmIRObj\SelectActiveMenus.obj \
    ..\..\PalmIRObj\PalmRegistration.obj ..\..\PalmIRObj\PalmSetup.obj \
    ..\..\PalmIRObj\Palm.obj ..\..\PalmIRObj\Comms.obj \
    ..\..\PalmIRObj\DataLink.obj ..\..\PalmIRObj\PalmMenus.obj \
    ..\..\PalmIRObj\debugger.obj ..\..\PalmIRObj\Port.obj \
    ..\..\PalmIRObj\Printing.obj ..\..\PalmIRObj\CrcTable.obj \
    ..\..\PalmIRObj\Requests.obj ..\..\PalmIRObj\CommonMenus.obj \
    ..\..\PalmIRObj\Data.obj ..\..\PalmIRObj\pause.obj \
    ..\..\PalmIRObj\Processing.obj ..\..\PalmIRObj\MMMessageBox.obj \
    ..\..\PalmIRObj\TCP.obj ..\..\PalmIRObj\TouchKeyboard.obj \
    ..\..\PalmIRObj\IBLogs.obj ..\..\PalmIRObj\Login.obj \
    ..\..\PalmIRObj\Secure.obj ..\..\PalmIRObj\PrintFormat.obj \
    ..\..\PalmIRObj\DbMod.obj ..\..\PalmIRObj\Version.obj \
    ..\..\PalmIRObj\Payments.obj ..\..\PalmIRObj\Locations.obj \
    ..\..\PalmIRObj\SystemConfig.obj ..\..\PalmIRObj\SimpleTCP.obj \
    ..\..\PalmIRObj\Registration.obj ..\..\PalmIRObj\MMRegistry.obj \
    ..\..\PalmIRObj\Connect.obj ..\..\PalmIRObj\CSV.obj \
    ..\..\PalmIRObj\MD5Checksum.obj ..\..\PalmIRObj\Rooms.obj \
    ..\..\PalmIRObj\Contact.obj ..\..\PalmIRObj\Tabs.obj \
    ..\..\PalmIRObj\Invoice.obj
RESFILES = PalmMateIR.res
MAINSOURCE = PalmMateIR.cpp
RESDEPEN = $(RESFILES) palmmain.dfm PrinterConfig.dfm SelectActiveMenus.dfm \
    PalmRegistration.dfm PalmSetup.dfm debugger.dfm ..\Data.dfm ..\pause.dfm \
    ..\Processing.dfm ..\POS\MMMessageBox.dfm ..\TCP.dfm \
    ..\POS\TouchKeyboard.dfm ..\IBLogs\IBLogs.dfm ..\Security\Login.dfm \
    ..\Security\Secure.dfm ..\DbMod.dfm ..\Locations.dfm ..\SystemConfig.dfm
LIBFILES = $(BCB)\Lib\Psdk\iphlpapi.lib
IDLFILES = 
IDLGENFILES = 
LIBRARIES = Indy60.lib CportLibCB6.lib ButtonPack.lib dbexpress.lib dsnap.lib cds.lib \
    nmfast.lib ibxpress.lib vcldb.lib dbrtl.lib bdertl.lib vclx.lib bcbsmp.lib \
    vcl.lib rtl.lib
PACKAGES = rtl.bpi vcl.bpi vclx.bpi bcbsmp.bpi dbrtl.bpi vcldb.bpi ibsmp.bpi \
    bdertl.bpi ibxpress.bpi nmfast.bpi dclocx.bpi DCLUSR.bpi indy.bpi \
    visualclx.bpi visualdbclx.bpi
SPARELIBS = rtl.lib vcl.lib bcbsmp.lib vclx.lib bdertl.lib dbrtl.lib vcldb.lib \
    ibxpress.lib nmfast.lib cds.lib dsnap.lib dbexpress.lib ButtonPack.lib \
    CportLibCB6.lib Indy60.lib
DEFFILE = 
OTHERFILES = 
# ---------------------------------------------------------------------------
DEBUGLIBPATH = $(BCB)\lib\debug
RELEASELIBPATH = $(BCB)\lib\release
USERDEFINES = PalmMate;IR_Palm;_DEBUG
SYSDEFINES = NO_STRICT
INCLUDEPATH = ..\;..;..\Security;..\IBLogs;..\POS;..\..\..\SimpleTCP;..\..\..\Registration\Source;"..\..\..\Components BCB6\Touch\Checkbox";"..\..\..\Components BCB6\Touch\button";"..\..\..\Components BCB6\CPort264\Sources";$(BCB)\Projects;$(BCB)\Imports;$(BCB)\include;$(BCB)\include\vcl;$(BCB)\Bin;$(BCB)\Source\Indy\C6
LIBPATH = ..\;..;..\Security;..\IBLogs;..\POS;..\..\..\Registration\Source;..\..\..\SimpleTCP;"..\..\..\Components BCB6\Touch\checkbox";"..\..\..\Components BCB6\Touch\button";"..\..\..\Components BCB6\CPort264\Sources";$(BCB)\Projects;$(BCB)\Imports;$(BCB)\Projects\Lib;$(BCB)\lib\obj;$(BCB)\lib;$(BCB)\Bin;$(BCB)\Source\Indy\C6
WARNINGS= -w-par
PATHCPP = .;..;..;..;..;..;..;..;..\POS;..;..\POS;..\IBLogs;..\Security;..\Security;..;..;..;..;..;..;..\..\..\SimpleTCP;..\..\..\Registration\Source;..;..;..\..\..\Registration\Source;..;..;..;..
PATHASM = .;
PATHPAS = .;
PATHRC = .;
PATHOBJ = .;$(LIBPATH)
# ---------------------------------------------------------------------------
CFLAG1 = -Od -H=$(BCB)\lib\vcl60.csm -Hc -Vx -Ve -X- -r- -a8 -b- -k -y -v -vi- -c \
    -tW -tWM
IDLCFLAGS = -I..\. -I.. -I..\Security -I..\IBLogs -I..\POS -I..\..\..\SimpleTCP \
    -I..\..\..\Registration\Source -I"..\..\..\Components BCB6\Touch\Checkbox" \
    -I"..\..\..\Components BCB6\Touch\button" \
    -I"..\..\..\Components BCB6\CPort264\Sources" -I$(BCB)\Projects \
    -I$(BCB)\Imports -I$(BCB)\include -I$(BCB)\include\vcl -I$(BCB)\Bin \
    -I$(BCB)\Source\Indy\C6 -src_suffix cpp -DPalmMate -DIR_Palm -D_DEBUG -boa
PFLAGS = -N2..\..\PalmIRObj -N0..\..\PalmIRObj -$Y+ -$W -$O- -$A8 -v -JPHNE -M
RFLAGS = 
AFLAGS = /mx /w2 /zi
LFLAGS = -I..\..\PalmIRObj -D"" -aa -Tpe -x -Gn -v
# ---------------------------------------------------------------------------
ALLOBJ = c0w32.obj sysinit.obj $(OBJFILES)
ALLRES = $(RESFILES)
ALLLIB = $(LIBFILES) $(LIBRARIES) import32.lib cp32mt.lib
# ---------------------------------------------------------------------------
!ifdef IDEOPTIONS

[Version Info]
IncludeVerInfo=0
AutoIncBuild=0
MajorVer=1
MinorVer=0
Release=0
Build=0
Debug=0
PreRelease=0
Special=0
Private=0
DLL=0

[Version Info Keys]
CompanyName=
FileDescription=
FileVersion=1.0.0.0
InternalName=
LegalCopyright=
LegalTrademarks=
OriginalFilename=
ProductName=
ProductVersion=1.0.0.0
Comments=

[Debugging]
DebugSourceDirs=$(BCB)\source\vcl

!endif





# ---------------------------------------------------------------------------
# MAKE SECTION
# ---------------------------------------------------------------------------
# This section of the project file is not used by the BCB IDE.  It is for
# the benefit of building from the command-line using the MAKE utility.
# ---------------------------------------------------------------------------

.autodepend
# ---------------------------------------------------------------------------
!if "$(USERDEFINES)" != ""
AUSERDEFINES = -d$(USERDEFINES:;= -d)
!else
AUSERDEFINES =
!endif

!if !$d(BCC32)
BCC32 = bcc32
!endif

!if !$d(CPP32)
CPP32 = cpp32
!endif

!if !$d(DCC32)
DCC32 = dcc32
!endif

!if !$d(TASM32)
TASM32 = tasm32
!endif

!if !$d(LINKER)
LINKER = ilink32
!endif

!if !$d(BRCC32)
BRCC32 = brcc32
!endif


# ---------------------------------------------------------------------------
!if $d(PATHCPP)
.PATH.CPP = $(PATHCPP)
.PATH.C   = $(PATHCPP)
!endif

!if $d(PATHPAS)
.PATH.PAS = $(PATHPAS)
!endif

!if $d(PATHASM)
.PATH.ASM = $(PATHASM)
!endif

!if $d(PATHRC)
.PATH.RC  = $(PATHRC)
!endif

!if $d(PATHOBJ)
.PATH.OBJ  = $(PATHOBJ)
!endif
# ---------------------------------------------------------------------------
$(PROJECT): $(OTHERFILES) $(IDLGENFILES) $(OBJFILES) $(RESDEPEN) $(DEFFILE)
    $(BCB)\BIN\$(LINKER) @&&!
    $(LFLAGS) -L$(LIBPATH) +
    $(ALLOBJ), +
    $(PROJECT),, +
    $(ALLLIB), +
    $(DEFFILE), +
    $(ALLRES)
!
# ---------------------------------------------------------------------------
.pas.hpp:
    $(BCB)\BIN\$(DCC32) $(PFLAGS) -U$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -O$(INCLUDEPATH) --BCB {$< }

.pas.obj:
    $(BCB)\BIN\$(DCC32) $(PFLAGS) -U$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -O$(INCLUDEPATH) --BCB {$< }

.cpp.obj:
    $(BCB)\BIN\$(BCC32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n$(@D) {$< }

.c.obj:
    $(BCB)\BIN\$(BCC32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n$(@D) {$< }

.c.i:
    $(BCB)\BIN\$(CPP32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n. {$< }

.cpp.i:
    $(BCB)\BIN\$(CPP32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n. {$< }

.asm.obj:
    $(BCB)\BIN\$(TASM32) $(AFLAGS) -i$(INCLUDEPATH:;= -i) $(AUSERDEFINES) -d$(SYSDEFINES:;= -d) $<, $@

.rc.res:
    $(BCB)\BIN\$(BRCC32) $(RFLAGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -fo$@ $<



# ---------------------------------------------------------------------------




