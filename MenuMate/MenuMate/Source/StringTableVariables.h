// Unique ID for String table resources.

UnicodeString CSREPORT_TOP =  "<html> \
	<head> \
	<title>Course Stock Display</title> \
	<meta http-equiv=""Content-Type"" content=""text/html; charset=iso-8859-1""> \
	</head> \
	<body bgcolor=""#E4C198"" text=""#000000""> \
	<table width=""100%"" border=""1""> ";

UnicodeString CSREPORT_COURSE_ROW = "<tr> \
	 <td colspan=""5""> \
		<div align=""center""><font size=""5"">Stock On Hand for Course : <b>%COURSENAME%</b></font></div> \
	 </td> \
	 </tr>";

UnicodeString CSREPORT_ITEM_ROW =  "<tr bgcolor=""#BFBDBB""> \
	 <td colspan=""5""><b>%ITEMNAME%</b>&nbsp;&nbsp;&nbsp;&nbsp;Size : <b>%SIZENAME%</b></td> \
  </tr>";

UnicodeString CSREPORT_RECEIPE_HEADER_ROW = "<tr bgcolor=""#BFBDBB""> \
	 <td width=""11%""> \
		<div align=""center""><b>Stock Code</b></div> \
	 </td> \
	 <td width=""16%""> \
		<div align=""center""><b>Stock Location</b></div> \
	 </td> \
	 <td width=""21%""> \
		<div align=""center""><b>On Hand (Min/Max) </b></div> \
	 </td> \
	 <td width=""14%""> \
		<div align=""center""><b>Unit</b></div> \
	 </td> \
	 <td width=""38%""> \
		<div align=""center""><b>Description</b></div> \
	 </td> \
	 </tr>";

UnicodeString CSREPORT_BOTTOM = "</table> \
	</body> \
	</html>";

UnicodeString STATUSREPORT_TOP = "<html> \
	<head> \
	<title>Status Report</title> \
	<meta http-equiv=""Content-Type"" content=""text/html; charset=iso-8859-1""> \
	</head> \
	<body bgcolor=""#E4C198"" text=""#000000""> \
	<table width=""100%"" border=""1""> ";

UnicodeString STATUSREPORT_MOD_ROW = "<tr> \
	 <td colspan=""5""> \
		<div align=""center""><font size=""5"">Module : <b>%MOD_NAME%</b> Properties.</font></div> \
	 </td> \
	 </tr>";

UnicodeString STATUSREPORT_VAL_ROW =  "<tr bgcolor=""#BFBDBB""> \
	 <td colspan=""5""><b>%VARNAME%</b>&nbsp;&nbsp;&nbsp;&nbsp; <b>%VAR_VALUE%</b></td> \
  </tr>";

UnicodeString STATUSREPORT_BOTTOM = "</table> \
	</body> \
	</html>";

UnicodeString HTML_START = "<html> \
	<head> \
	<title>%TITLE%</title> \
	<meta http-equiv=""Content-Type"" content=""text/html; charset=ANSI_CHARSET""> \
   <STYLE TYPE=""text/css""> \
   <!-- \
   body {overflow:hidden} \
   --> \
   </STYLE> \
	</head> \
	<body bgcolor=""#DADADA"" text=""#000000"">";

UnicodeString HTML_BODY_STOP = "</body> \
	</html>";

UnicodeString TABLE_START = "<table width=""100%"" border=""1"">";
UnicodeString TABLE_STOP =  "</table>";

UnicodeString TABLE_ROW4_HEADER =
  "<tr>                                                                       \
	 <td colspan=""4"">                                                        \
		<div align=""left""><font size=""5""><b>%TABLETITLE%</b> <b>%TABLEHEADER%</b> </font></div>   \
	 </td>                                                                     \
  </tr>";

UnicodeString TABLE_ROW4 = "<tr bgcolor=""#$BFBDBB"" width=""100%"">                                        \
	 <td width=""15%""><b>%ROWTITLE1%</b></td> 						               \
	 <td style=""word-break: break-all;word-wrap: break-word;"" width=""35%""><div>%ROWCONTENT1%</div></td>			                              \
	 <td width=""15%""><b>%ROWTITLE2%</b></td>                                 \
	 <td style=""word-break: break-all;word-wrap: break-word;"" width=""35%"">%ROWCONTENT2%</td>		                                 \
  </tr>";

UnicodeString TABLE_ROW4S = "<tr bgcolor=""#BFBDBB"">                                    \
	 <td style=""text-align: %ALIGNC1%; width: %SIZEC1%;"">%COL1%</td> 		\
	 <td style=""text-align: %ALIGNC2%; width: %SIZEC2%;"">%COL2%</td>		\
	 <td style=""text-align: %ALIGNC3%; width: %SIZEC3%;"">%COL3%</td>      \
	 <td style=""text-align: %ALIGNC4%; width: %SIZEC4%;"">%COL4%</td>		\
  </tr>";

UnicodeString TABLE_ROW6_HEADER =
  "<tr>                                                                       \
	 <td colspan=""3"">                                                        \
		<div align=""left""><font size=""5""><b>%TABLETITLE%</b></font></div>   \
	 </td>                                                                     \
	 <td colspan=""3""> <b>%TABLEHEADER%</b></td>                              \
  </tr>";

UnicodeString TABLE_ROW6 = "<tr bgcolor=""#BFBDBB"">                                        \
	 <td width=""10%""><b>%ROWTITLE1%</b></td> 						               \
	 <td width=""30%"">%ROWCONTENT1%</td>			                              \
	 <td width=""10%""><b>%ROWTITLE2%</b></td>                                 \
	 <td width=""30%"">%ROWCONTENT2%</td>                               			\
	 <td width=""10%""><b>%ROWTITLE3%</b></td>                                 \
	 <td width=""10%"">%ROWCONTENT3%</td>			                              \
  </tr>";

UnicodeString TABLE_ROW6A = "<tr bgcolor=""#BFBDBB"">                                        \
	 <td width=""10%""><b>%ROWTITLE1%</b></td> 						               \
	 <td width=""10%"">%ROWCONTENT1%</td>			                              \
	 <td width=""10%""><b>%ROWTITLE2%</b></td>                                 \
	 <td width=""50%"">%ROWCONTENT2%</td>                               			\
	 <td width=""10%""><b>%ROWTITLE3%</b></td>                                 \
	 <td width=""10%"">%ROWCONTENT3%</td>			                              \
  </tr>";

UnicodeString TABLE_ROW2_HEADER =
  "<tr>                                                                       \
	 <td colspan=""1"">                                                        \
		<div align=""left""><font size=""5""><b>%TABLETITLE%</b></font></div>   \
	 </td>                                                                     \
	 <td colspan=""1""> <b>%TABLEHEADER%</b></td>                              \
  </tr>";

UnicodeString TABLE_ROW2 = "<tr bgcolor=""#BFBDBB"">                                        \
	 <td width=""25%""><b>%ROWTITLE1%</b></td> 						               \
	 <td width=""75%"">%ROWCONTENT1%</td>			                              \
  </tr>";

UnicodeString HTML_HEAD = "<html> \
	<head> \
	<title>%TITLE%</title> \
	<meta http-equiv=""Content-Type"" content=""text/html; charset=iso-8859-1""> \
	</head>";

UnicodeString HTML_STOP = "</html>";

UnicodeString TABLE_ROW8_HEADER =
  "<tr>                                                                       \
	 <td colspan=""8"">                                                        \
		<div align=""left""><font size=""5""><b>%TABLETITLE%</b> <b>%TABLEHEADER%</b> </font></div>   \
	 </td>                                                                     \
  </tr>";

UnicodeString TABLE_ROW8S = "<tr bgcolor=""#BFBDBB"">                                    \
	 <td style=""text-align: %ALIGNC1%; width: %SIZEC1%;"">%COL1%</td>      \
	 <td style=""text-align: %ALIGNC2%; width: %SIZEC2%;"">%COL2%</td>		\
	 <td style=""text-align: %ALIGNC3%; width: %SIZEC3%;"">%COL3%</td>      \
	 <td style=""text-align: %ALIGNC4%; width: %SIZEC4%;"">%COL4%</td>		\
	 <td style=""text-align: %ALIGNC5%; width: %SIZEC5%;"">%COL5%</td>      \
	 <td style=""text-align: %ALIGNC6%; width: %SIZEC6%;"">%COL6%</td>		\
	 <td style=""text-align: %ALIGNC7%; width: %SIZEC7%;"">%COL7%</td>      \
	 <td style=""text-align: %ALIGNC8%; width: %SIZEC8%;"">%COL8%</td>		\
  </tr>";

UnicodeString TABLE_ROW7_HEADER =
  "<tr>                                                                       \
	 <td colspan=""7"">                                                        \
		<div align=""left""><font size=""5""><b>%TABLETITLE%</b> <b>%TABLEHEADER%</b> </font></div>   \
	 </td>                                                                     \
  </tr>";

UnicodeString TABLE_ROW7S = "<tr bgcolor=""#BFBDBB"">                                    \
	 <td style=""text-align: %ALIGNC1%; width: %SIZEC1%;"">%COL1%</td>      \
	 <td style=""text-align: %ALIGNC2%; width: %SIZEC2%;"">%COL2%</td>		\
	 <td style=""text-align: %ALIGNC3%; width: %SIZEC3%;"">%COL3%</td>      \
	 <td style=""text-align: %ALIGNC4%; width: %SIZEC4%;"">%COL4%</td>		\
	 <td style=""text-align: %ALIGNC5%; width: %SIZEC5%;"">%COL5%</td>      \
	 <td style=""text-align: %ALIGNC6%; width: %SIZEC6%;"">%COL6%</td>		\
	 <td style=""text-align: %ALIGNC7%; width: %SIZEC7%;"">%COL7%</td>      \
  </tr>";

UnicodeString TABLE_ROW9_HEADER =
  "<tr>                                                                       \
	 <td colspan=""9"">                                                        \
		<div align=""left""><font size=""5""><b>%TABLETITLE%</b> <b>%TABLEHEADER%</b> </font></div>   \
	 </td>                                                                     \
  </tr>";

UnicodeString TABLE_ROW9S = "<tr bgcolor=""#BFBDBB"">                                    \
	 <td style=""text-align: %ALIGNC1%; width: %SIZEC1%;"">%COL1%</td>      \
	 <td style=""text-align: %ALIGNC2%; width: %SIZEC2%;"">%COL2%</td>		\
	 <td style=""text-align: %ALIGNC3%; width: %SIZEC3%;"">%COL3%</td>      \
	 <td style=""text-align: %ALIGNC4%; width: %SIZEC4%;"">%COL4%</td>		\
	 <td style=""text-align: %ALIGNC5%; width: %SIZEC5%;"">%COL5%</td>      \
	 <td style=""text-align: %ALIGNC6%; width: %SIZEC6%;"">%COL6%</td>		\
	 <td style=""text-align: %ALIGNC7%; width: %SIZEC7%;"">%COL7%</td>      \
	 <td style=""text-align: %ALIGNC8%; width: %SIZEC8%;"">%COL8%</td>		\
	 <td style=""text-align: %ALIGNC9%; width: %SIZEC9%;"">%COL9%</td>      \
  </tr>";







