/********************************************************************
 * Header file for fabric_bitstream_utils.cpp
 *******************************************************************/
#ifndef FABRIC_BITSTREAM_UTILS_H
#define FABRIC_BITSTREAM_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>
#include "bitstream_manager.h"
#include "fabric_bitstream.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

size_t find_fabric_regional_bitstream_max_size(const FabricBitstream& fabric_bitstream);

size_t find_configuration_chain_fabric_bitstream_size_to_be_skipped(const FabricBitstream& fabric_bitstream,
                                                                    const BitstreamManager& bitstream_manager,
                                                                    const bool& bit_value_to_skip);

} /* end namespace openfpga */

#endif
