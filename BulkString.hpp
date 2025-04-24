#pragma once

#include "resp.hpp"
#include "optional"
namespace redis {
	class BulkString : public Resp {
		std::optional<std::string> s;
	public:
		BulkString(const std::optional<std::string> &st) : s(st) {}
		std::string serialize() const override;
		bool operator==(const redis::Resp&) const override;
	};

	BulkString deserializeBulkString(const std::string&);
}