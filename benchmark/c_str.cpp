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
        auto ptr = cadapt::c_str(cadapt::null_term, test);
        benchmark::DoNotOptimize(ptr);
    }
}

BENCHMARK(unverified_c_str)
    ->Arg(4)->Arg(6)->Arg(8)->Arg(10)->Arg(12)->Arg(16)
    ->Arg(20)->Arg(32)->Arg(64)->Arg(128)->Arg(256)
    ->Arg(512)->Arg(1024)->Arg(2048)->Arg(4096);

#if __has_include(<QString>)
#include <QStringEncoder>
static void qt_to_std(benchmark::State& state) {
    QString test(state.range(0) - 1, 'A');
    for (auto _ : state) {
        auto str = cadapt::qt_to_std<char>(test);
        benchmark::DoNotOptimize(str);
    }
}

std::string qStringToStdString(QString const& data) {
    return data.toStdString();
}

std::string qStringViewToStdString1(QStringView const data) {
    return data.toString().toStdString();
}

std::string qStringViewToStdString2(QStringView const data) {
    auto const bytes = data.toUtf8();
    return std::string(bytes.constData(), bytes.length());
}

std::string qStringViewToStdString3(QStringView const data) {
    auto toUtf8 = QStringEncoder(QStringEncoder::Utf8);
    auto len = toUtf8.requiredSpace(data.length());
    std::string result;
#ifdef __cpp_lib_string_resize_and_overwrite
    result.resize_and_overwrite(len, [len](char*, std::size_t) { return len; });
#else
    result.resize(len);
#endif
    char* end = toUtf8.appendToBuffer(result.data(), data);
    result.resize(end - result.data());
    return result;
}

BENCHMARK(qt_to_std)
    ->Arg(4)->Arg(6)->Arg(8)->Arg(10)->Arg(12)->Arg(16)
    ->Arg(20)->Arg(32)->Arg(64)->Arg(128)->Arg(256)
    ->Arg(512)->Arg(1024)->Arg(2048)->Arg(4096)
    ->Arg(8192)->Arg(16384)->Arg(32768)->Arg(65536);

static void qt_view_to_std_1(benchmark::State& state) {
    QString data(state.range(0) - 1, 'A');
    QStringView test(data);
    for (auto _ : state) {
        auto str = qStringViewToStdString1(test);
        benchmark::DoNotOptimize(str);
    }
}

BENCHMARK(qt_view_to_std_1)
    ->Arg(4)->Arg(6)->Arg(8)->Arg(10)->Arg(12)->Arg(16)
    ->Arg(20)->Arg(32)->Arg(64)->Arg(128)->Arg(256)
    ->Arg(512)->Arg(1024)->Arg(2048)->Arg(4096)
    ->Arg(8192)->Arg(16384)->Arg(32768)->Arg(65536);

static void qt_view_to_std_2(benchmark::State& state) {
    QString data(state.range(0) - 1, 'A');
    QStringView test(data);
    for (auto _ : state) {
        auto str = qStringViewToStdString2(test);
        benchmark::DoNotOptimize(str);
    }
}

BENCHMARK(qt_view_to_std_2)
    ->Arg(4)->Arg(6)->Arg(8)->Arg(10)->Arg(12)->Arg(16)
    ->Arg(20)->Arg(32)->Arg(64)->Arg(128)->Arg(256)
    ->Arg(512)->Arg(1024)->Arg(2048)->Arg(4096)
    ->Arg(8192)->Arg(16384)->Arg(32768)->Arg(65536);

static void qt_view_to_std_3(benchmark::State& state) {
    QString data(state.range(0) - 1, 'A');
    QStringView test(data);
    for (auto _ : state) {
        auto str = qStringViewToStdString3(test);
        benchmark::DoNotOptimize(str);
    }
}

BENCHMARK(qt_view_to_std_3)
    ->Arg(4)->Arg(6)->Arg(8)->Arg(10)->Arg(12)->Arg(16)
    ->Arg(20)->Arg(32)->Arg(64)->Arg(128)->Arg(256)
    ->Arg(512)->Arg(1024)->Arg(2048)->Arg(4096)
    ->Arg(8192)->Arg(16384)->Arg(32768)->Arg(65536);

#endif
