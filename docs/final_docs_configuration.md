# 4. Postać wszystkich plików konfiguracyjnych, logów, itp.

### W projekcie tworzenie obrazów jest skonfigurowane w **Dockerfile**:  
- instalowanie pakietów:
  - narzędzia, pakiety itd.
- przygotowanie środowiska: 
  - katalog roboczy
- ustawianie *just* jako komendy startowej

### Do kompilacji i budowania skonfigurowany jest plik **CMakeLists.txt**:
- wymagania minimalne
- standard C++
- katalogi z kodem źródłowym
- pliki źródłowe
- biblioteki
- testy

### Żeby uruchamiać kontenery na raz skonfigurowany jest plik **docker-compose.yaml**:
- daje możliwość uruchamiania kontenerów jednocześnie
- skonfigurowane są w nim dwa kontenery: client1 i client2
  - budowane są na podstawie tego samego obrazu
- podłączane są do tej samej sieci żeby mogły się komunikować

### Logi realizujemy przy użyciu biblioteki **spdlog**.  
- używany jest do wypisywania komunikatów o działaniach programu, wysłanych i odebranych wiadomościach itp.
- wypisuje informacje, ostrzeżenia, błędy
- użyty jest Logger (*Log.h*)
  - w zależności od użycia:
    - loguje na konsolę
    - zapisuje logi do pliku
- ma ustawiony poziom logowania na *trace*