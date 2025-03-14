// OptFrame 4.2 - Optimization Framework
// Copyright (C) 2009-2021 - MIT LICENSE
// https://github.com/optframe/optframe
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef OPTFRAME_MULTIESOLUTION_CTEST_HPP_
#define OPTFRAME_MULTIESOLUTION_CTEST_HPP_

#include <OptFrame/Concepts/BaseConcepts.ctest.hpp>
#include <OptFrame/Core/MultiESolution.hpp>

#ifndef NDEBUG

namespace optframe {

// basic compilation test (TODO: improve)
// static_assert(X2ESolution<MultiESolution<IsESolution<double>>,
// IsSolution<double>, IsEvaluation<int>, IsESolution<double>>);
static_assert(
    X2ESolution<MultiESolution<IsESolution<double>>, IsESolution<double>>);

}  // namespace optframe

#endif  // NDEBUG

#endif /* OPTFRAME_MULTISOLUTION_CTEST_HPP_ */
