#include "BulkString.hpp"

namespace redis {

	std::string BulkString::serialize() const {

		if (this->s) {

			return "$" + std::to_string(this->s->size()) + "\r\n" + this->s.value() + "\r\n";

		}

		return "$-1\r\n";
		
	}


	bool BulkString::operator==(const Resp& ss1) const {

		const auto* ss = dynamic_cast<const redis::BulkString*>(&ss1);

		if (ss && this->s == ss->s) return true;
		return false;

	}

	BulkString deserializeBulkString(const std::string& str) {

		if (str == "$-1\r\n") return redis::BulkString(std::nullopt);
		return BulkString{ str.substr(str.find('\n') + 1, str.size() - (str.find('\n') + 3))};
	}

}