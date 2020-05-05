/********************************************************************
 * This file includes functions to compress the hierachy of routing architecture
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_time.h"
#include "vtr_log.h"

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

/* Headers from openfpgautil library */
#include "openfpga_scale.h"
#include "openfpga_digest.h"

#include "circuit_library_utils.h"
#include "pnr_sdc_writer.h"
#include "analysis_sdc_writer.h"
#include "openfpga_sdc.h"

/* Include global variables of VPR */
#include "globals.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * A wrapper function to call the PnR SDC generator of FPGA-SDC
 *******************************************************************/
int write_pnr_sdc(OpenfpgaContext& openfpga_ctx,
                  const Command& cmd, const CommandContext& cmd_context) {

  CommandOptionId opt_output_dir = cmd.option("file");
  CommandOptionId opt_flatten_names = cmd.option("flatten_names");
  CommandOptionId opt_hierarchical = cmd.option("hierarchical");
  CommandOptionId opt_time_unit = cmd.option("time_unit");
  CommandOptionId opt_constrain_global_port = cmd.option("constrain_global_port");
  CommandOptionId opt_constrain_non_clock_global_port = cmd.option("constrain_non_clock_global_port");
  CommandOptionId opt_constrain_grid = cmd.option("constrain_grid");
  CommandOptionId opt_constrain_sb = cmd.option("constrain_sb");
  CommandOptionId opt_constrain_cb = cmd.option("constrain_cb");
  CommandOptionId opt_constrain_configurable_memory_outputs = cmd.option("constrain_configurable_memory_outputs");
  CommandOptionId opt_constrain_routing_multiplexer_outputs = cmd.option("constrain_routing_multiplexer_outputs");
  CommandOptionId opt_constrain_switch_block_outputs = cmd.option("constrain_switch_block_outputs");
  CommandOptionId opt_constrain_zero_delay_paths = cmd.option("constrain_zero_delay_paths");

  /* This is an intermediate data structure which is designed to modularize the FPGA-SDC
   * Keep it independent from any other outside data structures
   */
  std::string sdc_dir_path = format_dir_path(cmd_context.option_value(cmd, opt_output_dir));

  /* Create directories */
  create_directory(sdc_dir_path);

  PnrSdcOption options(sdc_dir_path);

  options.set_flatten_names(cmd_context.option_enable(cmd, opt_flatten_names));
  options.set_hierarchical(cmd_context.option_enable(cmd, opt_hierarchical));
  
  if (true == cmd_context.option_enable(cmd, opt_time_unit)) {
    options.set_time_unit(string_to_time_unit(cmd_context.option_value(cmd, opt_time_unit)));
  }

  options.set_constrain_global_port(cmd_context.option_enable(cmd, opt_constrain_global_port));
  options.set_constrain_non_clock_global_port(cmd_context.option_enable(cmd, opt_constrain_non_clock_global_port));
  options.set_constrain_grid(cmd_context.option_enable(cmd, opt_constrain_grid));
  options.set_constrain_sb(cmd_context.option_enable(cmd, opt_constrain_sb));
  options.set_constrain_cb(cmd_context.option_enable(cmd, opt_constrain_cb));
  options.set_constrain_configurable_memory_outputs(cmd_context.option_enable(cmd, opt_constrain_configurable_memory_outputs));
  options.set_constrain_routing_multiplexer_outputs(cmd_context.option_enable(cmd, opt_constrain_routing_multiplexer_outputs));
  options.set_constrain_switch_block_outputs(cmd_context.option_enable(cmd, opt_constrain_switch_block_outputs));
  options.set_constrain_zero_delay_paths(cmd_context.option_enable(cmd, opt_constrain_zero_delay_paths));

  /* We first turn on default sdc option and then disable part of them by following users' options */
  if (false == options.generate_sdc_pnr()) {
    options.set_generate_sdc_pnr(true);
  }

  /* Collect global ports from the circuit library:
   * TODO: should we place this in the OpenFPGA context?
   */
  std::vector<CircuitPortId> global_ports = find_circuit_library_global_ports(openfpga_ctx.arch().circuit_lib);
  
  /* Execute only when sdc is enabled */
  if (true == options.generate_sdc_pnr()) { 
    print_pnr_sdc(options,
                  1./openfpga_ctx.arch().sim_setting.programming_clock_frequency(),
                  1./openfpga_ctx.arch().sim_setting.operating_clock_frequency(),
                  g_vpr_ctx.device(),
                  openfpga_ctx.vpr_device_annotation(),
                  openfpga_ctx.device_rr_gsb(),
                  openfpga_ctx.module_graph(),
                  openfpga_ctx.mux_lib(),
                  openfpga_ctx.arch().circuit_lib,
                  global_ports,
                  openfpga_ctx.flow_manager().compress_routing());
  }

  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
} 

/********************************************************************
 * A wrapper function to call the analysis SDC generator of FPGA-SDC
 *******************************************************************/
int write_analysis_sdc(OpenfpgaContext& openfpga_ctx,
                       const Command& cmd, const CommandContext& cmd_context) {

  CommandOptionId opt_output_dir = cmd.option("file");
  CommandOptionId opt_flatten_names = cmd.option("flatten_names");
  CommandOptionId opt_time_unit = cmd.option("time_unit");

  /* This is an intermediate data structure which is designed to modularize the FPGA-SDC
   * Keep it independent from any other outside data structures
   */
  std::string sdc_dir_path = format_dir_path(cmd_context.option_value(cmd, opt_output_dir));

  /* Create directories */
  create_directory(sdc_dir_path);

  AnalysisSdcOption options(sdc_dir_path);
  options.set_generate_sdc_analysis(true);
  options.set_flatten_names(cmd_context.option_enable(cmd, opt_flatten_names));

  if (true == cmd_context.option_enable(cmd, opt_time_unit)) {
    options.set_time_unit(string_to_time_unit(cmd_context.option_value(cmd, opt_time_unit)));
  }

  /* Collect global ports from the circuit library:
   * TODO: should we place this in the OpenFPGA context?
   */
  std::vector<CircuitPortId> global_ports = find_circuit_library_global_ports(openfpga_ctx.arch().circuit_lib);

  if (true == options.generate_sdc_analysis()) {
    print_analysis_sdc(options,
                       1./openfpga_ctx.arch().sim_setting.operating_clock_frequency(),
                       g_vpr_ctx, 
                       openfpga_ctx,
                       global_ports,
                       openfpga_ctx.flow_manager().compress_routing());
  }

  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
