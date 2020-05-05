#ifndef PNR_SDC_GRID_WRITER_H
#define PNR_SDC_GRID_WRITER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include "vpr_context.h"
#include "vpr_device_annotation.h"
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_pnr_sdc_constrain_grid_timing(const std::string& sdc_dir,
                                         const float& time_unit,
                                         const DeviceContext& device_ctx,
                                         const VprDeviceAnnotation& device_annotation,
                                         const ModuleManager& module_manager,
                                         const bool& constrain_zero_delay_paths);

} /* end namespace openfpga */

#endif
