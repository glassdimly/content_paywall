(function($, Drupal, window, document) {
  'use strict';
  Drupal.behaviors.content_paywall_debug = {
    attach: function(context, settings) {
      var getCookie = function(name) {
        var re = new RegExp(name + "=([^;]+)");
        var value = re.exec(document.cookie);
        return (value != null) ? unescape(value[1]) : null;
      };
      console.log(Drupal.settings.content_paywall_debug);
      console.log('Nids accessed (cookie):', getCookie('content_paywall_js'));
      var subscriber_access_check = Drupal.settings.content_paywall_debug.subscriber_access_check ? 1 : 0;
      var username = Drupal.settings.content_paywall_debug.username;
      var session = Drupal.settings.content_paywall_debug.session;
      var roles = Drupal.settings.content_paywall_debug.roles;
      var debug_classes = 'subscriber_access_check-' + subscriber_access_check + ' username-' + username + ' session-' + session + ' roles-' + roles;
      $('body').addClass(debug_classes);
    }
  };
})(jQuery, Drupal, this, this.document);