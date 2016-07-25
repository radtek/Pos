{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RvDesignReg;

interface

  procedure RaveRegister;

implementation

uses
  RpDefine,
  RvClass,
  RvTool,
  RvCsStd,
  RvCsDraw,
  RvCsData,
  RvCsRpt,
  RvCsBars,
  RvData,
  RvProj,
  RvDatabase,
  RvDataField,
  RvSecurity,
  RvDirectDataView,
  RvDriverDataView;

procedure RegisterRvClass;
begin { RegisterRvClass }
{TransOff}
  RegisterRaveProperties(TRaveComponent,
   {Beginner}     'Name',
   {Intermediate} 'Locked;Tag',
   {Developer}    'DevLocked',
   {Hidden}       '');

  RegisterRaveProperties(TRaveControl,
   {Beginner}     'Height;Left;Top;Width',
   {Intermediate} '',
   {Developer}    '',
   {Hidden}       '');

  RegisterRaveProperties(TRaveProjectItem,
   {Beginner}     'Description;FullName',
   {Intermediate} '',
   {Developer}    '',
   {Hidden}       'CompileNeeded');

  RegisterRaveProperties(TRavePage,
   {Beginner}     'PageWidth;PageHeight',
   {Intermediate} 'Parameters;PIVars',
   {Developer}    '',
   {Hidden}       '');

  SetPropDesc(TRaveComponent,'Locked',Trans('Determines whether the properties ' +
   'and position of the component can be modified.'));
  SetPropDesc(TRaveComponent,'DevLocked',Trans('Determines whether the properties ' +
   'and position of the component can be modified.  Unlike Locked, this property ' +
   'is only visible to the developer so it can be used to protect certain areas ' +
   'or whole reports from end-user modification.'));
  SetPropDesc(TRaveComponent,'Name',Trans('The name of the component. ' +
   'The name can only contain the letters A-Z, 0-9 and the underscore character ' +
   '(_).  The name must be unique among all other components on the same page.'));
  SetPropDesc(TRaveComponent,'Tag',Trans('A holding place for numeric values. ' +
   'Typically used with scripting.'));
  SetPropDesc(TRaveComponent,'OnAfterPrint',Trans('This event is called each time ' +
   'after the component has printed.|none'));
  SetPropDesc(TRaveComponent,'OnAfterReport',Trans('This event is called after the ' +
   'entire report has printed.|none'));
  SetPropDesc(TRaveComponent,'OnBeforePrint',Trans('This event is called ' +
   'each time before the component is printed.|none'));
  SetPropDesc(TRaveComponent,'OnBeforeReport',Trans('This event is called ' +
   'before anything in the report is printed.|none'));
  SetPropDesc(TRaveComponent,'Visible',Trans('Determines whether the component will ' +
   'be printed or not.  Typically set through scripting in the OnBeforePrint or ' +
   'OnBeforeReport events.'));

  SetPropDesc(TRaveControl,'Cursor',Trans('The cursor shape that will ' +
   'be displayed when the mouse is over the component.'));
  SetPropDesc(TRaveControl,'FontJustify',Trans('The justification of the component''s ' +
   ' contents (Left, Right or Center) in its printed area.'));
  SetPropDesc(TRaveControl,'BackColor',Trans('The secondary color of the component.'));
  SetPropDesc(TRaveControl,'ForeColor',Trans('The primary color of the component.'));
  SetPropDesc(TRaveControl,'FillStyle',Trans('The style of fill that will be ' +
   'used to fill the interior of the component.'));
  SetPropDesc(TRaveControl,'Text',Trans('The text that the component will print.'));
  SetPropDesc(TRaveControl,'LineWidth',Trans('The width of the line or border of ' +
   'the component.  See also LineWidthType.'));
  SetPropDesc(TRaveControl,'LineWidthType',Trans('The units (Pixels or Points) that ' +
   'the line or border width is drawn in.  For line widths greater than one you ' +
   'will usually want to use Points for consistent printing results.'));
  SetPropDesc(TRaveControl,'LineStyle',Trans('The style (pattern) of the line ' +
   ' or border of the component.'));
  SetPropDesc(TRaveControl,'Left',Trans('The leftmost side of the component in units.'));
  SetPropDesc(TRaveControl,'Top',Trans('The topmost side of the component in units.'));
  SetPropDesc(TRaveControl,'Width',Trans('The width of the component in units.'));
  SetPropDesc(TRaveControl,'Height',Trans('The height of the component in units.'));
  SetPropDesc(TRaveControl,'Anchor',Trans('Defines how the component will move in ' +
   'relation to changes in the size of its parent component.'));
  SetPropDesc(TRaveControl,'Mirror',Trans('Defines another component, of the same ' +
   'type, that this component will use to duplicate property values excluding position ' +
   'and locked status. Normally used with Section components for the most flexibility.'));
  SetPropDesc(TRaveControl,'DisplayOn',Trans('Determines whether the component will ' +
   'show up on the printer only, the preview only, or both the printer and the preview.'));

  SetPropDesc(TRaveProjectItem,'Description',Trans('A description of the component, ' +
   'usually paragraph-sized, that can be used to tell the purpose and usage of the ' +
   'component.'));
  SetPropDesc(TRaveProjectItem,'FullName',Trans('A single line descriptive name of ' +
   'the component. Unlike the Name property, any letters including spaces can be used.'));

  SetPropDesc(TRavePage,'Bin',Trans('Sets the paper tray that you want used for ' +
   'this page. If necessary, pages can have different bins within the same print job.'));
  SetPropDesc(TRavePage,'BinCustom',Trans('If not blank, the page will ' +
   'attempt to print on the first bin whose name contains the text defined by ' +
   'BinCustom. If BinCustom is blank, the bin selection is determined by the Bin ' +
   'property.'));
  SetPropDesc(TRavePage,'GotoMode',Trans('If gmGotoDone, control is passed to GotoPage ' +
   'after this page is fully completed. If gmGotoNotDone, control is passed only if it ' +
   'is not fully completed.  If gmCallEach, control is passed after each page and is ' +
   'returned when the called page is complete.'));
  SetPropDesc(TRavePage,'GotoPage',Trans('Defines the page that will be ' +
   'printed after the current page according to the GotoMode property rule.'));
  SetPropDesc(TRavePage,'GridLines',Trans('Defines the grid lines that are ' +
   'visible. A setting of 5 means that every fifth grid line will be visible.'));
  SetPropDesc(TRavePage,'GridSpacing',Trans('Sets the spacing between grid lines.'));
  SetPropDesc(TRavePage,'Module',{Trans-}'Reserved for future use (scripting).');
  SetPropDesc(TRavePage,'Orientation',Trans('Sets page orientation to landscape ' +
   'or portrait. The poDefault value will use the default setting of the printer. ' +
   'If necessary, pages can have different orientations within the same print job.'));
  SetPropDesc(TRavePage,'PageHeight',Trans('Defines the height of the page. ' +
   'Normally this is set with PaperSize but can be used for custom paper sizes ' +
   'if they are supported by the printer driver.'));
  SetPropDesc(TRavePage,'PageWidth',Trans('Defines the width of the page. ' +
   'Normally this is set with PaperSize but can be used for custom paper sizes ' +
   'if they are supported by the printer driver.'));
  SetPropDesc(TRavePage,'PaperSize',Trans('Defines the size of paper being ' +
   'used for this page.  Unlike the Bin and Orientation properties, paper sizes ' +
   'cannot change within the same print job.'));
  SetPropDesc(TRavePage,'Parameters',Trans('Defines the project parameters for the page.  ' +
   'Values can be assigned to project parameters through calculation components and can be ' +
   'printed using DataText components.  Each parameter should be placed on a ' +
   'separate line in the editor.'));
  SetPropDesc(TRavePage,'PIVars',Trans('Defines the PIVars (Post Initialize Variables) for ' +
   'the page.  Values can be assigned to PIVars through calculation components and can be ' +
   'printed using DataText components.  Each PIVar should be placed on a separate line in the editor.'));
  SetPropDesc(TRavePage,'WasteFit',Trans('Determines whether the page will adjust ' +
   'the sides of any components so that they fit within the waste area of the ' +
   'destination printer.  A value of true will resize all components so that they ' +
   'fit on the current page.  Set the Anchor property for child controls so that ' +
   'they will adjust to changes in their parent.'));
{TransOn}
end;  { RegisterRvClass }

procedure RegisterRvCsStd;
begin { RegisterRvCsStd }
{TransOff}
  CreateTextControlProc := CreateTextControl;

  RegisterRaveProperties(TRaveFontMaster,
   {Beginner}     'Font',
   {Intermediate} '',
   {Developer}    '',
   {Hidden}       '');

  RegisterRaveProperties(TRavePageNumInit,
   {Beginner}     'InitValue',
   {Intermediate} 'InitDataField;InitDataView',
   {Developer}    '',
   {Hidden}       '');

  RegisterRaveProperties(TRaveCustomText,
   {Beginner}     'Color;Font;FontJustify;Text',
   {Intermediate} 'FontMirror',
   {Developer}    '',
   {Hidden}       '');

  RegisterRaveProperties(TRaveText,
   {Beginner}     '',
   {Intermediate} 'Truncate',
   {Developer}    '',
   {Hidden}       '');

  RegisterRaveProperties(TRaveSection,
   {Beginner}     '',
   {Intermediate} '',
   {Developer}    '',
   {Hidden}       '');

  RegisterRaveProperties(TRaveBitmap,
   {Beginner}     'Image',
   {Intermediate} 'DataField;DataView;MatchSide',
   {Developer}    '',
   {Hidden}       '');

  RegisterRaveProperties(TRaveMetaFile,
   {Beginner}     'Image',
   {Intermediate} 'DataField;DataView;MatchSide',
   {Developer}    '',
   {Hidden}       '');

  RegisterRaveProperties(TRaveMemo,
   {Beginner}     '',
   {Intermediate} 'ExpandParent;MailMergeItems',
   {Developer}    '',
   {Hidden}       '');

  SetPropDesc(TRaveFontMaster,'Font',Trans('Defines the font name, style and ' +
   'size of any text components that have this component defined in their ' +
   'FontMirror property.'));

  SetPropDesc(TRavePageNumInit,'InitValue',Trans('Defines the value that the ' +
   'relative page number will start from when this component is printed. Only ' +
   'the RelativePage report variable (not CurrentPage) will reflect the new ' +
   'page number.'));
  SetPropDesc(TRavePageNumInit,'InitDataView',Trans('Defines the default dataview ' +
   'for the InitDataField property.'));
  SetPropDesc(TRavePageNumInit,'InitDataField',Trans('Defines the value that the ' +
   'relative page number will start from when this component is printed. If the ' +
   'text cannot be converted to a valid integer, a default value of 1 will be used.'));

  SetPropDesc(TRaveCustomText,'FontJustify',Trans('Defines the justification ' +
   'of the text within the designed area. This property can also be modified with ' +
   'the Font Editor toolbar.'));
  SetPropDesc(TRaveCustomText,'FontMirror',Trans('Defines the FontMaster component ' +
   'that will control the font attributes of this component. When a FontMirror is ' +
   'assigned, the Font Editor toolbar will not be active.'));
  SetPropDesc(TRaveCustomText,'Color',Trans('Defines the color of the text to be ' +
   'printed. This property can also be modified with the Color Palette toolbar.'));
  SetPropDesc(TRaveCustomText,'Font',Trans('Defines the font name, style and size ' +
   'of the text to be printed. This property can also be modified with the Font ' +
   'Editor toolbar.'));
  SetPropDesc(TRaveCustomText,'OnGetText',Trans('The OnGetText event allows you to ' +
   'change the text that is going to be printed.  Initially, the Value parameter will ' +
   'contain the default text that will be printed.  You can assign a new value if ' +
   'necessary (note: do not modify Self.Text).|Value: string - The text value that ' +
   'will be printed.'));

  SetPropDesc(TRaveText,'Rotation',Trans('Defines the rotation, in degrees, of ' +
   'the text to be printed.  Valid valid are whole numbers between 0 and 359.'));
  SetPropDesc(TRaveText,'Text',Trans('The text that will be printed.'));
  SetPropDesc(TRaveText,'Truncate',Trans('Determines whether the text is ' +
   'truncated or printed completely, regardless of the designed width.'));

  SetPropDesc(TRaveSection,'WasteFit',Trans('Determines whether the section will ' +
   'adjust it''s size so that it fits within the waste area of the destination ' +
   'printer.  A value of true will resize the section so that it completely fits ' +
   'on the current page.  Set the Anchor property for child controls so that ' +
   'they will adjust to changes in the size of the section.'));
  SetPropDesc(TRaveSection,'OnPrint',Trans('The OnPrint event allows you to ' +
   'call and access the BaseReport object for the currently executing report.  ' +
   'This can be used to call printing API functionality within a visually designed report.' +
   '|Report: TBaseReport - The base report object for the currently executing report.'));

  SetPropDesc(TRaveBaseImage,'MatchSide',Trans('Defines how the image is sized. ' +
   'msWidth or msHeight will match the designed width or height and adjust the ' +
   'opposite side to keep the image proportional.  msBoth will match the designed ' +
   'width and height and is normally non-proportional.  msInside will size the ' +
   'image proportionally within the designed width and height.'));
  SetPropDesc(TRaveBaseImage,'FileLink',Trans('Defines a filename that will be ' +
   'used to load the image from. This can be useful for keeping large images ' +
   'outside the report project or allowing easy replacement of images such as ' +
   'company logos.'));
  SetPropDesc(TRaveBaseImage,'DataField',Trans('Defines the field where the ' +
   'image data is stored. If the field contains string information, it will be ' +
   'assumed to be a filename reference similar to the FileLink property.'));
  SetPropDesc(TRaveBaseImage,'DataView',Trans('Defines the default dataview for ' +
   'the DataField property.'));

  SetPropDesc(TRaveBitmap,'Image',Trans('Stores a bitmap to print. Other options for ' +
   'printing bitmaps are available in the FileLink and DataField properties.'));

  SetPropDesc(TRaveMetaFile,'Image',Trans('Stores a metafile to print. Other options for ' +
   'printing bitmaps are available in the FileLink and DataField properties.'));

  SetPropDesc(TRaveMemo,'ExpandParent',Trans('Determines whether the memo will ' +
   'expand the parent height to allow enough room to print the entire memo contents.'));
  SetPropDesc(TRaveMemo,'MailMergeItems',Trans('Through the Mail Merge Editor, defines ' +
   'search tokens and replacement text that will be used to modify the memo each time ' +
   'it is printed.  Useful for inserting data fields or project parameters inside memos ' +
   'at user defined positions.'));
  SetPropDesc(TRaveMemo,'Text',Trans('Defines the memo text that will be printed.'));
  SetPropDesc(TRaveMemo,'Truncate',Trans('Determines whether the memo will be ' +
   'truncated or continue on the next page if it does not completly print the ' +
   'first time.'));
{TransOn}
end;  { RegisterRvCsStd }

procedure RegisterRvCsBars;
begin { RegisterRvCsBars }
{TransOff}
  RegisterRaveProperties(TRaveBaseBarCode,
    {Beginner}     'Left;Right;Top;Bottom;Height;Width;Font;Center;Text;TextJustify;' +
                  'PrintReadable',
    {Intermediate} 'DataField;DataView;AutoSize;BarCodeJustify;BarWidth;BarHeight;' +
                  'BarTop;PrintTop;BarCodeRotation;PrintTop',
    {Developer}    '',
    {Hidden}       '');

  RegisterRaveProperties(TRavePostNetBarCode,
    {Beginner}     '',
    {Intermediate} '',
    {Developer}    '',
    {Hidden}       'BarHeight;BarWidth;Font;PrintChecksum;PrintReadable;' +
                  'PrintTop;TextJustify;UseChecksum;WideFactor');
(*
  RegisterRaveProperties(TRaveI2of5BarCode,
    {Beginner}     '',
    {Intermediate} '',
    {Developer}    '',
    {Hidden}       '');
*)

  RegisterRaveProperties(TRaveCode39BarCode,
    {Beginner}     '',
    {Intermediate} 'Extended',
    {Developer}    '',
    {Hidden}       '');

  RegisterRaveProperties(TRaveCode128BarCode,
    {Beginner}     '',
    {Intermediate} 'CodePage',
    {Developer}    '',
    {Hidden}       'UseChecksum');
(*
  RegisterRaveProperties(TRaveUPCBarCode,
    {Beginner}     '',
    {Intermediate} '',
    {Developer}    '',
    {Hidden}       '');
*)
(*
  RegisterRaveProperties(TRaveEANBarCode,
    {Beginner}     '',
    {Intermediate} '',
    {Developer}    '',
    {Hidden}       '');
*)

  SetPropDesc(TRaveBaseBarCode,'DataField',Trans('The name of the field you want to use.  The ' +
                            'field must be a valid field of the selected dataview, be defined ' +
                            'as a report parameter or be a valid report variable.'));
  SetPropDesc(TRaveBaseBarCode,'DataView',Trans('This defines the dataview that will ' +
                            'be used to provide the connection to your table data.'));
  SetPropDesc(TRaveBaseBarCode,'AutoSize',Trans('This determines whether the box surrounding ' +
                                'the barcode is automatically sized to that of the barcode.  ' +
                                'You can set this to false, adjust the design-time rectangle around ' +
                                'the barcode to a desired value and then use the BarCodeJustify ' +
                                'property to center the barcode or right or left justfy it within ' +
                                'the rectangle.'));
  SetPropDesc(TRaveBaseBarCode,'BarCodeJustify',Trans('BarCodeJustify is used to right, left or center ' +
                                'justify the barcode within the designtime rectangle that surrounds the ' +
                                'barcode. This property is only meaningful if AutoSize is set to false.  '));
  SetPropDesc(TRaveBaseBarCode,'Top',Trans('Sets the top of the barcode to the defined value.  If human ' +
                                'readable text is being printed then the top of the text will be printed ' +
                                'at this position.'));
  SetPropDesc(TRaveBaseBarCode,'Bottom',Trans('Sets the bottom of the barcode to the defined value.  If human ' +
                                'readable text is being printed then the bottom of the text will be printed ' +
                                'at this position.'));
  SetPropDesc(TRaveBaseBarCode,'Height',Trans('Specifies how high the barcode will be, including the human readable text.'));
  SetPropDesc(TRaveBaseBarCode,'Width',Trans('Specifies the width of the rectangle that surrounds the barcode.  ' +
                                'This only works if AutoSize is set to false.'));
  SetPropDesc(TRaveBaseBarCode,'Font',Trans('Specifies the font that will be used for the human readable text.'));
  SetPropDesc(TRaveBaseBarCode,'BarWidth',Trans('Specifies the width of the bars used in the barcode.'));
  SetPropDesc(TRaveBaseBarCode,'BarHeight',Trans('Specifies the height of the tallest bar.'));
  SetPropDesc(TRaveBaseBarCode,'BarTop',Trans('Sets the top of the barcode to the defined value.'));
  SetPropDesc(TRaveBaseBarCode,'Left',Trans('Sets the left of the barcode to the defined value.  ' +
                                'Setting this value will change the BarCodeJustification to be left justified.'));
  SetPropDesc(TRaveBaseBarCode,'Right',Trans('Sets the right of the barcode to the defined value.  ' +
                                'Setting this value will change the BarCodeJustification to be right justified.'));
  SetPropDesc(TRaveBaseBarCode,'Center',Trans('Centers the barcode around the defined value.  ' +
                                'Setting this value will change the BarCodeJustification to be center justified.'));
  SetPropDesc(TRaveBaseBarCode,'Text',Trans('Defines the value that you want printed as a barcode.'));
  SetPropDesc(TRaveBaseBarCode,'TextJustify',Trans('When printing human readable text, it will be printed left, ' +
                                'center or right justified against the barcode as defined by this value.'));
  SetPropDesc(TRaveBaseBarCode,'UseChecksum',Trans('Determines whether or not you want the barcode to use a ' +
                                'checksum value.  If this is set to true, the checksum will be calcluated for you.'));
  SetPropDesc(TRaveBaseBarCode,'WideFactor',Trans('This value is used to determine the difference between the ' +
                                'wide and the narrow bands.'));
  SetPropDesc(TRaveBaseBarCode,'PrintReadable',Trans('If this value is set to true then the human readable text ' +
                                'will be printed along with the barcode.'));
  SetPropDesc(TRaveBaseBarCode,'PrintChecksum',Trans('Determines whether to include ' +
                                'the checksum character when printing the barcode.'));
  SetPropDesc(TRaveBaseBarCode,'PrintTop',Trans('If this value is set to true then the human ' +
                                'readable text will be printed above the barcode instead of below it.  ' +
                                'This is only meaningful if PrintReadable is set to true.'));
  SetPropDesc(TRaveBaseBarCode,'BarCodeRotation',Trans('This sets the angle the barcode will be printed at.  ' +
                                'Rotation is in a counter-clockwise rotation with the left edge as the pivot point.'));
  SetPropDesc(TRaveCode39BarCode,'Extended',Trans('Determines whether use regular code39 or ' +
                                  'whether to use code39 extended.'));
  SetPropDesc(TRaveCode128BarCode,'CodePage',Trans('Determines the codepage that this barcode will use.'));
  SetPropDesc(TRaveCode128BarCode,'Optimize',Trans('Determines whether the codepage will be optimized ' +
    'for the most efficient bar code (i.e. smallest width) given the contents being printed.'));
{TransOn}
end;  { RegisterRvCsBars }

procedure RegisterRvCsData;
begin { RegisterRvCsData }
{TransOff}
  CreateDataTextControlProc := CreateDataTextControl;
  CreateDataMemoControlProc := CreateDataMemoControl;

  RegisterRaveProperties(TRaveDataText,
   {Beginner}     'DataField;DataView',
   {Intermediate} 'LookupDataView;LookupField;LookupDisplay',
   {Developer}    '',
   {Hidden}       'Text');

  RegisterRaveProperties(TRaveDataMemo,
   {Beginner}     'DataField;DataView',
   {Intermediate} 'ContainsRTF',
   {Developer}    '',
   {Hidden}       'Text');

  RegisterRaveProperties(TRaveCalcText,
   {Beginner}     'CalcType;Controller;DataView;DataField;DisplayFormat',
   {Intermediate} 'CountBlanks;CountValue;DisplayType;Initializer;RunningTotal',
   {Developer}    '',
   {Hidden}       'Text');

  RegisterRaveProperties(TRaveCalcVariable,
   {Beginner}     'DestParam;DisplayFormat',
   {Intermediate} 'DisplayType;DestPIVar',
   {Developer}    '',
   {Hidden}       '');

  RegisterRaveProperties(TRaveCalcOp,
   {Beginner}     'Operator;Src1CalcVar;Src1DataField;Src1DataView;Src1Value;' +
                  'Src2CalcVar;Src2DataField;Src2DataView;Src2Value;',
   {Intermediate} 'Src1Function;Src2Function',
   {Developer}    '',
   {Hidden}       '');

  RegisterRaveProperties(TRaveCalcTotal,
   {Beginner}     'CalcType;Controller;DataView;DataField;DisplayFormat',
   {Intermediate} 'CountBlanks;CountValue;Initializer;RunningTotal',
   {Developer}    '',
   {Hidden}       '');

  RegisterRaveProperties(TRaveCalcController,
   {Beginner}     'InitValue',
   {Intermediate} 'InitCalcVar;InitDataField',
   {Developer}    '',
   {Hidden}       '');

  RegisterRaveProperties(TRaveDataMirrorSection,
   {Beginner}     'DataField;DataView;DataMirrors',
   {Intermediate} 'CompareCase',
   {Developer}    '',
   {Hidden}       '');

  RegisterRaveProperties(TRaveRegion,
   {Beginner}     'Height;Left;Top;Width',
   {Intermediate} 'Columns;ColumnSpacing',
   {Developer}    '',
   {Hidden}       'DisplayOn;Mirror');

  RegisterRaveProperties(TRaveBand,
   {Beginner}     'BandStyle;ControllerBand;Height',
   {Intermediate} 'AllowSplit;GroupDataView;GroupKey',
   {Developer}    '',
   {Hidden}       'Anchor;DisplayOn;Mirror');

  RegisterRaveProperties(TRaveIterateBand,
   {Beginner}     '',
   {Intermediate} 'KeepBodyTogether;KeepRowTogether;OrphanRows;WidowRows',
   {Developer}    '',
   {Hidden}       '');

  RegisterRaveProperties(TRaveDataBand,
   {Beginner}     'DataView',
   {Intermediate} 'DetailKey;MasterDataView,MasterKey;SortKey',
   {Developer}    '',
   {Hidden}       '');

  RegisterRaveProperties(TRaveDataCycle,
   {Beginner}     'DataView',
   {Intermediate} 'DetailKey;MasterDataView,MasterKey;SortKey',
   {Developer}    '',
   {Hidden}       '');

  SetPropDesc(TRaveDataText,'Truncate',Trans('Determines whether the text is ' +
   'truncated or printed completely, regardless of the designed width. If a ' +
   'report variable such as Report.TotalPages is being printed, Truncate should ' +
   'be set to false to prevent the report variable token from being truncated.'));
  SetPropDesc(TRaveDataText,'DataField',Trans('Defines the data field to print. ' +
   'Using the Data Text Editor, any combination of data fields, report variables, ' +
   'project parameters or string constants (delimited by " or '' quotes) can be ' +
   'defined.'));
  SetPropDesc(TRaveDataText,'DataView',Trans('Defines the default dataview that ' +
   'will be used for the DataField property.'));
  SetPropDesc(TRaveDataText,'LookupDataView',Trans('Defines the default dataview that ' +
   'will be used for the LookupField and LookupDisplay properties.'));
  SetPropDesc(TRaveDataText,'LookupField',Trans('Defines the data field that will ' +
   'searched for a match of the current contents of the DataField property.'));
  SetPropDesc(TRaveDataText,'LookupDisplay',Trans('Defines the data field that will ' +
   'printed when a lookup match is found.  This can contain other data text items ' +
   'similar to the DataField property.'));
  SetPropDesc(TRaveDataText,'LookupInvalid',Trans('Defines the text that will be ' +
   'printed if a lookup match is not found.'));

  SetPropDesc(TRaveDataMemo,'ContainsRTF',Trans('Defines whether the memo text ' +
   'contains Rich Text Formatting (RTF) or not.'));
  SetPropDesc(TRaveDataMemo,'DataField',Trans('Defines the data field to print. ' +
   'Using the Data Text Editor, any combination of data fields, report variables, ' +
   'project parameters or string constants (delimited by " or '' quotes) can be ' +
   'defined.'));
  SetPropDesc(TRaveDataMemo,'DataView',Trans('Defines the default dataview that ' +
   'will be used for the DataField property.'));

  SetPropDesc(TRaveCalcText,'CalcType',Trans('Defines the type of summation ' +
   'calculation that will be performed.'));
  SetPropDesc(TRaveCalcText,'Controller',Trans('Defines the controller component ' +
   'that will execute the calculation when the controller is printed.  Normally ' +
   'the controller is tied to a particular dataview and will signal when new data ' +
   'is available.'));
  SetPropDesc(TRaveCalcText,'CountBlanks',Trans('Determines whether or not blank ' +
   '(zero) values are included in the ctCount and ctAverage calculation types.'));
  SetPropDesc(TRaveCalcText,'CountNulls',Trans('Determines whether or not null ' +
   '(empty) values are included in the ctCount and ctAverage calculation types.'));
  SetPropDesc(TRaveCalcText,'CountValue',Trans('Defines the field value ' +
   'that will cause a count to occur if there is a match for CalcType = ctCount.'));
  SetPropDesc(TRaveCalcText,'DataView',Trans('Defines the default dataview that ' +
   'will be used for the DataField property.'));
  SetPropDesc(TRaveCalcText,'DataField',Trans('Defines the data field or project ' +
   'parameter that will be used in the calculation.'));
  SetPropDesc(TRaveCalcText,'DisplayFormat',Trans('Defines the format codes ' +
   'that will be used to create a string representation of the calculated data.  See ' +
   'the Appendix in the Rave documentation for more details on formatting codes.'));
  SetPropDesc(TRaveCalcText,'DisplayType',Trans('Defines whether the contents ' +
   'represent numeric or date/time information.  The DisplayFormat property ' +
   'will be processed as either numeric or date/time format codes depending ' +
   'upon this setting.'));
  SetPropDesc(TRaveCalcText,'Initializer',Trans('Defines the initializer component ' +
   'that will initialize the calculation when the initializer is printed.  Initializers ' +
   'are useful for setting a calculation to a specific value or initializing it ' +
   'at key points in a report.'));
  SetPropDesc(TRaveCalcText,'RunningTotal',Trans('Determines whether the calculated ' +
   'value is reinitialized when it is printed.  If RunningTotal is true, the value ' +
   'will not be reinitialized and further calculations will be added to the existing ' +
   'value.'));
  SetPropDesc(TRaveCalcText,'OnCalcValue',Trans('The OnCalcValue event allows ' +
   'the value that is used in the calculation specified by CalcType to be changed.  ' +
   'Initially, the Value parameter will contain the default value that ' +
   'will be used.  A new value can be assigned if necessary.|Value: extended - The ' +
   'calculation value.'));

  SetPropDesc(TRaveCalcVariable,'DestParam',Trans('Defines the project parameter ' +
   'that the calculation will be written to when it is printed.  This value can ' +
   'be blank if the calculation is only going to be used as an intermediate result ' +
   'for other calculation components.'));
  SetPropDesc(TRaveCalcVariable,'DestPIVar',Trans('Defines the PIVar ' +
   'that the calculation will be written to when it is printed.  This value can ' +
   'be blank if the calculation is only going to be used as an intermediate result ' +
   'for other calculation components.'));
  SetPropDesc(TRaveCalcVariable,'DisplayFormat',Trans('Defines the format codes ' +
   'that will be used to create a string representation of the calculated data.  See ' +
   'the Appendix in the Rave documentation for more details on formatting codes.'));
  SetPropDesc(TRaveCalcVariable,'DisplayType',Trans('Defines whether the contents ' +
   'represent numeric or date/time information.  The DisplayFormat property ' +
   'will be processed as either numeric or date/time format codes depending ' +
   'upon this setting.'));
  SetPropDesc(TRaveCalcVariable,'OnGetValue',Trans('The OnGetValue event allows ' +
   'the value that is passed back when the value for this component is referenced ' +
   'for output or calculation purposes to be changed.  Initially, the Value parameter ' +
   'will contain the default value that will be used.  A new value can be assigned ' +
   'if necessary.|Value: extended - The component value.'));

  SetPropDesc(TRaveCalcOp,'Operator',Trans('Defines the type of operation that will be ' +
   'performed on the two source values.  The result of this calculation can be ' +
   'placed in a project parameter using the DestParam property or used in other ' +
   'calculations as a CalcVar component.'));
  SetPropDesc(TRaveCalcOp,'ResultFunction',Trans('Defines the function that will be ' +
   'applied to the result of the calculation after the operation has been performed.'));
  SetPropDesc(TRaveCalcOp,'Src1CalcVar',Trans('Defines a calculation component to ' +
   'use as the first source value for the calculation operation.  If this property ' +
   'is defined, Src1DataField and Src1Value will be ignored.'));
  SetPropDesc(TRaveCalcOp,'Src1DataField',Trans('If Src1CalcVar is blank, defines ' +
   'a data field to use as the first source value for the calculation operation.  ' +
   'If this property is defined, Src1Value will be ignored.'));
  SetPropDesc(TRaveCalcOp,'Src1DataView',Trans('Defines the default dataview that ' +
   'will be used for the Src1DataField property.'));
  SetPropDesc(TRaveCalcOp,'Src1Value',Trans('If Src1CalcVar and Src1DataField are ' +
   'blank, defines a constant value that will be used as the first source value ' +
   'for the calculation operation.'));
  SetPropDesc(TRaveCalcOp,'Src1Function',Trans('Defines the function that will be ' +
   'performed on the first source value before the calculation operation.'));
  SetPropDesc(TRaveCalcOp,'Src2CalcVar',Trans('Defines a calculation component to ' +
   'use as the first source value for the calculation operation.  If this property ' +
   'is defined, Src2DataField and Src2Value will be ignored.'));
  SetPropDesc(TRaveCalcOp,'Src2DataField',Trans('If Src2CalcVar is blank, defines ' +
   'a data field to use as the first source value for the calculation operation.  ' +
   'If this property is defined, Src2Value will be ignored.'));
  SetPropDesc(TRaveCalcOp,'Src2DataView',Trans('Defines the default dataview that ' +
   'will be used for the Src2DataField property.'));
  SetPropDesc(TRaveCalcOp,'Src2Value',Trans('If Src2CalcVar and Src2DataField are ' +
   'blank, defines a constant value that will be used as the first source value ' +
   'for the calculation operation.'));
  SetPropDesc(TRaveCalcOp,'Src2Function',Trans('Defines the function that will be ' +
   'performed on the first source value before the calculation operation.'));

  SetPropDesc(TRaveCalcTotal,'CalcType',Trans('Defines the type of summation ' +
   'calculation that will be performed.'));
  SetPropDesc(TRaveCalcTotal,'CalcVar',Trans('Defines a calculation component to ' +
   'use in the calculation.  If this property is defined, DataField will be ignored.'));
  SetPropDesc(TRaveCalcTotal,'Controller',Trans('Defines the controller component ' +
   'that will execute the calculation when the controller is printed.  Normally ' +
   'the controller is tied to a particular dataview and will signal when new data ' +
   'is available.'));
  SetPropDesc(TRaveCalcTotal,'CountBlanks',Trans('Determines whether or not blank ' +
   '(zero) values are included in the ctCount and ctAverage calculation types.'));
  SetPropDesc(TRaveCalcTotal,'CountNulls',Trans('Determines whether or not null ' +
   '(empty) values are included in the ctCount and ctAverage calculation types.'));
  SetPropDesc(TRaveCalcTotal,'CountValue',Trans('Defines the field value ' +
   'that will cause a count to occur if there is a match for CalcType = ctCount.'));
  SetPropDesc(TRaveCalcTotal,'DataView',Trans('Defines the default dataview that ' +
   'will be used for the DataField property.'));
  SetPropDesc(TRaveCalcTotal,'DataField',Trans('If CalcVar is blank, defines the ' +
   'data field or project parameter that will be used in the calculation.'));
  SetPropDesc(TRaveCalcTotal,'Initializer',Trans('Defines the initializer component ' +
   'that will initialize the calculation when the initializer is printed.  Initializers ' +
   'are useful for setting a calculation to a specific value or initializing it ' +
   'at key points in a report.'));
  SetPropDesc(TRaveCalcTotal,'RunningTotal',Trans('Determines whether the calculated ' +
   'value is reinitialized when it is printed.  If RunningTotal is true, the value ' +
   'will not be reinitialized and further calculations will be added to the existing ' +
   'value.'));
  SetPropDesc(TRaveCalcTotal,'OnCalcValue',Trans('The OnCalcValue event allows ' +
   'the value that is used in the calculation specified by CalcType to be changed.  ' +
   'Initially, the Value parameter will contain the default value that ' +
   'will be used.  A new value can be assigned if necessary.|Value: extended - The ' +
   'calculation value.'));

  SetPropDesc(TRaveCalcController,'InitCalcVar',Trans('When this component is acting ' +
   'as an initializer, defines the calculation component that will be used as ' +
   'the initializing value.  If this property is defined, InitDataField and ' +
   'InitValue will be ignored.'));
  SetPropDesc(TRaveCalcController,'InitDataField',Trans('If InitCalcVar is blank, ' +
   'defines the data field or project parameter that will be used as the initializing ' +
   'value for any components that this is an initializer for.  If this property ' +
   'is defined, InitValue will be ignored.'));
  SetPropDesc(TRaveCalcController,'InitValue',Trans('If InitCalcVar and InitDataField ' +
   'are blank, defines a constant value that will be used as the initializing value ' +
   'for any components that this is an initializer for.'));

  SetPropDesc(TRaveDataMirrorSection,'CompareCase',Trans('Determines whether the ' +
   'field values defined in the DataMirrors property are compared with case ' +
   'sensitivity to the value of the data field defined by DataField.  If CompareCase ' +
   'is true, there must be an exact match (Apple <> apple).'));
  SetPropDesc(TRaveDataMirrorSection,'DataField',Trans('Defines the data field that ' +
   'will be compared against the field values defined in the DataMirrors property.  ' +
   'When a match is found, the DataMirrorSection will mirror the section ' +
   'component associated with the matching field value.'));
  SetPropDesc(TRaveDataMirrorSection,'DataView',Trans('Defines the default dataview that ' +
   'will be used for the DataField property.'));
  SetPropDesc(TRaveDataMirrorSection,'DataMirrors',Trans('Using the Data Mirror Editor, ' +
   'defines combinations of field values and section components.  These are used to ' +
   'determine which section will be mirrored when this component is printed.  A default ' +
   'data mirror can be defined that will be used if no other match is found.'));
  SetPropDesc(TRaveDataMirrorSection,'OnMirrorValue',Trans('The OnMirrorValue event ' +
   'allows the value that is used in the selection of a data mirror to be changed.  ' +
   'Initially, the Value parameter will contain the default value that ' +
   'will be used.  A new value can be assigned if necessary.|Value: string - The ' +
   'mirror selection value.'));

  SetPropDesc(TRaveRegion,'Columns',Trans('Defines how many columns the region will ' +
   'print.  The width of each column is divided evenly across the width of the region ' +
   'and printing progresses from the left to the right column.'));
  SetPropDesc(TRaveRegion,'ColumnSpacing',Trans('Defines the width of a buffer ' +
   'between each column.  The spacing will not be applied before the first or ' +
   'after the last column.'));

  SetPropDesc(TRaveBand,'AllowSplit',Trans('Determines whether the band can be split ' +
   'across pages.  If AllowSplit is false, the entire band will move to the next page ' +
   'if there is not enough room on the current page.'));
  SetPropDesc(TRaveBand,'BandStyle',Trans('Using the Band Style Editor, defines the ' +
   'locations and occurrences where the band will print.  A band can be set to print ' +
   'multiple locations and occurrences.  The current band style setting is visible on ' +
   'right side of the band header in the page designer.'));
  SetPropDesc(TRaveBand,'ControllerBand',Trans('Defines the band that determines when ' +
   'this band prints.  In other words, this band will be act as a header or footer ' +
   'band for the ControllerBand depending upon the BandStyle property.  This relationship ' +
   'is shown visually in the page designer with a colored arrow that points towards the ' +
   'ControllerBand which contains a diamond shape of the same color.'));
  SetPropDesc(TRaveBand,'DesignerHide',Trans('Determines whether the band is shown ' +
   'in the page designer when it is not selected.  If DesignerHide is true, the band ' +
   'will be hidden when not selected to allow editing of a region with many large bands.'));
  SetPropDesc(TRaveBand,'FinishNewPage',Trans('Determines whether the region will ' +
   'continue on the next page after this band prints.  Useful for keeping report sections ' +
   'on separate pages from a footer band.'));
  SetPropDesc(TRaveBand,'GroupDataView',Trans('Defines the default dataview for the ' +
   'GroupKey property.'));
  SetPropDesc(TRaveBand,'GroupKey',Trans('Defines the data field(s) that make up the ' +
   'group key.  Bands defined as group headers or footers will print whenever the ' +
   'contents of the group key changes.  The group key can contain multiple fields ' +
   'which should be separated with + characters.'));
  SetPropDesc(TRaveBand,'Height',Trans('Defines the height of the band.  The designed ' +
   'height can change if there are components inside that are set to expand their ' +
   'parent such as the memo component.'));
  SetPropDesc(TRaveBand,'MinHeightLeft',Trans('Defines the minimum height that must ' +
   'remain in the region before this band will print.  If MinHeightLeft is greater ' +
   'than the remaining height in the region, the band will be printed on the next page.'));
  SetPropDesc(TRaveBand,'PositionMode',Trans('Defines the mode that will determine where ' +
   'this band prints vertically.  pmOffset and pmOverlay will print a band relative ' +
   'to the bottom of the previous band (the region cursor) while pmAbsolute will ' +
   'print the band relative to the top of the region.  pmOffset will advance the ' +
   'region cursor while pmOverlay and pmAbsolute will not.  See PositionValue.'));
  SetPropDesc(TRaveBand,'PositionValue',Trans('Defines a vertical offset that will ' +
   'applied to the band''s reference point (defined by PositionMode).  Positive values ' +
   'will move the band down the page while negative value will move it up the page.'));
  SetPropDesc(TRaveBand,'ReprintLocs',Trans('If this band is set to reprint on new pages ' +
   'or columns (print occurrence in BandStyle), the style of the band that causes the ' +
   'reprint must be in ReprintLocs for the band to actually reprint.  By default all ' +
   'locations will cause a reprint.'));
  SetPropDesc(TRaveBand,'StartNewPage',Trans('If this is not the first new band to print ' +
   'on this page, the region will print this band on the next page.  Useful for keeping ' +
   'report sections on separate pages from a header band.'));
  SetPropDesc(TRaveBand,'OnGetGroup',Trans('The OnGetGroup event allows the group value' +
   'that is calculated from the GroupDataView and GroupKey properties to be changed.  ' +
   'Initially, the Value parameter will contain the default text that ' +
   'will be printed.  A new value can be assigned if necessary.  Multiple fields in ' +
   'the GroupKey will be separated by a vertical bar character.|Value: string - The ' +
   'group value.'));

  SetPropDesc(TRaveIterateBand,'Columns',Trans('Defines how many columns the band will ' +
   'print.  The width of each column is divided evenly across the width of the band and ' +
   'printing progresses from the left to the right column before progressing to the next row.'));
  SetPropDesc(TRaveIterateBand,'ColumnSpacing',Trans('Defines the width of a buffer ' +
   'between each column.  The spacing will not be applied before the first or ' +
   'after the last column.'));
  SetPropDesc(TRaveIterateBand,'KeepBodyTogether',Trans('Determines whether the ' +
   'body of the band (from body header to body footer) will be kept on a single page.  ' +
   'Unless already at the top of the current page, the region will print the body of ' +
   'this band on the next page if it cannot all fit on the current page.'));
  SetPropDesc(TRaveIterateBand,'KeepRowTogether',Trans('Determines whether the ' +
   'row of the band (from row header to row footer) will be kept on a single page.  ' +
   'Unless already at the top of the current page, the region will print the current ' +
   'row of this band on the next page if it cannot all fit on the current page.'));
  SetPropDesc(TRaveIterateBand,'MaxRows',Trans('Defines the maximum number of rows ' +
   'that this band will print.  Useful for limiting print runs when testing or ' +
   'printing only a set number of records.'));
  SetPropDesc(TRaveIterateBand,'InitToFirst',Trans('Determines whether the band ' +
   'will initialize the dataview to the first record or start printing from the ' +
   'current record.'));
  SetPropDesc(TRaveIterateBand,'OrphanRows',Trans('Defines the minimum number of rows ' +
   'that must be able to print on the current page before the whole body is moved ' +
   'to the next page.  A value of 0 will allow orphan rows.'));
  SetPropDesc(TRaveIterateBand,'WidowRows',Trans('Defines the minimum number of rows ' +
   'that must be able to print on the current page before the whole body is moved ' +
   'to the next page.  A value of 0 will allow orphan rows.'));

  SetPropDesc(TRaveDataBand,'DataView',Trans('Defines the dataview that this databand ' +
   'will print from.  The databand will cycle through the records of the dataview ' +
   'until the dataview reaches the last record or MaxRows is reached.'));
  SetPropDesc(TRaveDataBand,'DetailKey',Trans('Defines the field(s) that will ' +
   'be used to build the detail key.  The master and detail keys are used to filter ' +
   'the records in the current dataview so that only the records that match will be ' +
   'printed.  The default dataview for DetailKey is the DataView property.  There ' +
   'must be the same number of fields in the master and detail keys and multiple ' +
   'fields must be separated with the + character.'));
  SetPropDesc(TRaveDataBand,'MasterDataView',Trans('Defines the default dataview ' +
   'for the MasterKey property.'));
  SetPropDesc(TRaveDataBand,'MasterKey',Trans('Defines the field(s) that will ' +
   'be used to build the master key.  The master and detail keys are used to filter ' +
   'the records in the current dataview so that only the records that match will be ' +
   'printed.  There must be the same number of fields in the master and detail keys ' +
   'and multiple fields must be separated with the + character.'));
  SetPropDesc(TRaveDataBand,'SortKey',Trans('Defines the fields that will be used ' +
   'to sort the dataview fields.  The data connection must support sorting on the ' +
   'specified fields or no sorting will occur.'));

  SetPropDesc(TRaveDataCycle,'DataView',Trans('Defines the dataview that this datacycle ' +
   'will print from.  The datacycle will cycle through the records of the dataview ' +
   'until the dataview reaches the last record or MaxRows is reached.'));
  SetPropDesc(TRaveDataCycle,'DetailKey',Trans('Defines the field(s) that will ' +
   'be used to build the detail key.  The master and detail keys are used to filter ' +
   'the records in the current dataview so that only the records that match will be ' +
   'printed.  The default dataview for DetailKey is the DataView property.  There ' +
   'must be the same number of fields in the master and detail keys and multiple ' +
   'fields must be separated with the + character.'));
  SetPropDesc(TRaveDataCycle,'InitToFirst',Trans('Determines whether the datacycle ' +
   'will initialize the dataview to the first record or start printing from the ' +
   'current record.'));
  SetPropDesc(TRaveDataCycle,'MasterDataView',Trans('Defines the default dataview ' +
   'for the MasterKey property.'));
  SetPropDesc(TRaveDataCycle,'MasterKey',Trans('Defines the field(s) that will ' +
   'be used to build the master key.  The master and detail keys are used to filter ' +
   'the records in the current dataview so that only the records that match will be ' +
   'printed.  There must be the same number of fields in the master and detail keys ' +
   'and multiple fields must be separated with the + character.'));
  SetPropDesc(TRaveDataCycle,'MaxRows',Trans('Defines the maximum number of rows ' +
   'that this datacycle will print.  Useful for limiting print runs when testing or ' +
   'printing only a set number of records.'));
  SetPropDesc(TRaveDataCycle,'SortKey',Trans('Defines the fields that will be used ' +
   'to sort the dataview fields.  The data connection must support sorting on the ' +
   'specified fields or no sorting will occur.'));
{TransOn}
end;  { RegisterRvCsData }

procedure RegisterRvCsDraw;
begin { RegisterRvCsDraw }
{TransOff}
  RegisterRaveProperties(TRaveGraphicBase,
    {Beginner}     '',
    {Intermediate} 'Height;Left;Top;Width',
    {Developer}    'Cursor',
    {Hidden}       '');

  RegisterRaveProperties(TRaveLine,
    {Beginner}     '',
    {Intermediate} 'Color;LineStyle;LineWidth;LineWidthType',
    {Developer}    '',
    {Hidden}       'NESW');

  RegisterRaveProperties(TRaveSurface,
    {Beginner}     '',
    {Intermediate} 'BorderColor;BorderStyle;BorderWidth;BorderWidthType;' +
    'FillColor;FillStyle',
    {Developer}    '',
    {Hidden}       '');

  RegisterRaveProperties(TRaveRectangle,
    {Beginner}     '',
    {Intermediate} 'HRadius;VRadius',
    {Developer}    '',
    {Hidden}       '');

  SetPropDesc(TRaveGraphicBase,'Cursor',Trans('Defines the cursor type that ' +
    'will be used when the mouse passes over the component.'));

  SetPropDesc(TRaveLine,'Color',Trans('Defines the color of the line.  The Color ' +
    'Palette Toolbar can also be used to edit this property.'));
  SetPropDesc(TRaveLine,'LineStyle',Trans('Defines the style (pattern) of the line.  ' +
    'The Line Editor Toolbar can also be used to edit this property.  Only line ' +
    'widths of 1 pixel can have line styles other than solid.'));
  SetPropDesc(TRaveLine,'LineWidth',Trans('Defines the width of the line.  The ' +
    'units of the width are defined by the LineWidthType property.'));
  SetPropDesc(TRaveLine,'LineWidthType',Trans('Defines the units that the LineWidth ' +
    'property are in.  To print the thinnest possible line, use a line width of 1 ' +
    'pixel.  For thicker lines, values in points should be used for consistency ' +
    'across printers with different resolutions.'));

  SetPropDesc(TRaveSurface,'BorderColor',Trans('Defines the border color of the ' +
    'shape.  The Color Palette Toolbar can also be used to edit this property.'));
  SetPropDesc(TRaveSurface,'BorderStyle',Trans('Defines the style (pattern) of the border.  ' +
    'The Line Editor Toolbar can also be used to edit this property.  Only border ' +
    'widths of 1 pixel can have border styles other than solid.'));
  SetPropDesc(TRaveSurface,'BorderWidth',Trans('Defines the width of the border.  The ' +
    'units of the width are defined by the BorderWidthType property.'));
  SetPropDesc(TRaveSurface,'BorderWidthType',Trans('Defines the units that the BorderWidth ' +
    'property are in.  To print the thinnest possible border, use a border width of 1 ' +
    'pixel.  For thicker borders, values in points should be used for consistency ' +
    'across printers with different resolutions.'));
  SetPropDesc(TRaveSurface,'FillColor',Trans('Defines the fill color of the ' +
    'shape.  The Color Palette Toolbar can also be used to edit this property.'));
  SetPropDesc(TRaveSurface,'FillStyle',Trans('Defines the style (pattern) that will ' +
    'be used to draw the interior of the shape.  The fsClear setting can be used ' +
    'to print objects without any fill.'));

  SetPropDesc(TRaveRectangle,'HRadius',Trans('Defines the horizontal radius that ' +
    'can be used to draw rounded corners.  Both HRadius and VRadius must be non-zero ' +
    'for rounded corners.  Use a value of 0 for square corners.'));
  SetPropDesc(TRaveRectangle,'VRadius',Trans('Defines the vertical radius that ' +
    'will be used to draw rounded corners.  Both HRadius and VRadius must be non-zero ' +
    'for rounded corners.  Use a value of 0 for square corners.'));
{TransOn}
end;  { RegisterRvCsDraw }

procedure RegisterRvProj;
begin { RegisterRvProj }
{TransOff}
  RegisterRaveProperties(TRaveReport,
   {Beginner}     'Copies;FirstPage;PageList;PaperSize',
   {Intermediate} 'AlwaysGenerate;Parameters;PIVars',
   {Developer}    '',
   {Hidden}       '');

  RegisterRaveProperties(TRaveProjectManager,
   {Beginner}     'Units',
   {Intermediate} 'Parameters;PIVars;UnitsFactor',
   {Developer}    'AdminPassword;Categories',
   {Hidden}       '');

  SetPropDesc(TRaveReport,'AlwaysGenerate',Trans('Determines whether the report will be ' +
   'generated before printing.  Set this property to true if the report uses the TotalPages ' +
   'report variable.'));
  SetPropDesc(TRaveReport,'Category',Trans('Defines the category that this report belongs to.  ' +
   'Categories are an effective way to organize your reports within the application.'));
  SetPropDesc(TRaveReport,'Copies',Trans('Defines the number of copies that this report will print.'));
  SetPropDesc(TRaveReport,'FirstPage',Trans('Defines the first page that will print when this report ' +
   'is executed.  If PageList is defined, this property will be ignored.'));
  SetPropDesc(TRaveReport,'PageList',Trans('Using the Page List Editor, defines a list of pages and ' +
   'the order to print them when this report is executed.  If this property is defined, FirstPage ' +
   'will be ignored.'));
  SetPropDesc(TRaveReport,'Collate',Trans('Defines whether multiple copies of the report will be ' +
   'collated.  This property will have no effect on printers that do not support collation.'));
  SetPropDesc(TRaveReport,'Duplex',Trans('Defines the duplex mode that the report will be printed ' +
   'in.  This property will have no effect on printers that do not support duplex printing.'));
  SetPropDesc(TRaveReport,'MaxPages',Trans('Defines the maximum number of pages that will ' +
   'be generated when this report is executed.  A value of 0 will cause all pages of the report ' +
   'to be printed.'));
  SetPropDesc(TRaveReport,'Parameters',Trans('Defines the project parameters for the report.  ' +
   'Values can be assigned to project parameters through calculation components and can be ' +
   'printed using DataText components.  Each parameter should be placed on a ' +
   'separate line in the editor.'));
  SetPropDesc(TRaveReport,'PIVars',Trans('Defines the PIVars (Post Initialize Variables) for ' +
   'the report.  Values can be assigned to PIVars through calculation components and can be ' +
   'printed using DataText components.  Each PIVar should be placed on a separate line in the editor.'));
  SetPropDesc(TRaveReport,'Printer',Trans('Defines the name of the printer to send the report to.  ' +
   'The name can be a sub-string such as "FAX" or "LASER".  A blank value will send the report to ' +
   'the default printer.'));
  SetPropDesc(TRaveReport,'Resolution',Trans('Defines the resolution (print quality) that ' +
   'the report will be printed in.'));
  SetPropDesc(TRaveReport,'SecurityControl',Trans('')); //!!!

  SetPropDesc(TRaveProjectManager,'Categories',Trans('Defines categories that reports can belong ' +
   'to for an effective way to organize your reports within the application.  Each category should ' +
   'be placed on a separate line in the editor.'));
  SetPropDesc(TRaveProjectManager,'Parameters',Trans('Defines the project parameters for the report ' +
   'project.  Values can be assigned to project parameters from the application or through calculation ' +
   'components and can be printed using DataText components.  Each parameter should be placed on a ' +
   'separate line in the editor.'));
  SetPropDesc(TRaveProjectManager,'PIVars',Trans('Defines the PIVars (Post Initialize Variables) for ' +
   'the report project.  Values can be assigned to PIVars through calculation components and can be ' +
   'printed using DataText components.  Each PIVar should be placed on a separate line in the editor.'));
  SetPropDesc(TRaveProjectManager,'SecurityControl',Trans('')); //!!!
  SetPropDesc(TRaveProjectManager,'Units',Trans('Defines the units that the report project will be ' +
   'edited in.  For custom unit modes, set the value of UnitsFactor directly.'));
  SetPropDesc(TRaveProjectManager,'UnitsFactor',Trans('Defines the units conversion factor.  This ' +
   'value represents the amount of units that equal one inch (e.g. 2.54 cm per inch).'));
{TransOn}
end;  { RegisterRvProj }

procedure RegisterRvDatabase;
begin { RegisterRvDatabase }
{TransOff}
  RegisterRaveProperties(TRaveDatabase,
   {Beginner}     'Datasource;DesignDatasource;LinkType;Options;Password;Username',
   {Intermediate} '',
   {Developer}    '',
   {Hidden}       '');

  SetPropDesc(TRaveDatabase,'AuthDesign',Trans('Defines the parameters ' +
   'used to establish a database connection when designing reports.'));
  SetPropDesc(TRaveDatabase,'AuthRun',Trans('Defines the parameters ' +
   'used to establish a database connection when the reports are deployed to an ' +
   'application or reporting server.'));
  SetPropDesc(TRaveDatabase,'LinkPoolSize',Trans('The initial size of the connection ' +
   'pool to create when running on the reporting server.'));
  SetPropDesc(TRaveDatabase,'LinkType',Trans('The type of database connection ' +
   'to create.'));
{TransOn}
end;  { RegisterRvDatabase }

procedure RegisterRvDataField;
begin { RegisterRvDataField }
{TransOff}
  RegisterRaveProperties(TRaveDataField,
    {Beginner}     'FieldName;FullName;Description;DisplayFormat',
    {Intermediate} 'Calculated,NullText;Size',
    {Developer}    '',
    {Hidden}       '');

  SetPropDesc(TRaveDataField,{Trans-}'Calculated',Trans('!!!'));
  SetPropDesc(TRaveDataField,{Trans-}'Description',Trans('A description of the data field, ' +
   'usually paragraph-sized, that can be used to tell the purpose and usage of the ' +
   'field data.'));
  SetPropDesc(TRaveDataField,{Trans-}'FieldName',Trans('Defines the name of the field that ' +
   'the field component will retrieve the data from.'));
  SetPropDesc(TRaveDataField,{Trans-}'FullName',Trans('A single line descriptive name of ' +
   'the data field. Unlike the Name property, any letters including spaces can be used.'));
  SetPropDesc(TRaveDataField,{Trans-}'NullText',Trans('Defines the text to be printed ' +
   'if the field data is null (undefined).'));
  SetPropDesc(TRaveDataField,{Trans-}'Size',Trans('Defines the character width of the ' +
   'data field. This is normally used with design time activities such as ' +
   'dragging and dropping datatext components to determine an approximate ' +
   'width that it will take to print the component.'));

  SetPropDesc(TRaveFormattedField,{Trans-}'DisplayFormat',Trans('Defines the format codes ' +
   'that will be used to create a string representation of the field data.  See ' +
   'the Appendix in the Rave documentation for more details on formatting codes.'));

  SetPropDesc(TRaveBooleanField,{Trans-}'TextFalse',Trans('Defines the text that will be' +
   'printed if the field value is false.'));
  SetPropDesc(TRaveBooleanField,{Trans-}'TextTrue',Trans('Defines the text that will be' +
   'printed if the field value is true.'));
{TransOn}
end;  { RegisterRvDataField }

type
  IRaveDataEventHandler = interface
    [{Trans-}'{E89848DC-28E3-4BBB-A8C8-ADC36904EDA4}']
    procedure DataConOpen(DataCon: TRaveDataConnection);
    procedure DataConClose(DataCon: TRaveDataConnection);
    procedure DataConConnect(DataCon: TRaveDataConnection);
    procedure DataSystemOpen(DataSystem: TRaveDataSystem);
    procedure DataSystemClose(DataSystem: TRaveDataSystem);
    procedure DataSystemCallEvent(DataSystem: TRaveDataSystem; EventData: TDataSystemEventData);
    procedure DataSystemEventCalled(DataSystem: TRaveDataSystem; EventData: TDataSystemEventData);
  end; { IRaveDataEventHandler }

{ TRaveDataEventHandler }

type
  TRaveDataEventHandler = class(TRaveEventHandler, IRaveDataEventHandler)
  protected
    function GetEventInterface: TGUID; override;
    procedure Process(Index: integer); override;
  public
    procedure DataConOpen(DataCon: TRaveDataConnection);
    procedure DataConClose(DataCon: TRaveDataConnection);
    procedure DataConConnect(DataCon: TRaveDataConnection);
    procedure DataSystemOpen(DataSystem: TRaveDataSystem);
    procedure DataSystemClose(DataSystem: TRaveDataSystem);
    procedure DataSystemCallEvent(DataSystem: TRaveDataSystem; EventData: TDataSystemEventData);
    procedure DataSystemEventCalled(DataSystem: TRaveDataSystem; EventData: TDataSystemEventData);
  end; { TRaveDataEventHandler }

function TRaveDataEventHandler.GetEventInterface: TGUID;
begin { GetEventInterface }
  Result := IRaveDataEventHandler;
end;  { GetEventInterface }

procedure TRaveDataEventHandler.Process(Index: integer);
var
  IItem: IRaveDataEventHandler;
begin { Process }
  IItem := ProjectEditor as IRaveDataEventHandler;
  case Index of
    1: IItem.DataConOpen(TRaveDataConnection(FObj));
    2: IItem.DataConClose(TRaveDataConnection(FObj));
    3: IItem.DataConConnect(TRaveDataConnection(FObj));
    4: IItem.DataSystemOpen(TRaveDataSystem(FObj));
    5: IItem.DataSystemClose(TRaveDataSystem(FObj));
    6: IItem.DataSystemCallEvent(TRaveDataSystem(FObj),TDataSystemEventData(FPtr^));
    7: IItem.DataSystemEventCalled(TRaveDataSystem(FObj),TDataSystemEventData(FPtr^));
  end; { case }
end;  { Process }

procedure TRaveDataEventHandler.DataConOpen(DataCon: TRaveDataConnection);
begin { DataConOpen }
  BroadcastObj(1,DataCon);
end;  { DataConOpen }

procedure TRaveDataEventHandler.DataConClose(DataCon: TRaveDataConnection);
begin { DataConClose }
  BroadcastObj(2,DataCon);
end;  { DataConClose }

procedure TRaveDataEventHandler.DataConConnect(DataCon: TRaveDataConnection);
begin { DataConConnect }
  BroadcastObj(3,DataCon);
end;  { DataConConnect }

procedure TRaveDataEventHandler.DataSystemOpen(DataSystem: TRaveDataSystem);
begin { DataSystemOpen }
  BroadcastObj(4,DataSystem);
end;  { DataSystemOpen }

procedure TRaveDataEventHandler.DataSystemClose(DataSystem: TRaveDataSystem);
begin { DataSystemClose }
  BroadcastObj(5,DataSystem);
end;  { DataSystemClose }

procedure TRaveDataEventHandler.DataSystemCallEvent(DataSystem: TRaveDataSystem; EventData: TDataSystemEventData);
begin { DataSystemCallEvent }
  BroadcastObjPtr(6,DataSystem,@EventData);
end;  { DataSystemCallEvent }

procedure TRaveDataEventHandler.DataSystemEventCalled(DataSystem: TRaveDataSystem; EventData: TDataSystemEventData);
begin { DataSystemEventCalled }
  BroadcastObjPtr(7,DataSystem,@EventData);
end;  { DataSystemEventCalled }

var
  DataEventHandler: TRaveDataEventHandler;

procedure RegisterRvDirectDataView;
begin { RegisterRvDirectDataView }
{TransOff}
  DataEventHandler := RegisterRaveEventHandler(TRaveDataEventHandler) as
    TRaveDataEventHandler;

  RegisterRaveProperties(TRaveDataView,
    {Beginner}     'ConnectionName',
    {Intermediate} '',
    {Developer}    '',
    {Hidden}       '');

  SetPropDesc(TRaveDataView,{Trans-}'ConnectionName',Trans('Defines the data connection ' +
    'that this dataview is attached to.  This property is initialized when ' +
    'the dataview is created and should not normally be modified.'));
{TransOn}
end;  { RegisterRvDirectDataView }

procedure RegisterRvDriverDataView;
begin { RegisterRvDriverDataView }
{TransOff}
  RegisterRaveProperties(TRaveDriverDataView,
   {Beginner}     'Database;Query',
   {Intermediate} 'QueryParams',
   {Developer}    '',
   {Hidden}       'QueryStruct');

  SetPropDesc(TRaveDriverDataView,'Database',Trans('Defines the database component ' +
   'used to gain connections to the data.'));
  SetPropDesc(TRaveDriverDataView,'Query',Trans('Defines the Query that will be used to ' +
   'generate a result set from the database.'));
  SetPropDesc(TRaveDriverDataView,'QueryParams',Trans('Defines the Parameters that will be used to ' +
   'modify the Query before a result set is generated.'));
{TransOn}
end;  { RegisterRvDriverDataView }

procedure RegisterRvSecurity;
begin { RegisterRvSecurity }
{TransOff}
  RegisterRaveProperties(TRaveSimpleSecurity,
   {Beginner}     'UserList',
   {Intermediate} 'CaseMatters',
   {Developer}    '',
   {Hidden}       '');

  RegisterRaveProperties(TRaveLookupSecurity,
   {Beginner}     'DataView;UserField;PasswordField',
   {Intermediate} '',
   {Developer}    '',
   {Hidden}       '');

  SetPropDesc(TRaveSimpleSecurity,'UserList',Trans('Defines the list of ' +
   'valid username=password pairs.'));
  SetPropDesc(TRaveSimpleSecurity,'CaseMatters',Trans('Determines whether ' +
   'case matters for the password.'));

  SetPropDesc(TRaveLookupSecurity,'DataView',Trans('Defines the default dataview ' +
   'that will be used for the UserField and PasswordField properties.'));
  SetPropDesc(TRaveLookupSecurity,'UserField',Trans('Defines the data field ' +
   'to compare against the provided user name.'));
  SetPropDesc(TRaveLookupSecurity,'PasswordField',Trans('Defines the data field ' +
   'to compare against the provided password.'));
{TransOn}
end;  { RegisterRvSecurity }

procedure RaveRegister;
begin { RaveRegister }
  RegisterRvClass;
  RegisterRvCsStd;
  RegisterRvCsBars;
  RegisterRvCsData;
  RegisterRvCsDraw;
  RegisterRvProj;
  RegisterRvDatabase;
	RegisterRvDataField;
  RegisterRvDirectDataView;
  RegisterRvDriverDataView;
  RegisterRvSecurity;
end;  { RaveRegister }

initialization
  RegisterProc({Trans-}'RVCL',RaveRegister);
end.