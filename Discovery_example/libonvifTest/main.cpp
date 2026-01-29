// #include <iostream>

// extern "C" {
// #include "onvif.h"
// }



// int main() {
//     std::cout << "--- ONVIF Discovery Test ---" << std::endl;


//     // 堆分配 Session 避免栈溢出崩溃
//     OnvifSession* session = new OnvifSession();
//     initializeSession(session);

//     std::cout << "Scanning for devices..." << std::endl;
//     int deviceCount = broadcast(session);

//     if (deviceCount <= 0) {
//         std::cout << "No devices found. Ensure virtual NICs are disabled." << std::endl;
//     } else {
//         std::cout << "Success! Found " << deviceCount << " device(s):" << std::endl;
//         for (int i = 0; i < deviceCount; ++i) {
//             OnvifData data;
//             if (prepareOnvifData(i, session, &data)) {
//                 // 修复警告：检查字符串首字符是否为结束符，而非判断数组地址
//                 if (data.xaddrs[0] != '\0') {
//                     std::cout << "  - XAddrs: " << data.xaddrs << std::endl;
//                 }
//                 if (data.host[0] != '\0') {
//                     std::cout << "  - Host:   " << data.host << std::endl;
//                 }
//                 std::cout << "--------------------------------------" << std::endl;
//             }
//         }
//     }

//     closeSession(session);
//     delete session;

//     std::cout << "Done. Press Enter to exit." << std::endl;
//     std::cin.get();
//     return 0;
// }
