#!/usr/bin/php -q
<?php
require 'Client.php';

use Theapi\Solar\Monitor\Client;


error_reporting(E_ALL);

/* Allow the script to hang around waiting for connections. */
set_time_limit(0);

/* Turn on implicit output flushing so we see what we're getting
 * as it comes in. */
ob_implicit_flush();

if (empty($argv[1])) {
  exit("Ip address needed: socket_client.php 192.168.0.145 23");
} else {
  $address = $argv[1];
}
if (empty($argv[2])) {
  exit("Port number needed: socket_client.php 192.168.0.145 23");
} else {
  $port = $argv[2];
}

$client = new Client();
$fp = $client->start($address, $port);
$client->read();

// send microtime when its our turn
$ignored = true;
while(1) {
    $line = $client->read();
    if ($line === "") {
       continue;
    } else {
        echo $line;
    }
    usleep(200000);
}
