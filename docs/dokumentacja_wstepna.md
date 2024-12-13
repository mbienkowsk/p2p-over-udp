# PSI 24Z - Wstępna dokumentacja projektu

**Maksym Bieńkowski** (<01178511@pw.edu.pl>) | Jędrzej Grabski | Aleksander Drwal | Tomasz Kowalski

## 1. Temat i treść zadania

### Program obsługujący prosty protokół P2P (Peer-to-Peer)

#### Założenia

- Zasób to obiekt z danymi binarnymi identyfikowany pewną nazwą. Za takie same zasoby uważa się zasoby o takich samych nazwach.
- Rozmiar zasobu jest znaczny (tj. większy od jednorazowego transferu sieciowego).
- Początkowo dany zasób znajduje się w jednym hoście, a następnie może być propagowany do innych hostów w ramach inicjowanego przez użytkownika “ręcznie” transferu. Raz pobrany zasób zostaje zachowany jako kopia.
- Po pewnym czasie działania systemu ten sam zasób może znajdować się w kilku hostach sieci.
- Program ma informować o posiadanych lokalnie (tj. w danym węźle) zasobach i umożliwiać ich pobranie.

#### Funkcjonalność programu

- Dodawanie nowych zasobów przez użytkownika – wprowadzanie z lokalnego systemu plików.
- Pobieranie zasobów:
  - Użytkownik może pobrać konkretny zasób po nazwie ze zdalnego hosta (jeden zasób na raz).
  - Użytkownik decyduje, z którego hosta dany zasób zostanie pobrany.
- Rozgłaszanie informacji o posiadanych lokalnie zasobach.

#### Dodatkowe założenia

- Zasób pobrany do lokalnego hosta jest kopią oryginału. Kopia jest traktowana tak samo jak oryginał (są nierozróżnialne), tj.:
  - Istnienie kopii jest rozgłaszane w taki sam sposób jak istnienie oryginału.
- Program powinien obsługiwać różne sytuacje wyjątkowe, np. przerwanie transmisji spowodowane błędem sieciowym.
- Lokalizacja zasobów odbywa się poprzez rozgłaszanie:
  - Wskazówka: użyć protokołu UDP, ustawić opcje gniazda `SO_BROADCAST`, wykorzystać adresy IP rozgłaszające (same bity “1” w części hosta).

#### Interfejs użytkownika

- Wystarczy prosty interfejs tekstowy.
- Interfejs powinien obsługiwać współbieżny transfer zasobów – tj. nie powinien się blokować w oczekiwaniu na przesłanie danego zasobu.

##### Wariant zadania W13/W22

- całość komunikacji (przesyłania zasobu) zrealizować na UDP, dodatkowo dla uproszczenia można przyjąć,
że zasób mieści się w całości w jednym datagramie (datagram danych może być zgubiony – należy to uwzględnić)
- implementacja w C/C++

## 2. Interpretacja treści zadania

**Główne funkcje programu:**

1. **Dodawanie zasobów lokalnych:**

   - Pobranie pliku binarnego z systemu plików lokalnego.
   - Rejestracja nazwy zasobu w katalogu lokalnym.

2. **Rozgłaszanie lokalnych zasobów:**

   - Periodyczne wysyłanie listy dostępnych zasobów przy użyciu protokołu UDP w trybie broadcast.

3. **Pobieranie zasobu:**

   - Otrzymanie nazwy zasobu od użytkownika.
   - Zapytanie zdalnych hostów o lokalizację zasobu.
   - Transfer zasobu z wybranego hosta przy użyciu UDP.

4. **Obsługa sytuacji wyjątkowych:**

   - Ponawianie zapytań w przypadku utraty datagramów.
   - Informowanie użytkownika o niepowodzeniach transferu.

---

## 4. Krótki opis funkcjonalny ("black-box")

### Użytkownik

- Może dodawać zasoby do katalogu lokalnego.
- Może wywołać pobieranie zasobu o podanej nazwie z określonego hosta.
- Może przeglądać lokalnie dostępne zasoby.

### Program

- Rozgłasza informacje o lokalnych zasobach.
- Obsługuje zapytania o zasoby od zdalnych hostów.
- Przesyła i odbiera zasoby w trybie UDP.
- Zarządza transferami bez blokowania interfejsu użytkownika.

---

## 5. Opis i analiza protokołów komunikacyjnych

### Wykorzystany protokół: **UDP (User Datagram Protocol)**

1. **Rozgłaszanie zasobów:**

   - **Format komunikatu:**

     | Pole           | Typ      | Opis                         |
     | -------------- | -------- | ---------------------------- |
     | Typ            | uint8_t  | 1 = Rozgłoszenie             |
     | Liczba zasobów | uint16_t | Liczba zasobów w komunikacie |
     | Nazwa zasobu   | string   | Unikalna nazwa zasobu        |

   - **Diagram komunikacji:**

   ![Broadcast diagram](./broadcast.png)

2. **Pobieranie zasobu:**

   - **Format zapytania:**

     | Pole         | Typ     | Opis                  |
     | ------------ | ------- | --------------------- |
     | Typ          | uint8_t | 2 = Zapytanie         |
     | Nazwa zasobu | string  | Unikalna nazwa zasobu |

   - **Diagram komunikacji:**

   ![Transfer diagram](./transfer.png)

---

## 6. Planowany podział na moduły

### Moduły

1. **Moduł zarządzania zasobami:**

   - Obsługuje dodawanie, usuwanie i przegląd zasobów lokalnych.

2. **Moduł sieciowy:**

   - Rozgłaszanie zasobów.
   - Obsługa zapytań i transferu zasobów przy użyciu UDP.

3. **Moduł interfejsu użytkownika:**

   - Prosty tekstowy interfejs umożliwiający wykonywanie operacji przez użytkownika.

4. **Moduł obsługi wyjątków:**

   - Zarządza retransmisją utraconych datagramów i obsługą błędów.

### Rysunek struktury

## ![Structure drawing](./structure.png)

## 7. Zarys koncepcji implementacji

### Język programowania

- **C++**

### Biblioteki

- **Boost.Asio**: Obsługa gniazd sieciowych i operacji asynchronicznych.
- **STL**: Przechowywanie i zarządzanie danymi lokalnymi.

### Narzędzia

- **CMake**: System budowy projektu.
- **GCC/Clang**: Kompilator.

### Ogólne podejście do implementacji

1. Klasa `Resource` reprezentująca zasób.
2. Implementacja klasy `NetworkManager` do obsługi rozgłaszania i transferów.
3. Implementacja klasy `ResourceManager` do zarządzania zasobami.
4. Implementacja klasy `ExceptionManager` do obługi wyjątków.
5. Wdrożenie prostego interfejsu tekstowego opartego na pętlach zdarzeń.
