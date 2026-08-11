FROM ubuntu:22.04

# avoid interactive prompts during apt installs
ENV DEBIAN_FRONTEND=noninteractive

# install build tools and Python
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    python3 \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

# copy simulator source and build it
COPY simulator/ /app/simulator
WORKDIR /app/simulator
RUN mkdir -p build && cd build && cmake .. && make

# copy backend
COPY backend/ /app/backend
WORKDIR /app/backend

# install Python deps
RUN pip3 install virtualenv
RUN virtualenv /app/venv
RUN /app/venv/bin/pip install fastapi uvicorn pydantic

# update binary path to where it is inside container
ENV BINARY_PATH=/app/simulator/build/riscv-sim

EXPOSE 8000

CMD ["/app/venv/bin/uvicorn", "server:app", "--host", "0.0.0.0", "--port", "8000", "--app-dir", "/app/backend"]