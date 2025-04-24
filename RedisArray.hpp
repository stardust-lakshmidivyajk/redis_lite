#pragma once

#include "resp.hpp"
#include <vector>
#include <optional>
namespace redis {
	class Resp;
	class RedisArray : public Resp {
	public:
		std::optional<std::vector<std::shared_ptr<redis::Resp>>> arr;
		RedisArray(const std::optional<std::vector<std::shared_ptr<redis::Resp>>>& a);
		std::string serialize() const override;
		
		bool operator==(const redis::Resp&) const override;
	};

	RedisArray deserializeRedisArray(const std::string &);
}