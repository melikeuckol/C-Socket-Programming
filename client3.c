#include <winsock2.h>
#include <stdio.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib") // Winsock kütüphanesi baðlanýr.

int main() {
    WSADATA wsa;
    SOCKET server_socket;
    struct sockaddr_in server;
    char message[50], server_reply[1024];

    // Winsock baþlatma
    printf("Winsock baslatiliyor...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Baslatma basarisiz. Hata Kodu: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Baslatildi.\n");

    // Soket oluþturma
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Soket olusturulamadi . Hata Kodu: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Soket olusturuldu.\n");

    // Sunucu bilgileri
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // Sunucu IP'si
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);

    // Baðlantý
    if (connect(server_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Baglanti basarisiz. Hata Kodu: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Baglanti basarili.\n");

    // Kullanýcýdan þehir adý al
    printf(" Asagidaki sehirlerden hava durumunu goruntulemek istediginiz sehri yaziniz:\n 1-Balikesir\n 2-Trabzon\n 3-Ankara\n 4-Bursa\n 5-Adana\n 6-Van\n 7-Yalova\n ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0'; // Yeni satýrý kaldýr

    // Sunucuya mesaj gönder
    send(server_socket, message, strlen(message), 0);

    // Sunucudan yanýt al
    int recv_size;
    if ((recv_size = recv(server_socket, server_reply, sizeof(server_reply), 0)) > 0) {
        server_reply[recv_size] = '\0';
        printf("\n\n***************************************************************\n");
        printf("\nHava durumu bilgisi: %s\n\n", server_reply);
          printf("*************************************************************");
        
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}

