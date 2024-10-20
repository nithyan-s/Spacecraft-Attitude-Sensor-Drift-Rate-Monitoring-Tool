# Spacecraft-Attitude-Sensor-Drift-Rate-Compensation-Monitoring-Tool
This project implements a robust real-time anomaly detection system that processes time-series data from inertial measurement units (IMUs), focusing on pitch, roll, and yaw drift rate compensation. Utilizing a sliding window algorithm, the system continuously monitors and detects irregularities in drift patterns using predefined anomaly criteria. The solution is designed for high precision in performance monitoring and fault detection in real-time applications.

Technical Features:
Orientation-Based Data Processing: The system tracks drift rate compensation for three critical IMU metrics: pitch, roll, and yaw, at hourly intervals.

Anomaly Detection Algorithm:

1. Outlier Detection: <br>A sliding window of size 5 is employed to detect spikes in drift values. An anomaly is flagged when a drift rate deviates more than ±0.5 from its neighboring values within the window.
2. Zero Drift Detection: <br>Identifies cases where a drift rate compensation reaches zero while neighboring values differ by more than ±0.5, indicating a sudden signal loss.
3. Frozen Rate Anomaly: <br>Detects periods of stagnation by identifying when a drift rate remains constant for more than 4 hours, indicating possible sensor calibration issues or mechanical stalling.
