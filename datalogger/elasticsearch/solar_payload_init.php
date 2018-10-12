<?php
/**
 * Create & set mapping for the Elasticsearch index.
 */

use Elasticsearch\ClientBuilder;

require_once dirname(__DIR__) . '/vendor/autoload.php';

// Build the elasticsearch client.
$client = ClientBuilder::create()
  ->setHosts(['192.168.0.2:9200']) // @todo configurable hosts.
  ->build();

$params = [
  'index' => 'solar_payload', // @todo configurable index name.
  'body' => [
    'mappings' => [
      '_doc' => [
        'properties' => [
          'cpu_temperature' => [
            'type' => 'float',
          ],
          'device_id' => [
            'type' => 'short',
          ],
          'light' => [
            'type' => 'integer',
          ],
          'ma' => [
            'type' => 'short',
          ],
          'msg_id' => [
            'type' => 'short',
          ],
          'msg_type' => [
            'type' => 'short',
          ],
          'mv' => [
            'type' => 'short',
          ],
          'temperature' => [
            'type' => 'float',
          ],
          'timestamp' => [
            'type' => 'date'
          ],
          'vcc' => [
            'type' => 'short',
          ],
        ]
      ]
    ]
  ]
];

// Create the index with mappings and settings now
$response = $client->indices()->create($params);
print_r($params);
