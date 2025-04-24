#include "simple_string.hpp"

namespace redis {
	
	std::string SimpleString::serialize() const {
		return "+" + this->s + "\r\n";
	}

	
	bool SimpleString::operator==(const redis::Resp& ss1) const{

		const auto *ss = dynamic_cast<const redis::SimpleString*>(&ss1);

		if (ss && this->s == ss->s) return true;
		return false;

	}

	SimpleString deserializeSimpleString(const std::string& str) {

		return SimpleString{ str.substr(1, str.size() - 3) };
	}

}