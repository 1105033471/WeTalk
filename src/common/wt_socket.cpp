#include <wt_socket.h>
#include <wt_logger.h>

#include <winsock2.h>
#include <string>
#include <utility>

using namespace std;

typedef struct sockaddr_in WtWtSockAddrIn;
typedef struct sockaddr WtSockAddr;

struct ClientSocket {
    WtWtSockAddrIn addr;
    int addr_len = 0;
    SOCKET soc {INVALID_SOCKET};
    bool needClose {true};

    ~ClientSocket()
    {
        if (soc != INVALID_SOCKET && needClose) {
            closesocket(soc);
            soc = INVALID_SOCKET;
        }
    }
};

/* 瀹炵幇绫伙紝鎵€鏈夌粏鑺傞兘鍦ㄨ繖閲?*/
struct WtSocket::WtSocketImpl {
    SOCKET soc {INVALID_SOCKET};
    WSADATA wsaData;
    
    // 鍒濆鍖栧鎺ュ瓧鐜
    int InitSocket();

    // 灏嗗鎺ュ瓧缁戝畾鍦ㄥ浐瀹氱鍙ｏ紝鍚屾椂鐩戝惉璇ョ鍙ｈ姹?    
    int BindAndListen(int port);

    // 绛夊緟骞舵帴鏀剁鍙ｈ姹傦紝杩斿洖瀹㈡埛绔鎺ュ瓧
    void* Accept();

    // 杩炴帴缁欏畾鐨勫湴鍧€鍜岀鍙?    
    void* Connect(string ip, int port);

    // 鍙戦€佽姹?    
    int Send(void* handle, const string& content);

    // 鎺ユ敹璇锋眰
    int Receive(void* handle, string& content);

    ~WtSocketImpl();
};

int WtSocket::WtSocketImpl::InitSocket()
{
    int ret = 0;
    
    ret = WSAStartup(MAKEWORD(2, 0), &wsaData);
    if (ret != 0) {
        return WSAGetLastError();
    }

    soc = socket(AF_INET, SOCK_STREAM, 0);
    if (soc == INVALID_SOCKET) {
        return WSAGetLastError();
    }
    return 0;
}

int WtSocket::WtSocketImpl::BindAndListen(int port)
{
    int ret = 0;
    WtWtSockAddrIn serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = ::bind(soc, (WtSockAddr*)&serv_addr, sizeof(serv_addr));
    if (ret == SOCKET_ERROR) {
        return WSAGetLastError();
    }

    listen(soc, SOMAXCONN);
    return 0;
}

void* WtSocket::WtSocketImpl::Accept()
{
    ClientSocket* clientSoc = new ClientSocket();
    clientSoc->addr_len = sizeof(clientSoc->addr);

    clientSoc->soc = accept(soc, (WtSockAddr*)&clientSoc->addr, &clientSoc->addr_len);
    if (clientSoc->soc == INVALID_SOCKET) {
        fprintf(stderr, "accept error, error=%d\n", WSAGetLastError());
        delete clientSoc;
        return nullptr;
    }

    return clientSoc;
}

void* WtSocket::WtSocketImpl::Connect(string ip, int port)
{
    ClientSocket* clientSoc = new ClientSocket();
    int ret = 0;

    clientSoc->addr.sin_family = AF_INET;
    clientSoc->addr.sin_port = htons(port);
    clientSoc->addr.sin_addr.s_addr = inet_addr(ip.c_str());
    clientSoc->soc = soc;
    clientSoc->needClose = false;

    ret = connect(clientSoc->soc, (WtSockAddr*)&clientSoc->addr, sizeof(clientSoc->addr));
    if (ret == SOCKET_ERROR) {
        fprintf(stderr, "connect error, ret=%d, error=%d\n", ret, WSAGetLastError());
        delete clientSoc;
        return nullptr;
    }

    return clientSoc;
}

int WtSocket::WtSocketImpl::Send(void* handle, const string& content)
{
    ClientSocket* clientSoc = (ClientSocket*)handle;
    int ret = 0;

    ret = send(clientSoc->soc, content.c_str(), content.length(), 0);
    if (ret == SOCKET_ERROR) {
        return WSAGetLastError();
    }
    return 0;
}

int WtSocket::WtSocketImpl::Receive(void* handle, string& content)
{
    ClientSocket* clientSoc = (ClientSocket*)handle;
    int ret = 0;
    char* buf = new char[2097152] {0};

    ret = recv(clientSoc->soc, buf, 2097152, 0);
    if (ret == SOCKET_ERROR) {
        delete [] buf;
        return WSAGetLastError();
    }

    content = string(buf, ret);
    delete [] buf;
    return 0;
}

WtSocket::WtSocketImpl::~WtSocketImpl()
{
    if (soc != INVALID_SOCKET) {
        closesocket(soc);
        soc = INVALID_SOCKET;
    }
    WSACleanup();
}

/* 鎺ュ彛绫伙紝杞彂鎵€鏈夎姹?*/
WtSocket::WtSocket()
{
    impl = make_shared<WtSocketImpl>();
}

int WtSocket::InitSocket()
{
    return impl->InitSocket();
}

int WtSocket::BindAndListen(int port)
{
    return impl->BindAndListen(port);
}

void* WtSocket::Accept()
{
    return impl->Accept();
}

void* WtSocket::Connect(string ip, int port)
{
    return impl->Connect(ip, port);
}

int WtSocket::Send(void* handle, const string& content)
{
    return impl->Send(handle, content);
}

int WtSocket::Receive(void* handle, string& content)
{
    return impl->Receive(handle, content);
}

WtSocket::~WtSocket()
{
    impl = nullptr;
}

// 閿€姣乧lient socket
void DestroySocketHandle(void* handle)
{
    ClientSocket* clientSoc = (ClientSocket*)handle;
    delete clientSoc;
}
