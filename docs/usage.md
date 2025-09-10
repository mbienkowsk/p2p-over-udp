## Working with the application

Start using [just](https://github.com/casey/just) (`curl --proto '=https' --tlsv1.2 -sSf https://just.systems/install.sh | bash -s -- --to /usr/local/bin`)

```bash
just run
```

build

```bash
just build
```

run tests

```bash
just test
```

linter

```bash
just lint
```

### Docker

**Note: Running the project via compose requires creating one hostX_resources directory in the project source folder for each client you want to run. These directories should be populated with the selected files.**

## Compose

Contains 3 targets for 3 clients.

```bash
docker compose run --rm --service-ports --build clientX sh
```

Starting a container:

```bash
docker compose up --rm --service-ports --build clientX
```

Run in separate terminals, they should see and cooperate with each other.

### Libraries to install for local work

* [spdlog](https://github.com/gabime/spdlog) (`sudo apt install libspdlog-dev`)

### Test run

Build the base image for all clients with required toolchains:

```bash
docker build -t psi-projekt .
```

Create a network:

```bash
docker network create psi-projekt_p2p_network
```

Start shell for client 1:

```bash
docker run --rm -it --name client1 --network psi-projekt_p2p_network -v ./:/app/ psi-projekt bash
```

Start shell for client 2:

```bash
docker run --rm -it --name client2 --network psi-projekt_p2p_network -v ./:/app/ psi-projekt bash
```

## Dockerfile

build the container

```bash
docker build . -t psi-projekt
```

entrypoint is just, commands run/build/lint/test are passed when starting:

```bash
docker run psi-projekt:latest [ run/build/lint/test ]
```

to work on the project without installing all libraries and rebuilding the image, you can mount the project directory to the container:

```bash
# only once.
docker build . -t psi-projekt

# Always works on the current code from the working directory.
docker run --rm -v ./:/app/ -it psi-projekt:latest just [ run/build/lint/test ]

# Example. Running `just format` in the container will format code on your local machine. No need to install tools.
docker run --rm -v ./:/app/ -it psi-projekt:latest just format
```
