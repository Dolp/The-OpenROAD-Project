// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "opendb/db.h"
#include "opendb/wOrder.h"
#include "opendb/lefin.h"
#include "opendb/defin.h"
#include "opendb/defout.h"

#include "Machine.hh"
#include "VerilogWriter.hh"
#include "StaMain.hh"

#include "db_sta/dbSta.hh"
#include "db_sta/MakeDbSta.hh"

#include "db_sta/dbReadVerilog.hh"
#include "openroad/OpenRoad.hh"
#include "openroad/InitOpenRoad.hh"
#include "flute3/flute.h"

#include "init_fp//MakeInitFloorplan.hh"
#include "ioPlacer/src/MakeIoplacer.h"
#include "resizer/MakeResizer.hh"
#include "opendp/MakeOpendp.h"
#include "tritonmp/MakeTritonMp.h"
#include "replace/src/MakeReplace.h"
#include "FastRoute/src/MakeFastRoute.h"
#include "TritonCTS/src/MakeTritoncts.h"
#include "tapcell/MakeTapcell.h"
#include "OpenRCX/MakeOpenRCX.h"
#include "tool/MakeTool.hh"

namespace sta {
extern const char *openroad_tcl_inits[];
}

// Swig uses C linkage for init functions.
extern "C" {
extern int Openroad_Init(Tcl_Interp *interp);
extern int Opendbtcl_Init(Tcl_Interp *interp);
}

namespace ord {

using odb::dbLib;
using odb::dbDatabase;
using sta::evalTclInit;
using sta::dbSta;
using sta::Resizer;

OpenRoad *OpenRoad::openroad_ = nullptr;

OpenRoad::OpenRoad()
{
  openroad_ = this;
}

OpenRoad::~OpenRoad()
{
  deleteDbVerilogNetwork(verilog_network_);
  deleteDbSta(sta_);
  deleteResizer(resizer_);
  deleteOpendp(opendp_);
  odb::dbDatabase::destroy(db_);
}

sta::dbNetwork *
OpenRoad::getDbNetwork()
{
  return sta_->getDbNetwork();
}

////////////////////////////////////////////////////////////////

void
initOpenRoad(Tcl_Interp *interp)
{
  OpenRoad *openroad = new OpenRoad;
  openroad->init(interp);
}

void
OpenRoad::init(Tcl_Interp *tcl_interp)
{
  tcl_interp_ = tcl_interp;

  // Make components.
  db_ = dbDatabase::create();
  sta_ = makeDbSta();
  verilog_network_ = makeDbVerilogNetwork();
  ioPlacer_ = (ioPlacer::IOPlacementKernel*) makeIoplacer();
  resizer_ = makeResizer();
  opendp_ = makeOpendp();
  fastRoute_ = (FastRoute::FastRouteKernel*) makeFastRoute();
  tool_ = makeTool();

  tritonCts_ = makeTritonCts();
  tapcell_ = makeTapcell();
  tritonMp_ = makeTritonMp();
  extractor_ = makeOpenRCX();
  replace_ = makeReplace();

  // Init components.
  Openroad_Init(tcl_interp);
  // Import TCL scripts.
  evalTclInit(tcl_interp, sta::openroad_tcl_inits);

  Opendbtcl_Init(tcl_interp);
  initInitFloorplan(this);
  Flute::readLUT();
  initDbSta(this);
  initResizer(this);
  initDbVerilogNetwork(this);
  initIoplacer(this);
  initReplace(this);
  initOpendp(this);
  initFastRoute(this);
  initTritonCts(this);
  initTapcell(this);
  initTritonMp(this);
  initOpenRCX(this);
  initTool(this);
  
  // Import exported commands to global namespace.
  Tcl_Eval(tcl_interp, "sta::define_sta_cmds");
  Tcl_Eval(tcl_interp, "namespace import sta::*");
}

////////////////////////////////////////////////////////////////

void
OpenRoad::readLef(const char *filename,
		  const char *lib_name,
		  bool make_tech,
		  bool make_library)
{
  odb::lefin lef_reader(db_, false);
  if (make_tech && make_library) {
    dbLib *lib = lef_reader.createTechAndLib(lib_name, filename);
    if (lib)
      sta_->readLefAfter(lib);
  }
  else if (make_tech)
    lef_reader.createTech(filename);
  else if (make_library) {
    dbLib *lib = lef_reader.createLib(lib_name, filename);
    if (lib)
      sta_->readLefAfter(lib);
  }
}

void
OpenRoad::readDef(const char *filename, bool order_wires)
{
  odb::defin def_reader(db_);
  std::vector<odb::dbLib *> search_libs;
  for (odb::dbLib *lib : db_->getLibs())
    search_libs.push_back(lib);
  def_reader.createChip(search_libs, filename);
  if (order_wires) {
    odb::orderWires(db_->getChip()->getBlock(),
                    nullptr /* net_name_or_id*/,
                    false /* force */);
  }
  sta_->readDefAfter();
}

static odb::defout::Version
stringToDefVersion(string version)
{
  if (version == "5.8")
    return odb::defout::Version::DEF_5_8;
  else if (version == "5.6")
    return odb::defout::Version::DEF_5_6;
  else if (version == "5.5")
    return odb::defout::Version::DEF_5_5;
  else if (version == "5.4")
    return odb::defout::Version::DEF_5_4;
  else if (version == "5.3")
    return odb::defout::Version::DEF_5_3;
  else 
    return odb::defout::Version::DEF_5_8;
}

void
OpenRoad::writeDef(const char *filename,
		   string version)
{
  odb::dbChip *chip = db_->getChip();
  if (chip) {
    odb::dbBlock *block = chip->getBlock();
    if (block) {
      odb::defout def_writer;
      def_writer.setVersion(stringToDefVersion(version));
      def_writer.writeBlock(block, filename);
    }
  }
}

void
OpenRoad::readDb(const char *filename)
{
  FILE *stream = fopen(filename, "r");
  if (stream) {
    db_->read(stream);
    sta_->readDbAfter();
    fclose(stream);
  }
}

void
OpenRoad::writeDb(const char *filename)
{
  FILE *stream = fopen(filename, "w");
  if (stream) {
    db_->write(stream);
    fclose(stream);
  }
}

void
OpenRoad::readVerilog(const char *filename)
{
  dbReadVerilog(filename, verilog_network_);
}

void
OpenRoad::linkDesign(const char *design_name)

{
  dbLinkDesign(design_name, verilog_network_, db_);
  sta_->readDbAfter();
}

void
OpenRoad::writeVerilog(const char *filename,
		       bool sort)
{
  sta::writeVerilog(filename, sort, sta_->network());
}

} // namespace
