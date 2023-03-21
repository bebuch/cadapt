
struct in_c_str {
    std::string_view ref;

    bool char_const_ptr(char const* c_str) const {
        return ref == c_str;
    }

    bool char_ptr(char* c_str) const {
        return ref == c_str;
    }

    bool char_const_ptr(char const* c_str) const {
        return ref == c_str;
    }
};
