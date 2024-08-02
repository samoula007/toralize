IMAGE_NAME := toralize
CONTAINER_NAME := toralize-container

clean:
	if [ -f toralize.so ]; then rm toralize.so; fi

compile:
	gcc toralize.c -o toralize.so -fPIC -shared -ldl -D_GNU_SOURCE

build:
	docker build -t $(IMAGE_NAME) .

run:
	docker run -d -p 9050:9050 --name $(CONTAINER_NAME) $(IMAGE_NAME)

exec:
	docker exec -it $(CONTAINER_NAME) /bin/sh

stop:
	docker stop $(CONTAINER_NAME)
	docker rm $(CONTAINER_NAME)

all: build run exec
