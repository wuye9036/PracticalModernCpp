#include <gtest/gtest.h>

#include <unifex/trampoline_scheduler.hpp>
#include <unifex/single_thread_context.hpp>

#include <unifex/just.hpp>
#include <unifex/transform.hpp>
#include <unifex/on.hpp>
#include <unifex/via.hpp>
#include <unifex/via_stream.hpp>
#include <unifex/range_stream.hpp>
#include <unifex/transform_stream.hpp>

#include <unifex/sync_wait.hpp>
#include <unifex/for_each.hpp>
#include <unifex/reduce_stream.hpp>
#include <unifex/sender_concepts.hpp>

#include <fmt/printf.h>

using namespace unifex;

TEST(Executor, ExecutorTest)
{
    unifex::trampoline_scheduler sched(2);
    // single_thread_context ctx;
    // auto sched = ctx.get_scheduler();

    sender auto s = range_stream{0, 10}
        | via_stream(sched)
        | transform_stream([](auto v) { fmt::print("v1:{:d}\n", v); return v * 2; })
        | transform_stream([](auto v) { fmt::print("v2:{:d}\n", v); return v + 1; })
        | reduce_stream(0, [](int st, int v) { fmt::print("st:{:d}\n", st); return st + v;})
        | then([](int v){return v;})
        ;
    int v = sync_wait(s).value();

    GTEST_ASSERT_EQ(v, 100);

    /*
    cache vertex_cache;
    triangle_list
    | tri_fifo | monitor
    | dispatch(round_robin, [](triangle const& tri) {return tri.index();},
        tri_fifo | monitor
        | mcg(vertex_cache)
        | converge_then(
            transform([](){ get.partA(); } ) | processA,
            transform([](){ get.partA(); } ) | processB,
            [](auto resultA, auto resultB) {
              return resultA.id == resultB.id;
            })
        | rasterizer
        | warp_generator
        | warp_executor
        | backend_executor
        | result_writer
    )
    */
    // s.
}