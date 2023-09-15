/********************************************************************
 * This file includes functions that outputs a clock network object to XML
 *format
 *******************************************************************/
/* Headers from system goes first */
#include <algorithm>
#include <string>

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpga util library */
#include "openfpga_digest.h"

/* Headers from arch openfpga library */
#include "write_xml_utils.h"

/* Headers from pin constraint library */
#include "module_name_map_xml_constants.h"
#include "write_xml_module_name_map.h"

namespace openfpga {  // Begin namespace openfpga

/********************************************************************
 * A writer to output a I/O name mapping to XML format
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture
 * Return 2 if fail when creating files
 *******************************************************************/
static int write_xml_module_name_binding(std::fstream& fp, const ModuleNameMap& module_name_map,
                                 const std::string& built_in_name) {
  /* Validate the file stream */
  if (false == openfpga::valid_file_stream(fp)) {
    return 2;
  }

  openfpga::write_tab_to_file(fp, 1);
  fp << "<" << XML_MODULE_NAME_NODE_NAME << "";
  write_xml_attribute(fp, XML_MODULE_NAME_ATTRIBUTE_DEFAULT,
                      built_in_name.c_str());

  std::string given_name = module_name_map.name(built_in_name);
  if (given_name.empty()) {
    VTR_LOG_ERROR("Default name '%s' is not mapped to any given name!\n", built_in_name.c_str());
    return 1;
  }
  write_xml_attribute(fp, XML_MODULE_NAME_ATTRIBUTE_GIVEN,
                      given_name.c_str());
  fp << ">"
     << "\n";

  return 0;
}

/********************************************************************
 * A writer to output an object to XML format
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture
 * Return 2 if fail when creating files
 *******************************************************************/
int write_xml_module_name_map(const char* fname, const ModuleNameMap& module_name_map) {
  vtr::ScopedStartFinishTimer timer("Write module renaming rules");

  /* Create a file handler */
  std::fstream fp;
  /* Open the file stream */
  fp.open(std::string(fname), std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  /* Write the root node */
  fp << "<" << XML_MODULE_NAMES_ROOT_NAME;
  fp << ">"
     << "\n";

  int err_code = 0;

  /* Write each port */
  for (std::string built_in_name : module_name_map.tags()) {
    /* Write bus */
    err_code = write_xml_module_name_binding(fp, module_name_map, built_in_name);
    if (0 != err_code) {
      return err_code;
    }
  }

  /* Finish writing the root node */
  fp << "</" << XML_MODULE_NAMES_ROOT_NAME << ">"
     << "\n";

  /* Close the file stream */
  fp.close();

  return err_code;
}

}  // End of namespace openfpga
