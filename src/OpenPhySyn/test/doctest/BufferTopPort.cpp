// BSD 3-Clause License

// Copyright (c) 2019, SCALE Lab, Brown University
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.

// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.

// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
#include "Psn.hpp"
#include "PsnException.hpp"

#include "FileUtils.hpp"
#include "Readers.hpp"
#include "doctest.h"

using namespace psn;

TEST_CASE("Should buffers to the top-level port")
{
    Psn& psn_inst = Psn::instance();
    try
    {
        psn_inst.clearDatabase();
        psn::readLiberty(psn_inst.sta(), "../test/data/libraries/Nangate45/"
                                         "NangateOpenCellLibrary_typical.lib");
        psn::readLef(psn_inst.database(), psn_inst.sta(),
                     "../test/data/libraries/Nangate45/"
                     "NangateOpenCellLibrary.mod.lef",
                     "nangate45", true, true);
        psn::readDef(psn_inst.database(), psn_inst.sta(),
                     "../test/data/designs/fanout/fanout_nan_top.def");
        CHECK(psn_inst.database()->getChip() != nullptr);
        CHECK(psn_inst.hasTransform("buffer_fanout"));
        auto result = psn_inst.runTransform(
            "buffer_fanout", std::vector<std::string>({"2", "BUF_X1"}));
        CHECK(result == 13);
    }
    catch (PsnException& e)
    {
        FAIL(e.what());
    }
}
