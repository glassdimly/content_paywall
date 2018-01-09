(function($, Drupal, window, document) {
  'use strict';
  Drupal.behaviors.content_paywall_debug = {
    attach: function(context, settings) {
      console.log(Drupal.settings.content_paywall_debug);
      var subscriber_access_check = Drupal.settings.content_paywall_debug.subscriber_access_check ? 1 : 0;
      var username = Drupal.settings.content_paywall_debug.username;
      var session = Drupal.settings.content_paywall_debug.session;
      var roles = Drupal.settings.content_paywall_debug.roles;
      var debug_classes = 'subscriber_access_check-' + subscriber_access_check + ' username-' + username + ' session-' + session + ' roles-' + roles;
      $('body').addClass(debug_classes);
    }
  };
})(jQuery, Drupal, this, this.document);