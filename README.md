# Source-Switch Network simulator
A network simulator with muli source and single switch simulation. The switch supports both **TDM and Packet Switching** and the sources support **BURSTY and FIXED** connection types. Refer to `config.h` for more configurable options.

## TODO
- [ ] Burst Mode debugging
- [ ] Queuing delay vs Burst Size
- [ ] Queuing delay vs Burst Time Delta
- [X] Queuing delay vs Packet Sending Rate
- [X] Queuing delay vs Queue Size at Source
- [X] Lost Packets at Source vs Queue size
- [X] Lost Packets at Switch vs Switch Queue size
- [ ] Out Queue vs Switch Queue
- [X] Move images to `/img`
- [ ] Move source files o `/src`
- [ ] ISSUE : `valgrind` reports memory leaks. Investigate.