FROM gcc:15.1.0 AS stage1

WORKDIR /app
COPY . .
RUN apt update && apt upgrade -y && apt install -y cmake \
  libxrandr-dev \
  make libxinerama-dev \
  libxcursor-dev \
  libxi-dev \
  git-all \
  freeglut3-dev && mkdir build-web && \
  git clone https://github.com/emscripten-core/emsdk.git && cd emsdk && ./emsdk install latest && ./emsdk activate latest && . ./emsdk_env.sh && \
  cd ../build-web && emcmake cmake -DPLATFORM=Web .. && make

FROM golang:1.24.3-bookworm as stage2
WORKDIR /app
COPY --from=stage1 /app/web ./web
RUN cd web && go mod download && CGO_ENABLED=0 GOOS=linux go build -ldflags "-w" -o main .

FROM scratch
COPY --from=stage2 /app/web/main /main
COPY --from=stage2 /app/web/resources /resources
ENTRYPOINT ["/main"]
