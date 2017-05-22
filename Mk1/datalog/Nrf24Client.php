<?php
namespace Theapi\Nrf24;

class Client
{

    protected $debug = 0;
    protected $server;
    protected $port;

    protected $fp;

    public function start($server, $port = 2000, $debug = 0)
    {
        $this->setDebug($debug);
        $this->connect($server, $port);
        return $this->fp;
    }

    public function setDebug($debug)
    {
        $this->debug = $debug;
    }

    public function connect($server, $port)
    {
        $this->server = $server;
        $this->port = $port;
        if ($this->debug > 0) {
            echo '>< Connecting to tcp://' . $this->server . ':' . $this->port . "\n";
        }

        $this->fp = stream_socket_client('tcp://' . $this->server . ':' . $this->port, $errno, $errstr, 30);

        if (!$this->fp) {
            throw new \Exception('Unable to connect to ' . $this->server . ':' . $this->port, $errno);
        }
        stream_set_timeout($this->fp, 2);

        if ($this->debug > 0) {
            echo ">< Connected!\n";
        }
    }

    public function read()
    {
        if (!$this->fp) {
            throw new \Exception('No connection to ' . $this->server . ':' . $this->port);
        }

        $line = fgets($this->fp);

        if ($this->debug > 2) {
            $info = stream_get_meta_data($link);
            echo " < $line".($info['timed_out'] ? " read timed out" : "")."\n";
        }

        if ($this->debug > 1) {
            echo " < $line\n";
        }
        return $line;
    }

    public function write($buf)
    {
        if (!$this->fp) {
            throw new \Exception('No connection to ' . $this->server . ':' . $this->port);
        }

        if ($this->debug > 1) {
            foreach(explode("\n", $buf) as $line) echo " > $line\n";
        }
        fwrite($this->fp, "$buf\n");
    }

    public function disconnect()
    {
        if ($this->debug > 1) {
            echo ">< Disconnecting\n";
        }

        $this->write('bye');
        fclose($this->fp);

        if ($this->debug > 1) {
            echo ">< Disconnected!\n";
        }
    }

}
