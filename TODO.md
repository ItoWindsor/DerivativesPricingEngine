# ✅ TODO — C++ Derivatives Pricing Library (Linear Roadmap)

This roadmap follows a top-down build order:
1. Start with core abstractions
2. Add simple instruments/models
3. Implement analytic pricing
4. Expand to advanced models/engines
5. Add infra, tests, and docs

---

## 1️⃣ Core Abstractions

**Branch**: `feature/core-interfaces`

- [ ] Create `core/Instrument.hpp` interface
- [ ] Create `core/Model.hpp` interface
- [ ] Create `core/PricingEngine.hpp` interface
- [ ] Add `MarketDataProvider` base class
- [ ] Create `core/PricingContext.hpp` to bind market data + model + engine

---

## 2️⃣ First Instrument: European Option

**Branch**: `feature/instruments-european`

- [ ] Implement `EuropeanOption` instrument
- [ ] Add enum/types for option style, direction, payoff type
- [ ] Write instrument factory class (for future plug-and-play)

---

## 3️⃣ First Model: Black-Scholes

**Branch**: `feature/black-scholes-model`

- [ ] Implement `BlackScholesModel` (volatility, drift, risk-free rate)
- [ ] Expose parameters via `Model::getParams()`
- [ ] Add support for time-dependent volatility (as extension)

---

## 4️⃣ First Engine: Analytic Black-Scholes

**Branch**: `feature/analytic-engine`

- [ ] Implement `AnalyticEngine` for European Options
- [ ] Bind `BlackScholesModel` to `EuropeanOption` via `AnalyticEngine`
- [ ] Implement `PricingEngine::price(const Instrument&) -> double`
- [ ] Write unit test comparing against known closed-form result

---

## 5️⃣ DevOps Setup

**Branch**: `devops`

- [ ] Create modular CMake build
- [ ] Integrate Conan for dependencies (e.g., Boost, Eigen)
- [ ] Add Dockerfile for isolated builds
- [ ] Add GitHub Actions workflow for build + test
- [ ] Add coverage reports (with gcov/lcov)

---

## 6️⃣ Expand: Heston Model

**Branch**: `feature/heston-model`

- [ ] Implement `HestonModel` (variance process, correlation)
- [ ] Add `Parameter` objects for calibration later
- [ ] Add shared base for stochastic volatility models

---

## 7️⃣ Expand: Monte Carlo Engine

**Branch**: `feature/montecarlo-engine`

- [ ] Implement path generator for Black-Scholes
- [ ] Integrate with `EuropeanOption` via `MonteCarloEngine`
- [ ] Add payoff sampling + discounting
- [ ] Run convergence tests (100, 1k, 10k paths)

---

## 8️⃣ Expand: Binomial Tree & FDM Engines

**Branch**: `feature/tree-fdm-engines`

- [ ] Implement binomial tree pricing for European Options
- [ ] Implement Crank-Nicolson FD method
- [ ] Compare against analytic + MC results

---

## 0 - Testing -> Should always be done in parallal of everything 

**Branch**: `feature/unit-tests`

- [ ] Add unit tests for each instrument
- [ ] Add model sanity tests (e.g., Greeks)
- [ ] Add Monte Carlo regression tests
- [ ] Add test helpers for reproducible pricing

---

## 🔟 Documentation

**Branch**: `docs/architecture`

- [ ] Document core architecture + object model
- [ ] Add diagrams for model/engine/instrument interaction
- [ ] Write “how to add a model” guide
- [ ] Add `README.md` with project structure

---

## ✅ Legend

- [x] = Complete
- [ ] = To Do

Optional: Add GitHub issues for each box and link them.

