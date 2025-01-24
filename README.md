## Praca z aplikacją

uruchomienie przez [just](https://github.com/casey/just) (`curl --proto '=https' --tlsv1.2 -sSf https://just.systems/install.sh | bash -s -- --to /usr/local/bin`)

```bash
just run
```

zbudowanie

```bash
just build
```

uruchomienie testów

```bash
just test
```

linter

```bash
just lint
```

### Docker

**uwaga: uruchomienie projektu przez compose wymaga utworzenia w katalogu źródłowym projektu po jednym katalogu hostX_resources na każdego
uruchamianego klienta. Te katalogi należy spopulować wybranymi plikami.**

## Compose

zawiera 3 targety do 3 klientów.

```bash
docker compose run --rm --service-ports --build clientX sh
```

Uruchomienie kontenera:

```bash
docker compose up --rm --service-ports --build clientX
```

Uruchamiamy na oddzielnych terminalach, powinny się widzeć i współpracować.

### Biblioteki do dociągnięcia do pracy lokalnej

* [spdlog](https://github.com/gabime/spdlog) (`sudo apt install libspdlog-dev`)

### Uruchamianie testowe

Budowa bazowego obrazu dla wszystkich klientów z wymaganymi toolchainami:

```bash
docker build -t psi-projekt .
```

Utworzenie sieci:

```bash
docker network create psi-projekt_p2p_network
```

Uruchomienie powłoki klienta 1:

```bash
docker run --rm -it --name client1 --network psi-projekt_p2p_network -v ./:/app/ psi-projekt bash
```

Uruchomienie powłoki klienta 2:

```bash
docker run --rm -it --name client2 --network psi-projekt_p2p_network -v ./:/app/ psi-projekt bash
```

## Dockerfile

zbudowanie kontenera

```bash
docker build . -t psi-projekt
```

entrypoint to just, komendy run/build/lint/test przekazuje się przy uruchomieniu:

```bash
docker run psi-projekt:latest [ run/build/lint/test ]
```

żeby pracować nad projektem bez konieczności instalowania wszystkich bibliotek i przebudowywania obrazu, można podmontować katalog z projektem do kontenera:

```bash
# tylko raz.
docker build . -t psi-projekt

# Zawsze działa na aktualnym kodzie z katalogu roboczego.
docker run --rm -v ./:/app/ -it psi-projekt:latest just [ run/build/lint/test ]

# Przykład. Uruchomienie `just format` w kontenerze, sformatuje kod na lokalnej maszynie. Bez konieczności instalowania narzędzi.
docker run --rm -v ./:/app/ -it psi-projekt:latest just format
```
