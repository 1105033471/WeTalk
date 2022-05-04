#ifndef WT_SOCKET_H
#define WT_SOCKET_H

#include <memory>
#include <string>

#define HTTP_BUF_SIZE       (1024)  /* 缂撳啿鍖哄ぇ灏?*/

class WtSocket {
    struct WtSocketImpl;
public:
    WtSocket();

    WtSocket(const WtSocket& other) = delete;
    WtSocket(WtSocket&& other) = delete;
    WtSocket& operator=(const WtSocket& other) = delete;
    WtSocket& operator=(WtSocket&& other) = delete;

    // 鍒濆鍖栧鎺ュ瓧鐜
    int InitSocket();

    // 灏嗗鎺ュ瓧缁戝畾鍦ㄥ浐瀹氱鍙ｏ紝鍚屾椂鐩戝惉璇ョ鍙ｈ姹?    
    int BindAndListen(int port);

    // 绛夊緟骞舵帴鏀剁鍙ｈ姹傦紝杩斿洖濂楁帴瀛楀彞鏌?    
    void* Accept();

    // 杩炴帴缁欏畾鐨勫湴鍧€鍜岀鍙?    
    void* Connect(std::string ip, int port);

    // 鍙戦€佽姹?    
    int Send(void* handle, const std::string& content);

    // 鎺ユ敹璇锋眰
    int Receive(void* handle, std::string& content);

    ~WtSocket();
private:
    std::shared_ptr<WtSocketImpl> impl;
};

// 閿€姣佸鎺ュ瓧鍙ユ焺
void DestroySocketHandle(void* handle);
#endif // WT_SOCKET_H
