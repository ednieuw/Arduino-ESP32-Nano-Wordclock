//
// NoClock.h - NOCLOCK variant: no display at all (headless BLE / HC-12 build).
// ---------------------------------------------------------------------------
// Included once from the .ino when NOCLOCK is selected. The trunk .ino calls a
// fixed set of display functions and symbols regardless of variant (rotary-press
// time-set flow, demo/fire mode, the '#' self-test command, the strip-object
// constructors, the Q display-choice command, the version banner). WordClock.h
// and Fibonacci.h both provide real or no-op versions of all of these; NoClock.h
// supplies only no-op stubs / minimal placeholders, since there is no LED strip.
// ---------------------------------------------------------------------------

constexpr const char* CLOCK_VARIANT = "NOCLOCK";                                              // Build variant name (shown in version/log)

const uint32_t NUM_LEDS = 1;                                                                  // No physical strip; keeps strip ctors + core LED helpers compiling harmlessly
const  uint32_t LEDNEARLDR = 9999;                                                            // 9999 is not used. Pos of LDR near a LED then. Turn ot one second ofd to read LDR
// Q display-choice command has nothing to offer without a display.
const char* const QchoiceNames[] = { "None" };
const byte LASTITEM = 0;

// --- No-op stubs for the display functions the trunk calls unconditionally -//
void Displaytime(void)      {}
void SetSecondColour(void)  {}
void Demomode(void)         {}
void FireMode(void)         {}
void Selftest(int Delayms)  {}
void Selftest(void)         {}
void BlinkUUR(int NoofBlinks, int Delayms)      {}
void BlinkHETISWAS(int NoofBlinks, int Delayms) {}
void BlinkTWAALF(int NoofBlinks, int Delayms)   {}
