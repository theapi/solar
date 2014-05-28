<?php

if (empty($argv[1])) {
    echo "Dir to log to?\n";
    exit(1);
} else {
    $dir = $argv[1];
}

if (empty($argv[2])) {
    $tty = '/dev/ttyUSB0';
} else {
    $tty = $argv[2];
}

if (empty($argv[3])) {
    $baud = '9600';
} else {
    $baud = $argv[3];
}

/*
cs8 = 8 bit characters
-cstopb = one stop bit
raw = no messing with the data, like adding newlines
57600 = BAUD
*/
exec("stty -F $tty cs8 -cstopb raw $baud");
$serial = fopen($tty, "w+");
if( !$serial) {
    echo "Error opening serial\n";
    exit(1);
}

$chars = '';
while (1) {
  $c = fread($serial, 1);

  if ($c == "\n") {
    // new line
    $line = str_replace('R: ', '', $chars);
    $str = date('Y-m-d H:i:s') . ',' . $line . "\n";
    $filename = $dir . '/' . date('Y-m-d') . '.csv';
    file_put_contents($filename, $str, FILE_APPEND);

    $chars = '';
  } else {
    $chars .= $c;
  }

}
