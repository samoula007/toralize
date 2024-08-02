# Setup the container
FROM gcc:13.2 AS builder
WORKDIR /app
COPY toralize.c toralize.h Makefile /app/
RUN make compile
FROM debian:latest

# Get the necessary packages
RUN apt-get update && apt-get install -y \
    tor \
    curl \
    net-tools \
    iputils-ping \
    dnsutils \
    && rm -rf /var/lib/apt/lists/*

# Copy files to container, update permissions
COPY --from=builder /app/toralize.so /usr/local/bin/toralize.so
COPY toralize /usr/local/bin/toralize
RUN chmod 755 /usr/local/bin/toralize
RUN chmod 755 /usr/local/bin/toralize.so

# Expose the proxy port
EXPOSE 9050

# Start Tor and keep the container running
CMD ["tor"]
