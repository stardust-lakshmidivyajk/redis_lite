#pragma once

#include "resp.hpp"
namespace redis {
	class Resp;
	class SimpleString : public Resp {
		std::string s;
	public:
		SimpleString(const std::string& str) : s(str) {}
		std::string serialize() const override;
		bool operator==(const redis::Resp&) const override;
	};

	SimpleString deserializeSimpleString(const std::string&);
}