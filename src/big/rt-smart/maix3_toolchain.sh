#!/bin/bash
mkdir -p ~/.tools/gnu_gcc/

# get tollchain
curl -s https://download.rt-thread.org/rt-smart/riscv64/riscv64-unknown-linux-musl-rv64imafdcv-lp64d-20230420.tar.bz2 | tar -xjf - -C ~/.tools/gnu_gcc/
