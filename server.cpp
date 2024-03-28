#include "../Common.h"
#include <iostream>
#include <sstream>

// 달력 생성 함수
std::string generateCalendar(int year, int month) {
    std::ostringstream oss;
    oss << "**** " << year << "년 " << std::setw(2) << std::setfill('0') << month << "월 ****\n\n";

    // 해당 연월의 달력 생성
    // 간단한 예시로 달력을 생성하는 코드를 추가하였습니다. 더 복잡한 경우에는 외부 라이브러리를 활용하거나 직접 구현해야 합니다.
    oss << "일  월  화  수  목  금  토\n";
    int daysInMonth = 31; // 간단한 예시로 31일까지만 출력
    for (int i = 1; i <= daysInMonth; ++i) {
        oss << std::setw(2) << i << "  ";
        if (i % 7 == 0) // 한 주가 끝나면 줄 바꿈
            oss << "\n";
    }

    return oss.str();
}

int main(int argc, char *argv[]) {
    int retval;

    // 소켓 생성
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");

    // bind()
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");

    // 데이터 통신에 사용할 변수
    SOCKET client_sock;
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    char buf[BUFSIZE + 1];

    while (1) {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (struct sockaddr *)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }

        // 접속한 클라이언트 정보 출력
        char addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
            addr, ntohs(clientaddr.sin_port));

        // 클라이언트와 데이터 통신
        while (1) {
            // 데이터 받기
            retval = recv(client_sock, buf, BUFSIZE, 0);
            if (retval == SOCKET_ERROR) {
                err_display("recv()");
                break;
            } else if (retval == 0)
                break;

            // 받은 데이터 파싱하여 연도와 월 추출
            buf[retval] = '\0';
            int year, month;
            sscanf(buf, "%d.%d", &year, &month);

            // 해당 연월의 달력 생성
            std::string calendar = generateCalendar(year, month);

            // 생성된 달력을 클라이언트에 전송
            retval = send(client_sock, calendar.c_str(), calendar.length(), 0);
            if (retval == SOCKET_ERROR) {
                err_display("send()");
                break;
            }
        }

        // 소켓 닫기
        close(client_sock);
        printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
            addr, ntohs(clientaddr.sin_port));
    }

    // 소켓 닫기
    close(listen_sock);
    return 0;
}
