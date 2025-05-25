# Option Derivatives Engine
![C++](https://img.shields.io/badge/-C++-2C41CB?style=for-the-badge&logo=C%2B%2B&logoColor=white)
![Python](https://img.shields.io/badge/-Python-E15622?style=for-the-badge&logo=Python&logoColor=white)
![Docker](https://img.shields.io/badge/Docker-2496ED?style=for-the-badge&logo=docker&logoColor=fff)

# Description
This is a toy but production-grade project designed to dive deep into **modern C++** through the lens of **quantitative finance**.

The goal is to create a **modular and extensible derivatives pricing library**, with support for multiple instruments, models, and pricing engines using modern design patterns like **Composition**, **Dependency Injection**, and the **Factory Pattern**.



## 🧠 Goals

- Learn C++ by building something non-trivial  
- Get comfortable with CMake, Conan, and numerical programming in C++  
- Have fun and build a small usable library! 


## 🛠️ Project Structure

```
DerivativesPricingEngine/
├── src/                                    # Main source code
│   ├── core/                               # Core abstractions and interfaces
│   │   ├── Instrument.cpp
│   │   ├── InterestRateCurve.cpp
│   │   ├── MarketData.cpp
│   │   ├── PricingEngine.cpp
│   │   └── UnderlyingModel.cpp
│   ├── curves/                             # Interest rate curve implementations
│   │   └── FlatCurve.cpp
│   ├── engines/                            # Pricing engines
│   │   ├── AnalyticalEngine.cpp
│   │   ├── BinomialTreeEngine.cpp
│   │   ├── FiniteDifferenceEngine.cpp
│   │   └── MonteCarloEngine.cpp
│   ├── instruments/                        # Financial instruments
│   │   ├── equity/
│   │   │   ├── Option.cpp
│   │   │   └── CallOption.cpp
│   │   └── interest_rates/
│   │       ├── Bond.cpp
│   │       └── Swap.cpp
│   ├── models/                             # Underlying stochastic models
│   │   └── BlackScholeModel.cpp
│   └── utils/                              # Utility functions and helpers
│       ├── AnalyticalFormulas.cpp
│       ├── CurveInterpolation.cpp
│       ├── DayCount.cpp
│       ├── EnumToString.cpp
│       ├── Simulation.cpp
│       ├── TimeSchedule.cpp
│       └── UsualPayoffs.cpp
├── external/                               # Third-party libraries and Conan dependencies
├── tests/                                  # Unit tests
│   ├── test_black_scholes_analytical.cpp
│   └── test_black_scholes_binomial_tree.cpp
├── CMakeLists.txt                          # CMake build configuration
├── conanfile.txt                           # Conan dependencies
├── build.sh                                # Build script
└── README.md                               # Project documentation
```

---

## 🚀 Getting Started

1. **Clone the repository**:
   ```bash
   git clone https://github.com/ItoWindsor/DerivativesPricingEngine.git
   cd DerivativesPricingEngine
   ```

2. **Build the project**:
   ```bash
   ./build.sh
   ```

3. **Run the main**:
   ```bash
   ./build/debug/dpe_main
   ```
---
