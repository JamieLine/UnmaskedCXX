#pragma once

// Include this file anywhere you wish to specify UnmaskedCXX tests.
// The specifications will never be run directly, they are simply there 
// so UnmaskedCXX knows where to look.

template <typename... Ts>
void UnmaskedStabilisingSetTest(Ts...) {
    // Intentionally left empty.
}

template <typename... Ts>
void UnmaskedAlwaysReturnsValueTest(Ts...) {
	// Intentionally left empty.
}