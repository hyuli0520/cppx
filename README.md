# cppx

cppx는 Windows가 제공하는 IOCP를 사용한 간단한 C++ 네트워크 라이브러리입니다.

### 예제 코드
---
- 기본 연결
```
// Server
#include "cppx/socket.h"

int main() {
	cppx::native::init(5); // () 안에 원하는 스레드 개수
	cppx::socket sock(cppx::protocol::tcp); // TCP 소켓 생성
	if (!sock.not_invalid()) { }// 소켓이 유요한지 확인
	
	cppx::socket sock2;
	sock2.create(cppx::protocol::udp); // 메서드를 사용한 소켓 생성
	sock2.close(); // 소켓 닫기
}
```
```
// Client
#include "cppx/socket.h"
#include <iostream>

int main() {
    cppx::native::init(5);
    cppx::socket sock(cppx::protocol::tcp);
    if (!sock.not_invalid()) return -1;
    if (!sock.connect(cppx::endpoint(cppx::ip_address::loopback, 7777))) return -1;
    std::cout << "connected" << std::endl;
}
```

- 비동기 IO
```
// include "cppx/context"
cppx::socket sock(cppx::protocol::tcp); // 소켓 생성
cppx::context context; // 비동기 IO context 생성
context.endpoint = ep; // 연결 endpoint 지정
context.complete_callback = [](cppx::context* ctx, bool success) {
    if (success)
        std::cout << "connected" << std::endl;
    else
        std::cout << "fail connect" << std::endl; 
}
sock.connect(&context);
```

- DNS
```
// include "cppx/dns"
cppx::dns::get_host_name() // 호스트 이름 받아오기
cppx::dns::get_host_entry("www.google.com") // www.google.com의 호스트 엔트리 받아오기
```