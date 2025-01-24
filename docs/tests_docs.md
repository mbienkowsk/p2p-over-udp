## Testowanie

### Podczas testowania aplikacji pojawiły się różne błędy:

#### Użytkownik widzący swoje własne pliki:
- BroadcastUDP prowadził do tego, że użytkownik miał wyświetlane pliki znajdujace się w jego własny lokalnym folderze zasobów
- Rowiązanie: Host ignoruje własne pakiety

#### Segfaulty
- Przez niepoprawną synchronizację i zabijanie wątków dochodziło do błędów segmentacji (segfault)
- Rozwiązanie: Żeby tego uniknąć zastowana została flaga z atomic boolem  dla pobocznych wątków

#### Błędne formatowanie CLI
- Dochodziło do błędnego formatowania CLI:
  - pobieranie pliku o arbitralnej nazwie, który nie był udostępniany przez hosta skutkowało wiadomościami o błędzie w cli hosta, od którego próbowano pobrać plik
  - plik o tej nazwie tworzony był w katalogu pobierającego
- Powodowany przez asynchroniczne otrzymanie komunikatów o powodzeniu bądź niepowodzeniu pobierania
- Rozwiązanie: Sekwencja escape code i std::flush