#include <winsock2.h>
#include <stdio.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib") // Winsock kütüphanesi baðlanýr.

char weather_data[][50] = {
    "Balikesir: Gunesli, 25 Santigrat Derece",
    "Trabzon: Yagmurlu, 20 Santigrat Derece",
    "Ankara: Ruzgarli, 22 Santigrat Derece",
    "Bursa: Bulutlu, 18 Santigrat Derece",
    "Adana: Sicak, 30 Santigrat Derece",
    "Van: Parcali Bulutlu, 17 Santigrat Derece",
    "Yalova: Hafif Yagisli, 21 Santigrat Derece"
    
};

// Þehir adýna göre hava durumu bilgisini döner
char* get_weather(const char* city) {
	int i;
    for (i = 0; i<7; i++) {
        if (strstr(weather_data[i], city)) {
            return weather_data[i];
        }
    }
    return "Hava durumu bilgisi bulunamadi.";
}

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int c;
    char client_message[1024];

    // Winsock baþlatma
    printf("Winsock baslatiliyor...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Baslatma basarisiz. Hata Kodu: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Baslatildi.\n");

    // Soket oluþturma
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Soket olusturulamadi. Hata Kodu: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Soket olusturuldu.\n");

    // Sunucu ayarlarý
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    // Baðlama
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Baglama basarisiz. Hata Kodu: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Baglama basarili.\n");

    // Dinleme
    listen(server_socket, 3);
    printf("Baglantilar bekleniyor...\n");

    c = sizeof(struct sockaddr_in);
    client_socket = accept(server_socket, (struct sockaddr *)&client, &c);
    if (client_socket == INVALID_SOCKET) {
        printf("Baglanti kabul edilemedi. Hata Kodu: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Baglanti kabul edildi.\n");

    // Ýstemci mesajýný alma ve yanýt gönderme
    int recv_size;
    while ((recv_size = recv(client_socket, client_message, sizeof(client_message), 0)) > 0) {
        client_message[recv_size] = '\0';
        printf("Ýstemciden gelen sehir: %s\n", client_message);

        char* response = get_weather(client_message);
        send(client_socket, response, strlen(response), 0);
    }

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
    return 0;
}

