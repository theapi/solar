<?php
/**
 * Listen for the broadcast UDP for the binary payloads and send it to Elasticsearch.
 *
 * @see /src/GardenPayload.h
 * @see /src/SolarPayload.h
 */

use Elasticsearch\ClientBuilder;
use Symfony\Component\Filesystem\Filesystem;

require_once dirname(__DIR__) . '/vendor/autoload.php';

define('GARDEN', 50);
define('SOLAR', 55);

if (php_sapi_name() != 'cli') {
    throw new Exception('This application must be run on the command line.');
}

if (!($socket = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP))) {
    $errorcode = socket_last_error();
    $errormsg = socket_strerror($errorcode);

    die("Couldn't create socket: [$errorcode] $errormsg \n");
}

$binded = socket_bind($socket, '0.0.0.0', 12345);
$rval = socket_set_option($socket, getprotobyname("ip"), MCAST_JOIN_GROUP,
    array("group" => "239.0.0.57", "interface" => 0));
socket_set_option($socket, SOL_SOCKET, SO_REUSEADDR, 1);

// Write the data to the filesytem too.
$fileSystem = new Filesystem();

// Build the elasticsearch client.
$client = ClientBuilder::create()
  ->setHosts(['192.168.0.2:9200']) // @todo configurable hosts.
  ->build();

// Only handle each message once, so start with fake one.
$current_garden_msg_id = 999;
$current_solar_msg_id = 999;

while (1) {
    try {
        $from = '';
        $port = 0;
        $num = socket_recvfrom($socket, $buf, 53, MSG_WAITALL, $from, $port);
        //echo "Received $num bytes from $from, port $port" . PHP_EOL;
        //echo "UDP packet contents: $buf" . PHP_EOL;

        if ($buf[0] == "\t") {
            // A payload.
            $binarydata = trim($buf);
            if (strlen($binarydata) > 1) {
                $data = unpack("Cmessage_type", $binarydata);
                switch ($data['message_type']) {
                    case GARDEN:
                        $current_garden_msg_id = processGardenPayload(
                            $client,
                            $current_garden_msg_id,
                            $binarydata,
                            $fileSystem
                        );
                    break;

                    case SOLAR:
                        $current_solar_msg_id = processSolarPayload(
                            $client,
                            $current_solar_msg_id,
                            $binarydata,
                            $fileSystem
                        );
                    break;
                }
            }
        }
    } catch (\Exception $e) {
        // Keep calm & carry on.
    }

}

function processGardenPayload($client, $current_garden_msg_id, $binarydata, $fileSystem) {
  $doc = unpack(
    "Cmsg_type/Cmsg_id/nvcc/nmv/nma/nlight/ncpu_temperature/ntemperature/nrssi/nsnr/nfrq_error",
    $binarydata
  );
  foreach (['ma', 'cpu_temperature', 'temperature', 'rssi', 'snr', 'frq_error'] as $k) {
    // There is no option "signed short (always 16 bit, big endian byte order)"
    // Unpacked in the unsigned form,
    // and then if the result is >= 2^15, subtract 2^16 from it.
    $v = $doc[$k];
    if ($v >= pow(2, 15)) $v -= pow(2, 16);
    $doc[$k] = $v;
  }

  if ($doc['msg_id'] != $current_garden_msg_id) {
    $current_garden_msg_id = $doc['msg_id'];

    // Prepare the doc.
    // Convert the temperature to the float value.
    $doc['temperature'] = $doc['temperature'] / 10;
    // Record a timestamp as milliseconds since epoch (epoch_millis).
    $doc['timestamp'] = date("U") * 1000;
    print_r($doc);

    $file = __DIR__ . '/csv/garden_payload/' . date('Y') . '/' . date('m') . '.csv';
    $fileSystem->appendToFile($file, join(',', $doc) . PHP_EOL);

    $params = [
      'index' => 'garden_payload', // @todo configurable index name.
      'type' => '_doc',
      'body' => $doc,
    ];
    $response = $client->index($params);
  }

  return $current_garden_msg_id;
}

function processSolarPayload($client, $current_solar_msg_id, $binarydata, $fileSystem) {
    $doc = unpack(
        "Cmsg_type/Cdevice_id/Cmsg_id/Cflags/nvcc/nmv/nma/nlight/ncpu_temperature/ntemperature/nrssi/nsnr/nfrq_error",
        $binarydata
    );
    foreach (['ma', 'cpu_temperature', 'temperature', 'rssi', 'snr', 'frq_error'] as $k) {
        // There is no option "signed short (always 16 bit, big endian byte order)"
        // Unpacked in the unsigned form,
        // and then if the result is >= 2^15, subtract 2^16 from it.
        $v = $doc[$k];
        if ($v >= pow(2, 15)) $v -= pow(2, 16);
        $doc[$k] = $v;
    }

    if ($doc['msg_id'] != $current_solar_msg_id) {
      $current_solar_msg_id = $doc['msg_id'];

      // Convert the temperature to the float value.
      $doc['temperature'] = $doc['temperature'] / 10;
      // Record a timestamp as milliseconds since epoch (epoch_millis).
      $doc['timestamp'] = date("U") * 1000;
      print_r($doc);

      $file = __DIR__ . '/csv/solar_payload/' . date('Y') . '/' . date('m') . '.csv';
      $fileSystem->appendToFile($file, join(',', $doc) . PHP_EOL);

      $params = [
        'index' => 'solar_payload', // @todo configurable index name.
        'type' => '_doc',
        'body' => $doc,
      ];
      $response = $client->index($params);
      //print_r($response);
    }

    return $current_solar_msg_id;
}