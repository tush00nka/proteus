#include "ue.h"

bool UE::turnOn()
{
	if (_on)
	{
		log<LogLevel::ERROR>("Already ON");
		return true;
	}
	_on = true;

	std::string cmd_r = "R|" + std::to_string(_imsi) + "|" + std::to_string(_position);
	std::string resp = _client.sendAndReceive(cmd_r);
	if (resp.empty())
	{
		return false;
	}

	auto parts = split(resp, '|');
	if (parts.size() < 4)
	{
		log<LogLevel::ERROR>("Invalid response to R");
		return false;
	}

	int best = -1;
	double best_signal = -1.0F;
	for (size_t i = 0; i+1 < parts.size(); i+=2)
	{
		int enb_id = std::stoi(std::string(parts[i]));   	// TODO(tushonka): add proper exception handling
		double signal = std::stod(std::string(parts[i+1])); // TODO(tushonka): add proper exception handling
		if (best_signal < signal)
		{
			best_signal = signal;
			best = enb_id;
		}
	}
	if (-1 == best || 0 >= best_signal)
	{
		log<LogLevel::ERROR>("No signal?");
		_on = false;
		return false;
	}

	std::string cmd_a = "A|"
		+ std::to_string(_imsi)   + "|"
		+ std::to_string(_imei)   + "|"
		+ std::to_string(_msisdn) + "|"
		+ std::to_string(best);
		
	resp = _client.sendAndReceive(cmd_a);
	if (resp.empty())
	{
		return false;
	}
	
	// TMSI|1234
	auto resp_parts = split(resp, '|');
	if (resp_parts.size() >= 2 && resp_parts[0] == "TMSI") {
		_tmsi = std::stoi(std::string(resp_parts[1])); // TODO(tushonka): add proper exception handling
		log<LogLevel::INFO>(std::format("Registered! TMSI = {}\n", _tmsi));
		return true;
	} 

	log<LogLevel::ERROR>(std::format("Registration failed: {}\n", resp));
	_on = false;
	return false;
}

bool UE::turnOff()
{
	if (!_on)
	{
		log<LogLevel::ERROR>("Already OFF");
		return true;
	}
	std::string resp = _client.sendAndReceive("OFF");
	log<LogLevel::INFO>(std::format("Server response: {}\n", resp));
	_on = false;
	_tmsi = -1;
	return true;
}