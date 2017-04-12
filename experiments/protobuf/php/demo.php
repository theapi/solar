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

$charge = new Sensor();
$charge->setId(2);
$charge->setValue(6453);

$temperature = new Sensor();
$temperature->setId(3);
$temperature->setValue(23);

$light = new Sensor();
$light->setId(4);
$light->setValue(123456);

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
//  print $sensor->getId() . ' : ' . $sensor->getValue() . "\n";
//}

// @link https://github.com/google/protobuf/issues/2686
$garden = new GardenSensors();
$repeated = $garden->getSensors();
$repeated[] = $vcc;
$repeated[] = $charge;
$repeated[] = $temperature;
$repeated[] = $light;
$data = $garden->encode();
var_dump($data);

$receiver = new GardenSensors();
$receiver->decode($data);
$sensors = $garden->getSensors();
foreach ($sensors as $sensor) {
  print $sensor->getId() . ' : ' . $sensor->getValue() . "\n";
}
