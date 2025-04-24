#include "RedisArray.hpp"
#include "BulkString.hpp"
#include "simple_string.hpp"
#include "RedisInteger.hpp"
#include "error.hpp"

#include <iostream>
#include <print>
#include <ranges>
#include <regex>
#include <sstream>
#include <string>

namespace redis {

	RedisArray::RedisArray(const std::optional<std::vector<std::shared_ptr<redis::Resp>>>& a) : arr(a) {}

	std::string RedisArray::serialize() const {

		if (this->arr) {

			std::string msg = "*" + std::to_string(this->arr->size()) + "\r\n";

			for (auto i : *(this->arr)) {

				msg += i->serialize();

			}

			return msg;

		}

		return "*-1\r\n";
		
	}


	bool RedisArray::operator==(const Resp& ss1) const {

		const auto* ss = dynamic_cast<const redis::RedisArray*>(&ss1);

		for (const auto &[i, j] : std::views::zip(*(this->arr), *(ss->arr))) {
			if (!(*i == *j)) return false;
		}

		return true;
		
	}

	int recur(const std::string& str, int start, std::vector<std::shared_ptr<redis::Resp>> &arr) {
		auto iter = str.find_first_of(":+-$*", start + 1);
		auto arrLength = std::stoi(str.substr(start + 1, iter - start - 3));
		for (auto i = 0; i < arrLength; i++)
		{
			auto next = str.find_first_of(":+-$*", iter + 1);
			next = (next == std::string::npos) ? str.size() : next;

			switch (str[iter]) {
				case '+':
				{
					auto res = deserializeSimpleString(str.substr(iter, next - iter));
					arr.push_back(std::make_shared<SimpleString>(res));
					iter = next;
				}
				break;
				case '-':
				{
					auto res = deserializeError(str.substr(iter, next - iter));
					arr.push_back(std::make_shared<Error>(res));
					iter = next;
				}
				break;
				case ':':
				{
					auto res = deserializeRedisInteger(str.substr(iter, next - iter));
					arr.push_back(std::make_shared<RedisInteger>(res));
					iter = next;
				}
				break;
				case '$':
				{
					auto res = deserializeBulkString(str.substr(iter, next - iter));
					arr.push_back(std::make_shared<BulkString>(res));
					iter = next;
				}
				break;
				case '*':
				{
					auto arr1 = std::vector<std::shared_ptr<redis::Resp>>{};
					iter = recur(str, iter, arr1);
					auto redisArr = RedisArray{ std::make_optional(arr1) };
					arr.push_back(std::make_shared<RedisArray>(redisArr));
				}
				break;
			}
		}
		return iter;
	}

	RedisArray deserializeRedisArray(const std::string& str) {
		if ("*-1\r\n" == str) {
			return RedisArray{ std::nullopt };
		}
		auto arr = std::vector<std::shared_ptr<redis::Resp>>{};
		auto _ = recur(str, 0, arr);
		return RedisArray{ std::make_optional(arr) };
	}
}