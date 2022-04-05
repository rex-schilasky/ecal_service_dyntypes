#include <ecal/protobuf/ecal_proto_dyn.h>
#include "math.pb.h"

#include <iostream>
#include <memory>

bool GetServiceMessageDescFromType(const google::protobuf::ServiceDescriptor* service_desc_, const std::string& type_name_, std::string& type_desc_, std::string& error_s_ )
{
  std::shared_ptr<eCAL::protobuf::CProtoDynDecoder> msg_decoder = std::make_shared<eCAL::protobuf::CProtoDynDecoder>();

  const google::protobuf::FileDescriptor* file_desc = service_desc_->file();
  if (!file_desc) return false;

  std::string file_desc_s = file_desc->DebugString();
  google::protobuf::FileDescriptorProto file_desc_proto;
  if (!msg_decoder->GetFileDescriptorFromString(file_desc_s, &file_desc_proto, error_s_)) return false;

  google::protobuf::FileDescriptorSet pset;
  google::protobuf::FileDescriptorProto* pdesc = pset.add_file();
  pdesc->CopyFrom(file_desc_proto);

  auto req_msg = msg_decoder->GetProtoMessageFromDescriptorSet(pset, type_name_, error_s_);
  if (!req_msg) return false;

  auto req_desc = req_msg->GetDescriptor();
  if (!req_desc) return false;

  // TODO: Extract "real" protobuf descriptors here (not that human readable ones).
  type_desc_ = req_desc->DebugString();

  return true;
}

bool GetServiceMessageTypeFromMethod(const google::protobuf::MethodDescriptor* method_desc_, std::string& req_type_, std::string& resp_type_)
{
  if (!method_desc_) return false;
  req_type_  = method_desc_->input_type()->name();
  resp_type_ = method_desc_->output_type()->name();
  return true;
}

bool GetServiceMessageDescFromMethod(const google::protobuf::ServiceDescriptor* service_desc_, const google::protobuf::MethodDescriptor* method_desc_, std::string& req_desc_, std::string& resp_desc_, std::string& error_s_)
{
  std::string req_type, resp_type;

  if (!GetServiceMessageTypeFromMethod(method_desc_, req_type, resp_type))  return false;
  if (!GetServiceMessageDescFromType(service_desc_, req_type, req_desc_, error_s_))   return false;
  if (!GetServiceMessageDescFromType(service_desc_, resp_type, resp_desc_, error_s_)) return false;
  
  return true;
}

int main(int /*argc*/, char** /*argv*/)
{
  class MathServiceImpl : public MathService {};
  std::shared_ptr<MathService> math_service = std::make_shared<MathServiceImpl>();

  const google::protobuf::ServiceDescriptor* service_desc = math_service->GetDescriptor();
  auto mcount = service_desc->method_count();
  for (auto m = 0; m < mcount; ++m)
  {
    auto method_desc = service_desc->method(m);

    std::string req_type, resp_type;
    GetServiceMessageTypeFromMethod(method_desc, req_type, resp_type);

    std::string req_desc, resp_desc, error_s;
    if (!GetServiceMessageDescFromMethod(service_desc, method_desc, req_desc, resp_desc, error_s))
    {
      std::cout << "Could not get service message descriptors, error: " << error_s << std::endl;
    }
    else
    {
      std::cout << "mname          : " << method_desc->name() << std::endl;
      std::cout << "req_type       : " << req_type << std::endl;
      std::cout << "req_type_desc  : " << req_desc << std::endl;
      std::cout << "resp_type      : " << resp_type << std::endl;
      std::cout << "resp_type_desc : " << resp_desc << std::endl;
    }
  }

  return 0;
}
