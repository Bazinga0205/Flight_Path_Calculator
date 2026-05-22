# **3D Flight Path Calculator \& Data Exporter**

A C++ tool designed for deterministic kinematic analysis of aircraft or projectile motion across a three-dimensional spatial plane.

It is specialised to imitate the MCDU operations of an aircraft on a more visual level



The application handles independent Pitch (elevation) and Yaw (azimuth) vector inputs to calculate real-time flight coordinates under Newtonian gravitational constraints.

To streamline engineering workflows, the simulator features a zero-overhead binary data serializer that exports raw dynamic arrays directly into a structured ".bin" file, optimized for rapid matrix ingestion in MATLAB.



**Key Features3D Kinematic System**: Decouples velocity vectors into accurate Cartesian coordinates (V\_x, V\_y, V\_z) using explicit spherical-to-Cartesian mapping. Memory-Optimized Pipeline: Utilizes vector pre-allocation to eliminate heap fragmentation and reallocation overhead during runtime calculations.



Bulk Binary I/O: Bypasses slow ASCII text formatting pipelines. Writes contiguous, memory-aligned blocks of double arrays directly to disk using sequential byte-stream casting (reinterpret\_cast).

Input Boundary Safety: Features robust mathematical constraints to guard against illegal launch bounds, negative velocities, and floating-point step drift.

