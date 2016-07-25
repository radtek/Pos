bRemoveImagesFromTooltips = false;
bRemoveDescriptionFromTooltips = false;
bTooltipPositioning = true;
bTooltipPositioningInline = true;

var constShowTimeout = 300;
var constHideTimeout = 750;
var ShowNum = 0;
var HideNum = 0;
var TShowTime = new Array();
var THideTime = new Array();
var Positioned = new Array();
var imgArr = new Array();
var divRightPadding = 100;
var epsilon = 30;
var eventClientX, eventClientY, eventPageX, eventPageY;
function clearTooltipHideIfNecessary(objid)
{
 if (HideNum == objid)
 {
  window.clearTimeout(THideTime[HideNum]);
  THideTime[HideNum];
  HideNum = 0;
 }
}
function TMouseOverF(objid) 
{ 
 return "clearTooltipHideIfNecessary(\"" + objid + "\");";
}
function TMouseOutF(objid) 
{
 return "TooltipHide(\"" + objid + "\");";
}
document.getElementsByClassName=function(tagName, clsName){ 
 var arr = new Array(); 
 var elems = document.getElementsByTagName(tagName);
 for ( var cls, i = 0; ( elem = elems[i] ); i++ ){
 if ( elem.className && elem.className.indexOf(clsName) != -1 )
arr[arr.length] = elem;
 }
 return arr;
}

                    
function ShowItem(event, objid)
{
 var curobj = document.getElementById("Tooltip_"+objid);
 var curBrowser = getBrowserType();
 var x=-1; var dx = -1; var y=-1; var dy = -1;
{
 if (event.pageX) x =  event.pageX;
 else if (event.clientX)
   x = event.clientX + (document.documentElement.scrollLeft ?
   document.documentElement.scrollLeft :
   document.body.scrollLeft);
else x = null;
}
{
if (event.pageY) y = event.pageY;
else if (event.clientY)
   y = event.clientY + (document.documentElement.scrollTop ?
   document.documentElement.scrollTop :
   document.body.scrollTop);
else y = null;
}
 curobj.style.left = x +dx+ 10 + "px";
 curobj.style.top = y +dy+ 10 + "px";
 curobj.style.display = "inline";
 if (bTooltipPositioning)
 {
 var visArea = new Object;

    if (curBrowser.indexOf("MSIE") != -1)
    { 
        visArea.left    = document.documentElement.scrollLeft;
        visArea.top     = document.documentElement.scrollTop;
        visArea.width   = document.documentElement.offsetWidth;
        visArea.height  = document.documentElement.offsetHeight;
    }
    if (curBrowser.indexOf("Nav6") != -1)
    {
        visArea.left    = document.documentElement.scrollLeft;
        visArea.top     = document.documentElement.scrollTop;
        visArea.width   = window.innerWidth;
        visArea.height  = window.innerHeight;
    }
    if (curBrowser.indexOf("Opera") != -1)
    { 
        visArea.left    = document.body.scrollLeft;
        visArea.top     = document.body.scrollTop;
        visArea.width   = window.innerWidth;
        visArea.height  = window.innerHeight;
    }


    visArea.right   = visArea.left + visArea.width;
    visArea.bottom  = visArea.top + visArea.height;

    var divArea = new Object;
    divArea.left    = x + dx + 30;
    divArea.top     = y + dy + 30;
 
    if (curBrowser.indexOf("MSIE") != -1)
    { 
        divArea.width   = curobj.scrollWidth;
        divArea.height  = curobj.scrollHeight;
    }
    if (curBrowser.indexOf("Nav6") != -1)
    {
        divArea.width   = curobj.firstChild.offsetWidth;
        divArea.height  = curobj.offsetHeight;
    }
    if (curBrowser.indexOf("Opera") != -1)
    {
        divArea.width   = curobj.firstChild.offsetWidth;
        divArea.height  = curobj.offsetHeight;
    }    
    divArea.right   = divArea.left + divArea.width;
    divArea.bottom  = divArea.top + divArea.height;
    
    var xdeltaR = divArea.right - visArea.right;
    if (xdeltaR > 0)
        divArea.left -= xdeltaR;
    var xdeltaL = divArea.left - visArea.left;
    if (xdeltaL < 0)
        divArea.left -= xdeltaL-10;
    var ydeltaB = divArea.bottom - visArea.bottom;
    if (ydeltaB > 0)
        divArea.top -= ydeltaB-10;
    var ydeltaT = divArea.top - visArea.top;
    if (ydeltaT < 0)
        divArea.top -= ydeltaT;

 curobj.style.left = divArea.left -20 + "px";
 curobj.style.top  = divArea.top  -20 + "px";
 }
 
 Positioned[objid] = true;
 curobj.style.display = "inline";
 ShowNum = objid;
 TShowTime[objid] = null;
 THideTime[objid] = null;
}

function TooltipShow(event, objid)
{
 if (HideNum == objid)
 {
 window.clearTimeout(THideTime[HideNum]);
 THideTime[HideNum];
 HideNum = 0;
 return;
 }
 if (HideNum != 0)
 {
 var HideTimeout = THideTime[HideNum];
 window.clearTimeout(HideTimeout);
 document.getElementById("Tooltip_"+HideNum).style.display = "none"; 
 HideNum = 0;
 ShowItem(event, objid);
 return;
 }
 var ShowTimeout = null;
 if (ShowNum != 0)
ShowTimeout = TShowTime[ShowNum]; 
 if (ShowTimeout != null) 
 { 
window.clearTimeout(ShowTimeout); 
document.getElementById("Tooltip_"+ShowNum).style.display = "none"; 
ShowItem(event, objid);
 } 
 else
 {
var TimeoutFunction = function(objid)
{
var curobj = document.getElementById("Tooltip_"+objid);
var curBrowser = getBrowserType();
var x=-1; var dx = -1; var y=-1; var dy = -1;
{
 if (eventPageX) x =  eventPageX;
 else if (eventClientX)
   x = eventClientX  + (document.documentElement.scrollLeft ?
   document.documentElement.scrollLeft :
   document.body.scrollLeft);
 else x = 0;
}
{
if (eventPageY) y = eventPageY ;
else if (eventClientY)
   y = eventClientY + (document.documentElement.scrollTop ?
   document.documentElement.scrollTop :
   document.body.scrollTop);
else y = 0;
}

curobj.style.left = x + dx + 10 + "px";
curobj.style.top = y + dy + 10 + "px";
curobj.style.display = "inline";
if (bTooltipPositioningInline)
{

var visArea = new Object;
    if (curBrowser.indexOf("MSIE") != -1)
    { 
        visArea.left    = document.documentElement.scrollLeft;
        visArea.top     = document.documentElement.scrollTop;
        visArea.width   = document.documentElement.offsetWidth;
        visArea.height  = document.documentElement.offsetHeight;
    }

    if (curBrowser.indexOf("Nav6") != -1)
    {
        visArea.left    = document.documentElement.scrollLeft;
        visArea.top     = document.documentElement.scrollTop;
        visArea.width   = window.innerWidth;
        visArea.height  = window.innerHeight;
    }
    if (curBrowser.indexOf("Opera") != -1)
    { 
        visArea.left    = document.body.scrollLeft;
        visArea.top     = document.body.scrollTop;
        visArea.width   = window.innerWidth;
        visArea.height  = window.innerHeight;
    }

    visArea.right   = visArea.left + visArea.width;
    visArea.bottom  = visArea.top + visArea.height;

    var divArea = new Object;
    divArea.left    = x + dx + 30;
    divArea.top     = y + dy + 30;


    if (curBrowser.indexOf("MSIE") != -1)
    { 
        divArea.width   = curobj.scrollWidth;
        divArea.height  = curobj.scrollHeight;
    }
    if (curBrowser.indexOf("Nav6") != -1)
    {
        divArea.width   = curobj.firstChild.offsetWidth;
        divArea.height  = curobj.offsetHeight;
    }
    if (curBrowser.indexOf("Opera") != -1)
    {
        divArea.width   = curobj.firstChild.offsetWidth;
        divArea.height  = curobj.offsetHeight;
    }
    divArea.right   = divArea.left + divArea.width;
    divArea.bottom  = divArea.top + divArea.height;

    var xdeltaR = divArea.right - visArea.right;
    if (xdeltaR > 0)
        divArea.left -= xdeltaR;
    var xdeltaL = divArea.left - visArea.left;
    if (xdeltaL < 0)
        divArea.left -= xdeltaL;
    var ydeltaB = divArea.bottom - visArea.bottom;
    if (ydeltaB > 0)
        divArea.top -= ydeltaB;
    var ydeltaT = divArea.top - visArea.top;
    if (ydeltaT < 0)
        divArea.top -= ydeltaT;

 curobj.style.left = divArea.left -20 + "px";
 curobj.style.top  = divArea.top  -20 + "px";
}
Positioned[objid] = true;
curobj.style.display = "inline";
TShowTime[objid] = null;
}//TimeoutFunction
ShowNum = objid;
eventClientX = event.clientX;
eventClientY = event.clientY;
eventPageX = event.pageX;
eventPageY = event.pageY;
TShowTime[objid] = window.setTimeout(function(){TimeoutFunction(objid)}, constShowTimeout);
 }
}
function TooltipHide(objid)
{
 var ShowTimeout = null;
 if (ShowNum != 0)
 ShowTimeout = TShowTime[ShowNum];
 if (ShowTimeout != null && document.getElementById("Tooltip_"+ShowNum).style.display == "none")
 {
 window.clearTimeout(ShowTimeout);
 TShowTime[ShowNum] = null;
 ShowNum = 0;
 return;
 }
 var TimeoutFunction = function(objid)
 {
document.getElementById("Tooltip_"+objid).style.display = "none";
THideTime[objid] = null;
HideNum = 0;
ShowNum = 0; 
 }
 THideTime[objid] = window.setTimeout(function(){TimeoutFunction(objid)}, constHideTimeout);
 HideNum = objid;
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
if (isOpera6)return "Opera6";
if (isOpera7)return "Opera7";
if (isMSIE)return "MSIE";
if (isMSIE6)return "MSIE6";
if(isNN4)return "Nav4";
if(isNN6)return "Nav6";
if (isW3C_compatible) return "w3c";
return null;
}
function attachMyAttrib(anElement, aName, aValue)
{
 if (getBrowserType().indexOf("MSIE") != -1)
    anElement[aName] = aValue;
 else
 {
var myNewAttr = document.createAttribute(aName);
myNewAttr.value = aValue;
var myOldAttr = anElement.setAttributeNode(myNewAttr);
 }
}
function DrExplain_Make_Tooltips()
{
 
 var wndImgs = document.getElementsByClassName("img","de_wndimg");
 
 if (!wndImgs || wndImgs.length == 0) return;
 var areas = document.getElementsByTagName("area");
 for (var i=0; i<areas.length; i++)
 {
  var curid = areas[i].href.substring(areas[i].href.lastIndexOf("#")+4, areas[i].href.length);
  attachMyAttrib(areas[i], "id", "area_"+curid);
 }
 for (var i = 0; i < wndImgs.length; ++i)
if (getBrowserType().indexOf("MSIE") != -1)
{
wndImgs[i].alt = "";
}
else
{
var altAttr = wndImgs[i].getAttributeNode("alt");
if (altAttr)
wndImgs[i].removeAttributeNode(altAttr);
}
 var divs = document.getElementsByClassName("div","de_ctrl");
 for (var i=0; i<divs.length; i++)
 {
  var curid = divs[i].firstChild.rows[0].cells[0].firstChild.name;
  if (curid.lastIndexOf('id_')!= -1)
    curid = curid.substring(3, curid.length);
  if (curid != "top")
  {
var apparea = document.getElementById("area_"+curid);
olddiv = divs[i].innerHTML;
{
  hiddenImg= new Image();
  if (divs[i].getElementsByTagName("img").length)
hiddenImg.src= divs[i].getElementsByTagName("img")[0].src;
  imgArr[curid] = hiddenImg;
}
 if (bRemoveDescriptionFromTooltips)
 {
 
  var D1stIMG = divs[i].getElementsByTagName("img")[0];
 var dIMG = new Image();
 dIMG.src = D1stIMG.src;
 dIMG.alt = D1stIMG.alt;
 divs[i].innerHTML = '';
 divs[i].appendChild(dIMG);
 }
 if (bRemoveImagesFromTooltips)
 {
 var D1stIMG = divs[i].getElementsByTagName("img")[0];
 if (D1stIMG)
D1stIMG.parentNode.removeChild(D1stIMG);
 var d1stBR = divs[i].getElementsByTagName("br")[0];
 if (d1stBR)
d1stBR.parentNode.removeChild(d1stBR);
 }
 //Remove [Top] links from divs
 var curTrs = divs[i].getElementsByTagName("tr");
 var trsToRemove = new Array();
 for (var k = 0; k < curTrs.length; ++k)
if (curTrs[k].className.indexOf("topLinkRow") != -1)
curTrs[k].parentNode.removeChild(curTrs[k]);
 //End of removing [Top] links from divs
if (!bRemoveImagesFromTooltips &&  divs[i].getElementsByTagName("img").length && divs[i].getElementsByTagName("img")[0].style.display=="none")
 divs[i].getElementsByTagName("img")[0].style.display="inline";
var comText = divs[i].getElementsByTagName("div");
 if (comText.length>0)
 {
 comText = comText[0];
if (comText.id == "ComText_"+curid)
{
var comTextHTML = comText.innerHTML;
comText.parentNode.removeChild(comText);
divs[i].innerHTML += comTextHTML;
}
 }
 var sALT =divs[i].innerHTML;
if (sALT.length != 0)
{
divs[i].innerHTML = olddiv;
document.write(
"<div class=\"Tooltip\" style=\""+
"position: absolute; display: none; font-family: Verdana, Arial, Helvetica, sans-serif;"+
 "font-size: 10pt; background-color: #ffffff; text-decoration: none;\" id = \"Tooltip_"+curid+"\"></div>");
 var curTooltip = document.getElementById("Tooltip_"+curid);
 curTooltip.innerHTML = 
"<table cellpadding = '5' style='border-width: 1px; border-color: black; border-style: solid; background-color: #ffffff;"+ 
"font-family: Verdana, Arial, Helvetica, sans-serif; font-size: 10pt; text-decoration: none;' ID=\"Table1\"><tr><td>"+
sALT
+"</tr></td></table >";
var timgs = curTooltip.getElementsByTagName("img");
for (var j = 0; j < timgs.length; ++j)
 if (timgs[j].className && timgs[j].className.indexOf("de_ctrlbullet") != -1)
timgs[j].parentNode.removeChild(timgs[j]);
var ps = curTooltip.getElementsByTagName("p");
for (var j = 0; j < ps.length; ++j)
 if (ps[j].style.marginLeft == "50px")
   ps[j].style.marginLeft = "0px";
var curBrowser = getBrowserType();
if (curBrowser.indexOf("MSIE") == -1)
{
attachMyAttrib(apparea, "onmouseover","javascript:TooltipShow(event,\""+curid+"\");");
attachMyAttrib(apparea, "onmouseout", "javascript:TooltipHide(\""+curid+"\");");
attachMyAttrib(document.getElementById("Tooltip_"+curid), "onmouseover", TMouseOverF(curid)); 
attachMyAttrib(document.getElementById("Tooltip_"+curid), "onmouseout", TMouseOutF(curid)); 
if (curBrowser.indexOf("Opera") != -1)
{
apparea.removeAttributeNode(apparea.getAttributeNode("href"));
attachMyAttrib(apparea, "onclick", "javascript:window.open('#" +curid+ "','_self');");
apparea.parentNode.style.cursor = 'pointer';
}
}
else
{
apparea.attachEvent('onmouseover', Function("TooltipShow(event,\""+curid+"\")"));
apparea.attachEvent('onmouseout', Function("TooltipHide(\""+curid+"\")"));
document.getElementById("Tooltip_"+curid).attachEvent("onmouseover", Function(TMouseOverF(curid))); 
document.getElementById("Tooltip_"+curid).attachEvent("onmouseout", Function(TMouseOutF(curid))); 
}
}

  }
 }
} 