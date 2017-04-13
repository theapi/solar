#!/usr/bin/php -q
<?php
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

echo "TCP/IP Connection\n";


/* Create a TCP/IP socket. */
$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
if ($socket === false) {
  echo "socket_create() failed: reason: " . socket_strerror(socket_last_error()) . "\n";
} else {
  echo "OK.\n";
}

echo "Attempting to connect to '$address' on port '$port'...";
$result = socket_connect($socket, $address, $port);
if ($result === false) {
  echo "socket_connect() failed.\nReason: ($result) " . socket_strerror(socket_last_error($socket)) . "\n";
} else {
  echo "OK.\n";
}


while (1) {
  $data = socket_read($socket, 32);
  if ($data === false) {
    // Lost connection.
    echo socket_strerror(socket_last_error($socket));
    break;
  }


  // @todo parse the payload.
  echo $data;

}

echo "\nClosing socket...";
socket_close($socket);
echo "OK.\n\n";
