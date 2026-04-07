#include "mme.h"

TMSI MME::registerSubscriber(IMSI imsi, IMEI imei, MSISDN msisdn, int enodeb_id, int position, int fd)
{
	TMSI tmsi = _next_tmsi++;
	_subscribers[tmsi] = (Subscriber){._msisdn=imsi, ._imei=imei, ._imsi=msisdn, ._tmsi=enodeb_id, ._enodeb_id=position};
	_msisdn_to_tmsi[msisdn] = tmsi;
	_tmsi_to_fd[tmsi] = fd;
	return tmsi;
}

void MME::unregister(TMSI tmsi) {
	auto it = _subscribers.find(tmsi);
	if (it != _subscribers.end()) {
		_msisdn_to_tmsi.erase(it->second._msisdn);
		_tmsi_to_fd.erase(tmsi);
		_subscribers.erase(it);
	}
}