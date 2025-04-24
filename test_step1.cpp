#include "gtest/gtest.h"
#include "simple_string.hpp"
#include "error.hpp"
#include "RedisInteger.hpp"
#include "BulkString.hpp"
#include "RedisArray.hpp"

#include <print>

TEST(Step1Tests, SimpleStringTest) {
	std::string str = "hello world";
	redis::SimpleString ss{ str };
	std::string serializedString = ss.serialize();
	ASSERT_EQ("+hello world\r\n", serializedString);
	redis::SimpleString obj1 = redis::deserializeSimpleString(serializedString);
	ASSERT_EQ(*dynamic_cast<redis::Resp*>(&ss), *dynamic_cast<redis::Resp*>(&obj1));
}

TEST(Step1Tests, ErrorTest) {
	std::string errorMsg = "Error message";
	redis::Error error{ errorMsg };
	auto serializedMsg = error.serialize();
	ASSERT_EQ("-Error message\r\n", serializedMsg);
	redis::Error error1 = redis::deserializeError(serializedMsg);
	ASSERT_EQ(*dynamic_cast<redis::Resp*>(&error), *dynamic_cast<redis::Resp*>(&error1));
}

TEST(Step1Tests, IntegerTest) {
	int i = 999;
	redis::RedisInteger ri{ i };
	auto serializedMsg = ri.serialize();
	ASSERT_EQ(":999\r\n", serializedMsg);
	auto deserializedInt = redis::deserializeRedisInteger(serializedMsg);
	ASSERT_EQ(*dynamic_cast<redis::Resp*>(&ri), *dynamic_cast<redis::Resp*>(&deserializedInt));
}

TEST(Step1Tests, BulkStringTestNullOpt) {
	redis::BulkString bs{ std::nullopt };
	auto serializedMsg = bs.serialize();
	ASSERT_EQ("$-1\r\n", serializedMsg);
	auto deserializedMsg = redis::deserializeBulkString(serializedMsg);
	ASSERT_EQ(*dynamic_cast<redis::Resp*>(&bs), *dynamic_cast<redis::Resp*>(&deserializedMsg));
}

TEST(Step1Tests, BulkStringTest) {
	std::string bulkMsg = "echo";
	redis::BulkString bs{ bulkMsg };
	auto serializedMsg = bs.serialize();
	ASSERT_EQ("$4\r\necho\r\n", serializedMsg);
	auto deserializedMsg = redis::deserializeBulkString(serializedMsg);
	ASSERT_EQ(*dynamic_cast<redis::Resp*>(&bs), *dynamic_cast<redis::Resp*>(&deserializedMsg));
}

TEST(Step1Tests, ArrayTest) {
	
	std::shared_ptr<redis::Resp> r1 = std::make_shared<redis::BulkString>("");
	std::shared_ptr<redis::Resp>  r2 = std::make_shared<redis::RedisInteger>(3);
	std::vector<std::shared_ptr<redis::Resp>> vec{ r1,r2 };
	std::shared_ptr<redis::Resp> r3 = std::make_shared<redis::RedisArray>(vec);
	std::shared_ptr<redis::Resp> r4 = std::make_shared<redis::SimpleString>("hello world");
	vec.push_back(r3);
	vec.push_back(r4);
	auto arr = redis::RedisArray{ std::make_optional(vec) };
	auto serializedMsg = arr.serialize();
	ASSERT_EQ("*4\r\n$0\r\n\r\n:3\r\n*2\r\n$0\r\n\r\n:3\r\n+hello world\r\n", serializedMsg);
	auto deserializedMsg = redis::deserializeRedisArray(serializedMsg);
	ASSERT_EQ(*dynamic_cast<redis::Resp*>(&arr), *dynamic_cast<redis::Resp*>(&deserializedMsg));
}

TEST(Step1Tests, ArrayTest1) {

	std::shared_ptr<redis::Resp> r1 = std::make_shared<redis::BulkString>("echo");
	std::shared_ptr<redis::Resp> r2 = std::make_shared<redis::BulkString>("hello world");	
	auto vec = std::vector<std::shared_ptr<redis::Resp>>{ r1, r2 };
	auto arr = redis::RedisArray{ std::make_optional(vec) };
	auto serializedMsg = arr.serialize();
	ASSERT_EQ("*2\r\n$4\r\necho\r\n$11\r\nhello world\r\n", serializedMsg);
	auto deserializedMsg = redis::deserializeRedisArray(serializedMsg);
	ASSERT_EQ(*dynamic_cast<redis::Resp*>(&arr), *dynamic_cast<redis::Resp*>(&deserializedMsg));
}

TEST(Step1Tests, ArrayTest2) {
	auto vec = std::vector<std::shared_ptr<redis::Resp>>{ std::make_shared<redis::BulkString>("ping") };
	auto arr = redis::RedisArray{ std::make_optional(vec)};
	auto serializedMsg = arr.serialize();
	ASSERT_EQ("*1\r\n$4\r\nping\r\n", serializedMsg);
	auto deserializedMsg = redis::deserializeRedisArray(serializedMsg);
	ASSERT_EQ(*dynamic_cast<redis::Resp*>(&arr), *dynamic_cast<redis::Resp*>(&deserializedMsg));
}

TEST(Step1Tests, ArrayTest3) {

	std::shared_ptr<redis::Resp> r1 = std::make_shared<redis::BulkString>("get");
	std::shared_ptr<redis::Resp> r2 = std::make_shared<redis::BulkString>("key");
	auto vec = std::vector<std::shared_ptr<redis::Resp>>{ r1, r2 };
	auto arr = redis::RedisArray{ std::make_optional(vec) };
	auto serializedMsg = arr.serialize();
	ASSERT_EQ("*2\r\n$3\r\nget\r\n$3\r\nkey\r\n", serializedMsg);
	auto deserializedMsg = redis::deserializeRedisArray(serializedMsg);
	ASSERT_EQ(*dynamic_cast<redis::Resp*>(&arr), *dynamic_cast<redis::Resp*>(&deserializedMsg));
}

TEST(Step1Tests, NullArrayTest) {

	auto arr = redis::RedisArray{ std::nullopt };
	auto serializedMsg = arr.serialize();
	ASSERT_EQ("*-1\r\n", serializedMsg);
	auto deserializedMsg = redis::deserializeRedisArray(serializedMsg);
	ASSERT_EQ(*dynamic_cast<redis::Resp*>(&arr), *dynamic_cast<redis::Resp*>(&deserializedMsg));
}