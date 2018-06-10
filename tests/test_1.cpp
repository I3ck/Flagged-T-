/*
    Copyright (c) 2016 Martin Buck
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation the rights to
    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
    DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#define CATCH_CONFIG_MAIN
#include "../dependencies/Catch.h" //https://github.com/philsquared/Catch

#include <cmath>
#include <vector>

#include "flaggedT.h"

using namespace std;
using namespace flaggedT;

//examples (move these to example files) [also the example test case]
//generator
NonNull<std::unique_ptr<int> > safe_generator()
{
    return NonNull<std::unique_ptr<int> >(std::unique_ptr<int>(new int(5)));
}

int no_need_to_check_for_null(NonNull<std::unique_ptr<int> >&& in)
{
    return *(in.get().get()) + 1;
}

int add_one(int in)
{
    return in + 1;
}

//examples (move these to example files) [also the example test case]

TEST_CASE("FlaggedT")
{
    SECTION("Immutable")
    {
        auto im = Immutable<int>(3);

        int copied = im;

        //ref not allowed
        //int& ref = im;

        int const& cref = im;

        auto safe_sharing = shared_im<int>(4);

        REQUIRE(safe_sharing.get() == 4);
    }

    SECTION("NonNull")
    {
        //won't even compile, since nullptr_t overload is deleted
        //auto wontCompile = NonNull<int*>::make_non_null(nullptr);

        int* i = nullptr;
        REQUIRE_THROWS(NonNull<int*>(std::move(i)));

        int* i2 = new int(3);
        auto nn = NonNull<int*>(std::move(i2));
        int* i2s = std::move(nn);
        delete i2s;
    }

    SECTION("NonZero")
    {
        int ifail = 0;
        float ffail = 0.0f;
        double dfail = 0.0;

        int iworks = 1;
        float fworks = 1.0f;
        double dworks = 1.0;

        REQUIRE_THROWS(NonZero<int>(std::move(ifail)));
        REQUIRE_THROWS(NonZero<float>(std::move(ffail)));
        REQUIRE_THROWS(NonZero<double>(std::move(dfail)));

        auto i = NonZero<int>(std::move(iworks));
        auto f = NonZero<float>(std::move(fworks));
        auto d = NonZero<double>(std::move(dworks));

        REQUIRE(i.get() == 1);
        REQUIRE(f.get() == 1.0f);
        REQUIRE(d.get() == 1.0);

        auto p = NonZero<double>(Positive<double>(3.0));
        REQUIRE(p.get() == 3.0);

        auto n = NonZero<double>(Negative<double>(-3.0));
        REQUIRE(n.get() == -3.0);

        auto fi = NonZero<double>(FlooredInclusive<double, 1>(3.0));
        REQUIRE(fi.get() == 3.0);

        auto fe = NonZero<double>(FlooredExclusive<double, 0>(3.0));
        REQUIRE(fe.get() == 3.0);

        auto ci = NonZero<double>(CeiledInclusive<double, -1>(-3.0));
        REQUIRE(ci.get() == -3.0);

        auto ce = NonZero<double>(CeiledExclusive<double, 0>(-3.0));
        REQUIRE(ce.get() == -3.0);

        auto bi = NonZero<double>(BoundedInclusive<double, -2, -1>(-1.5));
        REQUIRE(bi.get() == -1.5);

        auto be = NonZero<double>(BoundedExclusive<double, 2, 3>(2.5));
        REQUIRE(be.get() == 2.5);
    }

    SECTION("Positive")
    {
        int ifail = -1;
        float ffail = -1.0f;
        double dfail = -1.0;

        int ifail2 = 0;
        float ffail2 = 0.0f;
        double dfail2 = 0.0;

        int iworks = 1;
        float fworks = 1.0f;
        double dworks = 1.0;

        REQUIRE_THROWS(Positive<int>(std::move(ifail)));
        REQUIRE_THROWS(Positive<float>(std::move(ffail)));
        REQUIRE_THROWS(Positive<double>(std::move(dfail)));

        REQUIRE_THROWS(Positive<int>(std::move(ifail2)));
        REQUIRE_THROWS(Positive<float>(std::move(ffail2)));
        REQUIRE_THROWS(Positive<double>(std::move(dfail2)));

        auto i = Positive<int>(std::move(iworks));
        auto f = Positive<float>(std::move(fworks));
        auto d = Positive<double>(std::move(dworks));

        REQUIRE(i.get() == 1);
        REQUIRE(f.get() == 1.0f);
        REQUIRE(d.get() == 1.0);

        auto fi = Positive<double>(FlooredInclusive<double, 1>(3.0));
        REQUIRE(fi.get() == 3.0);

        auto fe = Positive<double>(FlooredExclusive<double, 0>(3.0));
        REQUIRE(fe.get() == 3.0);

        auto bi = Positive<double>(BoundedInclusive<double, 1, 2>(1.3));
        REQUIRE(bi.get() == 1.3);

        auto be = Positive<double>(BoundedExclusive<double, 0, 1>(0.3));
        REQUIRE(be.get() == 0.3);
    }

    SECTION("NonPositive")
    {
        int ifail = 1;
        float ffail = 1.0f;
        double dfail = 1.0;

        int iworks = -1;
        float fworks = -1.0f;
        double dworks = -1.0;

        REQUIRE_THROWS(NonPositive<int>(std::move(ifail)));
        REQUIRE_THROWS(NonPositive<float>(std::move(ffail)));
        REQUIRE_THROWS(NonPositive<double>(std::move(dfail)));

        auto i = NonPositive<int>(std::move(iworks));
        auto f = NonPositive<float>(std::move(fworks));
        auto d = NonPositive<double>(std::move(dworks));

        REQUIRE(i.get() == -1);
        REQUIRE(f.get() == -1.0f);
        REQUIRE(d.get() == -1.0);

        auto n = NonPositive<double>(Negative<double>(-3.0));
        REQUIRE(n.get() == -3.0);

        auto ci = NonPositive<double>(CeiledInclusive<double, 0>(-3.0));
        REQUIRE(ci.get() == -3.0);

        auto ce = NonPositive<double>(CeiledExclusive<double, 0>(-3.0));
        REQUIRE(ce.get() == -3.0);

        auto bi = NonPositive<double>(BoundedInclusive<double, -2, -1>(-1.3));
        REQUIRE(bi.get() == -1.3);

        auto be = NonPositive<double>(BoundedExclusive<double, -1, 0>(-0.3));
        REQUIRE(be.get() == -0.3);
    }

    SECTION("Negative")
    {
        int ifail = 1;
        float ffail = 1.0f;
        double dfail = 1.0;

        int ifail2 = 0;
        float ffail2 = 0.0f;
        double dfail2 = 0.0;

        int iworks = -1;
        float fworks = -1.0f;
        double dworks = -1.0;

        REQUIRE_THROWS(Negative<int>(std::move(ifail)));
        REQUIRE_THROWS(Negative<float>(std::move(ffail)));
        REQUIRE_THROWS(Negative<double>(std::move(dfail)));

        REQUIRE_THROWS(Negative<int>(std::move(ifail2)));
        REQUIRE_THROWS(Negative<float>(std::move(ffail2)));
        REQUIRE_THROWS(Negative<double>(std::move(dfail2)));

        auto i = Negative<int>(std::move(iworks));
        auto f = Negative<float>(std::move(fworks));
        auto d = Negative<double>(std::move(dworks));

        REQUIRE(i.get() == -1);
        REQUIRE(f.get() == -1.0f);
        REQUIRE(d.get() == -1.0);

        auto ci = Negative<double>(CeiledInclusive<double, -1>(-3.0));
        REQUIRE(ci.get() == -3.0);

        auto ce = Negative<double>(CeiledExclusive<double, 0>(-3.0));
        REQUIRE(ce.get() == -3.0);

        auto bi = Negative<double>(BoundedInclusive<double, -2, -1>(-1.3));
        REQUIRE(bi.get() == -1.3);

        auto be = Negative<double>(BoundedExclusive<double, -1, 0>(-0.3));
        REQUIRE(be.get() == -0.3);
    }

    SECTION("NonNegative")
    {
        int ifail = -1;
        float ffail = -1.0f;
        double dfail = -1.0;

        int iworks = 1;
        float fworks = 1.0f;
        double dworks = 1.0;

        REQUIRE_THROWS(NonNegative<int>(std::move(ifail)));
        REQUIRE_THROWS(NonNegative<float>(std::move(ffail)));
        REQUIRE_THROWS(NonNegative<double>(std::move(dfail)));

        auto i = NonNegative<int>(std::move(iworks));
        auto f = NonNegative<float>(std::move(fworks));
        auto d = NonNegative<double>(std::move(dworks));

        REQUIRE(i.get() == 1);
        REQUIRE(f.get() == 1.0f);
        REQUIRE(d.get() == 1.0);

        auto p = NonNegative<double>(Positive<double>(3.0));
        REQUIRE(p.get() == 3.0);

        auto fi = NonNegative<double>(FlooredInclusive<double, 1>(3.0));
        REQUIRE(fi.get() == 3.0);

        auto fe = NonNegative<double>(FlooredExclusive<double, 0>(3.0));
        REQUIRE(fe.get() == 3.0);

        auto bi = NonNegative<double>(BoundedInclusive<double, 1, 2>(1.3));
        REQUIRE(bi.get() == 1.3);

        auto be = NonNegative<double>(BoundedExclusive<double, 0, 1>(0.3));
        REQUIRE(be.get() == 0.3);
    }

    SECTION("CeiledInclusive") {
        constexpr int iMax = 5;
        using iCeiledIncl = CeiledInclusive<int, iMax>;

        REQUIRE_THROWS(iCeiledIncl(6));
        auto incl = iCeiledIncl(5);
        REQUIRE(incl.get() == 5);

        auto n = CeiledInclusive<int, 1>(Negative<int>(-2));
        REQUIRE(n.get() == -2);

        auto np = CeiledInclusive<int, 1>(NonPositive<int>(-2));
        REQUIRE(np.get() == -2);

        auto bi = CeiledInclusive<int, 1>(BoundedInclusive<int, -3, 1>(-2));
        REQUIRE(bi.get() == -2);

        auto be = CeiledInclusive<int, 1>(BoundedExclusive<int, -3, 1>(-2));
        REQUIRE(be.get() == -2);
    }

    SECTION("CeiledExclusive") {
        constexpr int iMax = 5;
        using iCeiledExcl = CeiledExclusive<int, iMax>;

        REQUIRE_THROWS(iCeiledExcl(5));
        auto excl = iCeiledExcl(4);
        REQUIRE(excl.get() == 4);

        auto n = CeiledExclusive<int, 1>(Negative<int>(-2));
        REQUIRE(n.get() == -2);

        auto np = CeiledExclusive<int, 1>(NonPositive<int>(-2));
        REQUIRE(np.get() == -2);

        auto bi = CeiledExclusive<int, 1>(BoundedInclusive<int, -3, 0>(-2));
        REQUIRE(bi.get() == -2);

        auto be = CeiledExclusive<int, 1>(BoundedExclusive<int, -3, 1>(-2));
        REQUIRE(be.get() == -2);
    }

    SECTION("FlooredInclusive") {
        constexpr int iMin = 2;
        using iFlooredIncl = FlooredInclusive<int, iMin>;

        REQUIRE_THROWS(iFlooredIncl(1));
        auto incl = iFlooredIncl(2);
        REQUIRE(incl.get() == 2);

        auto p = FlooredInclusive<int, -1>(Positive<int>(2));
        REQUIRE(p.get() == 2);

        auto nn = FlooredInclusive<int, -1>(NonNegative<int>(2));
        REQUIRE(nn.get() == 2);

        auto bi = FlooredInclusive<int, -3>(BoundedInclusive<int, -3, 0>(-2));
        REQUIRE(bi.get() == -2);

        auto be = FlooredInclusive<int, -3>(BoundedExclusive<int, -3, 1>(-2));
        REQUIRE(be.get() == -2);
    }

    SECTION("FlooredExclusive") {
        constexpr int iMin = 2;
        using iFlooredExcl = FlooredExclusive<int, iMin>;

        REQUIRE_THROWS(iFlooredExcl(2));
        auto excl = iFlooredExcl(3);
        REQUIRE(excl.get() == 3);

        auto p = FlooredExclusive<int, -1>(Positive<int>(2));
        REQUIRE(p.get() == 2);

        auto nn = FlooredExclusive<int, -1>(NonNegative<int>(2));
        REQUIRE(nn.get() == 2);

        auto bi = FlooredExclusive<int, -4>(BoundedInclusive<int, -3, 0>(-2));
        REQUIRE(bi.get() == -2);

        auto be = FlooredExclusive<int, -3>(BoundedExclusive<int, -3, 1>(-2));
        REQUIRE(be.get() == -2);
    }

    SECTION("BoundedInclusive") {
        constexpr int iMin = 3;
        constexpr int iMax = 5;
        using iBoundedIncl = BoundedInclusive<int, iMin, iMax>;

        REQUIRE_THROWS(iBoundedIncl(2));
        REQUIRE_THROWS(iBoundedIncl(6));

        auto incl1 = iBoundedIncl(3);
        REQUIRE(incl1.get() == 3);

        auto incl2 = iBoundedIncl(5);
        REQUIRE(incl2.get() == 5);

        auto be = BoundedInclusive<int, 0, 3>(BoundedExclusive<int, 0, 3>(2));
        REQUIRE(be.get() == 2);
    }

    SECTION("BoundedExclusive") {
        constexpr int iMin = 3;
        constexpr int iMax = 5;
        using iBoundedExcl = BoundedExclusive<int, iMin, iMax>;

        REQUIRE_THROWS(iBoundedExcl(3));
        REQUIRE_THROWS(iBoundedExcl(5));

        auto excl = iBoundedExcl(4);
        REQUIRE(excl.get() == 4);

        auto b = BoundedExclusive<int, 0, 3>(BoundedInclusive<int, 1, 2>(2));
        REQUIRE(b.get() == 2);
    }

    SECTION("NonEmpty")
    {
        using v = std::vector<int>;
        auto emptyVec = v();
        REQUIRE_THROWS(NonEmpty<v>(std::move(emptyVec)));

        auto goodVec = v({ 1, 2 });
        auto ne = NonEmpty<v>(v(goodVec));
        REQUIRE(ne.get().size() == 2);

        auto mt = NonEmpty<v>(MoreThan<v, 1>(v(goodVec)));
        REQUIRE(mt.get().size() == 2);

        auto fs = NonEmpty<v>(FixedSized<v, 2>(v(goodVec)));
        REQUIRE(fs.get().size() == 2);

        auto fri = NonEmpty<v>(FixedRangeInclusive<v, 2, 4>(v(goodVec)));
        REQUIRE(fri.get().size() == 2);
    }

    SECTION("MoreThan")
    {
        using v = std::vector<int>;
        auto tooSmall = v({ 1, 2, 3 });
        using more3 = MoreThan<v, 3>;
        REQUIRE_THROWS(more3(std::move(tooSmall)));

        auto bigEnough = v({ 1, 2, 3, 4 });
        auto works = more3(v(bigEnough));
        REQUIRE(works.get().size() == 4);

        auto fs = MoreThan<v, 3>(FixedSized<v, 4>(v(bigEnough)));
        REQUIRE(fs.get().size() == 4);

        auto fri = MoreThan<v, 3>(FixedRangeInclusive<v, 4, 4>(v(bigEnough)));
        REQUIRE(fri.get().size() == 4);
    }

    SECTION("LessThan")
    {
        using v = std::vector<int>;
        auto tooBig = v({ 1, 2, 3 });
        using less3 = LessThan<v, 3>;
        REQUIRE_THROWS(less3(std::move(tooBig)));

        auto smallEnough = std::vector<int>({ 1, 2 });
        auto works = less3(v(smallEnough));
        REQUIRE(works.get().size() == 2);

        auto fs = LessThan<v, 3>(FixedSized<v, 2>(v(smallEnough)));
        REQUIRE(fs.get().size() == 2);

        auto fri = LessThan<v, 3>(FixedRangeInclusive<v, 1, 2>(v(smallEnough)));
        REQUIRE(fri.get().size() == 2);
    }

    SECTION("FixedSized")
    {
        auto tooBig = std::vector<int>({ 1, 2, 3 });
        auto tooSmall = std::vector<int>({ 1 });
        auto correct = std::vector<int>({ 1, 2 });
        using fixed2 = FixedSized<std::vector<int>, 2>;
        REQUIRE_THROWS(fixed2(std::move(tooBig)));
        REQUIRE_THROWS(fixed2(std::move(tooSmall)));

        auto works = fixed2(std::move(correct));
        REQUIRE(works.get().size() == 2);
    }

    SECTION("FixedRangeInclusive")
    {
        using v = std::vector<int>;
        constexpr size_t nMin = 3;
        constexpr size_t nMax = 5;

        using rangedIncl = FixedRangeInclusive<v, nMin, nMax>;

        REQUIRE_THROWS(rangedIncl({1,2}));
        REQUIRE_THROWS(rangedIncl({1,2,3,4,5,6}));

        auto incl1 = rangedIncl({1,2,3});
        REQUIRE(incl1.get().size() == 3);

        auto incl2 = rangedIncl({1,2,3,4,5});
        REQUIRE(incl2.get().size() == 5);

        auto fs = FixedRangeInclusive<v, 1, 3>(FixedSized<v, 2>({1, 2}));
        REQUIRE(fs.get().size() == 2);
    }

    SECTION("Sorted")
    {
        std::vector<int> unsorted({ 1, 7, 8, 14, 3 });

        REQUIRE(!std::is_sorted(unsorted.begin(), unsorted.end()));

        auto sorted = Sorted<std::vector<int> >(std::move(unsorted));
        REQUIRE(std::is_sorted(sorted.get().begin(), sorted.get().end()));
    }

    SECTION("UNIQUE")
    {
        std::vector<int> duped({ 1, 1, 2, 2, 3, 3, 7, 9, 11 });

        auto unduped = Unique<std::vector<int> >(std::move(duped));
        REQUIRE(unduped.get().size() == 6);
    }

    SECTION("UNIQUESORTED")
    {
        std::vector<int> dupedUnsorted({ 11, 9, 7, 3, 3, 2, 2, 1, 1 });

        auto fixed = UniqueAndSorted<std::vector<int> >(std::move(dupedUnsorted));
        REQUIRE(std::is_sorted(fixed.get().begin(), fixed.get().end()));
        REQUIRE(fixed.get().size() == 6);
    }

    SECTION("Move this to an example file")
    {
        auto tmp = safe_generator();
        auto res = no_need_to_check_for_null(std::move(tmp));

        auto autoUnwrap = NonNegative<int>(3);
        int result = add_one(autoUnwrap);
    }
}
