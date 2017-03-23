/** Generated TOC
    Stuart Langridge, July 2007

    Generate a table of contents, based on headings in the page.

    To place the TOC on the page, add

    <div id="generated-toc"></div>

    to the page where you want the TOC to appear. If this element
    is not present, the TOC will not appear.

    The TOC defaults to displaying all headings that are contained within
    the same element as it itself is contained in (or all headings on the
    page if you did not provide a generated-toc container). To override this,
    provide a "highest heading" value by adding class="generate_from_h3"
    (or h2, h4, etc) to the container. (If unspecified, this will display all
    headings, as if class="generate_from_h1" was specified.)

    The TOC defaults to operating only on headings contained within the same
    element as it itself, i.e., in a page like this:

    <div>
      <div>
        <div id="generated-toc"></div>
        <h1>foo</h1>
        <h2>bar</h2>
      </div>
      <h1>quux</h1>
    </div>

    The "quux" heading will not appear in the TOC. To override this,
    add class="generate_for_page" to the container, which will process
    all headings on the page wheresoever they may be.

    06 May 2010 - Dmitri Smirnov <ds@plexoos.com>

       - Introduced global variables
       - Main improvement: put the toggle status of all headings into a single
       cookie
*/


gCookieValue = "";
headings     = [];


generated_toc = {

  generate: function() {

    // Identify our TOC element, and what it applies to
    generate_from = '0';
    generate_for = 'unset';
    tocparent = document.getElementById('generated-toc');
    if (tocparent) {
      // there is a div class="generated-toc" in the document
      // dictating where the TOC should appear
      classes = tocparent.className.split(/\s+/);
      for (var i=0; i<classes.length; i++) {
        // if it specifies which heading level to generate from,
        // or what level to generate for, save these specifications
        if (classes[i].match(/^generate_from_h[1-6]$/)) {
          generate_from = classes[i].substr(classes[i].length-1,1);
        } else if (classes[i].match(/^generate_for_[a-z]+$/)) {
          generate_for = classes[i].match(/^generate_for_([a-z])+$/)[1];
        }
      }
    } else {
      // They didn't specify a TOC element; exit
      return;
    }

    // set top_node to be the element in the document under which
    // we'll be analysing headings
    if (generate_for == 'page') {
      top_node = document.getElementsByTagName('body');
    } else {
      // i.e., explicitly set to "parent", left blank (so "unset"),
      // or some invalid value
      //top_node = tocparent.parentNode;
      top_node = document.getElementById('main_text_body');
    }

    // If there isn't a specified header level to generate from, work
    // out what the first header level inside top_node is
    // and make that the specified header level
    var first_header_found;

    if (generate_from == 0) {
      first_header_found = this.findFirstHeader(top_node);
      if (!first_header_found) {
        // there were no headers at all inside top_node!
        return;
      } else {
        generate_from = first_header_found.toLowerCase().substr(1);
      }
    }

    // add all levels of heading we're paying attention to to the
    // headings_to_treat dictionary, ready to be filled in later
    headings_to_treat = {"h6":''};
    for (var i=5; i>= parseInt(generate_from); i--) {
      headings_to_treat["h" + i] = '';
    }

    // get headings. We can't say
    // getElementsByTagName("h1" or "h2" or "h3"), etc, so get all
    // elements and filter them ourselves
    // need to use .all here because IE doesn't support gEBTN('*')
    nodes = top_node.all ? top_node.all : top_node.getElementsByTagName('*');

    // put all the headings we care about in headings
    for (var i=0; i<nodes.length;i++) {
      if (nodes[i].nodeName.toLowerCase() in headings_to_treat) {
        // if heading has class no-TOC, skip it
        if ((' ' + nodes[i].className + ' ').indexOf('no-TOC') != -1) {
          continue;
        }

        headings.push(nodes[i]);
      }
    }

    gCookieValue = this.readCookie2();

    // Check that number of headings agrees with the cookie length
    if (headings.length != gCookieValue.length) {

       var value = "";

       for (var i=0; i<headings.length; i++) {
          if (headings[i].nodeName.toLowerCase() == "h1") value += "1";
          else value += "0";
       }

       this.createCookie2(value, 30);
       gCookieValue = this.readCookie2();
    }

    // loop over all valid headings and display corresponding div sections
    for (var i=0; i<headings.length; i++) {

       var divSection = document.getElementById("div:" + headings[i].id);

       if (divSection) {
          var toggle = gCookieValue.charAt(i) * 1;
          divSection.style.display = 'block';
       }
    }

    // make the basic elements of the TOC itself, ready to fill into

    // first, check if there's a cookie defined to save the state as open
    olstatus = this.readCookie("generated_toc_display");

    if (olstatus && olstatus == "closed") {
      display_initially = "none";
      toggle_initially = "Show table of contents";
    } else {
      display_initially = "block";
      toggle_initially = "Hide table of contents";
    }

    cur_head_lvl = "h" + generate_from;
    cur_list_el = document.createElement('ol');
    //cur_list_el.style.display = display_initially;
    cur_list_el.className = "count";
    cur_list_el.id = "ol:"+tocparent.id;

    tocparent.appendChild(cur_list_el);

    lists = [];
    styleExtra = "";

    // now walk through our saved heading nodes
    for (var i=0; i<headings.length; i++) {
      this_head_el = headings[i];
      prev_head_el = i>0 ? headings[i-1] : "null";
      this_head_lvl = headings[i].nodeName.toLowerCase();
     
      if (this_head_lvl == "h1") styleExtra = " bold"; //li.className = "count bold";
      else styleExtra = " notbold";


      if (!this_head_el.id) {
        // if heading doesn't have an ID, give it one
        this_head_el.id = 'heading_toc_j_' + i;
      }

      while(this_head_lvl > cur_head_lvl) {
        // this heading is at a lower level than the last one;
        // create additional nested lists to put it at the right level

        // get the *last* LI in the current list, and add our new UL to it
        var last_listitem_el = null;

        for (var j=0; j<cur_list_el.childNodes.length; j++) {
          if (cur_list_el.childNodes[j].nodeName.toLowerCase() == 'li') {
            last_listitem_el = cur_list_el.childNodes[j];
          }
        }

        if (!last_listitem_el) {
          // there aren't any LIs, so create a new one to add the UL to
          last_listitem_el = document.createElement('li');
          last_listitem_el.className = "count";
        }

        new_list_el = document.createElement('ol');
        new_list_el.className = "count";
        new_list_el.id = "ol:"+prev_head_el.id;


        olstatus = gCookieValue.charAt(i-1) * 1;

        new_list_el.style.display = "block";

        lists.push(new_list_el);

        last_listitem_el.appendChild(new_list_el);
        cur_list_el.appendChild(last_listitem_el);
        cur_list_el = new_list_el;
        cur_head_lvl = 'h' + (parseInt(cur_head_lvl.substr(1,1)) + 1);
      }

      while (this_head_lvl < cur_head_lvl) {
        // this heading is at a higher level than the last one;
        // go back up the TOC to put it at the right level
        cur_list_el = cur_list_el.parentNode.parentNode;
        cur_head_lvl = 'h' + (parseInt(cur_head_lvl.substr(1,1)) - 1);
      }

      // create a link to this heading, and add it to the TOC
      li = document.createElement('li');
      li.className = "count" + styleExtra;

      a = document.createElement('a');
      a.href = '#' + this_head_el.id;
      a.appendChild(document.createTextNode(this.innerText(this_head_el)));
      li.appendChild(a);

      olstatus = gCookieValue.charAt(i) * 1;

      toggle = (olstatus == 1 ? "[-]" : "[+]");

      a = document.createElement('a');
      a.href = '#';
      a.appendChild(document.createTextNode(toggle));

      a2 = document.createElement('a');
      a2.href = '#';
      a2.appendChild(document.createTextNode(toggle));

      //a.firstChild.nodeValue = (olstatus ? '[-]' : '[+]');
      olstatus = (olstatus == 1 ? 0 : 1);
      a.onclick = toggleView(i, a, a2, olstatus, lists);

      a2.onclick = toggleView(i, a, a2, olstatus, lists);

      cur_list_el.appendChild(li);
    }

  },


  wrapOpenCloseDiv2: function(a, a2, headingIndex, headings, lists, sections) {
    // we return a function here so that it acts as a closure;
    // in essence the inner function, which is the event handler
    // for clicking on the toggle-toc link, remembers the a and cur_list_el
    // elements as they are when they're passed in to it.
    // This is an explicit function rather than an anonymous function
    // defined where it's called so it's easier to understand.
    return function(e) {

      var ds = "";

      for (var i=0; i<lists.length; i++) {
        //if (lists[i].id == "ol:"+heading.id) {
        if (lists[i].id == "ol:"+headings[headingIndex].id) {
           ds = lists[i].style.display;
           lists[i].style.display = (ds == 'block') ? 'none' : 'block';
           ds = lists[i].style.display;

           if (ds == "block") {
             generated_toc.setHeadingCookie(headingIndex, "1");
           } else {
             generated_toc.setHeadingCookie(headingIndex, "0");
           }

           break;
        }
      }

      for (var i=0; i<sections.length; i++) {
        if (sections[i].id == "div:" + headings[headingIndex].id) {
           ds = sections[i].style.display;
           sections[i].style.display = (ds == 'block') ? 'none' : 'block';
           ds = sections[i].style.display;

           if (ds == "block") {
             generated_toc.setHeadingCookie(headingIndex, "1");
           } else {
             generated_toc.setHeadingCookie(headingIndex, "0");
           }

           break;
        }
      }

      a.firstChild.nodeValue = (ds == 'block' ? '[-]' : '[+]');
      a2.firstChild.nodeValue = (ds == 'block' ? '[-]' : '[+]');

      if (window.event) {
        window.event.returnValue = false;
        window.event.cancelBubble = true;
      } else {
        e.preventDefault();
        e.stopPropagation();
      }
    }
  },

  /* cookie handling: http://www.quirksmode.org/js/cookies.html */
  createCookie: function(name, value, days)
  {
    if (days) {
      var date = new Date();
      date.setTime(date.getTime()+(days*24*60*60*1000));
      var expires = "; expires="+date.toGMTString();
    }
    else var expires = "";

    document.cookie = name+"="+value+expires+"; path=/";
  },


  createCookie2: function(value, days)
  {
    if (days) {
      var date = new Date();
      date.setTime(date.getTime()+(days*24*60*60*1000));
      var expires = "; expires="+date.toGMTString();
    }
    else var expires = "";

    document.cookie = "sections="+value+expires+";";
  },


  readCookie: function(name) {
    var nameEQ = name + "=";
    var ca = document.cookie.split(';');
    for(var i=0;i < ca.length;i++) {
      var c = ca[i];
      while (c.charAt(0)==' ') c = c.substring(1,c.length);
      if (c.indexOf(nameEQ) == 0)
        return c.substring(nameEQ.length,c.length);
    }
    return null;
  },


  readCookie2: function(sectionNum) {

    var nameEQ = "sections=";
    var ca = document.cookie.split(';');
    var value = "";

    for(var i=0; i < ca.length; i++) {

      var c = ca[i];
      while (c.charAt(0) == ' ') c = c.substring(1,c.length);

      if (c.indexOf(nameEQ) == 0) {
         return c.substring(nameEQ.length, c.length);
      } else {
         for (var i=0; i<20; i++) { value += "0";}
         this.createCookie2(value, 30);
         return value;
      }
    }

    return null;
  },


  getSection: function(sectionNum) {
  },


  // change this: must include cookie reading! because it is executed by click
  setHeadingCookie: function(headingIndex, value)
  {
     var newstr = "";
     var newchr = "";

     cookieValue = this.readCookie2();

     for (var i=0; i<cookieValue.length; i++) {
        if (i == headingIndex) newchr = value;
        else newchr = cookieValue[i];

        newstr += newchr;
     }

     this.createCookie2(newstr, 30);
     alert(headingIndex + ", " + value + "\n" + cookieValue + "\n" + newstr);
  },


  eraseCookie: function(name) {
    createCookie(name,"",-1);
  },


  innerText: function(el) {
    return (typeof(el.innerText) != 'undefined') ? el.innerText :
          (typeof(el.textContent) != 'undefined') ? el.textContent :
          el.innerHTML.replace(/<[^>]+>/g, '');
  },


  findFirstHeader: function(node) {
    // a recursive function which returns the first header it finds inside
    // node, or null if there are no functions inside node.
    var nn = node.nodeName.toLowerCase();
    if (nn.match(/^h[1-6]$/)) {
      // this node is itself a header; return our name
      return nn;
    } else {
      for (var i=0; i<node.childNodes.length; i++) {
        var subvalue = generated_toc.findFirstHeader(node.childNodes[i]);
        // if one of the subnodes finds a header, abort the loop and return it
        if (subvalue) return subvalue;
      }
      // no headers in this node at all
      return null;
    }
  },

  init: function() {
    // quit if this function has already been called
    if (arguments.callee.done) return;

    // flag this function so we don't do the same thing twice
    arguments.callee.done = true;

    generated_toc.generate();
  }

};

(function(i) {
   var u =navigator.userAgent;
   var e=/*@cc_on!@*/false;
   var st = setTimeout;
   if(/webkit/i.test(u)){
      st(function(){
         var dr=document.readyState;
      if(dr=="loaded"||dr=="complete"){i()}
      else{st(arguments.callee,10);}},10);}
      else if((/mozilla/i.test(u)&&!/(compati)/.test(u)) || (/opera/i.test(u))){
         document.addEventListener("DOMContentLoaded",i,false);
      } else if(e){
         (function(){
            var t=document.createElement('doc:rdy');
            try {
               t.doScroll('left');
               i();
               t=null;
            } catch(e) { st(arguments.callee,0);}
         })();
      } else { window.onload=i;}
   })(generated_toc.init);


function toggleView(index, link, link2, toggle, lists)
{

   return function(e) {

      gCookieValue = generated_toc.readCookie2();
      var myCookieValue = "";

      link.firstChild.nodeValue = (toggle ? '[-]' : '[+]');
      link.onclick = toggle ? toggleView(index, link, link2, 0, lists) : toggleView(index, link, link2, 1, lists);

      link2.firstChild.nodeValue = (toggle ? '[-]' : '[+]');
      link2.onclick = toggle ? toggleView(index, link, link2, 0, lists) : toggleView(index, link, link2, 1, lists);

      for (var i=0; i<gCookieValue.length; i++) {
         if (i == index) myCookieValue += toggle ? "1" : "0";
         else myCookieValue += gCookieValue.charAt(i);
      }

      generated_toc.createCookie2(myCookieValue, 30);

      var list = document.getElementById("ol:" + headings[index].id);
      if (list) list.style.display = toggle ? 'block' : 'none';

      var divSection = document.getElementById("div:" + headings[index].id);
      if (divSection) divSection.style.display = toggle ? 'block' : 'none';

      if (window.event) {
        window.event.returnValue = false;
        window.event.cancelBubble = true;
      } else {
        e.preventDefault();
        e.stopPropagation();
      }
   }
}


function openTocAll()
{
   var myCookieValue = "";

   // loop over all valid headings
   for (var i=0; i<headings.length; i++) {
      myCookieValue += "1";
   }

   generated_toc.createCookie2(myCookieValue, 30);
}


function resetToc() {

   var myCookieValue = "";

   // loop over all valid headings
   for (var i=0; i<headings.length; i++) {
      myCookieValue += "1";
   }

   generated_toc.createCookie2(myCookieValue, 30);
}
