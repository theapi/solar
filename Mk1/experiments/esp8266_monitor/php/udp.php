<?php

/*
    Broadcast UDP socket client
*/


if(!($socket = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP)))
{
    $errorcode = socket_last_error();
    $errormsg = socket_strerror($errorcode);

    die("Couldn't create socket: [$errorcode] $errormsg \n");
}

$binded = socket_bind($socket, '0.0.0.0', 12345);
$rval = socket_set_option($socket, getprotobyname("ip"), MCAST_JOIN_GROUP,
array("group"=>"239.0.0.57","interface"=>0));


while(1)
{
    $from = '';
    $port = 0;
    $num = socket_recvfrom($socket, $buf, 53, MSG_WAITALL, $from, $port);
    echo "Received $num bytes from $from, port $port" . PHP_EOL;
    echo "UDP packet contents: $buf" . PHP_EOL;
}
