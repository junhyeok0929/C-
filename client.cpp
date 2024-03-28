#include "../Common.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    int retval;

    char *SERVERIP = (char *)"127.0.0.1";
    int SERVERPORT = 9000;
    int BUFSIZE = 512;

    // 명령행 인수가 있으면 IP 주소로 사용
    if (argc > 1) SERVERIP = argv[1];

    // 소켓 생성
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) err_quit("socket()");

    // connect()
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = connect(sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("connect()");

    // 데이터 통신에 사용할 변수
    char buf[BUFSIZE + 1];

    // 서버와 데이터 통신
    while (1) {
        // 연도와 월 입력 받기
        int year, month;
        std::cout << "연도와 월을 입력하세요 (예: 2024. 03.): ";
        std::cin >> year >> month;

        // 입력한 연도와 월을 서버에 전송
        sprintf(buf, "%d.%d", year, month);
        retval = send(sock, buf, strlen(buf), 0);
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            break;
        }

        // 달력 정보 받기
        retval = recv(sock, buf, BUFSIZE, 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        } else if (retval == 0)
            break;

        // 받은 데이터 출력 (달력 출력)
        buf[retval] = '\0';
        std::cout << "\n[받은 달력]\n" << buf << std::endl;
    }

    //
