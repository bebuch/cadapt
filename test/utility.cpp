#include <cadapt/utility.hpp>

#include "test.hpp"


static_assert(!cadapt::same_as_one_of<int>);
static_assert(!cadapt::same_as_one_of<int, char>);
static_assert(!cadapt::same_as_one_of<int, float, long>);
static_assert(cadapt::same_as_one_of<int, int>);
static_assert(cadapt::same_as_one_of<int, int, long>);
static_assert(cadapt::same_as_one_of<int, float, int>);


template <typename Ref, typename Test>
[[nodiscard]] static constexpr bool is_equal(Ref* ref, Test* test) noexcept {
    return ref == test;
}


TEST(utility, non_const) {
    using cadapt::non_const;

    static_assert(std::same_as<char*, decltype(non_const(std::declval<char const*>()))>);
    static_assert(std::same_as<char&, decltype(non_const(std::declval<char const&>()))>);
    static_assert(std::same_as<char&&, decltype(non_const(std::declval<char const&&>()))>);
    static_assert(std::same_as<char*, decltype(non_const(std::declval<char*>()))>);
    static_assert(std::same_as<char&, decltype(non_const(std::declval<char&>()))>);
    static_assert(std::same_as<char&&, decltype(non_const(std::declval<char&&>()))>);
    static_assert(std::same_as<char(*)[1], decltype(non_const(std::declval<char const(*)[1]>()))>);
    static_assert(std::same_as<char(&)[1], decltype(non_const(std::declval<char const(&)[1]>()))>);
    static_assert(std::same_as<char(&&)[1], decltype(non_const(std::declval<char const(&&)[1]>()))>);
    static_assert(std::same_as<char(*)[1], decltype(non_const(std::declval<char(*)[1]>()))>);
    static_assert(std::same_as<char(&)[1], decltype(non_const(std::declval<char(&)[1]>()))>);
    static_assert(std::same_as<char(&&)[1], decltype(non_const(std::declval<char(&&)[1]>()))>);
    static_assert(std::same_as<char(&)[2], decltype(non_const("x"))>);

    static constexpr int value = 5;
    CT_EXPECT_TRUE(is_equal(&value, non_const(&value)));

    static constexpr char const* text = "test";
    CT_EXPECT_TRUE(is_equal(text, non_const(text)));

    static constexpr char array[] = "test";
    CT_EXPECT_TRUE(is_equal(array, non_const(array)));
}

#include <QStringEncoder>
#include <QStringView>

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


TEST(utility, qStringViewToStdString) {
    EXPECT_EQ(QString("宇宙よりも遠い場所"), QStringView(u"宇宙よりも遠い場所"));
    EXPECT_EQ(QString("宇宙よりも遠い場所"), QString::fromStdString(QString("宇宙よりも遠い場所").toStdString()));
    EXPECT_EQ(QString("宇宙よりも遠い場所"), QString::fromStdString(qStringViewToStdString1(QStringView(u"宇宙よりも遠い場所"))));
    EXPECT_EQ(QString("宇宙よりも遠い場所"), QString::fromStdString(qStringViewToStdString2(QStringView(u"宇宙よりも遠い場所"))));
    EXPECT_EQ(QString("宇宙よりも遠い場所"), QString::fromStdString(qStringViewToStdString3(QStringView(u"宇宙よりも遠い場所"))));
}
