#include <wt_logger.h>
#include <wt_util.hpp>
#include <wt_socket.h>

#include <cstdint>
#include <string>
#include <thread>

using namespace std;

static const int servPort = 11011;

int SendResponse(shared_ptr<WtSocket> wtSocket, void* handle)
{
    int ret = 0;
    char buf[HTTP_BUF_SIZE] = {0};
    int bufLen = 0;

    // 鏋勯€犺繑鍥炲唴瀹?    
    bufLen = sprintf(buf, "This is server response content.");
    ret = wtSocket->Send(handle, string(buf, bufLen));
    if (ret != 0) {
        WtLogErr("server response send content error, ret=%d", ret);
        return -1;
    }
    return 0;
}

int ProcessClientRequest(shared_ptr<WtSocket> wtSocket, void* handle)
{
    string content;
    int ret = 0;

    ret = wtSocket->Receive(handle, content);
    if (ret != 0) {
        WtLogErr("WtSocket Receive error, ret=%d", ret);
        return ret;
    }

    WtLogInfo("server received client request, content: %s", content.c_str());

    ret = SendResponse(wtSocket, handle);
    if (ret != 0) {
        WtLogErr("WtSocket Receive error, ret=%d", ret);
        return ret;
    }

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

    ret = wtSocket->BindAndListen(servPort);
    if (ret != 0) {
        WtLogErr("WtSocket BindAndListen error, ret=%d", ret);
        return 0;
    }

    while (true) {
        void* handle = wtSocket->Accept();
        if (handle == nullptr) {
            WtLogErr("WtSocket accept error.");
            break;
        }

        ret = ProcessClientRequest(wtSocket, handle);
        DestroySocketHandle(handle);
        if (ret != 0) {
            break;
        }
    }
    return 0;
}
