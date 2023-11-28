#ifndef VERILOG_MUX_H
#define VERILOG_MUX_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include <vector>

#include "circuit_library.h"
#include "fabric_verilog_options.h"
#include "module_manager.h"
#include "module_name_map.h"
#include "mux_graph.h"
#include "mux_library.h"
#include "netlist_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_submodule_muxes(
  ModuleManager& module_manager, NetlistManager& netlist_manager,
  const MuxLibrary& mux_lib, const CircuitLibrary& circuit_lib,
  const ModuleNameMap& module_name_map, const std::string& submodule_dir,
  const std::string& submodule_dir_name, const FabricVerilogOption& options);

} /* end namespace openfpga */

#endif
