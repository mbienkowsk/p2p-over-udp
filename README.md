## Praca z apką

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
docker run --rm -v ./:/app/ -it psi-projekt:latest [ run/build/lint/test ]

# Przykład. Uruchomienie `just format` w kontenerze, sformatuje kod na lokalnej maszynie. Bez konieczności instalowania narzędzi.
docker run --rm -v ./:/app/ -it psi-projekt:latest format
```

### Biblioteki do dociągnięcia do pracy lokalnej

* [boost](https://www.boost.org/) (`sudo apt install libboost-all-dev`)
* [spdlog](https://github.com/gabime/spdlog) (`sudo apt install libspdlog-dev`)
