#ifndef MODULE_NAME_MAP_H
#define MODULE_NAME_MAP_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <string>
#include <vector>
#include <map>

/* Begin namespace openfpga */
namespace openfpga {

/**
 * @brief Module name map is a data structure to show mapping between a tag (built-in name) and customized names (may be given by users)
 */
class ModuleNameMap {
 public: /* Public accessors */
  /** @brief Get customized name with a given tag */
  std::string name(const std::string& tag) const;
  /** @brief return a list of all the current keys */
  std::vector<std::string> tags() const;

 public: /* Public mutators */
  /** @brief Create the one-on-one mapping between an built-in name and a customized name. Return 0 for success, return 1 for fail */
  int set_tag_to_name_pair(const std::string& tag, const std::string& name);
 private: /* Internal Data */
  /* built-in name -> customized_name
   * Create a double link to check any customized name is mapped to more than 1 built-in name!
   */
  std::map<std::string, std::string> tag2names_;
  std::map<std::string, std::string> name2tags_;
};

} /* End namespace openfpga*/

#endif
