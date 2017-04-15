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

while(1) {
    $line = $client->read();
    if ($line === "") {
       continue;
    } else {
        //echo $line;
        if ($line[0] == "\t") {
            // A payload.
            $binarydata = trim($line);
            if (strlen($binarydata) > 1) {
                $array = unpack("Cdevice_id/Cmsg_id/n*", $binarydata);
                foreach ($array as $k => $v) {
                    // There is no option "signed short (always 16 bit, big endian byte order)"
                    if (is_int($k)) {
                        // Unpacked in the unsigned form,
                        // and then if the result is >= 2^15, subtract 2^16 from it.
                        if($v >= pow(2, 15)) $v -= pow(2, 16);
                        $array[$k] = $v;
                    }
                    //$a = $a - 32766;
                }
                print_r($array);
            }
        }
    }
    usleep(200000);
}
