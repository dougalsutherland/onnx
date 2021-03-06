#pragma once

#include <stdexcept>
#include "onnx/onnx_pb.h"
#include "onnx/string_utils.h"

namespace onnx {
namespace checker {
class ValidationError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
  const char* what() const noexcept override {
    if (!expanded_message_.empty()) {
      return expanded_message_.c_str();
    }
    return std::runtime_error::what();
  }
  void AppendContext(const std::string& context) {
    expanded_message_ = onnx::MakeString(
        std::runtime_error::what(), "\n\n==> Context: ", context);
  }

 private:
  std::string expanded_message_;
};

#define fail_check(...) \
  throw onnx::checker::ValidationError(onnx::MakeString(__VA_ARGS__));

using IR_VERSION_TYPE = decltype(Version::IR_VERSION);
void check_value_info(const ValueInfoProto& value_info, int ir_version);
void check_tensor(const TensorProto& tensor, int ir_version);
void check_attribute(const AttributeProto& attr, int ir_version);
void check_node(const NodeProto& node, int ir_version);
void check_graph(const GraphProto& graph, int ir_version);
void check_model(const ModelProto& model, int ir_version);
} // namespace checker
} // namespace onnx
