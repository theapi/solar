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
    $data = parse_line($chars);

    $str = date('d-m-Y h:i:s') . ',' . $data['mv'] . ',' . $data['mid'] . "\n";
    $filename = $dir . '/' . date('d-m-Y') . '.csv';
    file_put_contents($filename, $str, FILE_APPEND);

    $chars = '';
  } else {
    $chars .= $c;
  }

}

/**
 * Parse the line from the transmitter
 * "%lu,wd=%d,mv=%u"
 *
 * @param string $line
 *
 * @return array
 */
function parse_line($line)
{
  $parts = explode(',', $line);

  return array(
    'mid' => $parts[0],
    'mv' => $parts[2],
  );
}

