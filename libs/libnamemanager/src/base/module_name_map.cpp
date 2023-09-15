/******************************************************************************
 * Memember functions for data structure ModuleNameMap
 ******************************************************************************/
/* Headers from vtrutil library */
#include "module_name_map.h"

#include <algorithm>

#include "command_exit_codes.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Public Accessors
 *************************************************/
std::string ModuleNameMap::name(const std::string& tag) const {
  auto result = tag2names_.find(tag);
  if (result == tag2names_.end()) {
	VTR_LOG_ERROR("The given built-in name '%s' does not exist!\n", tag.c_str());
    return std::string();
  }
  return result->second;
}

int ModuleNameMap::set_tag_to_name_pair(const std::string& tag, const std::string& name) {
  /*  tagA <--x--> nameA
   *        | 
   *        +----> nameB
   *  tagB <--x--> nameB
   * Scenarios to be considered:
   * - Remove the double links between tagA and nameA
   * - nameB should NOT be mapped to any other tags!
   */
  auto result = name2tags_.find(name);
  if (result != name2tags_.end() && result->second != tag) {
	VTR_LOG_ERROR("The customized name '%s' has already been mapped to a built-in name '%s'! Fail to bind it to a new built-in name '%s'\n", name.c_str(), result->second.c_str(), tag.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  /* Create double link */
  name2tags_[name] = tag;
  tag2names_[tag] = name;
  /* Clean up */
  name2tags_.erase(name);
}

} /* end namespace openfpga */
