#include <algorithm>
#include <chrono>
#include <format>
#include <iostream>
#include <random>
#include <string_view>

#include <gtest/gtest.h>

#include <utils/Str.h>

template <uint32_t LEN>
static inline void
RandStr(char* const p_pOut)
{
	std::mt19937
	rng(std::chrono::steady_clock::now().time_since_epoch().count());

	constexpr std::string_view
	chars = "abcdefghijklmnopqrstuvwxyz"
    		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "0123456789"
            ".,;:!?+-*/_=()[]{}<>";

	static std::discrete_distribution<int>
	randCharType({19, 27, 27, 27});

	static std::uniform_int_distribution<uint32_t>
	randChar(0u, chars.size() - 1u);

	for (uint32_t j = 0u; j < LEN - 1u; ++j) {
		switch (randCharType(rng)) {
		case 0:
			p_pOut[j] = chars[randChar(rng)];
			break;
		case 1:
			p_pOut[j] = ' ';
			break;
		case 2:
			p_pOut[j] = '\t';
			break;
		case 3:
			p_pOut[j] = '\n';
			break;
		}
	}
	p_pOut[LEN - 1u] = '`';
}

TEST(Str, FromHex64)
{
	std::mt19937
	rng(std::chrono::steady_clock::now().time_since_epoch().count());

	std::uniform_int_distribution<uint64_t>
	dist(0ull, UINT64_MAX);
	
	for (uint32_t i = 0u; i < static_cast<uint32_t>(1e5); ++i) {
		uint64_t in = dist(rng);
		char out[17];
		strFromHex64(in, out);
		out[16] = '\0';
		EXPECT_EQ(std::string(out), std::format("{:016X}", in));
	}
}

TEST(Str, Strip)
{
	char in[1000];
	for (uint32_t i = 0u; i < static_cast<uint32_t>(1e4); ++i) {
		RandStr<1000>(in);
		std::string ans(in);
		ans.erase(std::remove_if(ans.begin(), ans.begin() + ans.find('`'), isspace),
				  ans.end());
		char out[1000];
		char const* pIt = in;
		*strStrip(&pIt, out, '`') = '\0';
		EXPECT_EQ(std::string(out), ans);
	}
}

TEST(Str, StrpN)
{
	char in[1000];
	for (uint32_t i = 0u; i < static_cast<uint32_t>(1e4); ++i) {
		RandStr<1000>(in);
		std::string ans(in);
		ans.erase(std::remove_if(ans.begin(), ans.begin() + ans.find('`'), isspace),
				  ans.end());
		char out[1000];
		*strStripN(in, out, 999) = '\0';
		EXPECT_EQ(std::string(out), ans);
	}
}

TEST(Str512, Match)
{
	alignas(64) char in[64];
	for (uint32_t i = 0u; i < static_cast<uint32_t>(1e5); ++i) {
		RandStr<64>(in);
		EXPECT_TRUE(str512Match(in, in));
	}
}

TEST(Str512, SubMatch)
{
	std::mt19937
	rng(std::chrono::steady_clock::now().time_since_epoch().count());

	std::uniform_int_distribution<uint32_t>
	randSubstrIdx(0u, 51u);

	std::uniform_int_distribution<uint32_t>
	randSubstrLen(8u, 13u);

	alignas(64) char in[64] = "abcdefghijklmnopqrstuvwkyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,";
	alignas(64) char pat[64];
	for (uint32_t i = 0u; i < static_cast<uint32_t>(1e7); ++i) {
		uint32_t j = randSubstrIdx(rng);
		uint32_t k = randSubstrLen(rng);
		memcpy(pat, &in[j], k);
		pat[k] = '\0';
		EXPECT_EQ(str512SubMatch(pat, in, k), j);
	}
}

