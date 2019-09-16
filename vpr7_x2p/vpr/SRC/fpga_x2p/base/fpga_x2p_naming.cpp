/********************************************************************
 * This file includes functions to 
 * generate module/port names for Verilog 
 * and SPICE netlists 
 *
 * IMPORTANT: keep all the naming functions in this file to be 
 * generic for both Verilog and SPICE generators 
 ********************************************************************/
#include "vtr_assert.h"

#include "fpga_x2p_naming.h"

/************************************************
 * Generate the node name for a multiplexing structure 
 * Case 1 : If there is an intermediate buffer followed by,
 *          the node name will be mux_l<node_level>_in_buf
 * Case 1 : If there is NO intermediate buffer followed by,
 *          the node name will be mux_l<node_level>_in
 ***********************************************/
std::string generate_verilog_mux_node_name(const size_t& node_level, 
                                           const bool& add_buffer_postfix) {
  /* Generate the basic node_name */
  std::string node_name = "mux_l" + std::to_string(node_level) + "_in";

  /* Add a postfix upon requests */
  if (true == add_buffer_postfix)  {
    /* '1' indicates that the location is needed */
    node_name += "_buf";
  }

  return node_name;
}

/************************************************
 * Generate the module name for a multiplexer in Verilog format
 * Different circuit model requires different names: 
 * 1. LUTs are named as <model_name>_mux
 * 2. MUXes are named as <model_name>_size<num_inputs>
 ***********************************************/
std::string generate_verilog_mux_subckt_name(const CircuitLibrary& circuit_lib, 
                                             const CircuitModelId& circuit_model, 
                                             const size_t& mux_size, 
                                             const std::string& postfix) {
  std::string module_name = circuit_lib.model_name(circuit_model); 
  /* Check the model type and give different names */
  if (SPICE_MODEL_MUX == circuit_lib.model_type(circuit_model)) {
    module_name += "_size";
    module_name += std::to_string(mux_size);
  } else {  
    VTR_ASSERT(SPICE_MODEL_LUT == circuit_lib.model_type(circuit_model));
    module_name += "_mux";
  }

  /* Add postfix if it is not empty */
  if (!postfix.empty()) {
    module_name += postfix;
  }

  return module_name;
}

/************************************************
 * Generate the module name of a branch for a
 * multiplexer in Verilog format
 ***********************************************/
std::string generate_verilog_mux_branch_subckt_name(const CircuitLibrary& circuit_lib, 
                                                    const CircuitModelId& circuit_model, 
                                                    const size_t& mux_size, 
                                                    const size_t& branch_mux_size, 
                                                    const std::string& postfix) {
  /* If the tgate spice model of this MUX is a MUX2 standard cell,
   * the mux_subckt name will be the name of the standard cell
   */
  CircuitModelId subckt_model = circuit_lib.pass_gate_logic_model(circuit_model);
  if (SPICE_MODEL_GATE == circuit_lib.model_type(subckt_model)) {
    VTR_ASSERT (SPICE_MODEL_GATE_MUX2 == circuit_lib.gate_type(subckt_model));
    return circuit_lib.model_name(subckt_model);
  }
  std::string branch_postfix = postfix + "_size" + std::to_string(branch_mux_size);

  return generate_verilog_mux_subckt_name(circuit_lib, circuit_model, mux_size, branch_postfix);
}

/************************************************
 * Generate the module name of a local decoder
 * for multiplexer
 ***********************************************/
std::string generate_mux_local_decoder_subckt_name(const size_t& addr_size, 
                                                   const size_t& data_size) {
  std::string subckt_name = "decoder";
  subckt_name += std::to_string(addr_size);
  subckt_name += "to";
  subckt_name += std::to_string(data_size);

  return subckt_name;
} 

/************************************************
 * Generate the module name of a routing track wire
 ***********************************************/
std::string generate_segment_wire_subckt_name(const std::string& wire_model_name, 
                                              const size_t& segment_id) {
  std::string segment_wire_subckt_name = wire_model_name + "_seg" + std::to_string(segment_id);

  return segment_wire_subckt_name;
} 

/*********************************************************************
 * Generate the port name for the mid-output of a routing track wire
 * Mid-output is the output that is wired to a Connection block multiplexer.
 *      
 *                  |    CLB     |
 *                  +------------+
 *                        ^
 *                        |
 *           +------------------------------+
 *           | Connection block multiplexer |
 *           +------------------------------+
 *                        ^
 *                        |  mid-output         +--------------
 *              +--------------------+          |
 *    input --->| Routing track wire |--------->| Switch Block
 *              +--------------------+  output  |
 *                                              +--------------
 *
 ********************************************************************/
std::string generate_segment_wire_mid_output_name(const std::string& regular_output_name) {
  /* TODO: maybe have a postfix? */
  return std::string("mid_" + regular_output_name);
} 

/*********************************************************************
 * Generate the module name for a memory sub-circuit 
 ********************************************************************/
std::string generate_memory_module_name(const CircuitLibrary& circuit_lib,
                                        const CircuitModelId& circuit_model, 
                                        const CircuitModelId& sram_model, 
                                        const std::string& postfix) {
  return std::string( circuit_lib.model_name(circuit_model) + "_" + circuit_lib.model_name(sram_model) + postfix );
}

/*********************************************************************
 * Generate the netlist name for a unique routing block 
 * It could be 
 * 1. Routing channel
 * 2. Connection block
 * 3. Switch block
 * A unique block id should be given
 *********************************************************************/
std::string generate_routing_block_netlist_name(const std::string& prefix, 
                                                const size_t& block_id,
                                                const std::string& postfix) {
  return std::string( prefix + std::to_string(block_id) + postfix );
}

/*********************************************************************
 * Generate the netlist name for a routing block with a given coordinate
 * It could be 
 * 1. Routing channel
 * 2. Connection block
 * 3. Switch block
 *********************************************************************/
std::string generate_routing_block_netlist_name(const std::string& prefix, 
                                                const vtr::Point<size_t>& coordinate,
                                                const std::string& postfix) {
  return std::string( prefix + std::to_string(coordinate.x()) + std::string("_") + std::to_string(coordinate.y()) + postfix );
}

/*********************************************************************
 * Generate the module name for a unique routing channel
 *********************************************************************/
std::string generate_routing_channel_module_name(const t_rr_type& chan_type, 
                                                 const size_t& block_id) {
  /* Channel must be either CHANX or CHANY */
  VTR_ASSERT( (CHANX == chan_type) || (CHANY == chan_type) );

  /* Create a map between chan_type and module_prefix */
  std::map<t_rr_type, std::string> module_prefix_map;
  /* TODO: use a constexpr string to replace the fixed name? */
  module_prefix_map[CHANX] = std::string("chanx");
  module_prefix_map[CHANY] = std::string("chany");

  return std::string( module_prefix_map[chan_type] + std::string("_") + std::to_string(block_id) + std::string("_") );
}

/*********************************************************************
 * Generate the module name for a routing channel with a given coordinate
 *********************************************************************/
std::string generate_routing_channel_module_name(const t_rr_type& chan_type, 
                                                 const vtr::Point<size_t>& coordinate) {
  /* Channel must be either CHANX or CHANY */
  VTR_ASSERT( (CHANX == chan_type) || (CHANY == chan_type) );

  /* Create a map between chan_type and module_prefix */
  std::map<t_rr_type, std::string> module_prefix_map;
  /* TODO: use a constexpr string to replace the fixed name? */
  module_prefix_map[CHANX] = std::string("chanx");
  module_prefix_map[CHANY] = std::string("chany");

  return std::string( module_prefix_map[chan_type] + std::to_string(coordinate.x()) + std::string("_") + std::to_string(coordinate.y()) + std::string("_") );
}
