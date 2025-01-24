### 2. Opis najważniejszych rozwiązań funkcjonalnych wraz z uzasadnieniem (np opis: struktur danych, kluczowych funkcji, itp.)

W architekturze programu można wyróżnić 5 główne komponenty:

#### CLI - interfejs linii poleceń.

Pozwala na interakcję użytkownika ze wszystkimi funkcjami programu: pobieranie pliku, wyświetlanie dostępnych plików, itd. CLI jest uruchamiane w głównym wątku programu i zakończenie go, kończy działanie programu.

#### Rozgłaszacz broadcast
Komponent uruchomiony w osobnym wątku, który cyklicznie rozgłasza informacje o dostępnych plikach.


#### Serwer
Komponent uruchomiony w osobnym wątku, który nasłuchuje na przychodzące pakiety i wykonuje wymagane nie blokując zadania np. zapisuje plik na dysku oraz zleca zadania innym komponentom.

#### Moduł pobierania plików
Komponent przechowujący hash mapę wątków pobierających pliki.

Do zlecenia pobrania pliku przypisywany jest nowy wątek (identyfikowany po nazwie pliku), który do 5 razy próbuje pobrać plik od innego klienta. Wątek jest usuwany z hash mapy po udanej lub nieuadanej próbie pobrania pliku.

Takie rozwiązanie z dynamicznym tworzeniem wątków pozwala na pobieranie wielu plików jednocześnie bez blokowania innych operacji.

Sekcja krytyczna zabezpieczona jest mutexem, który zapewnia, że dostęp do hash mapy i tworzenie nowych zleceń pobrania pliku jest bezpieczne w środowisku wielowątkowym.


#### Moduł zarządzania plikami
Komponent przechowujący listę plików dostępnych do pobrania od każdego połączonego klienta. Jest to Hash mapa z kluczami będącymi IP innych użytkowników i wartościami listą plików dostępnych od nich do pobrania.

Moduł ten pozwala na aktualizaowanie listy plików dostępnych do pobrania oraz rozgłoszania.

Sekcja krytyczna zabezpieczona jest mutexem, który zapewnia stabilne działanie komponentu nawet w gdy jednocześnie wiele użytkowników powoduje zmiany w liście plików dostępnych do pobrania.
