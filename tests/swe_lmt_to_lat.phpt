--TEST--
Basic test
--SKIPIF--
<?php
if (!extension_loaded('swephp')) {
    echo 'skip';
}
?>
--FILE--
<?php
swe_set_ephe_path('./sweph/ephe');
var_dump(swe_lmt_to_lat(2452275.5, 121.34));
?>
--EXPECT--
array(3) {
  ["rc"]=>
  int(0)
  ["tjd_lat"]=>
  float(2452275.4978254)
  ["serr"]=>
  string(0) ""
}
