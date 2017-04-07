<?php
require __DIR__ . '/vendor/autoload.php';

/**
 * PSR-4 autoloader (for now)
 */
spl_autoload_register(function ($class) {
  if (file_exists((__DIR__ . '/src/' . str_replace('\\', '/', $class) . '.php'))) {
    include(__DIR__ . '/src/' . str_replace('\\', '/', $class) . '.php');
  }
});



$vcc = new Sensor();
$vcc->setId(1);
$vcc->setValue(5495);
$data = $vcc->encode();

$to = new Sensor();
$to->decode($data);
print $to->getId() . ' : ' . $to->getValue() . "\n";

//$garden = new GardenSensors();
//$repeated = new \Google\Protobuf\Internal\RepeatedField(Google\Protobuf\Internal\GPBType::MESSAGE, Sensor::class);
//$repeated[] = $vcc;
//$garden->setSensors($repeated);
//$data = $garden->encode();
//
//$receiver = new GardenSensors();
//$receiver->decode($data);
//$sensors = $garden->getSensors();
//foreach ($sensors as $sensor) {
//  print $to->getId() . ' : ' . $to->getValue() . "\n";
//}

// @link https://github.com/google/protobuf/issues/2686
$garden = new GardenSensors();
$repeated = $garden->getSensors();
$repeated[] = $vcc;
$data = $garden->encode();

$receiver = new GardenSensors();
$receiver->decode($data);
$sensors = $garden->getSensors();
foreach ($sensors as $sensor) {
  print $to->getId() . ' : ' . $to->getValue() . "\n";
}
