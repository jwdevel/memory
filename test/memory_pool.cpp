// Copyright (C) 2015-2020 Jonathan Müller <jonathanmueller.dev@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include "memory_pool.hpp"

#include <algorithm>
#include <catch.hpp>
#include <random>
#include <vector>

#include "allocator_storage.hpp"
#include "test_allocator.hpp"

using namespace foonathan::memory;

// don't test actual node allocationg, but the connection between arena and the implementation
// so only  test for memory_pool<node_pool>
TEST_CASE("memory_pool", "[pool]")
{
    using pool_type = memory_pool<node_pool, allocator_reference<test_allocator>>;
    test_allocator alloc;
    {
        pool_type pool(4, pool_type::min_block_size(4, 25), alloc);
        REQUIRE(pool.node_size() >= 4u);
        REQUIRE(pool.capacity_left() >= 25 * 4u);
        REQUIRE(pool.next_capacity() >= 25 * 4u);
        REQUIRE(alloc.no_allocated() == 1u);

        SECTION("normal alloc/dealloc")
        {
            std::vector<void*> ptrs;
            auto               capacity = pool.capacity_left();
            REQUIRE(capacity / 4 >= 25);
            for (std::size_t i = 0u; i != 25; ++i)
                ptrs.push_back(pool.allocate_node());
            REQUIRE(pool.capacity_left() >= 0u);
            REQUIRE(alloc.no_allocated() == 1u);

            std::shuffle(ptrs.begin(), ptrs.end(), std::mt19937{});

            for (auto ptr : ptrs)
                pool.deallocate_node(ptr);
            REQUIRE(pool.capacity_left() == capacity);
        }
        SECTION("multiple block alloc/dealloc")
        {
            std::vector<void*> ptrs;
            auto               capacity = pool.capacity_left();
            for (std::size_t i = 0u; i != capacity / pool.node_size(); ++i)
                ptrs.push_back(pool.allocate_node());
            REQUIRE(pool.capacity_left() >= 0u);

            ptrs.push_back(pool.allocate_node());
            REQUIRE(pool.capacity_left() >= capacity - pool.node_size());
            REQUIRE(alloc.no_allocated() == 2u);

            std::shuffle(ptrs.begin(), ptrs.end(), std::mt19937{});

            for (auto ptr : ptrs)
                pool.deallocate_node(ptr);
            REQUIRE(pool.capacity_left() >= capacity);
            REQUIRE(alloc.no_allocated() == 2u);
        }
    }
    REQUIRE(alloc.no_allocated() == 0u);
}

TEST_CASE("memory_pool::min_block_size()")
{
    SECTION("node_pool, small")
    {
        auto                   min_size = memory_pool<node_pool>::min_block_size(1, 1);
        memory_pool<node_pool> pool(1, min_size);
        CHECK(pool.capacity_left() >= 1);

        // First allocation should not require realloc.
        auto ptr = pool.try_allocate_node();
        CHECK(ptr);

        // Second allocation might require it, but should still succeed then.
        ptr = pool.allocate_node();
        CHECK(ptr);
    }
    SECTION("node_pool, big")
    {
        auto                   min_size = memory_pool<node_pool>::min_block_size(16, 1);
        memory_pool<node_pool> pool(16, min_size);
        CHECK(pool.capacity_left() >= 16);

        // First allocation should not require realloc.
        auto ptr = pool.try_allocate_node();
        CHECK(ptr);

        // Second allocation might require it, but should still succeed then.
        ptr = pool.allocate_node();
        CHECK(ptr);
    }
    SECTION("array_pool, small")
    {
        auto                    min_size = memory_pool<array_pool>::min_block_size(1, 1);
        memory_pool<array_pool> pool(1, min_size);
        CHECK(pool.capacity_left() >= 1);

        // First allocation should not require realloc.
        auto ptr = pool.try_allocate_node();
        CHECK(ptr);

        // Second allocation might require it, but should still succeed then.
        ptr = pool.allocate_node();
        CHECK(ptr);
    }
    SECTION("array_pool, big")
    {
        auto                    min_size = memory_pool<array_pool>::min_block_size(16, 1);
        memory_pool<array_pool> pool(16, min_size);
        CHECK(pool.capacity_left() >= 16);

        // First allocation should not require realloc.
        auto ptr = pool.try_allocate_node();
        CHECK(ptr);

        // Second allocation might require it, but should still succeed then.
        ptr = pool.allocate_node();
        CHECK(ptr);
    }
    SECTION("small_node_pool, small")
    {
        auto                         min_size = memory_pool<small_node_pool>::min_block_size(1, 1);
        memory_pool<small_node_pool> pool(1, min_size);
        CHECK(pool.capacity_left() >= 1);

        // First allocation should not require realloc.
        auto ptr = pool.try_allocate_node();
        CHECK(ptr);

        // Second allocation might require it, but should still succeed then.
        ptr = pool.allocate_node();
        CHECK(ptr);
    }
    SECTION("small_node_pool, big")
    {
        auto                         min_size = memory_pool<small_node_pool>::min_block_size(16, 1);
        memory_pool<small_node_pool> pool(16, min_size);
        CHECK(pool.capacity_left() >= 16);

        // First allocation should not require realloc.
        auto ptr = pool.try_allocate_node();
        CHECK(ptr);

        // Second allocation might require it, but should still succeed then.
        ptr = pool.allocate_node();
        CHECK(ptr);
    }
}

