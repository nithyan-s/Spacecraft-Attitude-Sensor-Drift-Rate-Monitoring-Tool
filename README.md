# Drift Rate Anomaly Detection System for Real-Time Orientation Monitoring
This project implements a robust real-time anomaly detection system that processes time-series data from inertial measurement units (IMUs), focusing on drift rates for pitch (DTG1), roll (DTG2), and yaw (DTG3). Utilizing a sliding window algorithm, the system continuously monitors and detects irregularities in drift patterns using predefined anomaly criteria. The solution is designed for high precision in performance monitoring and fault detection in real-time applications.

Technical Features:
Orientation-Based Data Processing: The system tracks drift rates for three critical IMU metrics: pitch (DTG1), roll (DTG2), and yaw (DTG3), at hourly intervals.

Anomaly Detection Algorithm:

1. Outlier Detection: <br>A sliding window of size 5 is employed to detect spikes in drift values. An anomaly is flagged when a drift rate deviates by more than ±0.5 from its neighboring values within the window.
2. Zero Drift Detection: <br>Identifies potential sensor failures by flagging cases where a drift rate reaches zero while neighboring values differ by more than ±0.5, indicating a sudden loss of signal.
3. Stability Anomaly: <br>Detects periods of stagnation by identifying when a drift rate remains constant for more than 4 hours, indicating possible sensor calibration issues or mechanical stalling.