�
 TMAINFORM 0/  TPF0	TMainFormMainFormLeftTop� WidthqHeight�CaptionBCB disk span demoColor	clBtnFaceFont.CharsetDEFAULT_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameArial
Font.Style Menu	MainMenu1OldCreateOrderPositionpoScreenCenterScaled
OnActivateFormActivateOnClose	FormCloseOnResize
FormResizePixelsPerInch`
TextHeight TPanelPanel2Left Top WidthiHeightmAlignalTopTabOrder  TPanelPanel3Left�TopWidth� HeightkAlignalClient
BevelOuter	bvLoweredFont.CharsetDEFAULT_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameArial
Font.Style 
ParentFontTabOrder TLabelLabel4LeftTopWidthwHeightCaptionKeep free on first disk  TLabel
FilesLabelLeftTopUWidthHeightCaption0  TLabelLabel1Left0TopUWidthHeightCaptionfiles  TLabelLabel5LefthTop$WidthrHeightCaptionMin. free volume size  TLabelLabel6LeftTop$Width\HeightCaptionMax. volume size  TEditMaxVolSizeEditLeftTop4WidthEHeightHint)Maximum size of any archive part in bytesParentShowHintShowHint	TabOrderText0OnChangeMaxVolSizeEditChange  TEditFreeDisk1EditLeft� TopWidthUHeightHint>Bytes on the first disk which will not be used for the zipfileParentShowHintShowHint	TabOrder Text0OnChangeFreeDisk1EditChange  	TCheckBoxFormatCBLefthTopTWidthuHeightHintfFormat each disk before writing the zipfile
You will get the choice: Fast, Full format or not at allCaption&Format each diskChecked	ParentShowHintShowHint	State	cbCheckedTabOrder  TEditMinFreeVolEditLefthTop4WidthEHeightHint7Min. number of bytes available before disk will be usedParentShowHintShowHint	TabOrderText65536OnChangeMinFreeVolEditChange   TPanelPanel4LeftTopWidth�HeightkAlignalLeft
BevelOuter	bvLoweredFont.CharsetDEFAULT_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameArial
Font.Style 
ParentFontTabOrder  TLabelZipFNameLeft+TopVWidthNHeightAutoSizeCaption<none>Font.CharsetANSI_CHARSET
Font.ColorclBlackFont.Height�	Font.NameArial
Font.StylefsBold 
ParentFontWordWrap	  TLabelLabel2LeftTopVWidth HeightCaptionZipfile:Font.CharsetDEFAULT_CHARSET
Font.ColorclBlackFont.Height�	Font.NameArial
Font.Style 
ParentFont  TBevelBevel1LeftTopWidthzHeightL  TButtonAddButLeft� TopWidth� HeightHint1Open and Add to an existing zipfile and then SpanCaptionOpen, &Add and SpanParentShowHintShowHint	TabOrderOnClickAddButClick  TButton
ExtractButLeftTop0Width� HeightHint,Open a spanned zipfile and extract all filesCaption&Extract from spanned ZipParentShowHintShowHint	TabOrderOnClickExtractButClick  TButton	WriteBttnLeftTopWidthmHeightHint!Open an existing zipfile and SpanCaption&Open and SpanParentShowHintShowHint	TabOrderOnClickWriteBttnClick  TButtonReadBttnLeft� Top0Width� HeightHint,Transform a spanned archive to a default oneCaption&&Make Standard Zip from  a spanned oneParentShowHintShowHint	TabOrderOnClickReadBttnClick  TButton	NewZipButLeftTopWidthzHeightHint"Create a new zipfile, Add and SpanCaption&New, Add and SpanParentShowHintShowHint	TabOrder OnClickNewZipButClick    TStringGridStringGrid1Left TopmWidthiHeightAlignalClientColCount	FixedCols TabOrder
OnDrawCellStringGrid1DrawCellOnSelectCellStringGrid1SelectCell	ColWidths{Yde4�     TOpenDialogOpenDialog1Left@TopV  	TMainMenu	MainMenu1Left�TopV 	TMenuItemFile1Caption&File 	TMenuItemExit1CaptionE&xitOnClick
Exit1Click   	TMenuItemProject1Caption&View 	TMenuItemShowlasterror1Caption
Last errorOnClickShowlasterror1Click  	TMenuItemDLLversioninfo1CaptionDLL version infoOnClickDLLversioninfo1Click  	TMenuItem	Messages1CaptionMessagesOnClickMessages1Click    TZipBuilderZipBuilder1AddStoreSuffixesassGIFassPNGassZassZIPassZOOassARCassLZHassARJassTAZassTGZassLHAassRARassACEassCABassGZassGZIPassJAR LanguageUS: Default
SFXCaptionSelf-extracting ArchiveSFXLanguageUS
SFXOptionsSFXCheckSize OnDirUpdateZipBuilder1DirUpdate	OnMessageZipBuilder1MessageOnProgressDetailsZipBuilder1ProgressDetailsLeft�TopP   