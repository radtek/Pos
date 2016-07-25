function replaceExtChars(text,output) {
var textneu = text.replace(/&/,"&amp;");
textneu = textneu.replace(/</,"&lt;");
textneu = textneu.replace(/>/,"&gt;");
textneu = textneu.replace(/\r\n/,"<br>");
textneu = textneu.replace(/\n/,"<br>");
textneu = textneu.replace(/\r/,"<br>");
return textneu;
}
// call from the onMouseDown event, passing the event if standards compliant
function ContextMouseDown(e)
{
    var ctxMenu = document.getElementById("ieCtxMenu");
	if (!e)
		e = event;
	if (e.srcElement)
		if (e.srcElement.onclick && e.srcElement.onclick != openIndexEntry)
			return e.srcElement.onclick();
		else
			if (ctxMenu == e.srcElement)
				return;
	if (e.target)
		if (e.target.onclick && e.target.onclick != openIndexEntry)
			return e.target.onclick();
		else
			if (ctxMenu == e.target)
				return;
			
    document.body.onmousedown = ctxMenu.oldonmousedown;
    ctxMenu.parentNode.removeChild(ctxMenu);
	//alert("ContextMouseDown");
    if (document.body.onmousedown)
        return document.body.onmousedown();
}
function onHoverIe(id)
{
    var target = document.getElementById(id);
    var tbl = target.parentNode;
    while (tbl && tbl.tagName.toUpperCase() != "TABLE")
        tbl = tbl.parentNode;
    var cells = tbl.getElementsByTagName("td");
    for (var i = 0; i < cells.length; ++i)
        cells[i].className = "ieCtxMenuEntry";
    target.className = "ieCtxMenuEntry_Highlighted";
}

function openIndexEntry(e)
{
    //var ieDiv = document.getElementById("ie" + num);
    var ieDiv = this.parentNode;
    if (ieDiv.links.length == 0)
        return;
    if (ieDiv.links.length == 1)
    {
        OpnNxtPage(dirname() + "/" + ieDiv.links[0].src);
        return;
    }
    var menu = document.createElement("div");
    menu.style.maxWidth = "98%";
    menu.style.maxHeight = "98%";
    menu.style.overflow = "auto";
    menu.id = "ieCtxMenu";
    //menu.style = "border: 1px solid blue; display: block; position: absolute";
//<div id="divContext" style="border: 1px solid blue; display: none; position: absolute">  <ul class="cmenu">  <li><a id="aContextNav" href="#">Navigate to</a></li>  <li><a id="aAddWebmark" href="#">Add to WebMark</a></li>  <li class="topSep">  <a id="aDisable" href="#">disable this menu</a>  </li>  </ul> </div>    
    menu.className = "ieCtxMenu";
    if (!e)
        e = event;
	// Temporary variables to hold mouse x-y pos.s
	var pos_x = 0;
	var pos_y = 0;	
	if (document.all)
	{ // grab the x-y pos.s if browser is IE
		pos_x = event.clientX + document.body.scrollLeft;
		pos_y = event.clientY + document.body.scrollTop;
	}
	else
	{  // grab the x-y pos.s if browser is NS
		pos_x = e.pageX;
		pos_y = e.pageY;
	}  
	// catch possible negative values in NS4
	pos_x -= 5;
	pos_y -= 5;
		
	if (pos_x < 5){pos_x = 0;}
	if (pos_y < 5){pos_y = 0;}  		
		

    var tbl = "<table class=\"ieCtxMenuTbl\">";
    //var tbl = "<ul class=\"cmenu\">";
    for (var i = 0; i < ieDiv.links.length; ++i)
    {
        var curTitle = ieDiv.links[i].title;
        var curSrc = ieDiv.links[i].src;
        //tbl += "<li><a href=\"javascript:OpnNxtPage('" + curSrc + "');\">" + curTitle + "</a></li>";
        tbl += "<tr><td id=\"ln"+i+"\"class=\"ieCtxMenuEntry\" onMouseOver=\"onHoverIe('ln" + i + "')\" onclick=\"javascript:OpnNxtPage('" + curSrc + "');\">"
        + replaceExtChars(curTitle, true)
        + "</td></tr>\n";
    }
    tbl += "</table>";
    //tbl += "</ul>"

	
    menu.innerHTML = tbl;

    menu.onmouseout = function() { this.out = true; };;
    menu.onmouseover = function() { this.out = false; };
    menu.oldonmousedown = document.body.onmousedown;
    document.body.onmousedown = ContextMouseDown;
    document.body.appendChild(menu);
    
	var mw = menu.offsetWidth;
	var mh = menu.offsetHeight;
	
	var pageWidth = pageSize().width;
	var pageHeight = pageSize().height;
	
	if (pos_x + mw > pageWidth)
		pos_x -= pos_x + mw - pageWidth;
	
	if (pos_y + mh > pageHeight)
		pos_y -= pos_y + mh - pageHeight;
	if (pos_x < 0)
		pos_x = 0;
	if (pos_y < 0)
		pos_y = 0;
	if (mh > pageHeight)
		menu.style.height = pageHeight + "px";
	if (mw > pageWidth)
		menu.style.width = pageWidth + "px";

    menu.style.left = pos_x + "px";
    menu.style.top = pos_y + "px";

}
function parseIndexEntry(curNode, container, parIndent)
{
    var curTitle = curNode.attributes.getNamedItem("t");
    if (curTitle)
    {
        var curDiv = document.createElement("div");
        container.appendChild(curDiv);
        curDiv.style.textIndent = parIndent + "px";
        curDiv.id = "ie" + container.entryCount;
        var curAnchor = document.createElement("a");
        curDiv.appendChild(curAnchor);
        curAnchor.className = "menu";

        curAnchor.onclick = openIndexEntry;
        //curAnchor.onclick = Function("alert(e);");
        curAnchor.innerHTML = curTitle.value;
        //Settings links
        curDiv.links = new Array();
        var links = curNode.childNodes[1];
        for (var i = 0; i < links.childNodes.length; ++i)
        {
            var curLink = links.childNodes[i];
            var curTitle = curLink.attributes.getNamedItem("t").value;
            var curSrc = curLink.attributes.getNamedItem("s").value;
            curDiv.links.push({title: curTitle, src: curSrc});
        }
		if (curDiv.links.length > 0)
		{
	        curAnchor.style.textDecoration = "underline";
			curAnchor.style.cursor = "pointer";
	        if (document.all)
				curAnchor.style.cursor = "hand";
		}
        ++container.entryCount;
    }
    var ch = curNode.childNodes[0];
    //Parsing childNodes
    for (var i = 0; i < ch.childNodes.length; ++i)
        parseIndexEntry(ch.childNodes[i], container, parIndent + 16);
}
function loadIndexes()
{
    var xmlDoc = XMLload("./index.xml");
    var xmlDoc = xmlDoc.documentElement;
    var ti = document.getElementById("tabIndex");
    ti.entryCount = 0;
    parseIndexEntry(xmlDoc, ti, 0);
}