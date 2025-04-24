#pragma once

#include "resp.hpp"
namespace redis {
	class Resp;
	class RedisInteger : public Resp {
		int integer;
	public:
		RedisInteger(const int& i) : integer(i) {}
		std::string serialize() const override;
		bool operator==(const redis::Resp&) const override;
	};

	RedisInteger deserializeRedisInteger(const std::string&);
}