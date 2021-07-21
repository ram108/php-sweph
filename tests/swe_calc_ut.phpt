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
var_dump(swe_calc_ut(2452275.499255786, 0, SEFLG_SWIEPH|SEFLG_SPEED));
?>
--EXPECT--
array(9) {
  [0]=>
  float(280.38296810621)
  [1]=>
  float(0.00014968070565525)
  [2]=>
  float(0.98329783914845)
  [3]=>
  float(1.0188772348975)
  [4]=>
  float(1.7232637573749E-5)
  [5]=>
  float(-1.0220875853441E-5)
  ["serr"]=>
  string(0) ""
  ["rc"]=>
  int(258)
  ["tjd_ut"]=>
  float(2452275.4992558)
}
