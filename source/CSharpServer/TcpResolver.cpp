#include "stdafx.h"

#include "TcpResolver.h"

namespace CSharpServer {

    TcpResolver::TcpResolver(CSharpServer::Service^ service) :
        _service(service),
        _resolver(new std::shared_ptr<CppServer::Asio::TCPResolver>(std::make_shared<CppServer::Asio::TCPResolver>(service->_service.Value)))
    {
    }

}
