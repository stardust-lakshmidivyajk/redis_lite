#pragma once
#include <memory>
#include <string>
#include <vector>

namespace redis {
	//Abstract class/ interface
	class Resp {
	public:
		virtual ~Resp() = default; //we are virtual so that destructuor is called on the actual object types
		virtual std::string serialize() const = 0; // Based on the use cases, initially its okay to make these pure virtual methods
		virtual bool operator==(const redis::Resp&) const = 0;
	};
}