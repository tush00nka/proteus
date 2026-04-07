#pragma once

#include "mme.h"
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <chrono>

struct PendingSMS {
	int _sms_id;
	TMSI _source_tmsi;
	MSISDN _destination_msisdn;
	std::string _msg;
	std::chrono::steady_clock::time_point _expiry;
};

class ENodeB {
public:
	int _id;
	int _position;
	std::uint32_t _radius;
	std::vector<TMSI> _attached_tmsi;
	std::unordered_map<int, PendingSMS> _sms_buffer;
	std::mutex _buffer_mux;

	double signalStrength(int clientPosition) const
	{
		int distance = std::abs(clientPosition - _position);
		if (distance > _radius)
		{
			return 0.0;
		}
		return 1.0 - (static_cast<double>(distance) / _radius);
	}
};