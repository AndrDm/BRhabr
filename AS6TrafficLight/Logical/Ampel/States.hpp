#include "machine.hpp"

// data shared between FSM states and outside code
struct Context {
	unsigned cycleCount = 0;
	unsigned countGreenBlinking = 0;
	unsigned countGreen = 0;
	unsigned countYellow = 0;
	unsigned countRed = 0;
};

// convenience typedef
using M = hfsm2::MachineT<hfsm2::Config::ContextT<Context>>;

// macro magic invoked to simplify FSM structure declaration
#define S(s) struct s

// State machine structure
using FSM = M::PeerRoot<
	// sub-machine ..
	M::Composite<S(On),
	// .. with 5 sub-states
	S(Red),
	S(YellowDownwards),
	S(YellowUpwards),
	S(GreenBlinking),
	S(Green)
	>,
				S(Off)
			>;
#undef S

//------------------------------------------------------------------------------

static_assert(FSM::regionId<On>()			  ==  1, "");

static_assert(FSM::stateId<On>()			  ==  1, "");
static_assert(FSM::stateId<Red>()			  ==  2, "");
static_assert(FSM::stateId<YellowDownwards>() ==  3, "");
static_assert(FSM::stateId<YellowUpwards>()	  ==  4, "");
static_assert(FSM::stateId<GreenBlinking>()	  ==  5, "");
static_assert(FSM::stateId<Green>()			  ==  6, "");
static_assert(FSM::stateId<Off>()			  ==  7, "");

////////////////////////////////////////////////////////////////////////////////

// top-level region in the hierarchy
struct On
	: FSM::State // necessary boilerplate!
{
	// called on state entry
	void enter(Control& control) {
		control.context().cycleCount = 0;
		TraceMessage("On");
	}
};

//------------------------------------------------------------------------------

// sub-states
struct Red
	: FSM::State
{
	void enter(Control& control) {
		++control.context().cycleCount;
		LightRed = true;
		LightYellow = false;
		LightGreen = false;
		control.context().countRed = 0;
		CountDown = DurationRed + DurationYellow + 2;
		TraceMessage("Red");
		
	}

	// state can initiate transitions to _any_ other state
	void update(FullControl& control) {

		// multiple transitions can be initiated, can be useful in a hierarchy
		if (control.context().cycleCount > 10)
			control.changeTo<Off>();
		else {
			if (control.context().countRed++ >= DurationRed){
				control.changeTo<YellowDownwards>();
			}
		}
		CountDown--;
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct YellowDownwards
	: FSM::State
{
	void enter(Control& control) {
		TraceMessage("Yellow v");
		LightYellow = true; //Keep Red On
		control.context().countYellow = 0;
	}

	void update(FullControl& control) {
		if (control.context().countYellow++ > DurationYellow) { //was 3
			control.changeTo<Green>();
		}
		CountDown--;
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct YellowUpwards
	: FSM::State
{
	void enter(Control& control) {
		TraceMessage("Yellow ^");
		LightGreen = false;
		LightYellow = true;
		CountDownGreenON = false;
		control.context().countYellow = 0;
	}

	void update(FullControl& control) {
		if (control.context().countYellow++ > DurationYellow) {
			control.changeTo<Red>();
		}
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct GreenBlinking
	: FSM::State
{
	void enter(Control& control) {
		TraceMessage("Green Blinking");
		CountDownGreenON = true;
		control.context().countGreenBlinking = 0;
	}

	void update(FullControl& control) {
		LightGreen = !LightGreen; // Blinking
		if ((control.context().countGreenBlinking++ > DurationGreenBlinking) && !LightGreen) {
			control.changeTo<YellowUpwards>();
		}
		CountDown--;
	}
};


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Green
	: FSM::State
{
	void enter(Control& control) {
		TraceMessage("Green");
		control.context().countGreen = 0;
		LightRed = false;
		LightYellow = false;
		LightGreen = true;
		CountDownGreenON = true;
		CountDown = DurationGreen + DurationGreenBlinking + 4;
	}

	void update(FullControl& control) {
		if (control.context().countGreen++ > DurationGreen){
			control.changeTo<GreenBlinking>();
		}
		CountDown--;
	}
};

//------------------------------------------------------------------------------

// another top-level state
struct Off
	: FSM::State
{
	void enter(Control&) {
		TraceMessage("Off");
	}
};
