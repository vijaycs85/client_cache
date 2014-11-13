Client Cache
============

Client cache is a Drupal caching backend implementation to use user agent storage cache bin(s).
It does not provide any user interface. Check installation section below for usage.

Status
------
<a href="https://travis-ci.org/vijaycs85/client_cache" target="_blank"><img src="https://travis-ci.org/vijaycs85/client_cache.svg?branch=7.x-1.x" /></a>
<a href="https://insight.sensiolabs.com/projects/9986732d-3327-444f-900a-d752f76d23b4" target="_blank"><img src="https://insight.sensiolabs.com/projects/9986732d-3327-444f-900a-d752f76d23b4/mini.png" /></a>



Installation
-----------

 1. Install the client_cache module

 2. Edit settings.php to make one of the client cache
    plugin (cookie, HTML 5 storage etc..) as cache class for cache_client bin,
    for example:
    <pre><code>
      $conf['cache_backends'][] = 'sites/all/modules/client_cache/client_cache.cookie.inc
      $conf['cache_class_client_cache'] = 'ClientCacheCookieDrupal';
    </code></pre>
