#include <wt_logger.h>
#include <wt_util.hpp>
#include <wt_socket.h>

#include <cstdint>
#include <string>
#include <thread>

static const char* servIP = "127.0.0.1";
static const int servPort = 11011;

using namespace std;

int DoCommunicate(shared_ptr<WtSocket> wtSocket, void* handle)
{
    int ret = 0;
    string message = "this is client request.";

    ret = wtSocket->Send(handle, message);
    if (ret != 0) {
        WtLogErr("send message error, ret=%d", ret);
        return -1;
    }

    ret = wtSocket->Receive(handle, message);
    if (ret != 0) {
        WtLogErr("receive message error, ret=%d", ret);
        return -1;
    }

    WtLogInfo("client received server response: %s", message.c_str());
    return 0;
}

int main(int argc, char* argv[])
{
    WT_UNUSEDPARAM(argc);
    WT_UNUSEDPARAM(argv);
    int ret = 0;
    shared_ptr<WtSocket> wtSocket = make_shared<WtSocket>();

    ret = wtSocket->InitSocket();
    if (ret != 0) {
        WtLogErr("WtSocket InitSocket error, ret=%d", ret);
        return 0;
    }

    void* handle = wtSocket->Connect(servIP, servPort);
    if (handle == nullptr) {
        WtLogErr("client connect error.");
        return -1;
    }

    ret = DoCommunicate(wtSocket, handle);
    DestroySocketHandle(handle);
    if (ret != 0) {
        WtLogErr("client do communicate error.");
        return -1;
    }
    return 0;
}
