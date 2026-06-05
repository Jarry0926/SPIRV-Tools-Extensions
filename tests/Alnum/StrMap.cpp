#include <chrono>
#include <random>
#include <string>
#include <unordered_map>

#include <src/Alnum/StrMap.h>

#include <gtest/gtest.h>

template <uint32_t LEN>
static inline void
RandStr(std::string& p_out)
{
	std::mt19937
	rng(std::chrono::steady_clock::now().time_since_epoch().count());

	std::uniform_int_distribution<char>
	randChar(32, 126);

	p_out.resize(LEN);
	for (uint32_t i = 0u; i < LEN - 1u; ++i) {
		p_out[i] = randChar(rng);
	}
	p_out[LEN - 1u] = '\0';
}

TEST(Alnum, StrMap)
{
	static constexpr uint32_t N = 2048;

	// Fill `strPool` with random strings
	static std::string strPool[N];
	for (uint32_t i = 0u; i < N; ++i) {
		RandStr<256>(strPool[i]);
	}

	// Set the first half of them to true
	std::unordered_map<std::string, bool> ans;
	
	static struct StrMap out;
	strmapInit(&out);

	for (uint32_t i = 0u; i < N; ++i) {
		ans[strPool[i]] = (i < (N >> 1));

		const uint64_t v = strmapHash(strPool[i].c_str());
		if (i < (N >> 1)) {
			strmapPush(&out, strPool[i].c_str(), v);
		}
	}

	std::mt19937
	rng(std::chrono::steady_clock::now().time_since_epoch().count());

	std::uniform_int_distribution<uint32_t>
	randStr(0u, N - 1);

	for (uint32_t i = 0u; i < N; ++i) {
		const uint32_t j = randStr(rng);
		const uint64_t v = strmapHash(strPool[i].c_str());
		EXPECT_EQ(strmapFind(&out, strPool[i].c_str(), v),
				  (int)ans[strPool[i]]);
	}
}

