# Stage 1: Build the shared library
FROM gcc:13.2 AS builder
WORKDIR /app
COPY toralize.c toralize.h Makefile /app/
RUN make compile

# Stage 2: Set up the Tor SOCKS server
FROM debian:latest

# Install Tor and necessary packages
RUN apt-get update && apt-get install -y \
    tor \
    curl \
    net-tools \
    iputils-ping \
    dnsutils \
    && rm -rf /var/lib/apt/lists/*

# Copy the built shared library and script into the container
COPY --from=builder /app/toralize.so /usr/local/bin/toralize.so
COPY toralize /usr/local/bin/toralize

# Make the script executable
RUN chmod 755 /usr/local/bin/toralize
RUN chmod 755 /usr/local/bin/toralize.so

# Expose the necessary port
EXPOSE 9050

# Start Tor and keep the container running
CMD ["tor"]
