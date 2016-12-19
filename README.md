Content Paywall truncates node content based on a user-configurable user role. You can set an access period, like 60 days, and grant access to a limited number of full nodes per month. After the limit is reach, the content is truncated after x paragraphs and a configurable paywall message is set. This module is compatible with SEO and site crawling because it uses Javascript to truncate content so bots (who usually crawl no Javscript) can still see content. The trade-off is that savvy users could disable Javascript and see content.


Installation:
Install this module. I use drush: `drush en content_paywall`
Configure options at: /admin/config/content_paywall
Configure permissions at /admin/people/permissions
Grant the "Administer Paywall" to trusted admins only.
Grant "Access Premium Content" to the subscriber role.
Set node fields as described below

Node fields:

Create a field on any content you want truncated whose machine name is `field_premium_options` (@TODO set up this field automatically).

Allowed values list should include at least some of the following options:

* premium|Premium
* metered_permanently|Metered (permanently)
* metered_timed|Metered (becomes premium)
* free|Free

Content that is `premium` is available only to the subscriber role. 
A user may view only x number of pieces of `metered` content is within y time period (configurable).
Content that is `metered_timed` starts as metered but after z time, becomes `premium`.
Content that is `free` is unrestricted.
 
Premium content or metered content where the number of views has been reached gets truncated by javascript, and a configurable paywall message appears.

This allows the content to be crawled by non-javascript spiders like Google so it's still indexed.

This module can be used with authcache or varnish. The content is truncated by client-side javascript and usage is tracked by a javascript cookie so content is the same for all users.

Note: standard drupal cron job must be configured so that `metered_timed` is moved to `premium`. This is generally already the case on most sites, not special jobs must be configured.