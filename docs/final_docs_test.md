### Scenariusze testów manualnych - zamierzone użycie

Poniżej przedstawiono scenariusze testowe, oraz otrzymane rezultaty.

---

#### **Test 1: Wyświetlenie listy dostępnych zasobów (`list-resources`)**
**Kroki testowe:**
1. Uruchom aplikację.
2. Wpisz komendę `list-resources`.

**Oczekiwana odpowiedź (sukces):**
```
   Listing resources available for download:

   file1.txt
   [192.168.1.2, 192.168.1.3]

   file2.jpg
   [192.181.2.3, 192.172.5.2, 192.151.2.5]

```
---

#### **Test 2: Wyszukanie pliku w sieci (`find <filename>`)**
**Kroki testowe:**
1. Uruchom aplikację.
2. Wpisz komendę `find example.txt`.

**Oczekiwana odpowiedź (sukces):**
```
The file 'example.txt' is available from the following hosts:
1) 192.168.1.5
2) 192.168.1.7
```
---

#### **Test 3: Pobranie pliku z hosta (`download <host-ip> <filename>`)**
**Kroki testowe:**
1. Uruchom aplikację.
2. Wpisz komendę `download 192.168.1.5 example.txt`.

**Oczekiwana odpowiedź (sukces):**
```
Downloading 'example.txt' from 192.168.1.5
```

---


#### **Test 5: Wyświetlenie listy komend (`help`)**
**Kroki testowe:**
1. Uruchom aplikację.
2. Wpisz komendę `help`.

**Oczekiwana odpowiedź (sukces):**
```
Available commands:
  list-resources
  find <filename>
  download <host-ip> <filename>
  change-resource-folder <new-folder-path>
  help
  exit
```

---

#### **Test 6: Zamknięcie aplikacji (`exit`)**
**Kroki testowe:**
1. Uruchom aplikację.
2. Wpisz komendę `exit`.

**Oczekiwana odpowiedź (sukces):**
```
Exiting CLI.
```

---

### Scenariusze testów manualnych - niepowodzenia

Poniżej przedstawiono scenariusze testowe dla przypadków, w których działanie aplikacji kończy się niepowodzeniem.

---

#### **Test 1: Wpisanie nieznanej komendy**
**Kroki testowe:**
1. Uruchom aplikację.
2. Wpisz nieznaną komendę, np. `unknown-command`.

**Oczekiwana odpowiedź:**
```
Unknown command: unknown-command
```

---

#### **Test 2: Wyświetlenie zasobów gdy nie ma dostępnych**
**Kroki testowe:**
1. Uruchom aplikację.
2. Wpisz komendę `list-resources`, gdy nie ma dostępnych danych

**Odpowiedź aplikacji:**
```
Listing resources available for download:
```

---

#### **Test 3: Wyszukiwanie pliku, który nie istnieje w sieci**
**Kroki testowe:**
1. Uruchom aplikację.
2. Wpisz komendę `find non-existing-file.txt`.

**Oczekiwana odpowiedź:**
```
No hosts have the file 'non-existing-file.txt'
```

---

#### **Test 4: Pobieranie pliku od hosta, który nie istnieje**
**Kroki testowe:**
1. Uruchom aplikację.
2. Wpisz komendę `download 192.168.1.100 example.txt`, gdzie `192.168.1.100` to nieistniejący host.

**Odpowiedź aplikacji:**
```
> download 192.168.1.100 example.txt
Downloading 'example.txt' from 192.168.1.100
> Download failed: example.txt
```

---

#### **Test 5: Pobieranie pliku, który nie jest udostępniany przez hosta**
**Kroki testowe:**
1. Uruchom aplikację.
2. Wpisz komendę `download 192.168.1.5 non-existing-file.txt`, gdzie plik nie istnieje na hoście.

**Odpowiedź aplikacji:**
```
> download 172.22.0.3 non-existing-file.txt
Downloading 'non-existing-file.txt' from 172.22.0.3
> Download failed: non-existing-file.txt
```

---

#### **Test 7: Wyszukiwanie pliku bez podania nazwy**
**Kroki testowe:**
1. Uruchom aplikację.
2. Wpisz komendę `find`.

**Oczekiwana odpowiedź:**
```
Usage: find <filename>
```

---

#### **Test 8: Pobieranie pliku bez podania adresu hosta**
**Kroki testowe:**
1. Uruchom aplikację.
2. Wpisz komendę `download example.txt` (brak adresu hosta).

**Oczekiwana odpowiedź:**
```
Usage: download <host-ip> <filename>
```

---

#### **Test 9: Pobieranie pliku bez podania nazwy pliku**
**Kroki testowe:**
1. Uruchom aplikację.
2. Wpisz komendę `download 192.168.1.5`.

**Oczekiwana odpowiedź:**
```
Usage: download <host-ip> <filename>
```
