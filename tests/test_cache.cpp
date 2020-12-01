#include <gtest/gtest.h>

#include "lfu_cache.h"

namespace config {
    static constexpr const std::size_t capacity = 1000;

    using AddrHandlerT = pagrnd::cache::FullyAssociativeAddrHandler<std::size_t>;
    using DataHandlerT = pagrnd::cache::FullyAssociativeDataHandler<std::size_t, std::size_t>;
    using ReplacerT    = pagrnd::cache::LFUReplacer<std::size_t>;

    using CacheT       = pagrnd::cache::LFUFullyAssociativeCache<std::size_t, std::size_t>;
}

TEST(LFUCacheTest, LFUCacheTestAddrHandlerConstructor)
{
    config::AddrHandlerT addr_handler = config::AddrHandlerT();
}

TEST(LFUCacheTest, LFUCacheTestDataHandlerConstructor)
{
    config::DataHandlerT data_handler = config::DataHandlerT(config::capacity);
}

TEST(LFUCacheTest, LFUCacheTestReplacerConstructor)
{
    config::ReplacerT replacer = config::ReplacerT();
}

TEST(LFUCacheTest, LFUCacheTestConstructor)
{
    config::AddrHandlerT addr_handler = config::AddrHandlerT();
    config::DataHandlerT data_handler = config::DataHandlerT(config::capacity);
    config::ReplacerT replacer = config::ReplacerT();

    config::CacheT cache = config::CacheT(&addr_handler, &data_handler, &replacer);
}


TEST(LFUCacheTest, LFUCacheReadEmpty)
{
    config::AddrHandlerT addr_handler = config::AddrHandlerT();
    config::DataHandlerT data_handler = config::DataHandlerT(config::capacity);
    config::ReplacerT replacer = config::ReplacerT();

    config::CacheT cache = config::CacheT(&addr_handler, &data_handler, &replacer);

    const std::size_t addr = 28;

    auto read_result = cache.read(addr);
    ASSERT_FALSE(read_result.has_value());
}


TEST(LFUCacheTest, LFUCacheTestReadWrite)
{
    config::AddrHandlerT addr_handler = config::AddrHandlerT();
    config::DataHandlerT data_handler = config::DataHandlerT(config::capacity);
    config::ReplacerT replacer = config::ReplacerT();

    config::CacheT cache = config::CacheT(&addr_handler, &data_handler, &replacer);

    const std::size_t addr = 14;
    const std::size_t data = 5;

    cache.write(addr, data);
    auto read_result = cache.read(addr);

    ASSERT_TRUE(read_result.has_value());
    ASSERT_EQ(read_result.value(), data);
}


TEST(LFUCacheTest, LFUCacheTestReadWriteScanning)
{
    const std::size_t capacity = 3;

    config::AddrHandlerT addr_handler = config::AddrHandlerT();
    config::DataHandlerT data_handler = config::DataHandlerT(capacity);
    config::ReplacerT replacer = config::ReplacerT();

    config::CacheT cache = config::CacheT(&addr_handler, &data_handler, &replacer);

    // M, M, M, H, H, H, M, M, M, H, H
    const auto access_pattern = {0, 1, 2, 0, 1, 2, 11, 12, 0, 1, 2};
    const std::size_t def_data = 0xBADF00D;

    std::size_t num_hits = 0;

    for (const auto addr : access_pattern) {
        auto read_result = cache.read(addr);
        
        bool is_hit = read_result.has_value();

        if (!is_hit)
            cache.write(addr, def_data);
        else
            num_hits++;
    }

    const std::size_t correct_num_hits = 5;
    ASSERT_EQ(num_hits, correct_num_hits);
}


TEST(LFUCacheTest, LFUCacheTestReadWriteSpecificPattern)
{
    const std::size_t capacity = 3;

    config::AddrHandlerT addr_handler = config::AddrHandlerT();
    config::DataHandlerT data_handler = config::DataHandlerT(capacity);
    config::ReplacerT replacer = config::ReplacerT();

    config::CacheT cache = config::CacheT(&addr_handler, &data_handler, &replacer);

    // M, M, M, H, M, M, H, H, H, M, H, M, M
    const auto access_pattern = {0, 1, 2, 2, 5, 6, 6, 2, 6, 3, 3, 0, 1};
    const std::size_t def_data = 0xBADF00D;

    std::vector<bool> hit_pattern;

    for (const auto addr : access_pattern) {
        auto read_result = cache.read(addr);
        
        bool is_hit = read_result.has_value();

        if (!is_hit)
            cache.write(addr, def_data);

        hit_pattern.push_back(is_hit);
    }

    const auto correct_hit_pattern  = {false, false, false, true, false, false, true, true, true, false, true, false, false};
    ASSERT_TRUE(std::equal(hit_pattern.begin(), hit_pattern.end(), std::begin(correct_hit_pattern)));
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}