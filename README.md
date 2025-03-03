# QGD-OE: Quaternion Gradient Descent Algorithm for IMU Orientation Estimation

## Overview
The **Quaternion Gradient Descent for IMU Orientation Estimation (QGD-OE)** algorithm provides an efficient and accurate method for estimating the orientation of an **Inertial Measurement Unit (IMU)**. It is designed to solve **Wahba’s Problem**, a fundamental challenge in attitude determination, where the objective is to find the optimal orientation given a set of vector measurements.

This repository includes:

- A **C++ implementation** of the QGD-OE algorithm as described in our paper:
 
     - **"QGD-OE: Quaternion Gradient Descent Algorithm for IMU Orientation Estimation,"** published in IEEE Transactions on Instrumentation and Measurement (Volume: 74).

- A **Python implementation** for performance analysis, as presented in our paper:

     - **"Performance Comparison of Real-Time Algorithms for IMU-based Orientation Estimation,"** published in the Proceedings of the European Wireless 2023; 28th European Wireless Conference, Rome, Italy, 2023, pp. 162-167.


## QGD-OE Algorithm Key Features
The QGD-OE algorithm is based on a gradient descent approach in the quaternion domain, optimizing orientation estimation by iteratively minimizing errors. The method computes accelerometer and magnetometer gradients directly in the quaternion field using generalized Hamilton-real algebra, ensuring greater accuracy. This makes the algorithm highly robust and suitable for real-time applications in fields such as robotics, navigation, and tracking.

## Erratum
It has come to our attention that there is an error in Eq. (9) of our published paper. The originally stated formula:

$$
u_{\rm{m}}  \mathrel{\mathop:}= \frac{u_{\rm{a}} h}{|h|} = (0,0,-1,0)',
$$

is incorrect. The correct formula should be:

$$
u_{\rm{m}} \mathrel{\mathop:}= \frac{(0, u_{\rm{a}} \times h)}{|u_{\rm{a}} \times h|} = (0,0,-1,0)'
$$

where \(u_{\rm{a}} \) and \( h \) are as originally defined in the paper. This correction ensures the mathematical consistency of the derivation and aligns with the intended formulation.

We apologize for any confusion this may have caused and appreciate the readers' understanding.



## Installation Requirements

#### 1. Install Required Libraries
Run the following commands to install dependencies:

- Boost C++ library (for modern C++ extensions)
```bash
    sudo apt-get install libboost-all-dev
```
- Armadillo mathematics library (linear algebra operations)
```bash
    sudo apt-get install libopenblas-openmp-dev libarmadillo-dev
```

- Serial interface library (to read serial data from an MCU)
```bash
    sudo apt-get install libserial-dev
```

#### 2. Environment Settings
Ensure your development environment is configured with:

- C++17
- Linked libraries: armadillo, pthread, serial


## Running the QGD-OE Algorithm
The `ExampleData` folder contains sample sensor data, including:

- Gyroscope, Accelerometer, and Magnetometer Measurements
- Configuration files (`*.cfg`) for running the algorithm

### Execute the Algorithm
Run the following command to execute QGD-OE with a given configuration file:
```bash
    ./qgd -c ExampleData/sense_0001.cfg
```

## Citation Request
If you use this algorithm in your research or projects, please **cite the following paper**:

**Hristina Radak, et al. (2025),**  
*"Quaternion Gradient Descent Algorithm for IMU Orientation Estimation,"*  
IEEE Transactions on Instrumentation and Measurement, vol. 74, Art. no. 9503816,  
DOI: [10.1109/TIM.2025.3533661](https://doi.org/10.1109/TIM.2025.3533661)

### BibTeX Citation
```bibtex
@ARTICLE{rad2025qgd,
  author={Radak, Hristina and Scheunert, Christian and Reisslein, Martin and Fitzek, Frank H. P.},
  journal={IEEE Transactions on Instrumentation and Measurement},
  title={{QGD-OE}: {IMU} Orientation Estimation Based on Gradient Descent in the Quaternion Field},
  year={2025},
  volume={74},
  pages={Art. no. 9503816},
  doi={10.1109/TIM.2025.3533661}
}
```

Your support in citing this work helps promote continued research in IMU-based orientation estimation. Thank you! 🚀
