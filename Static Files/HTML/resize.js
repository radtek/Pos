document.getElementsByClassName=function(tagName, clsName){ 
 var arr = new Array(); 
 var elems = document.getElementsByTagName(tagName);
  for ( var cls, i = 0; ( elem = elems[i] ); i++ ){
    if ( elem.className == clsName )
	arr[arr.length] = elem;
  }
 return arr;
}
function getBrowserType()
{
	var BODY_EL = (document.compatMode && document.compatMode != "BackCompat")?
				 document.documentElement : 
				 document.body ? document.body : null;
	var user_Agent = navigator.userAgent.toLowerCase();
	var Agent_version = navigator.appVersion;
	var isOpera = !!(window.opera && document.getElementById);
	var isOpera6 = isOpera && !document.defaultView;
	var isOpera7 = isOpera && !isOpera6;
	var isMSIE = (user_Agent.indexOf("msie") != -1) && document.all && BODY_EL && !isOpera;
	var isMSIE6 = isMSIE && parseFloat(Agent_version.substring(Agent_version.indexOf("MSIE")+5)) >= 5.5;
	var isNN4 = (document.layers && typeof document.classes != "undefined");
	var isNN6 = (!isOpera && document.defaultView && typeof document.defaultView.getComputedStyle != "undefined");
	var isW3C_compatible = !isMSIE && !isNN6 && !isOpera && document.getElementById;
	if			 (isOpera6)	return "Opera6";
	if			 (isOpera7)	return "Opera7";
	if			 (isMSIE)	return "MSIE";
	if			 (isMSIE6)	return "MSIE6";
	if				(isNN4)	return "Nav4";
	if				(isNN6)	return "Nav6";
	if	 (isW3C_compatible) return "w3c";
	return null;
}
function replaceExtChars(text,output)
{
	var textneu = text.replace(/&/,"&amp;");
	textneu = textneu.replace(/</,"&lt;");
	textneu = textneu.replace(/>/,"&gt;");
	textneu = textneu.replace(/\r\n/,"<br>");
	textneu = textneu.replace(/\n/,"<br>");
	textneu = textneu.replace(/\r/,"<br>");
	return textneu;
}
function dirname()
{
	var retValue=window.location.href;
	retValue = retValue.substring(0, retValue.lastIndexOf('/'));
  	return retValue;
}
function resizeSearchOutput()
{
	//Resizing search_output
	var ts = document.getElementById("tabSearch");
	if (ts)
	{
		document.getElementById("search").size = 10;
		if (ts.display != "none")
		{
			var so = document.getElementById("search_output");
			var sf = document.getElementById("search_controls");
			so.style.height = (ts.offsetHeight - sf.offsetHeight) + "px";
		}
	}
	//console.log(so.style.height);
}
function resizeTabs()
{
	if (typeof document.tabs == "undefined")
	{
		document.tabs = new Array();
		document.tabs.push("tabMenu");
		document.tabs.push("tabIndex");
		document.tabs.push("tabSearch");
	}
	var ms = document.getElementById("menutabber");
	if (ms)
	{
		var ul = ms.getElementsByTagName("ul")[0];
		if (ul)
		{
			var iPaddings = 0;
			if (ie6 || ie5)
				iPaddings = 8;
			if (ie7)
				iPaddings = 11;
			var menuTabberHeight = parseInt(ms.style.height);
			if (isNaN(menuTabberHeight))
				menuTabberHeight = document.getElementById("sectionNav").offsetHeight - (moz || saf ? 5 : 1);
			var ulHeight = 0;
			if (ul.tagName.toLowerCase() == "ul")
				ulHeight = ul.offsetHeight;
			var newHeight = (menuTabberHeight - ulHeight  + iPaddings);
			if (newHeight > 0)
			{
				for (var i = 0; i < document.tabs.length; ++i)
				{
					var curTab = document.getElementById(document.tabs[i]);
					if (curTab)
					{
						curTab.style.height = newHeight + "px";
						curTab.style.overflow="auto";
					}
				}
			}
		}
		else
		{
			for (var i = 0; i < document.tabs.length; ++i)
			{
				var curTab = document.getElementById(document.tabs[i]);
				if (curTab)
				{
					curTab.style.height = "100%";
					curTab.style.overflow="auto";
				}
			}
		}
	}
	var ts = document.getElementById("tabSearch");
	if (ts)
		ts.style.overflow = "hidden";

}
function resizeContentArea()
{
	var sm = document.getElementById("sectionMain");
    var ca = document.getElementById("contentArea");
    var cs = document.getElementById("contentSection");
    var nt = document.getElementById("navTable");
	
    if (ca && cs && nt)
    {
		cs.style.height = sm.style.height;
        var iPaddings = 0;
		//alert("ie5 = " + ie5 + ", ie5x = " + ie5x + ", ie6 = " + ie6);
		if (ie6 || ie5)
			iPaddings = 3;
        if (cs.offsetHeight > nt.offsetHeight + iPaddings)
            ca.style.height = (cs.offsetHeight - nt.offsetHeight - iPaddings) + "px";
    }
}
function pageSize()
{
  var myWidth = 0, myHeight = 0;
  if( typeof( window.innerWidth ) == 'number' ) {
    //Non-IE
    myWidth = window.innerWidth;
    myHeight = window.innerHeight;
  } else if( document.documentElement && ( document.documentElement.clientWidth || document.documentElement.clientHeight ) ) {
    //IE 6+ in 'standards compliant mode'
    myWidth = document.documentElement.clientWidth;
    myHeight = document.documentElement.clientHeight;
  } else if( document.body && ( document.body.clientWidth || document.body.clientHeight ) ) {
    //IE 4 compatible
    myWidth = document.body.clientWidth;
    myHeight = document.body.clientHeight;
  }
  return {"width" : myWidth, "height" : myHeight};
}
function resizeMainTable()
{

 var myWidth = pageSize().width;
 var myHeight = pageSize().height;


 var bd = document.getElementsByTagName("body")[0];
 bd.style.height = myHeight + "px";
 bd.style.overflowY = "hidden";
 var hd =  document.getElementById("de_hd");
 var ft = document.getElementById("de_ft");
 var mt = document.getElementById("mainTable");
 var hdHeight = 0;
 if (hd && hd.offsetWidth > 0)
   hdHeight = hd.offsetHeight;
 var ftHeight = 0;
 if (ft && ft.offsetWidth > 0)
   ftHeight = ft.offsetHeight;
 var mtHeight = myHeight - hdHeight - ftHeight - 3;
 if (mtHeight < 0)
	mtHeight = myHeight;
 mt.style.height = mtHeight + "px";
 
 var sl = document.getElementById("sectionLeft");
 var sn = document.getElementById("sectionNav");
 var tb = document.getElementById("menutabber");
 var sm = document.getElementById("sectionMain");
 var sr = document.getElementById("sectionRight");
 if (tb)
	tb.style.height = mt.style.height;
 if (sl)
	sl.style.height = mt.style.height;
 if (sn)
	sn.style.height = mt.style.height;
 if (sr)
	sr.style.height = mt.style.height;
 if (sm)
	sm.style.height = mt.style.height;

}
function onResizeHandler()
{
 resizeAll();
 //window.setTimeout("resizeAll()", 1);
}
function resizeAll()
{
 /*DREX_FIT_TO_WINDOW_START*/
 resizeMainTable();
 /*DREX_FIT_TO_WINDOW_END*/
 resizeContentArea();
 resizeTabs();
 resizeSearchOutput();
}
function installOnResize()
{
 window.onresize = onResizeHandler;
 resizeAll();
}
function scrollActiveTab()
{
    if (document.tabScrollTop)
    {
       var scrolElemId = document.defaultTab;
	if (scrolElemId == "tabSearch")
           scrolElemId = "search_output";
	document.getElementById(scrolElemId).scrollTop = document.tabScrollTop;
    }
}
function handleLocationParams()
{
	if (!location.search.substring(1))
		return;
	var Params = location.search.substring(1).split("?"); 
	var estr = Params[0];
	estr = estr.substr(estr.indexOf("MenuState=") + 10);
 	var bits = base64.decode(estr, 0, true, true);
	estr = ar2str(bits);
	estr = utf8_decode(estr);
	estr = lzw.decompress(estr);
	var hasMenu = estr.charCodeAt(0) == 1;
	estr = estr.substring(1);
	if (hasMenu)
	{
		var byteLen = (TopicCnt>>>2) + ((TopicCnt&3)?1:0);
		bits = estr.substring(0, byteLen);
		bits = str2ar(bits);
		estr = estr.substr(byteLen);
		
		var MyByte = 0;
		var MyBB= new Array();
		for (var i = 0; i < bits.length; i++)
		{
			for (var j = 0; j < 8; ++j)
			{
				MyBB.push(bits[i] & 1);
				bits[i] >>>= 1;
			}
		}

		for (var i=0; i<TopicCnt; ++i)
		{
			clsNames[i]=  (MyBB[(i<<1)]==1)?"menu_topic_opened":"menu_topic_closed";
			VarTOpnd[i]=  MyBB[(i<<1)+1];
		}

	}
	var curMenuWidth = estr.charCodeAt(0) + (estr.charCodeAt(1) << 8);
	document.getElementById("sectionNav").style.width = curMenuWidth + "px";
	estr = estr.substring(2);
	var tm = document.getElementById("tabMenu");
	var ti = document.getElementById("tabIndex");
	var ts = document.getElementById("tabSearch");
	var selTab = estr.charCodeAt(0);
	if (selTab != 0)
	{
		var scrollTop = estr.charCodeAt(1) + (estr.charCodeAt(2) << 8);
		var sInfo = estr.substr(3);
		var curTab = null;
		switch (selTab)
		{
			case 1: curTab = tm; break;
			case 2: curTab = ti; break;
			case 3:
			{
				curTab = ts;
				document.getElementById("search").value = sInfo;
				if (typeof isEmpty != "undefined" && !isEmpty(sInfo))
					document.onSearchCompleted = scrollActiveTab;
				break;
			}
		}
		if (curTab)
		{
			document.tabScrollTop = scrollTop;
			document.defaultTab = curTab.id;
			curTab.className += " tabbertabdefault";
		}
	}
	else
	{
		if (tm)
			tm.className = "tabbertab";
		else
			if (ti)
				ti.className = "tabbertab";
			else
				if (ts)
					ts.classname = "tabbertab";
	}
}
function str2ar(str)
{
	var retVal = new Array();
	for (var i = 0; i < str.length; ++i)
		retVal.push(str.charCodeAt(i));
	return retVal;
}
function ar2str(arr)
{
	var str = "";
	for (var i = 0; i < arr.length; ++i)
		str += String.fromCharCode(arr[i]);
	return str;
}

function OpnNxtPage(nextpage, atNewPage)
{
	atNewPage = typeof(atNewPage) != 'undefined' ? atNewPage : false;
	var menuItems = document.getElementById("Table2");
	if (menuItems)
		menuItems = menuItems.getElementsByTagName("tr");
	var paraString= "";
	var bits = new Array();
	var bt = getBrowserType();
    if (menuItems && menuItems.length > 0
    && (  (bt == "MSIE") && (typeof menuItems[0].opnd != "undefined") 
		||(bt != "MSIE") && (typeof menuItems[0].attributes.opnd != "undefined")
	   ) )
	{
		paraString += String.fromCharCode(1);
		for (var i=0; i<menuItems.length; i++)
		{
			bits.push((menuItems[i].className=="menu_topic_opened")?1:0);
			bits.push((
					    ((getBrowserType() == "MSIE") && menuItems[i].opnd == 1) ||
					    ((getBrowserType() != "MSIE")  && menuItems[i].attributes.opnd.value == 1)
						)?1:0);
		}
		while (bits.length & 7)
			bits.push(0);
		var MyByte=0;
		for (var i = 0; i < bits.length; ++i)
		{
			if ((i & 7) == 0)
				MyByte = 0;
			MyByte += (bits[i] << (i & 7));
			if ((i & 7) == 7)
				paraString += String.fromCharCode(MyByte);
		}
	}
	else
	{
		paraString += String.fromCharCode(0);
	}
	var sn = document.getElementById("sectionNav");
	var curMenuWidth = parseInt(sn.style.width) || sn.offsetWidth;
	paraString += String.fromCharCode(curMenuWidth & 0xff);
	paraString += String.fromCharCode(curMenuWidth >>> 8);
	var tm = document.getElementById("tabMenu");
	var ti = document.getElementById("tabIndex");
	var ts = document.getElementById("tabSearch");
	var scrollTop = 0;
	var sInfo = "";
	var tabNum = 0;
	if (tm && tm.className.indexOf("tabbertabhide") == -1)
	{
		tabNum = 1;
		scrollTop = tm.scrollTop;
	}
	if (ti && ti.className.indexOf("tabbertabhide") == -1)
	{
		tabNum = 2;
		scrollTop = ti.scrollTop;
	}
	if (ts && ts.className.indexOf("tabbertabhide") == -1)
	{
		tabNum = 3;
		scrollTop = document.getElementById("search_output").scrollTop;
		sInfo = document.getElementById("search").value;
	}
	paraString += String.fromCharCode(tabNum);
	paraString += String.fromCharCode(scrollTop & 0xff);
	paraString += String.fromCharCode(scrollTop >>> 8);
	paraString += sInfo;

	var outStr = lzw.compress(paraString);
	var encStr = utf8_encode(outStr);
	bits = str2ar(encStr);
	var menuState = base64.encode(bits, false, true);
	
	menuState = "?MenuState="+menuState;
	var targetUrl = nextpage;
	var anch = "";
	var anchPos = -1;
	if ((anchPos = nextpage.indexOf("#")) != -1)
	{
		targetUrl = nextpage.substr(0, anchPos);
		anch = nextpage.substr(anchPos);
	}
	targetUrl += menuState + anch;
	window.open(targetUrl, atNewPage ? "_new" : "_self");
}
 
function onDocumentLoaded()
{
	if (typeof initMenuState != "undefined")
		initMenuState();
	if (typeof GlChld != "undefined")
	{
		for (var curId in GlChld)
		{
			var curEl = document.getElementById(curId);
			if (curEl)
				curEl.className = "menu_topic_closed";
		}
	}
	handleLocationParams();
	var contentLinks = document.getElementsByClassName("a", "local_link");
	var navLinks = document.getElementsByClassName("a", "navigation");
	var menuLinks = document.getElementsByClassName("a", "menu");
	var menuActiveLinks = document.getElementsByClassName("a", "menu_active"); 
	var localLinks = new Array();
	localLinks = localLinks.concat(contentLinks);
	localLinks = localLinks.concat(navLinks); 
	localLinks = localLinks.concat(menuLinks); 
	localLinks = localLinks.concat(menuActiveLinks); 
	for (var i = 0; i < localLinks.length; ++i)
		if (localLinks[i].href && localLinks[i].href.indexOf("javascript:OpnNxtPage") == -1)
		{
			if (localLinks[i].target != "_new")
			{
				localLinks[i].href = "javascript:OpnNxtPage(\"" + localLinks[i].href + "\");";
			}
			else
			{
				localLinks[i].href = "javascript:OpnNxtPage(\"" + localLinks[i].href + "\", true);";
				localLinks[i].target = "_self";
			}
		}			
			
	if (  typeof tabberAutomatic != "undefined"
	    &&typeof tabberOptions != "undefined")
		tabberAutomatic(tabberOptions);
	if (typeof loadIndexes != "undefined")
		try{loadIndexes();}catch(e){}
	installOnResize();
	var anchPos = -1;
	if ((anchPos = location.href.indexOf("#")) != -1)
	{
		var anch = location.href.substr(anchPos+1);
		var anchs = document.getElementsByTagName("a");
		var destAnch = null;
		for (var i = 0; i < anchs.length; ++i)
		{
		  curAnch = anchs[i];
		  if (curAnch.name == anch || curAnch.id == anch)
		  {
		    destAnch = curAnch;
		    break;
		  }
		}
		if (destAnch)
		  destAnch.scrollIntoView();
	}
	if (typeof processMenu != "undefined")
		processMenu();
	var tbl2 = document.getElementById("Table2");
	//Showing menu
	if (tbl2)
		tbl2.className = "menutable";
	if (!document.onSearchCompleted)
		scrollActiveTab();
	else
		SubmitForm();
	var splitter = document.getElementById("drex_splitter");
	if (splitter)
		if (saf)
			splitter.style.width="4px";
}
var drex_event = {
    target: function(e) {
        // for Mozilla/Internet Explorer
        if(e.target) return e.target;
        else if(e.srcElement) return e.srcElement;
    }
    ,add: function(objct,type,fnctn) {
        // for Mozilla/Internet Explorer
        if (objct.addEventListener) {
            objct.addEventListener(type,fnctn,false); return true;
        } else if (objct.attachEvent) {
            var rtrn = objct.attachEvent('on'+type,fnctn); return rtrn;
        } else {
            objct.onclick = fnctn; //objct['on'+type] = fnctn;
        }
    }
    ,remove: function(objct,type,fnctn) {
        // for Mozilla/Internet Explorer
        if(objct.removeEventListener) {
            objct.removeEventListener(type,fnctn,false); return true;
        } else if(objct.detachEvent) {
            var rtrn = objct.detachEvent('on'+type,fnctn); return rtrn;
        } else {
            objct.onclick = null; //objct['on'+type] = null;
        }
    }
};

drex_drag = {
     optn: null //resize_x|resize_y|resize_a
    ,s_clntX: null
    ,s_clntY: null
    ,x_lmt : null
    ,y_lmt : null
    ,dragstart: function() { return false; }
    ,mousemove: function(e) {
        // for Internet Explorer
        if(!e) e = window.event;
        var delta_x = e.clientX-drex_drag.s_clntX;
        if(drex_drag.optn=='resize_x' || drex_drag.optn=='resize_a') {
           var frst_x = drex_drag.curW+delta_x;
            if(0<frst_x && frst_x<drex_drag.x_lmt) {
                document.getElementById('sectionNav').style.width = drex_drag.curW+delta_x+'px';
            }
        }
    }
    ,mouseup: function(e) {
        drex_event.remove(document,'mousemove',drex_drag.mousemove);
        drex_event.remove(document,'mouseup'  ,drex_drag.mouseup);
	document.body.style.cursor = drex_drag.oldCursor;
	document.onselectstart = drex_drag.oldOnSelectStart;
    }
    ,start: function(e,optn) {
        drex_drag.optn = optn;
        drex_drag.s_clntX = e.clientX;
	var sn = document.getElementById("sectionNav");
	drex_drag.curW = parseInt(sn.style.width) || sn.offsetWidth;
	var mt = document.getElementById('mainTable');
        drex_drag.x_lmt  = parseInt(mt.style.width) || mt.offsetWidth;
        // for Internet Explorer
        if(e.srcElement) drex_event.add(e.srcElement,'dragstart',drex_drag.dragstart);
        drex_event.add(document,'mousemove',drex_drag.mousemove);
        drex_event.add(document,'mouseup'  ,drex_drag.mouseup);
	drex_drag.oldCursor = document.body.style.cursor;
	document.body.style.cursor = 'e-resize';
	drex_drag.oldOnSelectStart = document.onselectstart;
	document.onselectstart = function() {return false;} // ie
    }
};