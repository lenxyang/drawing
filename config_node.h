#pragma once

#include <vector>
#include <map>

#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "azer/math/math.h"
#include "azer/util/xml/xml.h"

class ConfigNode;
typedef scoped_refptr<ConfigNode> ConfigNodePtr;
typedef std::vector<ConfigNodePtr> ConfigNodes;

class ConfigNode : public ::base::RefCounted<ConfigNode> {
 public:
  typedef std::pair<std::string, std::string> Attribute;
  typedef std::vector<Attribute> Attributes;

  static ConfigNodePtr InitFromXML(util::xml::Element* element);
  static ConfigNodePtr InitFromXMLStr(const std::string& str);

  ConfigNode();

  const std::string& tagname() const { return tagname_;}
  ConfigNode* parent() { return parent_;}
  const ConfigNode* parent() const { return parent_;}
  ConfigNode* root();
  const ConfigNode* root() const;

  void AddAttr(const std::string& name, const std::string& value);
  const Attributes& attributes() const { return attrs_;}
  const std::string GetAttr(const std::string& name) const;
  bool HasAttr(const std::string& name) const;
  const std::string& GetText() const;


  std::string GetChildTextString(const std::string& name) const;
  bool GetChildText(const std::string& name, std::string* text) const;
  const ConfigNodes& children() const { return children_;}
  ConfigNodes& children() { return children_;}
  int32 child_count() const { return static_cast<int>(children_.size());}
  const ConfigNodePtr& child_at(int32 index) const;
  ConfigNodePtr& child_at(int32 index);

  bool AddChild(ConfigNodePtr node);
  bool Contains(ConfigNode* node) const;
  bool HasTaggedChild(const std::string& name) const;
  ConfigNodes GetTaggedChildren(const std::string& name) const;

  // get children from path(not xpath, like file path)
  ConfigNodePtr GetNodeFromPath(const std::string& path);
  std::string GetNodePath() const;

  ConfigNodes GetNodeWithAttr(const std::string& name, const std::string& value);
  ConfigNodePtr GetFirstChildTagged(const std::string& tag) const;
  ConfigNodePtr GetLastChildTagged(const std::string& tag) const;

  std::string print_info();
 private:
  std::string print_node_info(int depth);
  std::string print_node_recusive(int depth);
  bool InitFromXMLNodeRecusive(util::xml::Element* element);
  bool InitFromXMLNode(util::xml::Element* element);
  ConfigNodes children_;
  ConfigNode* parent_;
  Attributes attrs_;
  std::string tagname_;
  std::string text_;
  DISALLOW_COPY_AND_ASSIGN(ConfigNode);
};
