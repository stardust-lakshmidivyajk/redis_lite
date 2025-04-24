#include "error.hpp"

namespace redis {

	std::string Error::serialize() const {
		return "-" + this->errorMsg + "\r\n";
	}


	bool Error::operator==(const Resp& error) const {

		const auto* ss = dynamic_cast<const redis::Error*>(&error);

		if (ss && this->errorMsg == ss->errorMsg) return true;
		return false;

	}

	Error deserializeError(const std::string& str) {

		return Error{ str.substr(1, str.size() - 3) };
	}

}