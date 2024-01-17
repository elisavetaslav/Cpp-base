#pragma once

#include <algorithm>
#include <cctype>
#include <cstring>
#include <string_view>

class CowString {
public:
    class Pr {
    public:
        Pr(CowString* cow_string, size_t idx);
        Pr& operator=(char ch);
        operator char() const;

    private:
        CowString* cow_str_;
        size_t idx_;
    };

    class It {
    public:
        It(CowString* cow_str, size_t idx);

        void operator++();
        It operator++(int);

        Pr operator*() const;

        bool operator==(const It& it) const;

    private:
        CowString* cow_str_;
        size_t idx_;
    };

    It begin();
    It end();

    class ConstPr {
    public:
        ConstPr(const CowString* cow_str, size_t idx) : cow_str_(cow_str), idx_(idx){};
        operator char() const;

    private:
        const CowString* cow_str_;
        size_t idx_;
    };

    class ConstIt {
    public:
        ConstIt(const CowString* cow_string, size_t idx);

        void operator++();
        ConstIt operator++(int);

        ConstPr operator*() const;

        bool operator==(const ConstIt& it) const;
        bool operator!=(const ConstIt& it) const;

    private:
        const CowString* cow_string_;
        size_t idx_;
    };

    ConstIt begin() const;
    ConstIt end() const;

    explicit CowString(std::string_view str_view);
    CowString(const CowString& cow_string);
    CowString(CowString&& cow_string);

    CowString& operator=(const CowString& cow_string);
    CowString& operator=(CowString&& cow_string);

    char* GetData();
    const char* GetData() const;

    size_t GetSize() const;
    bool Empty() const;

    Pr operator[](size_t idx);
    ConstPr At(size_t idx) const;

    operator std::string_view() const;

    CowString& operator+=(const CowString& cow_string);
    CowString& operator+=(std::string_view string_view);

    ~CowString();

private:
    struct StrData {
        char* data = nullptr;
        size_t size_d = 0;
        size_t cnt_r = 0;
        ~StrData();
    };

    StrData* str_data_ = nullptr;

    void update_ch(size_t idx, char ch);
};

bool operator==(const CowString& cow_string_a, const CowString& cow_string_b);
bool operator==(const CowString& cow_string, std::string_view string_view);

CowString operator+(const CowString& cow_string_a, const CowString& cow_string_b);
CowString operator+(const CowString& cow_string, std::string_view string_view);
CowString operator+(std::string_view string_view, const CowString& cow_string);

