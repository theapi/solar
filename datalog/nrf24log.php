<?php
require 'Nrf24Client.php';

use Theapi\Nrf24\Client;


if (empty($argv[1])) {
  echo "Nrf24 server ip?\n";
  exit(1);
} else {
  $server = $argv[1];
}

if (empty($argv[2])) {
  echo "Nrf24 port ip?\n";
  exit(1);
} else {
  $port = $argv[2];
}

if (empty($argv[3])) {
  echo "Dir to log to?\n";
  exit(1);
} else {
  $dir = $argv[3];
}


$client = new Client();
$fp = $client->start($server, $port);

while(!feof($fp)) {
  $line = $client->read();
  // Starts with ':' and is the 'E' nrf24 radio device.
  if (!empty($line) && $line[0] == ':' && $line[2] == 'E') {
    echo "> $line";

    // :,E,S,158,5384,6,8,807,5552
    $parsed = explode(',', trim($line));
    // "Time,MsgId,TempInt,TempExt,Soil,Solar,Battery\n";
    $csv = array(
      date('Y-m-d H:i:s'),
      $parsed[3],
      $parsed[5],
      $parsed[6],
      $parsed[7],
      $parsed[8],
      $parsed[4] . "\n",
    );
    $filename = $dir . '/' . date('Y-m-d') . '.csv';
    file_put_contents($filename, join(',', $csv), FILE_APPEND);
  }
}
