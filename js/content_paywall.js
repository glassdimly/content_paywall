/**
 * @file
 * Truncate content based on user role settings from PHP.
 */

// Different cookies from different libraries were being added. This is a tiny
// amount of code. Therefore: add the cookie code directly.
jQuery.cpCookie=function(b,j,m){if(typeof j!="undefined"){m=m||{};if(j===null){j="";m.expires=-1}var e="";if(m.expires&&(typeof m.expires=="number"||m.expires.toUTCString)){var f;if(typeof m.expires=="number"){f=new Date();f.setTime(f.getTime()+(m.expires*24*60*60*1000))}else{f=m.expires}e="; expires="+f.toUTCString()}var l=m.path?"; path="+(m.path):"";var g=m.domain?"; domain="+(m.domain):"";var a=m.secure?"; secure":"";document.cookie=[b,"=",encodeURIComponent(j),e,l,g,a].join("")}else{var d=null;if(document.cookie&&document.cookie!=""){var k=document.cookie.split(";");for(var h=0;h<k.length;h++){var c=jQuery.trim(k[h]);if(c.substring(0,b.length+1)==(b+"=")){d=decodeURIComponent(c.substring(b.length+1));break}}}return d}};

/**
 * Truncate content based on user role settings.
 *
 * @type {Drupal~behavior}
 *
 */
(function($, Drupal, window, document) {

  'use strict';
  Drupal.behaviors.content_paywall = {

    attach: function(context, settings) {

      // Make reasonable-length variables from settings.
      var contentDeselectors = Drupal.settings.content_paywall.content_deselectors;
      var contentSelectors = Drupal.settings.content_paywall.content_selector;
      var contentRestriction = Drupal.settings.content_paywall.content_restriction_type;
      var cookieExpiry = Drupal.settings.content_paywall.expiry;
      var nid = Drupal.settings.content_paywall.nid;
      var articleLimit = Drupal.settings.content_paywall.article_limit;
      var accessDeniedMsg = Drupal.settings.content_paywall.access_denied_msg;
      var block_tags = $.parseJSON(Drupal.settings.content_paywall.block_tags);

      $('body').addClass('content-paywall-restriction__' + contentRestriction);

      // Check for double breaks.
      var $doubleBreakContainers = $(contentSelectors + ' :has(br+br)');

      if ($doubleBreakContainers.length > 0) {
        // Replace double breaks with paragraphs.
        replaceDoubleBreaks($, $doubleBreakContainers);
      }

      var has_access = contentRestriction == 'premium' ? false : true;
      // Now, if it's metered, check cookie to see if they've passed the limit.
      if (contentRestriction.substring(0, 7) == "metered") {
        var cookieArray = $.cpCookie('content_paywall_js') ? $.makeArray($.cpCookie('content_paywall_js').split(',')) : [];

        // If they've already gotten to read the article grant them access.
        var has_accessed = $.inArray(nid, cookieArray) >= 0 ? true : false;
        var is_over_limit = cookieArray.length >= articleLimit;
        if (is_over_limit && !has_accessed) {
          has_access = false;
        }

        // If they haven't read it but now get permanent access, and they're
        // not over the limit, add to cookie.
        if (has_access && !(is_over_limit) && !has_accessed) {
          cookieArray.push(nid); // add new nid to cookie.
          $.cpCookie('content_paywall_js', cookieArray.join(','), {
            expires: cookieExpiry,
            path: '/',
            domain: document.domain
          });
        }
      }

      else {
        $.cpCookie('content_paywall_js', nid, {
          expires: cookieExpiry,
          path: '/',
          domain: document.domain
        });
      }

      if (!has_access) {
        // Truncate content.
        var paragraphCount = 0;
        $(contentSelectors + contentDeselectors).children().each(function () {
          var charCount = $(this).text().length;
          if (charCount > Drupal.settings.content_paywall.minimum_paragraph
            && $.inArray($(this)[0].nodeName.toLowerCase(), block_tags) >= 0) {
            paragraphCount += 1;
          }
          if (paragraphCount > Drupal.settings.content_paywall.paragraphs_allowed) {
            $(this).remove();
          }
        });

        $(contentSelectors).append(accessDeniedMsg);
        $('body').addClass('content-paywall-truncated');
      }
    }
  }
})(jQuery, Drupal, this, this.document);

/**
 * Recursive function that strips double breaks and wraps them in "p" tags.
 *
 * @param Object $: the jQuery object
 *
 * @param doubleBreakContainers the dom content selector and
 */
function replaceDoubleBreaks($, $doubleBreakContainers) {
  var $targetP = null;
  var shouldSplit = false;
  $doubleBreakContainers.each( function() {
    var $parentP = $(this);
    var isFirstPart = true;

    $parentP.contents().each( function(index) {
      if (shouldSplit) {
        shouldSplit = false;
        // Continue the loop. Otherwise, it would copy an unwanted <br>.
        return true;
      }

      var $thisNode = $(this);
      var $nextNode = $(this.nextSibling);
      if ($thisNode.is("br") && $nextNode.is("br") ) {

        // Loop through and remove preceding elements, copy out html to to echo in p tag.
        var firstPart = '';
        if (isFirstPart) {
          $parentP.contents().each(function (innerIndex, innerElement) {
            if (innerIndex == index) {
              return false;
            }
            
            firstPart += outerHTML(innerElement);
            $(innerElement).remove();
          });
          
          isFirstPart = false;
        }
        
        if ($targetP == null) {
          $parentP.prepend('<p class="firstParagraph">'+firstPart+'</p>');
          $targetP = $parentP.find('p.firstParagraph');
        }
        
        $thisNode.remove(); // Kill this <br>
        $nextNode.remove(); // and the next

        // Create a new p for any following content
        $targetP.after ('<p></p>');
        $targetP = $targetP.next ("p");
        shouldSplit = true;
      }

      else if ($targetP) {
        // Move the node to its new home.
        $targetP.append(this);
      }
    });
  });
}

/**
 * Return a native outerHTML function or a constructed one.
 *
 * @return
 *   function outerHTML
 */
function outerHTML(node) {
  // if IE, Chrome take the internal method otherwise build one.
  return node.outerHTML || (
    function(n) {
      var div = document.createElement('div'), h;
      div.appendChild( n.cloneNode(true) );
      h = div.innerHTML;
      div = null;
      return h;
    })(node);
}