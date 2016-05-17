# Source-Switch Network simulator: TCP Implementation
A network simulator with multi source and single switch simulation. The switch supports both **TDM and Packet Switching** and the sources support **BURSTY and FIXED** connection types. Refer to `config.h` for more configurable options.

The TCP layer congestion control has been implemented on top of the IP layer simulator.
## TODO
- [X] Burst Mode debugging
- [ ] Move source files o `/src`
- [ ] ISSUE : `valgrind` reports memory leaks. Investigate.