# PSI 24Z – Dokumentacja końcowa

## Opis interfejsu użytkownika

Interfejs CLI pozwala użytkownikom na zarządzanie wyświetlanie zasobów dostępnych w sieci peer-to-peer, wyszukiwanie plików w sieci, pobieranie plików od innych hostów. Poniżej opisano szczegółowo funkcje i dostępne komendy:

---

#### **1. Uruchomienie aplikacji**
Po uruchomieniu aplikacji użytkownik widzi powitanie oraz listę dostępnych komend:
```
Welcome to the P2P File Sharing CLI!
Available commands:
  list-resources
  find <filename>
  download <host-ip> <filename>
  help
  exit
```

#### **2. Dostępne komendy**
1. **`list-resources`**  
   Wyświetla listę zasobów dostępnych w sieci P2P, które można pobrać od innych użytkowników.

   **Przykład użycia:**  
   ```
   > list-resources
   Listing resources available for download:
   
   file1.txt
   [192.168.1.2, 192.168.1.3]

   file2.jpg
   [192.181.2.3, 192.172.5.2, 192.151.2.5]

   ```

2. **`find <filename>`**  
   Wyszukuje określony plik w sieci i podaje listę hostów, którzy go udostępniają.

   **Przykład użycia:**  
   ```
   > find example.txt
   The file 'example.txt' is available from the following hosts:
   1) 192.168.1.5
   2) 192.168.1.7
   ```

3. **`download <host-ip> <filename>`**  
   Pobiera plik od określonego hosta w sieci P2P.

   **Przykład użycia:**  
   ```
   > download 192.168.1.5 example.txt
   Downloading 'example.txt' from 192.168.1.5...
   ```

4. **`help`**  
   Wyświetla listę wszystkich dostępnych komend.

   **Przykład użycia:**  
   ```
   > help
   Available commands:
     list-resources
     find <filename>
     download <host-ip> <filename>
     help
     exit
   ```

5. **`exit`**  
   Zamyka aplikację.

   **Przykład użycia:**  
   ```
   > exit
   Exiting CLI.
   ```

---

#### **3. Obsługa błędów**
Interfejs CLI zapewnia obsługę błędów i podpowiedzi, gdy użytkownik wprowadzi nieprawidłowe polecenie lub pomyli składnię.

**Przykłady obsługi błędów:**
- W przypadku braku wymaganych argumentów:
   ```
   > find
   Usage: find <filename>
   ```
- W przypadku nieznanej komendy:
   ```
   > unknown-command
   Unknown command: unknown-command
   ```

---

#### **4. Wyjście z aplikacji**
Aby zamknąć aplikację, użytkownik może wpisać komendę `exit`. Aplikacja wyświetla komunikat i kończy działanie.