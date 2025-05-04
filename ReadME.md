# Option Derivatives Engine
![C++](https://img.shields.io/badge/-C++-2C41CB?style=for-the-badge&logo=C%2B%2B&logoColor=white)

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
├── src/
│   ├── Instruments/         # Option and bond classes
│   ├── Models/              # Black-Scholes, etc.
│   ├── PricingEngines/      # Analytical, MonteCarlo and FiniteDifference pricing engine
│   ├── Curves/              # Interest rate curves with interpolation
│   ├── Utils/               # Date handling, CSV loading, payment schedule generation, etc.
│   └── Core/                # Base interfaces and factories
├── tests/                   # Unit tests using Google Test
├── external/                # Conan-managed dependencies (e.g., Eigen)
├── CMakeLists.txt           # CMake configuration
├── conanfile.txt            # Conan dependencies
└── build.sh                 # Convenience build script
```

---

## 🚀 Getting Started

1. **Clone the repository**:
   ```bash
   git clone https://github.com/ItoWindsor/DerivativesPricingEngine.git
   cd CppLearn
   ```

2. **Build the project**:
   ```bash
   ./build.sh
   ```

3. **Run the tests**:
   ```bash
   ./build/tests/RunAllTests
   ```

---
