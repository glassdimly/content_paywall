Content Paywall truncates node content based on a user-configurable user role. You can set an access period, like 60 days, and grant access to a limited number of full nodes per month. After the limit is reach, the content is truncated after x paragraphs and a configurable paywall message is set. This module is compatible with SEO and site crawling because it uses Javascript to truncate content so bots (who usually crawl with no Javascript) can still see content. The trade-off is that savvy users could disable Javascript and see content.

Installation:
* Install this module. I use drush: `drush en content_paywall`
* Configure options at: /admin/config/content_paywall
* Configure permissions at /admin/people/permissions
* Grant the "Administer Paywall" to trusted admins only.
* Grant "Access Premium Content" to the subscriber role.
* Use the form at /admin/config/content_paywall labelled 'Node Bundles to Which Fields Should Be Added' to add the controlling field to your node bundles.
* You probably want to hide the field under your bundle's "Display Options" so it's not displayed. (@TODO automate this)

These options will be added on a field called 'field_premium_options':
* Premium
* Metered (permanently)
* Metered (becomes premium)
* Free

This is how these options work:
* Content that is "Premium" is available only to the subscriber role.
* A user may view only x number of pieces of "Metered (permanently)" content is within y time period (configurable).
* Content that is "Metered (becomes premium)" starts as metered but after z time, becomes "Premium".
* Content that is "Premium" is unrestricted. This is the default value

Any of these options can be deleted from a field instance to remove that particular functionality from that particular node bundle.

Premium content or metered content where the number of views has been reached gets truncated by javascript, and a configurable paywall message appears.

This allows the content to be crawled by non-javascript spiders like Google so it's still indexed.

This module can be used with authcache or varnish. The content is truncated by client-side javascript and usage is tracked by a javascript cookie so content is the same for all users.

Note: standard drupal cron job must be configured so that "Metered (becomes premium)" is moved to "Premium". This is generally already the case on most sites, not special jobs must be configured.

Part of the key to this module is regularizing your content markup using the drupal filter system so content truncation happens predictably.