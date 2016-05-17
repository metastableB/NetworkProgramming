# Network Programming Projects

These are some projects I undertook to understand socket programming, P2P communication and data transfer and other networking concepts. They were some major coding and implementation challenges and hence the github repository.

Visit individual folders for their `README.md`. I never intended these code to be read only so there is no detailed documentation, but I think the comments should be enough.

The different apps are sorted into corresponding folders:

- Network-Simulator(IP) : A discrete event network simulator similar to NS2. Supports a multi source single switch model.
- Network-Simulator (TCP/IP) : Built on top of the IP version, this adds support for transport layer congestion control using AIMD/QUBIC protocols.
- IRC-Model-Chat-Client: This is a P2P chat client and an associated authentication server. The communication is using TCP sockets. It supports group chats and direct user to user chat as well.
- CHORD-P2P-Lookup: CHORD is a simple but highly efficient and scalable protocol designed for key-value lookup over distributed network. This is used as part of P2P file transfer and storage protocols like the Bit Torrent protocols. The CHORD paper was presented at the ACM-SIGCOMM and this is a prototype implementation of the same.


