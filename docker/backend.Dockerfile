# Stage 1: Build
FROM --platform=linux/amd64 gcc:13.2 AS builder

WORKDIR /app

# Install system dependencies
RUN apt-get update && \
    apt-get install -y wget python3 python3-venv make && \
    wget -qO- https://github.com/Kitware/CMake/releases/download/v3.30.0/cmake-3.30.0-linux-x86_64.tar.gz | tar --strip-components=1 -xz -C /usr/local && \
    rm -rf /var/lib/apt/lists/*

# Create and activate virtual environment
RUN python3 -m venv /opt/venv
ENV PATH="/opt/venv/bin:$PATH"

# Install Conan and configure for x86
RUN pip install --no-cache-dir "conan<2.0" && \
    conan profile new default --detect --force && \
    conan profile update settings.arch=x86_64 default && \
    conan profile update settings.arch_build=x86_64 default && \
    conan profile update settings.compiler=gcc default && \
    conan profile update settings.compiler.version=13 default && \
    conan profile update settings.compiler.libcxx=libstdc++11 default

# Copy backend files
COPY backend/ /app/

# Build the project
RUN mkdir -p /app/build/release && \
    cd /app && \
    conan install . --output-folder=build/release --build=missing -s build_type=Release && \
    cd build/release && \
    cmake ../.. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release && \
    cmake --build .

# Stage 2: Runtime (reuse gcc image for compatibility)
FROM --platform=linux/amd64 gcc:13.2

# Only install minimal runtime dependencies if needed (optional)
# RUN apt-get update && apt-get install -y libssl-dev && rm -rf /var/lib/apt/lists/*

# Just copy what’s needed from the builder
COPY --from=builder /app/build/release/bin/dpe_main /usr/local/bin/dpe_main
COPY --from=builder /app/cpprest_api /app/cpprest_api
COPY --from=builder /app/data /app/data

EXPOSE 8080
CMD ["/usr/local/bin/dpe_main"]
