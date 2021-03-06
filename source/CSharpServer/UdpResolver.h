#pragma once

#include "Service.h"

#include <server/asio/udp_resolver.h>

namespace CSharpServer {

    //! UDP resolver
    /*!
        UDP resolver is used to resolve DNS while connecting UDP clients.

        Thread-safe.
    */
    public ref class UdpResolver
    {
    public:
        //! Initialize resolver with a given service
        /*!
            \param service - Service
        */
        UdpResolver(Service^ service);
        ~UdpResolver() { this->!UdpResolver(); }

        //! Get the service
        property Service^ Service { CSharpServer::Service^ get() { return _service; } }

        //! Cancel any asynchronous operations that are waiting on the resolver
        void Cancel() { return _resolver->get()->Cancel(); }

    protected:
        !UdpResolver() { _resolver.Release(); };

    internal:
        CSharpServer::Service^ _service;
        Embedded<std::shared_ptr<CppServer::Asio::UDPResolver>> _resolver;
    };

}
