#pragma once

#include <memory>
#include <unordered_map>
#include "../utils.h"

struct Subscriber {
	MSISDN _msisdn;
	IMEI _imei;
	IMSI _imsi;
	TMSI _tmsi;
	int _enodeb_id;
	int _position; // x coordinate
};

const TMSI kInitialTMSI = 1000;

class MME {
private:
	std::unordered_map<TMSI, Subscriber> _subscribers;
	std::unordered_map<MSISDN, TMSI> _msisdn_to_tmsi;
	std::unordered_map<TMSI, int> _tmsi_to_fd;
	TMSI _next_tmsi = kInitialTMSI; 
public:
	MME () = default;
	TMSI registerSubscriber(IMSI, IMEI, MSISDN, int enodeb_id, int position, int fd);
	void unregister(TMSI);
	TMSI getTMSIByMSISDN(MSISDN);
	std::shared_ptr<Subscriber> getSubscriber(TMSI);
	void setLocation(TMSI, int enodeb_id, int position);
};