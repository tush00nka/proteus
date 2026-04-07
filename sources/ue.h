#pragma once

#include "address.h"
#include "client.h"
#include "options.h"
#include "utils.h"

class UE {
private:
	IMEI _imei;
	IMSI _imsi;
	MSISDN _msisdn;
	int _position;

	TMSI _tmsi = -1;
	bool _on = false;
	TCPClient _client;
	Address _server_address;
public:
	UE () = default;
	explicit UE(Options& opts)
        : _imei(opts.getIMEI()), _imsi(opts.getIMSI()), _msisdn(opts.getMSISDN()), _position(opts.getPosition()), _server_address(opts.getAddress()) {}

	bool connectToServer()
	{
		return _client.connect(_server_address);
	}

	void disconnectFromServer()
	{
		_client.disconnect();
	}

	bool turnOn();

	bool turnOff();
};