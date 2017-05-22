<?php
/**
 * Listen for the broadcast UDP for the binary GardenPayload and send it to Google Sheets.
 *
 * @see /src/GardenPayload.h
 */

require_once __DIR__ . '/vendor/autoload.php';
require_once 'config.php';

define('APPLICATION_NAME', 'Google Sheets Solar Data');
define('CREDENTIALS_PATH', '~/.credentials/sheets.googleapis.com-php-solar.json');
define('CLIENT_SECRET_PATH', '~/.credentials/solar_client_secret.json');
// If modifying these scopes, delete the previously saved credentials
// at ~/.credentials/sheets.googleapis.com-php-solar.json
define('SCOPES', implode(' ', array(
        Google_Service_Sheets::SPREADSHEETS)
));

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

// Get the Google API client and construct the service object.
$google_client = getGoogleClient();
$google_service = new Google_Service_Sheets($google_client);

$msg_id = 500;

while (1) {
    $from = '';
    $port = 0;
    $num = socket_recvfrom($socket, $buf, 53, MSG_WAITALL, $from, $port);
    echo "Received $num bytes from $from, port $port" . PHP_EOL;
    //echo "UDP packet contents: $buf" . PHP_EOL;

    if ($buf[0] == "\t") {
        // A payload.
        $binarydata = trim($buf);
        if (strlen($binarydata) > 1) {
            $array = unpack("Cdevice_id/Cmsg_id/n*", $binarydata);
            foreach ($array as $k => $v) {
                // There is no option "signed short (always 16 bit, big endian byte order)"
                if (is_int($k)) {
                    // Unpacked in the unsigned form,
                    // and then if the result is >= 2^15, subtract 2^16 from it.
                    if ($v >= pow(2, 15)) $v -= pow(2, 16);
                    $array[$k] = $v;
                }
            }
            print_r($array);

            if ($array['device_id'] == 50 && $array['msg_id'] != $msg_id) {
                unset($array['device_id']);
                // Add data to the Google spreadsheet.
                $range = 'garden!A1:G';
                $requestBody = new Google_Service_Sheets_ValueRange();
                $requestBody->setValues(array(array_values($array)));
                $optParams['insertDataOption'] = 'INSERT_ROWS';
                $optParams['valueInputOption'] = 'RAW';
                try {
                    $response = $google_service->spreadsheets_values->append(
                        $spreadsheetId,
                        $range,
                        $requestBody,
                        $optParams
                    );
                    $updates = $response->getUpdates();
                    foreach ($updates as $k => $v) {
                        printf("%s: %s\n", $k, $v);
                    }
                    $msg_id = $array['msg_id'];
                } catch (Exception $e) {
                    print $e->getMessage();
                }
            }
        }
    }

}




/**
 * Returns an authorized API client.
 * @return Google_Client the authorized client object
 */
function getGoogleClient() {
    $client = new Google_Client();
    $client->setApplicationName(APPLICATION_NAME);
    $client->setScopes(SCOPES);
    $client->setAuthConfig(expandHomeDirectory(CLIENT_SECRET_PATH));
    $client->setAccessType('offline');

    // Load previously authorized credentials from a file.
    $credentialsPath = expandHomeDirectory(CREDENTIALS_PATH);
    if (file_exists($credentialsPath)) {
        $accessToken = json_decode(file_get_contents($credentialsPath), true);
    } else {
        // Request authorization from the user.
        $authUrl = $client->createAuthUrl();
        printf("Open the following link in your browser:\n%s\n", $authUrl);
        print 'Enter verification code: ';
        $authCode = trim(fgets(STDIN));

        // Exchange authorization code for an access token.
        $accessToken = $client->fetchAccessTokenWithAuthCode($authCode);

        // Store the credentials to disk.
        if(!file_exists(dirname($credentialsPath))) {
            mkdir(dirname($credentialsPath), 0700, true);
        }
        file_put_contents($credentialsPath, json_encode($accessToken));
        printf("Credentials saved to %s\n", $credentialsPath);
    }
    $client->setAccessToken($accessToken);

    // Refresh the token if it's expired.
    if ($client->isAccessTokenExpired()) {
        $client->fetchAccessTokenWithRefreshToken($client->getRefreshToken());
        file_put_contents($credentialsPath, json_encode($client->getAccessToken()));
    }
    return $client;
}

/**
 * Expands the home directory alias '~' to the full path.
 * @param string $path the path to expand.
 * @return string the expanded path.
 */
function expandHomeDirectory($path) {
    $homeDirectory = getenv('HOME');
    if (empty($homeDirectory)) {
        $homeDirectory = getenv('HOMEDRIVE') . getenv('HOMEPATH');
    }
    return str_replace('~', realpath($homeDirectory), $path);
}