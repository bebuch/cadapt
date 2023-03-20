#include <cadapt.hpp>

#include <cstring>
#include <iostream>
#include <sstream>

void set_name(char* buffer) {
    strcpy(buffer, "some text");
}

void set_name(char* buffer, size_t buffer_size) {
    char const text[] = "some text";
    if(buffer_size > sizeof(text) - 1) {
        memcpy(buffer, text, sizeof(text));
    }else if(buffer_size == sizeof(text) - 1) {
        memcpy(buffer, text, sizeof(text) - 1);
    }
    std::cerr << "buffer to small\n";
}

/// \brief Mask all non pritable characters with escape sequences
inline std::string mask_non_print(std::string const& str){
    std::ostringstream result;
    for(auto c: str){
        // Mask any UTF-8 characters
        auto const d = static_cast< std::uint8_t >(c);
        if(d > 127){
            result << "\\x";
            auto nipple_to_hex = [](std::uint8_t e){
                    return static_cast< char >(
                        e < 10
                        ? e + '0'
                        : e - 10 + 'A');
                };
            result << nipple_to_hex(d >> 4);
            result << nipple_to_hex(d & 0x0F);
            continue;
        }

        switch(c){
            case 0: result << "\\0"; break;
            case 1: result << "\\x01"; break;
            case 2: result << "\\x02"; break;
            case 3: result << "\\x03"; break;
            case 4: result << "\\x04"; break;
            case 5: result << "\\x05"; break;
            case 6: result << "\\x06"; break;
            case 7: result << "\\a"; break;
            case 8: result << "\\b"; break;
            case 9: result << "\\t"; break;
            case 10: result << "\\n"; break;
            case 11: result << "\\v"; break;
            case 12: result << "\\f"; break;
            case 13: result << "\\r"; break;
            case 14: result << "\\x0E"; break;
            case 15: result << "\\x0F"; break;
            case 16: result << "\\x10"; break;
            case 17: result << "\\x11"; break;
            case 18: result << "\\x12"; break;
            case 19: result << "\\x13"; break;
            case 20: result << "\\x14"; break;
            case 21: result << "\\x15"; break;
            case 22: result << "\\x16"; break;
            case 23: result << "\\x17"; break;
            case 24: result << "\\x18"; break;
            case 25: result << "\\x19"; break;
            case 26: result << "\\x1A"; break;
            case 27: result << "\\x1B"; break;
            case 28: result << "\\x1C"; break;
            case 29: result << "\\x1D"; break;
            case 30: result << "\\x1E"; break;
            case 31: result << "\\x1F"; break;
            case 127: result << "\\x7F"; break;
            case '\\': result << "\\\\"; break;
            default: result << c;
        }
    }
    return result.str();
}

using namespace cadapt;

void test_case() {
    std::string str;
    resize_for_out_ptr(str, 10);
    [](out_c_str_t<char>){}({str});
    [](out_c_str_t<char>){}({str, 10});
}

int main() {
    {
        int v=20;
        std::string str;
        std::cout
            << "      str: " << mask_non_print(str) << '\n'
            << "str.c_str: " << mask_non_print(str.c_str()) << '\n'
            << " str.size: " << str.size() << '\n';
        set_name(out_c_str(str, v));
        std::cout
            << "      str: " << mask_non_print(str) << '\n'
            << "str.c_str: " << mask_non_print(str.c_str()) << '\n'
            << " str.size: " << str.size() << '\n';
    }

    std::cout << '\n';

    {
        std::string str;
        str.resize(20);
        std::cout
            << "      str: " << mask_non_print(str) << '\n'
            << "str.c_str: " << mask_non_print(str.c_str()) << '\n'
            << " str.size: " << str.size() << '\n';
        set_name(out_c_str(str));
        std::cout
            << "      str: " << mask_non_print(str) << '\n'
            << "str.c_str: " << mask_non_print(str.c_str()) << '\n'
            << " str.size: " << str.size() << '\n';
    }

    std::cout << '\n';

    {
        std::string str;
        resize_for_out_ptr(str, 20);
        std::cout
            << "      str: " << mask_non_print(str) << '\n'
            << "str.c_str: " << mask_non_print(str.c_str()) << '\n'
            << " str.size: " << str.size() << '\n';
        set_name(out_c_str(str));
        std::cout
            << "      str: " << mask_non_print(str) << '\n'
            << "str.c_str: " << mask_non_print(str.c_str()) << '\n'
            << " str.size: " << str.size() << '\n';
    }
}
