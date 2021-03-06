#include "stdafx.h"

#include "TcpServer.h"

namespace CSharpServer {

    bool TcpSessionEx::SendAsync(const void* buffer, size_t size)
    {
        if (!root->InternalOnSending(size))
            return false;

        return TCPSession::SendAsync(buffer, size);
    }

    bool TcpSessionEx::SendAsync(std::string_view text)
    {
        if (!root->InternalOnSending(text.size()))
            return false;

        return TCPSession::SendAsync(text);
    }

    void TcpSessionEx::onConnected()
    {
        root->InternalOnConnected();
    }

    void TcpSessionEx::onDisconnected()
    {
        root->InternalOnDisconnected();
    }

    void TcpSessionEx::onReceived(const void* buffer, size_t size)
    {
        array<Byte>^ bytes = gcnew array<Byte>((int)size);
        if (size > 0)
        {
            pin_ptr<Byte> ptr = &bytes[bytes->GetLowerBound(0)];
            memcpy(ptr, buffer, size);
        }
        root->InternalOnReceived(bytes, size);
    }

    void TcpSessionEx::onSent(size_t sent, size_t pending)
    {
        root->InternalOnSent(sent, pending);
    }

    void TcpSessionEx::onEmpty()
    {
        root->InternalOnEmpty();
    }

    void TcpSessionEx::onError(int error, const std::string& category, const std::string& message)
    {
        String^ cat = marshal_as<String^>(category);
        String^ msg = marshal_as<String^>(message);
        root->InternalOnError(errno, cat, msg);
    }

    std::shared_ptr<CppServer::Asio::TCPSession> TcpServerEx::CreateSession(std::shared_ptr<TCPServer> server)
    {
        return root->InternalCreateSession()->_session.Value;
    }

    void TcpServerEx::onStarted()
    {
        root->InternalOnStarted();
    }

    void TcpServerEx::onStopped()
    {
        root->InternalOnStopped();
    }

    void TcpServerEx::onConnected(std::shared_ptr<CppServer::Asio::TCPSession>& session)
    {
        auto session_ex = std::dynamic_pointer_cast<TcpSessionEx>(session);
        if (session_ex)
        {
            root->InternalOnConnected(session_ex->root);
        }
    }

    void TcpServerEx::onDisconnected(std::shared_ptr<CppServer::Asio::TCPSession>& session)
    {
        auto session_ex = std::dynamic_pointer_cast<TcpSessionEx>(session);
        if (session_ex)
        {
            root->InternalOnDisconnected(session_ex->root);
        }
    }

    void TcpServerEx::onError(int error, const std::string& category, const std::string& message)
    {
        String^ cat = marshal_as<String^>(category);
        String^ msg = marshal_as<String^>(message);
        root->InternalOnError(errno, cat, msg);
    }

    TcpSession::TcpSession(TcpServer^ server) :
        _server(server),
        _session(new std::shared_ptr<TcpSessionEx>(std::make_shared<TcpSessionEx>(server->_server.Value)))
    {
        _session->get()->root = this;
    }

    TcpServer::TcpServer(CSharpServer::Service^ service, int port, CSharpServer::InternetProtocol protocol) : TcpServer(service, gcnew TcpEndpoint(port, protocol))
    {
    }

    TcpServer::TcpServer(CSharpServer::Service^ service, String^ address, int port) :
        _service(service),
        _server(new std::shared_ptr<TcpServerEx>(std::make_shared<TcpServerEx>(service->_service.Value, marshal_as<std::string>(address), port)))
    {
        _server->get()->root = this;
    }

    TcpServer::TcpServer(CSharpServer::Service^ service, TcpEndpoint^ endpoint) :
        _service(service),
        _server(new std::shared_ptr<TcpServerEx>(std::make_shared<TcpServerEx>(service->_service.Value, endpoint->_endpoint.Value)))
    {
        _server->get()->root = this;
    }

}
