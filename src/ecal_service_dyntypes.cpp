#include <ecal/protobuf/ecal_proto_dyn.h>
#include "math.pb.h"

#include <iostream>
#include <memory>

class MathServiceImpl : public MathService
{
public:
  virtual void Add     (::google::protobuf::RpcController* /* controller_ */, const ::SFloatTuple* /*request_*/, ::SFloat* /*response_*/, ::google::protobuf::Closure* /* done_ */) {}
  virtual void Multiply(::google::protobuf::RpcController* /* controller_ */, const ::SFloatTuple* /*request_*/, ::SFloat* /*response_*/, ::google::protobuf::Closure* /* done_ */) {}
};

void GetMessageType(const google::protobuf::ServiceDescriptor* service_desc_, const std::string& type_name_, std::string& type_)
{
  std::string error_s;

  const google::protobuf::FileDescriptor* file_desc = service_desc_->file();

  std::string file_desc_s = file_desc->DebugString();
  google::protobuf::FileDescriptorProto file_desc_proto;
  std::shared_ptr<eCAL::protobuf::CProtoDynDecoder> msg_decoder = std::make_shared<eCAL::protobuf::CProtoDynDecoder>();
  msg_decoder->GetFileDescriptorFromString(file_desc_s, &file_desc_proto, error_s);

  google::protobuf::FileDescriptorSet pset;
  google::protobuf::FileDescriptorProto* pdesc = pset.add_file();
  pdesc->CopyFrom(file_desc_proto);

  auto req_msg = msg_decoder->GetProtoMessageFromDescriptorSet(pset, type_name_, error_s);
  auto req_desc = req_msg->GetDescriptor();
  type_ = req_desc->DebugString();
}

void GetServiceMessageTypes(const google::protobuf::ServiceDescriptor* service_desc_, std::map<std::string, std::string>& types_)
{
  auto mcount = service_desc_->method_count();
  for (auto m = 0; m < mcount; ++m)
  {
    std::string msg_type;
    std::string msg_type_name;
    auto method_desc = service_desc_->method(m);

    msg_type_name = method_desc->input_type()->name();
    GetMessageType(service_desc_, msg_type_name, msg_type);
    types_[msg_type_name] = msg_type;

    msg_type_name = method_desc->output_type()->name();
    GetMessageType(service_desc_, msg_type_name, msg_type);
    types_[msg_type_name] = msg_type;
  }
}

int main(int /*argc*/, char** /*argv*/)
{
  std::shared_ptr<MathService> math_service = std::make_shared<MathServiceImpl>();

  const google::protobuf::ServiceDescriptor* service_desc = math_service->GetDescriptor();
  std::map<std::string, std::string> types_map;
  GetServiceMessageTypes(service_desc, types_map);

  auto mcount = service_desc->method_count();
  for (auto m = 0; m < mcount; ++m)
  {
    auto method_desc = service_desc->method(m);
    auto req_type = method_desc->input_type()->name();
    auto resp_type = method_desc->output_type()->name();
    std::cout << "mname          : " << method_desc->name() << std::endl;
    std::cout << "req_type       : " << req_type << std::endl;
    std::cout << "req_type_desc  : " << types_map[req_type] << std::endl;
    std::cout << "resp_type      : " << resp_type << std::endl;
    std::cout << "resp_type_desc : " << types_map[resp_type] << std::endl;
  }

  return(0);
}
