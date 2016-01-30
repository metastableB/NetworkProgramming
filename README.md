# Source-Switch Network simulator
A network simulator with muli source and single switch simulation. The switch supports both **TDM and Packet Switching** and the sources support **BURSTY and FIXED** connection types. Refer to `config.h` for more configurable options.

## TODO
- [ ] Burst Mode debugging
- [ ] Burst size vs queuing delay
- [X] Lost packets vs Queue size
- [X] Queuing delay vs Packet sending rate
- [X] Queuing delay vs Queue size at source
- [ ] Move images to `/img`
- [ ] Move source files o `/src`
- [ ] ISSUE : `valgrind` reports memory leaks. Investigate.