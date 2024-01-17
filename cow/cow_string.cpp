#include "cow_string.h"

CowString::Pr::Pr(CowString* cow_string, size_t idx) {
    cow_str_ = cow_string;
    idx_ = idx;
}
CowString::Pr& CowString::Pr::operator=(char ch) {
    cow_str_->update_ch(idx_, ch);
    return *this;
}
CowString::Pr::operator char() const {
    return cow_str_->str_data_->data[idx_];
}

CowString::It::It(CowString* cow_str, size_t idx) {
    cow_str_ = cow_str;
    idx_ = idx;
}
void CowString::It::operator++() {
    ++idx_;
}
CowString::It CowString::It::operator++(int) {
    return It(cow_str_, idx_ + 1);
}
CowString::Pr CowString::It::operator*() const {
    return Pr(cow_str_, idx_);
}
bool CowString::It::operator==(const It& it) const {
    return (cow_str_ == it.cow_str_ && idx_ == it.idx_);
}

CowString::It CowString::begin() {
    return It(this, 0);
}
CowString::It CowString::end() {
    return It(this, this->str_data_->size_d);
}

CowString::ConstPr::operator char() const {
    return cow_str_->str_data_->data[idx_];
}

CowString::ConstIt::ConstIt(const CowString* cow_string, size_t idx) {
    cow_string_ = cow_string;
    idx_ = idx;
}
void CowString::ConstIt::operator++() {
    ++idx_;
}
CowString::ConstIt CowString::ConstIt::operator++(int) {
    return ConstIt(cow_string_, idx_++);
}
CowString::ConstPr CowString::ConstIt::operator*() const {
    return ConstPr(cow_string_, idx_);
}
bool CowString::ConstIt::operator==(const ConstIt& it) const {
    return (cow_string_ == it.cow_string_ && idx_ == it.idx_);
}
bool CowString::ConstIt::operator!=(const ConstIt& it) const {
    return !(cow_string_ == it.cow_string_ && idx_ == it.idx_);
}

CowString::ConstIt CowString::begin() const {
    return ConstIt(this, 0);
}
CowString::ConstIt CowString::end() const {
    return ConstIt(this, this->str_data_->size_d);
}

CowString::CowString(std::string_view str_view) {
    str_data_ = new StrData();
    str_data_->cnt_r = 1;
    if (!str_view.empty()) {
        str_data_->size_d = str_view.size();
        str_data_->data = new char[str_data_->size_d];
        std::copy(str_view.begin(), str_view.end(), str_data_->data);
    }
}
CowString::CowString(const CowString& cow_string) {
    str_data_ = cow_string.str_data_;
    if (str_data_) {
        ++str_data_->cnt_r;
    }
}
CowString::CowString(CowString&& cow_string) {
    std::swap(str_data_, cow_string.str_data_);
}

CowString& CowString::operator=(const CowString& cow_string) {
    if (&cow_string == this) {
        return *this;
    }
    --str_data_->cnt_r;
    if (str_data_->cnt_r == 0) {
        delete str_data_;
    }
    str_data_ = cow_string.str_data_;
    ++str_data_->cnt_r;
    return *this;
}
CowString& CowString::operator=(CowString&& cow_string) {
    if (&cow_string == this) {
        return *this;
    }
    --str_data_->cnt_r;
    if (str_data_->cnt_r == 0) {
        delete str_data_;
    }
    str_data_ = nullptr;
    std::swap(str_data_, cow_string.str_data_);
    return *this;
}

char* CowString::GetData() {
    if (str_data_) {
        return this->str_data_->data;
    }
    return nullptr;
}
const char* CowString::GetData() const {
    if (str_data_) {
        return this->str_data_->data;
    }
    return nullptr;
}

size_t CowString::GetSize() const {
    if (str_data_) {
        return this->str_data_->size_d;
    }
    return 0;
}
bool CowString::Empty() const {
    return (GetSize() == 0);
}

CowString::Pr CowString::operator[](size_t idx) {
    return Pr(this, idx);
}
CowString::ConstPr CowString::At(size_t idx) const {
    return ConstPr(this, idx);
}

CowString::operator std::string_view() const {
    return std::string_view(GetData(), GetSize());
}

CowString& CowString::operator+=(const CowString& cow_string) {
    if (cow_string.Empty()) {
        return *this;
    }
    if (!str_data_) {
        str_data_ = new StrData();
        str_data_->cnt_r = 1;
        str_data_->size_d = cow_string.GetSize();
        str_data_->data = new char[str_data_->size_d];
        std::strncpy(str_data_->data, cow_string.GetData(), str_data_->size_d);
    } else if (str_data_->cnt_r == 1) {
        char* old_data = str_data_->data;
        size_t old_data_size = str_data_->size_d;
        str_data_->size_d = old_data_size + cow_string.GetSize();
        str_data_->data = new char[str_data_->size_d];
        std::strncpy(str_data_->data, old_data, old_data_size);
        std::strncpy(str_data_->data + old_data_size, cow_string.GetData(), cow_string.GetSize());
        delete old_data;
    } else {
        StrData* old_str_data = str_data_;
        --old_str_data->cnt_r;

        str_data_ = new StrData();
        str_data_->cnt_r = 1;
        str_data_->size_d = old_str_data->size_d + cow_string.GetSize();
        str_data_->data = new char[str_data_->size_d];
        std::strncpy(str_data_->data, old_str_data->data, old_str_data->size_d);
        std::strncpy(str_data_->data + old_str_data->size_d, cow_string.GetData(), cow_string.GetSize());
    }

    return *this;
}
CowString& CowString::operator+=(std::string_view string_view) {
    if (string_view.empty()) {
        return *this;
    }

    if (!str_data_) {
        str_data_ = new StrData();
        str_data_->cnt_r = 1;
        str_data_->size_d = string_view.size();
        str_data_->data = new char[str_data_->size_d];
        std::strncpy(str_data_->data, string_view.data(), str_data_->size_d);
    } else if (str_data_->cnt_r == 1) {
        char* old_data = str_data_->data;
        size_t old_data_size = str_data_->size_d;

        str_data_->size_d = old_data_size + string_view.size();
        str_data_->data = new char[str_data_->cnt_r];
        std::strncpy(str_data_->data, old_data, old_data_size);
        std::strncpy(str_data_->data + old_data_size, string_view.data(), string_view.size());
        delete old_data;
    } else {
        StrData* old_str_data = str_data_;
        --old_str_data->cnt_r;

        str_data_ = new StrData();
        str_data_->size_d = old_str_data->size_d + string_view.size();
        str_data_->cnt_r = 1;
        str_data_->data = new char[str_data_->size_d];
        std::strncpy(str_data_->data, old_str_data->data, old_str_data->size_d);
        std::strncpy(str_data_->data + old_str_data->size_d, string_view.data(), string_view.size());
    }
    return *this;
}

CowString::~CowString() {
    if (!str_data_) {
        return;
    }
    --str_data_->cnt_r;
    if (!str_data_->cnt_r) {
        delete str_data_;
    }
}

CowString::StrData::~StrData() {
    delete[] data;
}

void CowString::update_ch(size_t idx, char ch) {
    if (str_data_->data[idx] == ch) {
        return;
    }
    if (str_data_->cnt_r > 1) {
        StrData* old_str_data = str_data_;
        --old_str_data->cnt_r;

        str_data_ = new StrData();
        str_data_->size_d = old_str_data->size_d;
        str_data_->cnt_r = 1;
        str_data_->data = new char[str_data_->size_d];
        std::strncpy(str_data_->data, old_str_data->data, old_str_data->size_d);
    }
    str_data_->data[idx] = ch;
}

bool operator==(const CowString& cow_string_a, const CowString& cow_string_b) {
    if (cow_string_a.GetSize() != cow_string_b.GetSize()) {
        return false;
    }
    return std::strncmp(cow_string_a.GetData(), cow_string_b.GetData(), cow_string_a.GetSize()) == 0;
}
bool operator==(const CowString& cow_string, std::string_view string_view) {
    if (cow_string.GetSize() != string_view.size()) {
        return false;
    }
    return std::strncmp(cow_string.GetData(), string_view.begin(), cow_string.GetSize()) == 0;
}

CowString operator+(const CowString& cow_string_a, const CowString& cow_string_b) {
    CowString temp = cow_string_a;
    temp += cow_string_b;
    return temp;
}
CowString operator+(const CowString& cow_string, std::string_view string_view) {
    CowString temp = cow_string;
    temp += string_view;
    return temp;
}
CowString operator+(std::string_view string_view, const CowString& cow_string) {
    CowString temp(string_view);
    temp += cow_string;
    return temp;
}

