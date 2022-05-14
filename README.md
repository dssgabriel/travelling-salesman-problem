# Optimization - Operational Research

The aim of this project is to implement a branch-and-bound algorithm to solve a combinatorial optimization problem.
The problem to solve is the Travelling Salesman Problem, or TSP.

## Dependencies
- GNU Make

## Build
To build the projet, use to provided Makefile:
```
make build
```
You can then run it using either the `run` command (provided in the Makefile) which uses the `sample_config.txt` or run it directly using one of the configurations available in the `datasets` directory:
```
target/tsp datasets/17_nodes.txt
```
