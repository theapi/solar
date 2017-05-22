<?php
/**
 * Provide a csv to dygraph with the column headers added.
 */

$dir = "/mnt/storage/share/solar/datalog";

$filenames = array();
foreach (glob("$dir/*.csv") as $filename) {
    $filenames[] = $filename;
}
rsort($filenames);
$filename = current($filenames);

echo "Time,MsgId,TempInt,TempExt,Soil,Solar,Battery\n";
$handle = fopen($filename, "r");
if ($handle) {
    while (($buffer = fgets($handle, 4096)) !== false) {
        echo $buffer;
    }
    fclose($handle);
}
