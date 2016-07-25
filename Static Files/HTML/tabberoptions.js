//Tabber options
var tabberOptions = {

  /* Optional: instead of letting tabber run during the onload event,
     we'll start it up manually. This can be useful because the onload
     even runs after all the images have finished loading, and we can
     run tabber at the bottom of our page to start it up faster. See the
     bottom of this page for more info. Note: this variable must be set
     BEFORE you include tabber.js.
  */
  'manualStartup':true,

  /* Optional: code to run after each tabber object has initialized */

  'onLoad': function(argsObj) {
    /*alert("onLoad");*/
    /* Display an alert only after tab2 */
    if (argsObj.tabber.id == 'tab2') {
      alert('Finished loading tab2!');
    }
  },

  /* Optional: code to run when the user clicks a tab. If this
     function returns boolean false then the tab will not be changed
     (the click is canceled). If you do not return a value or return
     something that is not boolean false, */

  'onClick': function(argsObj) {

    var t = argsObj.tabber; /* Tabber object */
    var id = t.id; /* ID of the main tabber DIV */
    var i = argsObj.index; /* Which tab was clicked (0 is the first tab) */
    var e = argsObj.event; /* Event object */

    if (id == 'menutabber')
    {
        if (t.tabs[i].div.id == 'tabSearch')
        {
            window.setTimeout("resizeSearchOutput()", 0);
        }
        return true;
        /*return confirm('Swtich to '+.headingText+'?\nEvent type: '+e.type);*/
    }
  },

  /* Optional: set an ID for each tab navigation link */
  'addLinkId': true

};
//End of tabber options

