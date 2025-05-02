# CN_CHomeworks 4 with Bonus
CN_CHomeworks 4

**Contributors**:  
- Amirali Shahriary (810100173)
- Shahzad Momayez (810100272)  

## Overview
This project focuses on implementing a **TCP-based network simulator** to evaluate data transmission and congestion control algorithms in computer networks. The simulation replicates realistic scenarios using a simplified **BitTorrent-like protocol** to manage large file transfers between multiple devices.


## Objectives
- Implement **TCP** and **congestion control** mechanisms using algorithms like:
  - **TCP NewReno**
  - **TCP Vegas**
  - **TCP Veno**
- Simulate a **P2P file transfer** scenario using a simplified BitTorrent protocol.
- Visualize network congestion by plotting the number of packets in router buffers.

## Algorithm Comparison: TCP NewReno vs. TCP Vegas vs. TCP Veno

In this project, we implement and compare the performance of three TCP congestion control algorithms: **TCP NewReno**, **TCP Vegas**, and **TCP Veno**. Each algorithm has its unique approach to managing congestion, and the goal is to evaluate their effectiveness under different network conditions.

### 1. TCP NewReno
**Category:** Loss-based Congestion Control  
**Key Features:**
- An enhancement of TCP Reno that improves performance during the fast-recovery phase.
- Detects and recovers from multiple packet losses without waiting for a retransmission timeout.
- Uses **duplicate acknowledgments (ACKs)** as a signal for packet loss.
- Aggressive approach: increases the congestion window until a packet loss occurs.

**Advantages:**
- Better performance than TCP Reno in lossy networks.  
- Reduces the time required for congestion window recovery after multiple losses.

**Disadvantages:**
- Loss-based, so it reacts only after packet loss, which can lead to degraded performance in high-congestion scenarios.  
- Less efficient in low-latency networks.

---

### 2. TCP Vegas
**Category:** Delay-based Congestion Control  
**Key Features:**
- Monitors round-trip time (RTT) to detect early signs of congestion before packet loss occurs.
- Adjusts the congestion window based on measured RTT differences, aiming for a more proactive approach.
- Maintains a balance between throughput and fairness.

**Advantages:**
- Prevents congestion more effectively than loss-based algorithms.  
- Superior performance in low-latency, high-bandwidth networks.  
- Reduces packet loss and improves overall network stability.

**Disadvantages:**
- Susceptible to unfair competition with loss-based algorithms like TCP NewReno.  
- Performance may degrade in highly dynamic networks with fluctuating delays.

---

### 3. TCP Veno
**Category:** Hybrid Congestion Control (Combination of Loss-based and Delay-based)  
**Key Features:**
- A hybrid approach that combines elements of both TCP NewReno and TCP Vegas.  
- Uses RTT variations as an early indicator of congestion but also relies on packet loss to adjust the congestion window.  
- Balances proactive congestion avoidance with reactive recovery mechanisms.

**Advantages:**
- Provides better performance in lossy environments compared to TCP Vegas.  
- More responsive to congestion compared to TCP NewReno.  
- Reduces unnecessary retransmissions and packet loss.

**Disadvantages:**
- Slightly more complex to implement due to its hybrid nature.  
- Performance may vary depending on network conditions and congestion levels.

---

### Summary of Comparison
| Algorithm   | Approach    | Strengths                             | Weaknesses                          |
|-------------|-------------|---------------------------------------|-------------------------------------|
| **TCP NewReno** | Loss-based  | Effective in lossy networks | Reacts only after packet loss       |
| **TCP Vegas**   | Delay-based | Proactive congestion control, low packet loss | Poor fairness with loss-based algorithms |
| **TCP Veno**    | Hybrid      | Balances delay and loss-based control | Complexity in implementation        |

### Final Decision:
- **TCP NewReno** is suitable for high-loss networks where quick recovery from packet loss is essential.  
- **TCP Vegas** is ideal for stable, low-latency networks with predictable delay patterns.  
- **TCP Veno** offers a balanced approach, making it a good choice for mixed environments with moderate congestion and variable delays.



## TCP Connection Setup: Three-Way Handshake (Implementation):

In this project, we  implemented the **TCP connection setup** using the **three-way handshake** mechanism. This process allowed us to establish reliable communication between the sender and receiver in our network simulator. Below is a detailed breakdown of the steps we followed:

### 1. Three-Way Handshake Process
We implemented the handshake as follows:  
1. **SYN Packet**: The client (sender) sent a SYN packet to the server (receiver) to initiate the connection.  
2. **SYN-ACK Packet**: The server responded with a SYN-ACK packet, acknowledging the client’s request and signaling readiness to establish the connection.  
3. **ACK Packet**: Finally, the client sent an ACK packet back to the server, confirming the successful connection setup.

---

### 2. Packet Structure
We designed specific headers for each type of packet:  
- **SYN Packet**: The header had the **SYN flag set to 1** and **ACK flag set to 0**.  
- **SYN-ACK Packet**: Both **SYN and ACK flags were set to 1** to indicate acknowledgment of the SYN packet.  
- **ACK Packet**: Only the **ACK flag was set to 1**, confirming the connection.

---

### 3. Implementation
We implemented functions to generate packets with the appropriate flags for SYN, SYN-ACK, and ACK. Here’s how we managed the sequence and acknowledgment numbers:  
- For the **SYN packet**, we sent it with an initial sequence number.  
- For the **SYN-ACK packet**, the server responded with an acknowledgment number and its own sequence number.  
- For the **ACK packet**, the client sent an acknowledgment number that was **one more than the server's sequence number**.

---

### 4. Error Handling
To ensure a robust connection setup, we implemented error-handling mechanisms:  
- **Retransmission on Timeout**: If any packet was lost or corrupted, a timeout triggered a retransmission.  
- We ensured that each step of the handshake handled packet loss and ensured proper connection recovery.

---

### 5. Integration with the Network Simulator
We integrated the three-way handshake process into our larger network simulator. This ensured that:  
- Packets were routed correctly through the network.  
- The connection was reliably established between the sender and receiver nodes.  

---

### 6. Testing
We thoroughly tested our implementation under various network conditions:  
- **Normal Conditions**: The connection was established without issues.  
- **Congested Network**: The handshake mechanism remained stable, and timeout-based retransmissions ensured reliability.  
- **Packet Loss Scenarios**: The simulator handled lost packets and reestablished the connection with minimal delay.  

---

## Code Structure:

### handleReceivedPacket:

![handlereceivedpacket](https://github.com/AmiraliShahriary/CN_CHomeworks_3_5/blob/main/readmefiles/handlereceivedpacket.jpg)


We implemented a **TCPProtocol::handleReceivedPacket** function to manage incoming TCP packets and handle various stages of the **TCP connection setup** and data transmission. This function processes packets based on their control flags and triggers appropriate actions such as **SYN**, **SYN-ACK**, **ACK**, **PSH**, and **FIN**.

### createSynPacket:

![createSynPacket](https://github.com/AmiraliShahriary/CN_CHomeworks_3_5/blob/main/readmefiles/CreateSynPacket.jpg)


The **TCPProtocol::createSynPacket** function is responsible for creating a **SYN packet**, which initiates the **TCP three-way handshake**. This packet is sent by the client to request a connection with the server.


### HandleACK:

![HandleACK](https://github.com/AmiraliShahriary/CN_CHomeworks_3_5/blob/main/readmefiles/handleAck.jpg)


The **TCPProtocol::handleAck** function processes the **ACK packet**, marking the successful completion of the **TCP three-way handshake**. This step confirms that the connection has been fully established between the sender and receiver.


- **Packet Reception**: The function retrieves the TCP header from the received packet.  
- **Debug Message**: Outputs `"TCP 3-way handshake complete!"` to indicate that the handshake process was successful.  
- **Connection Status Update**: Calls `setConnectedTo` with the source IP of the received packet, updating the connection status to `true` for that IP address.  
 / / /
- **Preceding Step**: This function is called after the **SYN-ACK** packet has been sent by the server, and the client responds with an **ACK packet** to confirm the connection.  
- **Result**: The connection is now fully established, allowing data transmission between the sender and receiver.

### handleFin:

![handleFin](https://github.com/AmiraliShahriary/CN_CHomeworks_3_5/blob/main/readmefiles/HandleFin.jpg)

The **TCPProtocol::handleFin** function processes a **FIN packet**, which indicates that the sender wants to terminate the connection. This function is responsible for acknowledging the FIN request and transitioning the connection into the **CLOSE_WAIT** state while waiting for the final ACK from the other side.

- **Packet Reception**: The function retrieves the TCP header from the received packet.  
- **Create and Send FIN-ACK Packet**:  
  - It calls `createFinAckPacket` to generate a **FIN-ACK packet** in response.  
  - The `sendPacketToRemoteIP` function sends this packet back to the remote IP to acknowledge the FIN request.  
- **Update Connection State**:  
  - The source IP from the packet is checked in the `m_connections` map.  
  - If the connection exists, its state is updated to **CLOSE_WAIT**, indicating that the local side has acknowledged the FIN and is waiting for the final ACK to complete the teardown process.  
/ / / 
- **FIN Packet (from Peer)**: The remote peer requests to terminate the connection.  
- **FIN-ACK Response**: This function acknowledges the termination request by sending a **FIN-ACK**.  
- **CLOSE_WAIT State**: The connection enters this state while waiting for the final ACK from the remote peer.  

### createFinAckPacket:

![createFinAckPacket](https://github.com/AmiraliShahriary/CN_CHomeworks_3_5/blob/main/readmefiles/createFinACKPacket.jpg)


The **TCPProtocol::createFinAckPacket** function generates a **FIN-ACK packet**, which is sent in response to a received **FIN packet** during the connection termination process. This packet acknowledges the request to close the connection.

- **Packet Creation**: A new packet is created with the type set to `UT::PacketType::Control`.  
- **TCP Header Configuration**:  
  - The **ACK flag** is set to indicate that this packet acknowledges the received FIN.  
  - **Source and Destination Ports** are both set to `443`.  
  - The **Sequence Number** is set to `100` (example value), and the **Acknowledgment Number** is set to `101`. This indicates that the FIN packet with sequence number `100` has been acknowledged.  
- **Return the Packet**: The configured packet is returned for transmission.
/ / / 
- **After Receiving a FIN Packet**: This function is called to create a FIN-ACK response packet.  
- **Acknowledgment of FIN**: The FIN-ACK confirms the receipt of the FIN packet and signals that the local side is ready to close the connection.  


### CreateAckPacket:


![CreateAckPacket](https://github.com/AmiraliShahriary/CN_CHomeworks_3_5/blob/main/readmefiles/CreateAckPacket.jpg)

The **TCPProtocol::createAckPacket** function generates an **ACK packet**, which is used to acknowledge the successful receipt of data or connection-related packets during the TCP communication process.

- **Packet Creation**:  
  - A new packet of type `UT::PacketType::Control` is created.  
  - A TCP header (`tcpHeader`) is instantiated and configured to represent an **ACK packet**.  

- **TCP Header Configuration**:  
  - **ACK Flag**: Set to indicate that this is an acknowledgment packet.  
  - **Source and Destination Ports**: Set using `source_port` and `dest_port` variables.  
  - **Sequence Number (`seq_num`)**: Represents the sequence number for the acknowledgment.  
  - **Acknowledgment Number (`ack_num`)**: Indicates the next expected sequence number from the sender, confirming receipt of all preceding packets.  

- **Return the Packet**: The configured ACK packet is returned for transmission.

/ / / 


- **During Connection Setup**: This function is called to create the final **ACK packet** in the **three-way handshake**, completing the connection setup.  
- **Data Transmission**: The function is also used to acknowledge data packets during the established connection.  
- **Connection Teardown**: It plays a critical role in confirming the receipt of **FIN-ACK packets** during connection termination.

### CreateFinPacket:

![CreateFinPacket](https://github.com/AmiraliShahriary/CN_CHomeworks_3_5/blob/main/readmefiles/CreateFinPacket.jpg)


The **TCPProtocol::createFinPacket** function generates a **FIN packet**, which is used to initiate the termination of an established TCP connection. This packet informs the remote peer that the sender wants to close the connection.

- **Packet Creation**:  
  - A new packet of type `UT::PacketType::Control` is created.  
  - A TCP header (`tcpHeader`) is configured to represent a **FIN packet**.  

- **TCP Header Configuration**:  
  - **FIN Flag**: Set to indicate the intention to terminate the connection.  
  - **Source and Destination Ports**: Set using `source_port` and `dest_port` variables.  
  - **Sequence Number (`seq_num`)**: Represents the sequence number for the FIN packet.  
  - **Acknowledgment Number (`ack_num`)**: Used to acknowledge any previously received data packets.  

- **Return the Packet**: The configured FIN packet is returned for transmission.

- **Connection Termination**: This function is called when a peer wishes to close the connection.  
- **Initial Step in Teardown**: The **FIN packet** marks the first step in the connection teardown process. After sending this packet, the sender waits for a **FIN-ACK** response from the receiver.  

### CreateSynAckPacket:

![CreateSynAckPacket](https://github.com/AmiraliShahriary/CN_CHomeworks_3_5/blob/main/readmefiles/CreateSynAckPacket.jpg)


The **TCPProtocol::createSynAckPacket** function generates a **SYN-ACK packet**, which is a crucial part of the **TCP three-way handshake**. This packet is sent in response to an incoming **SYN packet**, signaling that the server acknowledges the connection request and is ready to establish the connection.

- **Packet Creation**:  
  - A new packet of type `UT::PacketType::Control` is created using `PacketPtr::create()`.  
  - A TCP header (`tcpHeader`) is initialized and configured to represent a **SYN-ACK packet**.  

- **TCP Header Configuration**:  
  - **ACK Flag**: The `UT::PacketControlType::TCP_ACK` flag is set to indicate acknowledgment of the received SYN.  
  - **Source and Destination Ports**: These are set using `source_port` and `dest_port`.  
  - **Sequence Number (`seq_num`)**: Defines the sequence number for this SYN-ACK packet.  
  - **Acknowledgment Number (`ack_num`)**: Confirms receipt of the previous SYN packet.  

- **Returning the Packet**:  
  - The configured SYN-ACK packet is returned for transmission.  

/ / / 

- **Connection Establishment**: This function is called when a system responds to an incoming **SYN request** from a remote peer.  
- **Second Step in Three-Way Handshake**: The **SYN-ACK packet** confirms the receipt of a connection request and signals readiness to establish a connection.  


---

### Challenges during project:
here was the bugs we faced at first, but then we tried to fix them

![output1](https://github.com/AmiraliShahriary/CN_CHomeworks_3_5/blob/main/readmefiles/output1.jpg)

![output2](https://github.com/AmiraliShahriary/CN_CHomeworks_3_5/blob/main/readmefiles/output2.jpg)



