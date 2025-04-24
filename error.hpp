#pragma once

#include "resp.hpp"
namespace redis {
	class Resp;
	class Error : public Resp {
		std::string errorMsg;
	public:
		Error(const std::string& str) : errorMsg(str) {}
		std::string serialize() const override;
		bool operator==(const redis::Resp&) const override;
	};

	Error deserializeError(const std::string&);
}