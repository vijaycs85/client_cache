<?php

/**
 * @file
 * Contains implementation of DrupalCacheInterface with cookie logic included.
 */

class CookieCacheDrupal implements DrupalCacheInterface {

  /**
   * {@inherits}
   */
  function __construct($bin) {
    $this->bin = $bin;
  }

  /**
   * {@inherits}
   */
  function get($cid) {
    $cids = array($cid);
    $cache = $this->getMultiple($cids);
    return reset($cache);
  }

  /**
   * {@inherits}
   */
  function getMultiple(&$cids) {
    try {
      $cache = array();
      foreach ($cids as $cid) {
        $item = new stdClass();
        if (isset($_COOKIE[$cid]) && !empty($_COOKIE[$cid])) {
          // We use JSON as interchance format.
          // This is due to the fact that sometimes we want to access data saved in cookies from JS.
          $item->data = drupal_json_decode($_COOKIE[$cid]);
        }
        else {
          $item->data = NULL;
        }
        $cache[] = $this->prepareItem($item);
      }

      return $cache;
    }
    catch (Exception $e) {
      // If the database is never going to be available, cache requests should
      // return FALSE in order to allow exception handling to occur.
      return array();
    }
  }

  /**
   * {@inherits}
   */
  function set($cid_raw, $data, $expire = CACHE_PERMANENT) {
    list($cid, $path) = self::getPathFromCid($cid_raw);
    $expire = $this->getExpirePeriod($expire);
    try {
      // We use JSON as interchance format.
      // This is due to the fact that sometimes we want to access data saved in cookies from JS.
      $data = drupal_json_encode($data);
      setcookie($cid, $data, $expire, $path);
    }
    catch (Exception $e) {
      watchdog('client_cache.cookie', 'Serializing of @data failed', array('@data' => print_r($data, TRUE)));
    }
  }

  /**
   * {@inherits}
   */
  function clear($cid_raw = NULL, $wildcard = FALSE) {
    list($cid, $path) = self::getPathFromCid($cid_raw);
    if (self::get($cid)) {
      self::set($cid_raw, NULL, -1000);
    }
  }
  /**
   * {@inherits}
   */
  function isEmpty() {
  }

  /**
   * Prepare a cached item.
   *
   * Checks that items are either permanent or did not expire, and decodes
   * data as appropriate.
   *
   * @param $cache
   *   An item loaded from cache_get() or cache_get_multiple().
   * @return
   *   The item with data decoded as appropriate or FALSE if there is no
   *   valid item to load.
   */
  protected function prepareItem($cache) {
    return $cache;
  }

  /**
   * Provides cache period.
   *
   * @param int $expire
   *  Integer of cache type or seconds to cache.
   *
   * @return int
   *  Integer of time to cache.
   */
  protected function getExpirePeriod($expire) {
    switch ($expire) {
      case CACHE_PERMANENT:
        // if its permanent, cache it for long time.
        $expire = time()+(60*60*24*500);
        break;
      case CACHE_TEMPORARY:
        // set to remove cache on this session end.
        $expire = 0;
        break;
      default:
        // if integer, consider it as seconds.
        if (is_int($expire)) {
          $expire = time() + $expire;
        }
        else {
          $expire = 0;
        }
        break;
    }
    return $expire;
  }

  /**
   * Provides path from cache id.
   *
   * @param string $cid
   *   A string cache ID.
   *
   * @return array
   *   An array of cache id and path.
   */
  protected function getPathFromCid($cid) {
    $path = '/';
    if (is_array($cid) && isset($cid['path'])) {
      $path = $cid['path'];
      $cid = $cid['cid'];
    }
    return array($cid, $path);
  }

}
