## Praca z apką

uruchomienie

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

### Biblioteki do dociągnięcia do pracy lokalnej

* [boost](https://www.boost.org/) (`sudo apt install libboost-all-dev`)
* [spdlog](https://github.com/gabime/spdlog) (`sudo apt install libspdlog-dev`)
