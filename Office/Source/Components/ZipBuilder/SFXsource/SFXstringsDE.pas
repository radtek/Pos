(******************************************************************)
(* Copyright 1997 - 1999                                          *)
(*                                                                *)
(* Translation by Markus Stephany  mirbir.st@t-online.de          *)
(*                                                                *)
(* Now maintained by Chris Vleghert                               *)
(* e-mail: cvleghrt@WorldOnline.nl                                *)
(* www:    http://www.geocities.com/SiliconValley/Orchard/8607/   *)
(* www:    http://members.tripod.lycos.nl/Vleghert/               *)
(******************************************************************)

unit SFXstrings; { deutsche Textmeldungen f�r den SFX-Creator }

interface

const
     N                 = #13#10;
     STR_CANNOTOPEN    = 'Kann Datei nicht �ffnen ..';
     STR_MAINDLG       = 'MainDialog';
     STR_CANNOTCLOSE   = 'Kann Datei nicht schlie�en.';
     STR_E             = 'Fehler..';
     STR_EXISTS        = '...existiert bereits, �berschreiben ?';
     STR_SELDLG        = 'DLGSEL';
     STR_NEWDLG        = 'DLGNEW';
     BSL               = '\';
     STR_EARCHIVE      = 'Fehler beim Lesen des Archivs.';
     STR_INVALIDNAME   = 'Ung�ltiger Dateiname.';
     STR_PDLG          = 'PASSWD';
     STR_FILEEXISTS    = 'FileExist';
     STR_EDIRECTORY    = 'Verzeichnisfehler ..';
     STR_PREXT         = 'Extrahiere: ';
     STR_ETYPE         = 'Unbekannte Komprimierung';
     STR_ALLEXT        = 'Alle Dateien wurden entpackt.';
     STR_OK            = 'Fertig.';
     STR_NOTSELEXT     = 'The markierten Dateien konnten nicht entpackt werden.';
     STR_APP           = 'DelZip Self-Extractor';
     STR_EXPT          = 'Bitte w�hlen Sie das Zielverzeichnis';
     STR_RUN_PRE       = 'Nach Entpacken, >< von : '; // belongs to the 2 lines below
     STR_RUN_RUN       = 'Ausf�hren'; 						// after extraction, run : xxx.exe
     STR_RUN_INST      = 'Installieren'; 					// after extraction, install : xxx.inf
     STR_BROWSECAP     = 'Entpacken nach : ';
     STR_BTNFOLD       = 'Neu...';
     STR_EINC_SIZE     = 'Falsche Dateigr��e, bitte versuchen Sie, diese Datei erneut hrunterzuladen.';
     STR_ABOUT_CAP     = '�ber '+STR_APP+'...';
     STR_ABOUT_MSG     = ' � 1997-1999 by C. Bunton, E. W. Engler, M. Stephany'+N+N+
                         'Die Autoren : '+N+N+'- Carl Bunton (http://www.concentric.net/~twojags)'+N+
                         N+'- Eric W. Engler (http://www.geocities.com/SiliconValley/Network/2114)'+N+
                         N+'- Markus Stephany (http://home.t-online.de/home/MirBir.St)'+N+
                         N+'Credits an : '+N+
                         N+'- Thomas Hoelzer (thoelzer@cityweb.de)'+
                         N+'- Deepu Chandy Thomas (deepuct@hotmail.com)'+
                         N+'- Todd Fast (tfast@eden.com)'+
                         N+'- Didier Havelange (Didier.Havelange@ping.be)'+
                         N+'- Frank Reichert (F.Rei@gmx.de)';

implementation

end.
