/*
 *   Copyright (c) 2010, Michael Lehn
 *
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   1) Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2) Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the
 *      distribution.
 *   3) Neither the name of the FLENS development group nor the names of
 *      its contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cxxstd/fstream.h>
#include <flens/debug/auxiliary/closurelog.h>
#include <flens/debug/auxiliary/closurelogstream.h>
#include <flens/debug/auxiliary/variablepool.h>
#include <flens/typedefs.h>

namespace flens { namespace verbose {

ClosureLogStream::ClosureLogStream(VariablePool &variablePool,
                                   std::ofstream &out)
    : variablePool_(variablePool), out_(out)
{
}

ClosureLogStream &
operator<<(ClosureLogStream &clStream, Side side)
{
    if (side==Left) {
        clStream.out_ << "Left";
        return clStream;
    }
    if (side==Right) {
        clStream.out_ << "Right";
        return clStream;
    }
    clStream.out_ << "?";
    return clStream;
}

ClosureLogStream &
operator<<(ClosureLogStream &clStream, Transpose trans)
{
    if (trans==NoTrans) {
        clStream.out_ << "NoTrans";
        return clStream;
    }
    if (trans==Conj) {
        clStream.out_ << "Conj";
        return clStream;
    }
    if (trans==Trans) {
        clStream.out_ << "Trans";
        return clStream;
    }
    clStream.out_ << "ConjTrans";
    return clStream;
}

ClosureLogStream &
operator<<(ClosureLogStream &clStream, const char *msg)
{
    clStream.out_ << msg;
    return clStream;
}

} } // namespace verbose, namespace flens
