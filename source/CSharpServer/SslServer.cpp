#include "stdafx.h"

#include "SslServer.h"

namespace CSharpServer {

    bool SslSessionEx::SendAsync(const void* buffer, size_t size)
    {
        if (!root->InternalOnSending(size))
            return false;

        return SSLSession::SendAsync(buffer, size);
    }

    bool SslSessionEx::SendAsync(std::string_view text)
    {
        if (!root->InternalOnSending(text.size()))
            return false;

        return SSLSession::SendAsync(text);
    }

    void SslSessionEx::onConnected()
    {
        root->InternalOnConnected();
    }

    void SslSessionEx::onHandshaked()
    {
        root->InternalOnHandshaked();
    }

    void SslSessionEx::onDisconnected()
    {
        root->InternalOnDisconnected();
    }

    void SslSessionEx::onReceived(const void* buffer, size_t size)
    {
        array<Byte>^ bytes = gcnew array<Byte>((int)size);
        if (size > 0)
        {
            pin_ptr<Byte> ptr = &bytes[bytes->GetLowerBound(0)];
            memcpy(ptr, buffer, size);
        }
        root->InternalOnReceived(bytes, size);
    }

    void SslSessionEx::onSent(size_t sent, size_t pending)
    {
        root->InternalOnSent(sent, pending);
    }

    void SslSessionEx::onEmpty()
    {
        root->InternalOnEmpty();
    }

    void SslSessionEx::onError(int error, const std::string& category, const std::string& message)
    {
        String^ cat = marshal_as<String^>(category);
        String^ msg = marshal_as<String^>(message);
        root->InternalOnError(errno, cat, msg);
    }

    std::shared_ptr<CppServer::Asio::SSLSession> SslServerEx::CreateSession(std::shared_ptr<SSLServer> server)
    {
        return root->InternalCreateSession()->_session.Value;
    }

    void SslServerEx::onStarted()
    {
        root->InternalOnStarted();
    }

    void SslServerEx::onStopped()
    {
        root->InternalOnStopped();
    }

    void SslServerEx::onConnected(std::shared_ptr<CppServer::Asio::SSLSession>& session)
    {
        auto session_ex = std::dynamic_pointer_cast<SslSessionEx>(session);
        if (session_ex)
        {
            root->InternalOnConnected(session_ex->root);
        }
    }

    void SslServerEx::onHandshaked(std::shared_ptr<CppServer::Asio::SSLSession>& session)
    {
        auto session_ex = std::dynamic_pointer_cast<SslSessionEx>(session);
        if (session_ex)
        {
            root->InternalOnHandshaked(session_ex->root);
        }
    }

    void SslServerEx::onDisconnected(std::shared_ptr<CppServer::Asio::SSLSession>& session)
    {
        auto session_ex = std::dynamic_pointer_cast<SslSessionEx>(session);
        if (session_ex)
        {
            root->InternalOnDisconnected(session_ex->root);
        }
    }

    void SslServerEx::onError(int error, const std::string& category, const std::string& message)
    {
        String^ cat = marshal_as<String^>(category);
        String^ msg = marshal_as<String^>(message);
        root->InternalOnError(errno, cat, msg);
    }

    SslSession::SslSession(SslServer^ server) :
        _server(server),
        _session(new std::shared_ptr<SslSessionEx>(std::make_shared<SslSessionEx>(server->_server.Value)))
    {
        _session->get()->root = this;
    }

    SslServer::SslServer(CSharpServer::Service^ service, SslContext^ context, int port, CSharpServer::InternetProtocol protocol) : SslServer(service, context, gcnew TcpEndpoint(port, protocol))
    {
    }

    SslServer::SslServer(CSharpServer::Service^ service, SslContext^ context, String^ address, int port) :
        _service(service),
        _context(context),
        _server(new std::shared_ptr<SslServerEx>(std::make_shared<SslServerEx>(service->_service.Value, context->_context.Value, marshal_as<std::string>(address), port)))
    {
        _server->get()->root = this;
    }

    SslServer::SslServer(CSharpServer::Service^ service, SslContext^ context, TcpEndpoint^ endpoint) :
        _service(service),
        _context(context),
        _server(new std::shared_ptr<SslServerEx>(std::make_shared<SslServerEx>(service->_service.Value, context->_context.Value, endpoint->_endpoint.Value)))
    {
        _server->get()->root = this;
    }

}
