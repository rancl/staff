// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// Protobuf bridge

#ifndef _ProtobufConnector_h_
#define _ProtobufConnector_h_

namespace google
{
namespace protobuf
{
  class RpcController;
  class RpcChannel;
  class ServiceDescriptor;
}
}

$(Project.OpeningNs)
  class ProtobufConnector
  {
  public:
    ProtobufConnector();
    virtual ~ProtobufConnector();

    //! create and initialize channel and controller
    /*! \param pServiceDescriptor - service descriptor
      */
    void Create(const ::google::protobuf::ServiceDescriptor* pServiceDescriptor);

    //! destroy channel and controller
    void Free();

    //! get channel
    /*! \return protobuf rpc channel
      */
    ::google::protobuf::RpcChannel* GetChannel();

    //! get controller
    /*! \return protobuf rpc controller
      */
    ::google::protobuf::RpcController* GetController();

  private:
//    RpcChannelImpl m_tChannel;       //!< protobuf rpc channel
//    RpcControllerImpl m_tController; //!< protobuf rpc controller
  };
$(Project.EndingNs)

#endif // _ProtobufConnector_h_
