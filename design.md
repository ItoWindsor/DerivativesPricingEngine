# Design

- Option Pricer -> one entity 

How should one price a derivative ? 
- Take into account the contract 
    - maturity, strike, ...
- Take into account the method used to generate the price 
    - analytical ? monte-carlo ? Finite Difference ?
- Take into account the market data 
    - the derivative's parameters should be fitted to the market data 
        - interest rate curve 
        - market price of the underlying 
        - find information about about the relations between each underlying (correlation, ...)


