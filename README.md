# Projekt Hostowania Stron

## Opis

Projekt Hostowania Stron to serwer stworzony w celu umożliwienia hostowania wielu witryn internetowych jednocześnie. Program został zaprojektowany z myślą o prostocie obsługi, efektywnym zarządzaniu witrynami oraz zapewnieniu podstawowych mechanizmów bezpieczeństwa w trakcie przesyłania danych.

## Funkcje Kluczowe

### Wielowątkowa Obsługa Witryn
- Każda witryna jest obsługiwana przez osobny wątek, co umożliwia równoczesne obsługiwania wielu stron internetowych.
- Architektura wielowątkowa zapewnia efektywne zarządzanie zasobami i skalowalność.

### Zabezpieczenia Danych
- Dane przesyłane między serwerem a klientem są zabezpieczane poprzez generowanie losowych haseł, przypisywanych do każdej witryny.
- Opcja aktywacji zabezpieczeń dla wybranych witryn, podnosząc poziom ochrony danych.

### Konfiguracja z Pliku
- Konfiguracja serwera odbywa się poprzez odczyt danych z pliku `config.txt`.
- Plik konfiguracyjny umożliwia precyzyjne określenie portów, ścieżek do plików, słowników i innych parametrów dla każdej witryny.

### Interaktywny Interfejs Użytkownika (GUI)
- Prosty interfejs tekstowy w terminalu umożliwia interaktywne zarządzanie witrynami.
- Monitorowanie statusu witryn, zatrzymywanie serwera i wysyłanie testowych danych na wybrane witryny.

### Operacje HTTP i Zabezpieczenia
- Obsługa podstawowych operacji HTTP, w tym odebranie żądania, przetworzenie go i wysłanie odpowiedzi.
- Dodanie hasła do witryn, chroniąc dostęp przed niepowołanymi użytkownikami.

## Instrukcja Użycia

1. Sklonuj repozytorium: `git clone https://github.com/twoj_uzytkownik/projekt-hostowania-stron.git`
2. Przejdź do katalogu projektu: `cd projekt-hostowania-stron`
3. Skompiluj kod: `gcc -o hosting-server main.c -lpthread`
4. Uruchom serwer: `./hosting-server`

## Kontrybucje

Zachęcamy do zgłaszania błędów, propozycji ulepszeń i wniosków dotyczących nowych funkcji. Każda kontrybucja jest mile widziana!

## Licencja

Ten projekt jest objęty licencją MIT - szczegóły można znaleźć w pliku [LICENSE](LICENSE).

---
