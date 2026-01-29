#define _CRT_SECURE_NO_WARNINGS // 解决 strncpy 警告
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

// 显式链接库，解决 inet_ntop 等符号无法解析的问题
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")

extern "C" {
#include "onvif.h"
}

// 自动获取本机 IP（仅用于控制台显示，确认当前网络环境）
std::string getPrimaryIPAddress() {
    char ip[INET_ADDRSTRLEN] = {0};
    ULONG outBufLen = 15000;
    PIP_ADAPTER_ADDRESSES pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);

    if (GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, pAddresses, &outBufLen) == NO_ERROR) {
        for (PIP_ADAPTER_ADDRESSES pCurr = pAddresses; pCurr != NULL; pCurr = pCurr->Next) {
            if (pCurr->IfType == IF_TYPE_SOFTWARE_LOOPBACK || pCurr->OperStatus != IfOperStatusUp) continue;

            for (PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pCurr->FirstUnicastAddress; pUnicast != NULL; pUnicast = pUnicast->Next) {
                sockaddr_in* sa_in = (sockaddr_in*)pUnicast->Address.lpSockaddr;
                inet_ntop(AF_INET, &(sa_in->sin_addr), ip, INET_ADDRSTRLEN);
                break;
            }
            if (strlen(ip) > 0) break;
        }
    }
    free(pAddresses);
    return std::string(ip);
}

int main() {
    std::cout << "--- ONVIF Discovery Test ---" << std::endl;

    // // 打印当前网卡 IP，确保你和相机在同一网段
    // std::string localIP = getPrimaryIPAddress();
    // std::cout << "Local IP Detected: " << localIP << std::endl;

    // 堆分配 Session 避免栈溢出崩溃
    OnvifSession* session = new OnvifSession();
    initializeSession(session);

    std::cout << "Scanning for devices..." << std::endl;
    int deviceCount = broadcast(session);

    if (deviceCount <= 0) {
        std::cout << "No devices found. Ensure virtual NICs are disabled." << std::endl;
    } else {
        std::cout << "Success! Found " << deviceCount << " device(s):" << std::endl;
        for (int i = 0; i < deviceCount; ++i) {
            OnvifData data;
            if (prepareOnvifData(i, session, &data)) {
                // 修复警告：检查字符串首字符是否为结束符，而非判断数组地址
                if (data.xaddrs[0] != '\0') {
                    std::cout << "  - XAddrs: " << data.xaddrs << std::endl;
                }
                if (data.host[0] != '\0') {
                    std::cout << "  - Host:   " << data.host << std::endl;
                }
                std::cout << "--------------------------------------" << std::endl;
            }
        }
    }

    closeSession(session);
    delete session;

    std::cout << "Done. Press Enter to exit." << std::endl;
    std::cin.get();
    return 0;
}
