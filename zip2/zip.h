#pragma once

#include <iterator>

template<typename ItA, typename ItB>
class ZipZip {
public:
    using ValueA = typename std::iterator_traits<ItA>::value_type;
    using ValueB = typename std::iterator_traits<ItB>::value_type;
    using ZPair = std::pair<const ValueA &, const ValueB &>;

    class ItZip {
    public:
        ItZip(ItA it_a, ItB it_b) {
            ita_ = it_a;
            itb_ = it_b;
        };

        ZPair operator*() const {
            return {*ita_, *itb_};
        };

        ItZip &operator++() {
            ++ita_;
            ++itb_;
            return *this;
        };

        bool operator!=(const ItZip &it) const {
            return (ita_ != it.ita_ || itb_ != it.itb_);
        };


    private:
        ItA ita_;
        ItB itb_;
    };

    ZipZip(ItA a_beg, ItA a_end, ItB b_beg, ItB b_end) {
        a_begin_ = a_beg;
        b_begin_ = b_beg;
        a_end_ = a_end;
        b_end_ = b_end;
    };

    ItZip begin() const {
        return ItZip(a_begin_, b_begin_);
    };

    ItZip end() const {
        ItA it_a = a_begin_;
        ItB it_b = b_begin_;
        while (it_a != a_end_ && it_b != b_end_) {
            ++it_a;
            ++it_b;
        }
        return ItZip(it_a, it_b);
    };

private:
    ItA a_begin_;
    ItA a_end_;
    ItB b_begin_;
    ItB b_end_;
};

template<typename Sequence1, typename Sequence2>
auto Zip(const Sequence1 &sequence1, const Sequence2 &sequence2) {
    return ZipZip(std::begin(sequence1), std::end(sequence1), std::begin(sequence2), std::end(sequence2));
}
