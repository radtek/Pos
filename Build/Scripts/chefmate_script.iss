;
; This script is intended to be included into InstallScript and UpdateScript.
; It sources all of the files needed for the newer Chefmate program.
;
[Files]
Source: "{#GetEnv('BASEDIR')}\ChefMate\Distribution\*.*"; \
	Excludes: "*.pdb*,*.vshost*"; \
	DestDir: "{app}\ChefmateV2"; \
	Flags: ignoreversion

[Icons]
Name: "{group}\ChefMate"; \
	Filename: "{app}\ChefmateV2\Chefmate.exe"; \
	IconIndex: 0; \
	WorkingDir: "{app}"


