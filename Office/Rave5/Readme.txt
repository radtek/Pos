=================
Table of contents
=================

A) Converting from ReportPrinter/Rave 1.1, 2.0, 3.0 and 4.0 to Rave 5.0
B) Archived components

-----------------------------------------------------------------------
A) Converting from ReportPrinter/Rave 1.1, 2.0, 3.0 and 4.0 to Rave 5.0
-----------------------------------------------------------------------
IMPORTANT!  If you have projects using previous versions of Rave you should
read the following information for details on upgrading to Rave 5.0.  In
general these changes should occur automatically with little interaction
required.  It is recommended that you load up all forms containing
ReportPrinter/Rave components to allow the conversion process to occur.

1) Component name changes.  In Rave 5.0, most of the components installed in
   Delphi and C++Builder have changed names to a more consistent style.  These
   name changes have been registered with with Delphi and C++Builder IDEs so
   when forms containing older component names are saved or compiled, you will
   receive something similar to the following notification:

   "Field Form1.ReportSystem1 should be of type TRvSystem but is declared as
    TReportSystem.  Correct the declaration?"

   Simply answer yes to all prompts to "Correct the declaration?" and Delphi
   will automatically convert to the new component names for you.

   The following is a list of changed component names:

   Old Component Name -> New Component Name
   ----------------------------------------
   TRaveProject -> TRvProject
   TReportSystem -> TRvSystem
   TReportFiler -> TRvNDRWriter
   TRPCustomConnection -> TRvCustomConnection
   TRPDataSetConnection -> TRvDataSetConnection
   TRPTableConnection -> TRvTableConnection
   TRPQueryConnection -> TRvQueryConnection
   TRPRenderPDF -> TRvRenderPDF
   TRPRenderHTML -> TRvRenderHTML

2) Unit name changes.  A few units have changed names in Rave 5.0.  In general
   Delphi should automatically add the new unit names into your uses
   statements when the forms are loaded into the Delphi IDE.  Compatibility
   units have been created with the older names to keep from encountering
   compilation errors. Once Delphi adds the newer unit names to the uses
   statement, you can remove the older unit names (see list below).

   Old Unit Name -> New Unit Name
   ------------------------------
   RpRender_HTML.pas/dcu -> RpRenderHTML.pas/dcu
   RpRender_PDF.pas/dcu -> RpRenderPDF.pas/dcu

3) Data connection component events.  Because of the change of the component
   name from TRPCustomConnection to TRvCustomConnection, any events created on
   any data connection component must have the Connection parameter changed
   from TRPCustomConnection To TRvCustomConnection (i.e. change the 3rd letter
   from a 'P' to a 'V').  When saving/compiling forms with older event
   signatures you will receive something similar to the following
   notification:

   "The CustomCXNGetCols method referenced by CustomCXN.OnGetCols has an
    incompatible parameter list.  Remove the reference?"

   Answer "No" to any prompts to "Remove the reference?" and then go into the
   source and replace any occurances of TRpCustomConnection with
   TRvCustomConnection

----------------------
B) Archived components
----------------------
The following components have been archived and are no longer being actively
developed or supported on new platforms such as CLX, Linux or the upcoming
.NET.  They will remain part of Rave for backwards compatibility but their
functionality is better supported by other areas in Rave and are not
recommended for use in new reporting projects.

  * TReportPrinter - Superceeded by TRvRenderPrinter
  * TFilePreview - Superceeded by TRvRenderPreview
  * TFilePrinter - Superceeded by TRvRenderPrinter
  * TRTFFiler - Superceeded by TRvRenderRTF
  * TRPHTMLFiler - Superceeded by TRvRenderHTML
  * TTextFiler - Superceeded by TRvRenderText

The following components are also being archived and are superceeded by the
visual reporting components for table style reports.

  * TDetailShell - Superceeded by visual reporting components (Region/Bands)
  * TMasterShell - Superceeded by visual reporting components (Region/Bands)
  * TReportShell - Superceeded by visual reporting components (Region/Bands)
  * TLabelShell - Superceeded by visual reporting components (Region/Bands
    using settings of Region.Columns and Region.ColumnWidth)
  * TDBTablePrinter - Superceeded by visual reporting components (Region/Bands)
  * TTablePrinter - Superceeded by visual reporting components (Region/Bands
    using custom data connection for non-database data)
