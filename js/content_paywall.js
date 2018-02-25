(function($, Drupal, window, document) {
  'use strict';
  Drupal.behaviors.content_paywall = {
    attach: function(context, settings) {
      
      var contentDeselectors = Drupal.settings.content_paywall.content_deselectors;
      var contentSelectors = Drupal.settings.content_paywall.content_selector;
      var contentRestriction = Drupal.settings.content_paywall.content_restriction_type;
      var cookieExpiry = Drupal.settings.content_paywall.expiry;
      var nid = Drupal.settings.content_paywall.nid;
      var articleLimit = Drupal.settings.content_paywall.article_limit;
      var accessDeniedMsg = Drupal.settings.content_paywall.access_denied_msg;
      var block_tags = $.parseJSON(Drupal.settings.content_paywall.block_tags);

      $('body').addClass('content-paywall-restriction__' + contentRestriction);

      //check for double breaks
      var doubleBreakContainers = $(contentSelectors + ' :has(br+br)');

      if (doubleBreakContainers.length > 0) {
        //replace double breaks with paragraphs.
        stripBeakTags($, doubleBreakContainers);
      }

      var has_access = contentRestriction == 'premium' ? false : true;
      // Now, if it's metered, check, cookie, see if they've passed the limit.
      if (contentRestriction.substring(0, 7) == "metered") {
        var cookieArray = $.cookie('content_paywall_js') ? $.makeArray($.cookie('content_paywall_js').split(',')) : [];
        if (cookieArray.length) {
          // If they've already gotten to read the article grant them access
          if (cookieArray.length >= articleLimit && $.inArray(nid, cookieArray) == -1) {
            has_access = false;
          }
          // If the nid is not in the array and they're not over the limit.
          else if ($.inArray(nid, cookieArray) == -1 && !(cookieArray.length > articleLimit)) {
            cookieArray.push(nid); // add new nid to cookie.
            $.cookie('content_paywall_js', cookieArray.join(','), {
              expires: cookieExpiry,
              path: '/',
              domain: document.domain
            });
          }
        }
        else {
          $.cookie('content_paywall_js', nid, {
            expires: cookieExpiry,
            path: '/',
            domain: document.domain
          });
        }
        $('body').addClass('paywall-accessed-nids--' + cookieArray.join('-'));
      }
      if (!has_access) {
        var paragraphCount = 0;
        $(contentSelectors + contentDeselectors).children().each(function () {
          var charCount = $(this).text().length;
          if (charCount > Drupal.settings.content_paywall.minimum_paragraph
            && $.inArray($(this).attr('tagName').toLowerCase(), block_tags) >= 0) {
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


//strips double breaks and wraps them in p tags
function stripBeakTags($, doubleBreakContainers) {
  var targetP = null;
  var justSplit = false;

  doubleBreakContainers.each( function() {
    var parentP = $(this);

    var isFirstPart = true;
    parentP.contents().each( function(index) {
      if (justSplit) {
        justSplit = false;
        // Continue the loop. Otherwise, it would copy an unwanted <br>.
        return true;
      }

      var thisjNode = $(this);
      var nextjNode = $(this.nextSibling);

      if (thisjNode.is("br") && nextjNode.is("br") ) {
        //loop through and remove preceding elements, copy out html to to echo in p tag
        var firstPart = '';
        if (isFirstPart) {
          parentP.contents().each(function (innerIndex, innerElement) {
            //console.log(innerIndex, 'innerindex');
            //console.log(outerHTML(innerElement), 'outerHTML(innerElement)');

            if (innerIndex == index) {
              return false;
            }
            firstPart += outerHTML(innerElement);
            $(innerElement).remove();
          });
          isFirstPart = false;
        }
        if (targetP == null) {
          //console.log(firstPart, 'firstPart');
          parentP.prepend('<p class="firstParagraph">'+firstPart+'</p>');
          targetP = parentP.find('p.firstParagraph');
        }
        thisjNode.remove(); //-- Kill this <br>
        nextjNode.remove(); //-- and the next

        //-- Create a new p for any following content

        targetP.after ('<p></p>');
        targetP = targetP.next ("p");

        justSplit = true;
      }
      else if (targetP) {
        //-- Move the node to its new home.
        targetP.append(this);
      }
    });
  } );
}

function outerHTML(node) {
  // if IE, Chrome take the internal method otherwise build one
  return node.outerHTML || (
    function(n) {
      var div = document.createElement('div'), h;
      div.appendChild( n.cloneNode(true) );
      h = div.innerHTML;
      div = null;
      return h;
    })(node);
}