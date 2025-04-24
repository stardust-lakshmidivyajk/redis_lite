#include "RedisInteger.hpp"

namespace redis {

	std::string RedisInteger::serialize() const {
		return ":" + std::to_string(this->integer) + "\r\n";
	}


	bool RedisInteger::operator==(const Resp& ri) const {

		const auto* ss = dynamic_cast<const redis::RedisInteger*>(&ri);

		if (this->integer == ss->integer) return true;
		return false;

	}

	RedisInteger deserializeRedisInteger(const std::string& str) {

		int i = std::stoi(str.substr(1, str.size() - 3));

		return RedisInteger{i};
	}

}