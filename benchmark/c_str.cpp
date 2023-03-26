#include <cadapt/c_str.hpp>

#include <benchmark/benchmark.h>


static void c_str(benchmark::State& state) {
    std::string test(state.range(0) - 1, 'A');
    for (auto _ : state) {
        auto ptr = cadapt::c_str(test);
        benchmark::DoNotOptimize(ptr);
    }
}

BENCHMARK(c_str)
    ->Arg(4)->Arg(6)->Arg(8)->Arg(10)->Arg(12)->Arg(16)
    ->Arg(20)->Arg(32)->Arg(64)->Arg(128)->Arg(256)
    ->Arg(512)->Arg(1024)->Arg(2048)->Arg(4096);

static void unverified_c_str(benchmark::State& state) {
    std::string test(state.range(0) - 1, 'A');
    for (auto _ : state) {
        auto ptr = cadapt::unverified_c_str(test);
        benchmark::DoNotOptimize(ptr);
    }
}

BENCHMARK(unverified_c_str)
    ->Arg(4)->Arg(6)->Arg(8)->Arg(10)->Arg(12)->Arg(16)
    ->Arg(20)->Arg(32)->Arg(64)->Arg(128)->Arg(256)
    ->Arg(512)->Arg(1024)->Arg(2048)->Arg(4096);
