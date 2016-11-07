.PHONY: all
all: build

.PHONY: build
build:
	go build -o jail.so jail.go

.PHONY: test
test:
	go test ./...
